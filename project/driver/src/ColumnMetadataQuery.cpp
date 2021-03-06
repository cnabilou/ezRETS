/*
 * Copyright (C) 2006-2009 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "ColumnMetadataQuery.h"
#include "MetadataView.h"
#include "librets/MetadataTable.h"
#include "ResultSet.h"
#include "RetsSTMT.h"
#include "RetsDBC.h"
#include "DataTranslator.h"
#include "Column.h"

using namespace odbcrets;
using namespace librets;
namespace b = boost;
using std::string;

class TableNameSorter {
  public:
    TableNameSorter(bool useStandardNames)
        : mUseStandardNames(useStandardNames)
    {
    }
    
    bool operator()(const MetadataTable* left,
                    const MetadataTable* right)
    {
        string leftName;
        string rightName;
        if (mUseStandardNames)
        {
            leftName = b::to_upper_copy(left->GetStandardName());
            rightName = b::to_upper_copy(right->GetStandardName());
        }
        else
        {
            leftName = b::to_upper_copy(left->GetSystemName());
            rightName = b::to_upper_copy(right->GetSystemName());
        }
        return (leftName < rightName);
    }

  private:
    bool mUseStandardNames;
};

ColumnMetadataQuery::ColumnMetadataQuery(RetsSTMT* stmt, string table,
                                         string column)
    : Query(stmt), mTable(table), mColumn(column)
{
    // This asks for all, so its the same as an empty string for us.
    if (mTable.compare("%") == 0)
    {
        mTable.clear();
    }
    
    if (mColumn.compare("%") == 0)
    {
        mColumn.clear();
    }
}

void ColumnMetadataQuery::prepareResultSet()
{
    DataTranslatorSPtr dt(DataTranslator::factory(mStmt));
    mResultSet.reset(newResultSet(dt));
    
    mResultSet->addColumn("TABLE_CAT", SQL_VARCHAR);
    mResultSet->addColumn("TABLE_SCHEM", SQL_VARCHAR);
    mResultSet->addColumn("TABLE_NAME", SQL_VARCHAR); // NOT NULL
    mResultSet->addColumn("COLUMN_NAME", SQL_VARCHAR); // NOT NULL
    mResultSet->addColumn("DATA_TYPE", SQL_SMALLINT); // smallint not null
    mResultSet->addColumn("TYPE_NAME", SQL_VARCHAR); // varchar not null
    mResultSet->addColumn("COLUMN_SIZE", SQL_INTEGER); // int
    mResultSet->addColumn("BUFFER_LENGTH", SQL_INTEGER);
    mResultSet->addColumn("DECIMAL_DIGITS", SQL_SMALLINT);
    mResultSet->addColumn("NUM_PREC_RADIX", SQL_SMALLINT);
    mResultSet->addColumn("NULLABLE", SQL_SMALLINT); // not null
    mResultSet->addColumn("REMARKS", SQL_VARCHAR);
    mResultSet->addColumn("COLUMN_DEF", SQL_VARCHAR);
    mResultSet->addColumn("SQL_DATA_TYPE", SQL_SMALLINT); // not null
    mResultSet->addColumn("SQL_DATETIME_SUB", SQL_SMALLINT);
    mResultSet->addColumn("CHAR_OCTET_LENGTH", SQL_INTEGER);
    mResultSet->addColumn("ORDINAL_POSITION", SQL_INTEGER); // not null
    mResultSet->addColumn("IS_NULLABLE", SQL_VARCHAR);
}

SQLRETURN ColumnMetadataQuery::execute()
{
    if (mTable.find("%") != string::npos)
    {
        mStmt->addError("HYC00",
                        "Search pattern for table names not (yet) supported");
        return SQL_ERROR;
    }

    if (mColumn.find("%") != string::npos)
    {
        mStmt->addError("HYC00",
                        "Search pattern for column names not (yet) supported");
        return SQL_ERROR;
    }
    
    MetadataViewPtr metadataViewPtr = mStmt->getMetadataView();
    
    ResourceClassPairVectorPtr rcpVectorPtr;
    bool tableEmpty = mTable.empty();
    if (tableEmpty)
    {
        // This is for "data:" tables
        rcpVectorPtr = metadataViewPtr->getResourceClassPairs();
    }
    else
    {
        // This is for a specific "data:" table
        rcpVectorPtr.reset(new ResourceClassPairVector());
        if (mTable.compare(0, 5, "data:") == 0)
        {
            ResourceClassPairPtr pair =
                metadataViewPtr->getResourceClassPairBySQLTable(mTable);
            if (pair != NULL)
            {
                rcpVectorPtr->push_back(pair);
            }
        }
    }

    SQLRETURN result = SQL_SUCCESS;
    for (ResourceClassPairVector::iterator i = rcpVectorPtr->begin();
         i != rcpVectorPtr->end() && result == SQL_SUCCESS; i++)
    {
        ResourceClassPairPtr rcp = *i;
        MetadataResource* res = rcp->first;
        MetadataClass* clazz = rcp->second;

        MetadataTable* rTable = NULL;
        if (mColumn.empty())
        {
            MetadataTableList tables =
                metadataViewPtr->getTablesForClass(clazz);
            std::sort(tables.begin(), tables.end(), TableNameSorter(
                          mStmt->mDbc->mDataSource.GetStandardNames()));
            for (MetadataTableList::iterator j = tables.begin();
                 j != tables.end(); j++)
            {
                rTable = *j;

                result = processColumn(res, clazz, rTable);
            }
        }
        else
        {
            rTable = metadataViewPtr->getTable(clazz, mColumn);
            result = processColumn(res, clazz, rTable);
        }
    }

    // Add processing for binary tables here?
//     if (tableEmpty)
//     {
//         // This is for "object:" tables
//         MetadataResourceList resources =
//             metadataViewPtr->getResources();

//         // For object tables we only need the resources
//         // -- get resources here
//     }
//     else if (mTable.compare(0, 7, "object:") == 0)
//     {
//         StringVector parts;
//         b::split(parts, mTable, b::is_any_of(":"));
//     }


    return result;
}

SQLRETURN ColumnMetadataQuery::processColumn(
    MetadataResource* res, MetadataClass* clazz, MetadataTable* rTable)
{
    MetadataViewPtr metadataView = mStmt->getMetadataView();
    string sqlTableName = metadataView->makeSQLDataTableName(res, clazz);

    if (sqlTableName.empty())
    {
        return SQL_SUCCESS;
    }

    string colName;
    if (mStmt->mDbc->mDataSource.GetStandardNames())
    {
        colName = rTable->GetStandardName();
        if (colName.empty())
        {
            return SQL_SUCCESS;
        }
    }
    else
    {
        colName = rTable->GetSystemName();
    }

    DataTranslatorSPtr dataTranslator(DataTranslator::factory(mStmt));
                               
    // A lot of the logic in this method is duplicated in Column as
    // the Column often needs to know this information about this.  To
    // avoid duplicated logic, we'll create a dummy resultset with a
    // dummy column that is build using the current rTable.
    // We'll need to come up with a cleaner way to do this.
    DummyResultSet dummyResult(mStmt->getLogger(), metadataView,
                               dataTranslator, mStmt->getArd());
    dummyResult.addColumn(colName, rTable,
                          mStmt->mDbc->mDataSource.GetUseCompactFormat());
    ColumnPtr dummyCol = dummyResult.getColumn(1);

    StringVectorPtr results(new StringVector());
    // TABLE_CAT
    results->push_back("");
    // TABLE_SCHEMA
    results->push_back("");
    // TABLE_NAME
    results->push_back(sqlTableName);
    // COLUMN_NAME
    results->push_back(colName);


    // DATA_TYPE
    SQLSMALLINT type = dummyCol->getDataType();
    string typeString = b::lexical_cast<string>(type);
    results->push_back(typeString);

    // TYPE_NAME
    results->push_back(dataTranslator->getOdbcTypeName(type));
    
    // COLUMN_SIZE
    int maxLen = dummyCol->getMaximumLength();
    string maxLenString = b::lexical_cast<string>(maxLen);
    results->push_back(maxLenString);

    // BUFFER_LENGTH
    if (type == SQL_VARCHAR || type == SQL_CHAR)
    {
        results->push_back(maxLenString);
    }
    else
    {
        int size = dataTranslator->getOdbcTypeLength(type);
        results->push_back(b::lexical_cast<string>(size));
    }

    // DECIMAL_DIGITS
    results->push_back(b::lexical_cast<string>(dummyCol->getPrecision()));

    // NUM_PREC_RADIX
    if (type == SQL_DECIMAL || type == SQL_DOUBLE)
    {
        results->push_back("10");
    }
    else
    {
        results->push_back("");
    }

    // NULLABLE
    results->push_back(b::lexical_cast<string>(SQL_NULLABLE));

    // REMARKS
    results->push_back(rTable->GetLongName());

    // COLUMN_DEF
    results->push_back("");

    // SQL_DATA_TYPE
    if (type == SQL_TYPE_DATE || type == SQL_TYPE_TIME ||
        type == SQL_TIMESTAMP_LEN)
    {
        results->push_back(b::lexical_cast<string>(SQL_DATETIME));
    }
    else
    {
        results->push_back(typeString);
    }

    // SQL_DATETIME_SUB
    results->push_back(""); // do we need to do something here?

    // CHAR_OCTECT_LENGTH
    // todo: should be put behind an if for char
    if (type == SQL_CHAR)
    {
        results->push_back(
            b::lexical_cast<string>(dummyCol->getMaximumLength()));
    }
    else
    {
        results->push_back("");
    }

    // ORDINAL_POSITION
    results->push_back("1");  // todo: fix ordinal

    // IS_NULLABLE
    results->push_back("YES");

    // Upcast the generic result set to the BulkResultSet we should
    // use here.  Needed to be done so we can get access to the addRow
    // method.
    BulkResultSet* rs = dynamic_cast<BulkResultSet*>(mResultSet.get());
    rs->addRow(results);

    return SQL_SUCCESS;
}

std::ostream & ColumnMetadataQuery::print(std::ostream & out) const
{
    out << "ColumnMetadataQuery: '" << mTable << "' '" << mColumn << "'";
    return out;
}
