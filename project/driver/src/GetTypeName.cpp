/*
 * Copyright (C) 2007 National Association of REALTORS(R)
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

// This file was generated by the gettypename_gen.rb script in the
// tools directory of the ezRETS source.

#include "utils.h"
#include <boost/lexical_cast.hpp>

using std::string;

string odbcrets::getTypeName(SQLSMALLINT type)
{
    string result;
    switch (type)
    {
        case SQL_VARCHAR:
            result = "SQL_VARCHAR";
            break;
        case SQL_DECIMAL:
            result = "SQL_DECIMAL";
            break;
        case SQL_C_CHAR:
            result = "SQL_C_CHAR";
            break;
        case SQL_C_LONG:
            result = "SQL_C_LONG";
            break;
        case SQL_C_SHORT:
            result = "SQL_C_SHORT";
            break;
        case SQL_C_FLOAT:
            result = "SQL_C_FLOAT";
            break;
        case SQL_C_DOUBLE:
            result = "SQL_C_DOUBLE";
            break;
        case SQL_C_NUMERIC:
            result = "SQL_C_NUMERIC";
            break;
        case SQL_C_DEFAULT:
            result = "SQL_C_DEFAULT";
            break;
        case SQL_C_DATE:
            result = "SQL_C_DATE";
            break;
        case SQL_C_TIME:
            result = "SQL_C_TIME";
            break;
        case SQL_C_TIMESTAMP:
            result = "SQL_C_TIMESTAMP";
            break;
        case SQL_C_BINARY:
            result = "SQL_C_BINARY";
            break;
        case SQL_C_BIT:
            result = "SQL_C_BIT";
            break;
        case SQL_C_TINYINT:
            result = "SQL_C_TINYINT";
            break;
        case SQL_C_SLONG:
            result = "SQL_C_SLONG";
            break;
        case SQL_C_SSHORT:
            result = "SQL_C_SSHORT";
            break;
        case SQL_C_STINYINT:
            result = "SQL_C_STINYINT";
            break;
        case SQL_C_ULONG:
            result = "SQL_C_ULONG";
            break;
        case SQL_C_USHORT:
            result = "SQL_C_USHORT";
            break;
        case SQL_C_UTINYINT:
            result = "SQL_C_UTINYINT";
            break;
        case SQL_C_TYPE_DATE:
            result = "SQL_C_TYPE_DATE";
            break;
        case SQL_C_TYPE_TIME:
            result = "SQL_C_TYPE_TIME";
            break;
        case SQL_C_TYPE_TIMESTAMP:
            result = "SQL_C_TYPE_TIMESTAMP";
            break;
        case SQL_C_INTERVAL_YEAR:
            result = "SQL_C_INTERVAL_YEAR";
            break;
        case SQL_C_INTERVAL_MONTH:
            result = "SQL_C_INTERVAL_MONTH";
            break;
        case SQL_C_INTERVAL_DAY:
            result = "SQL_C_INTERVAL_DAY";
            break;
        case SQL_C_INTERVAL_HOUR:
            result = "SQL_C_INTERVAL_HOUR";
            break;
        case SQL_C_INTERVAL_MINUTE:
            result = "SQL_C_INTERVAL_MINUTE";
            break;
        case SQL_C_INTERVAL_SECOND:
            result = "SQL_C_INTERVAL_SECOND";
            break;
        case SQL_C_INTERVAL_YEAR_TO_MONTH:
            result = "SQL_C_INTERVAL_YEAR_TO_MONTH";
            break;
        case SQL_C_INTERVAL_DAY_TO_HOUR:
            result = "SQL_C_INTERVAL_DAY_TO_HOUR";
            break;
        case SQL_C_INTERVAL_DAY_TO_MINUTE:
            result = "SQL_C_INTERVAL_DAY_TO_MINUTE";
            break;
        case SQL_C_INTERVAL_DAY_TO_SECOND:
            result = "SQL_C_INTERVAL_DAY_TO_SECOND";
            break;
        case SQL_C_INTERVAL_HOUR_TO_MINUTE:
            result = "SQL_C_INTERVAL_HOUR_TO_MINUTE";
            break;
        case SQL_C_INTERVAL_HOUR_TO_SECOND:
            result = "SQL_C_INTERVAL_HOUR_TO_SECOND";
            break;
        case SQL_C_INTERVAL_MINUTE_TO_SECOND:
            result = "SQL_C_INTERVAL_MINUTE_TO_SECOND";
            break;
        case SQL_C_SBIGINT:
            result = "SQL_C_SBIGINT";
            break;
        case SQL_C_UBIGINT:
            result = "SQL_C_UBIGINT";
            break;
        case SQL_C_GUID:
            result = "SQL_C_GUID";
            break;
        default:
            result = boost::lexical_cast<std::string>(type);
            break;
    };

    return result;
}
