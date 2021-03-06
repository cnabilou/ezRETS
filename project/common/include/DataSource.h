/*
 * Copyright (C) 2005,2006 National Association of REALTORS(R)
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

#ifndef EZRETS_DATA_SOURCE_H
#define EZRETS_DATA_SOURCE_H

#include <string>
#include <ostream>
#include "ezrets.h"
#include "commonfwd.h"
#include "librets/RetsVersion.h"
#include "librets/EncodingType.h"
#include "librets/UserAgentAuthType.h"
#include "librets/protocol_forward.h"

namespace odbcrets
{

class DataSource
{
  public:
    DataSource();

    DataSource(std::string name);

    void MergeFromConnectionString(std::string connectionString);

    void MergeFromIni();

    void CreateInIni(std::string driver, UWORD configMode);

    void RemoveFromIni();

    void GetOdbcConnectionString();

    void SetFromOdbcConnectionString(std::string odbcConnectionString);

    std::string GetName() const;

    void SetName(std::string name);

    std::string GetLoginUrl() const;

    void SetLoginUrl(std::string loginUrl);

    std::string GetUsername() const;

    void SetUsername(std::string username);

    std::string GetPassword() const;

    void SetPassword(std::string password);

    bool GetStandardNames() const;

    void SetStandardNames(bool standardNames);

    std::string GetUserAgent() const;

    std::string GetCustomUserAgent() const;

    void SetCustomUserAgent(std::string customUserAgent);

    bool GetUseHttpGet() const;

    void SetUseHttpGet(bool useHttpGet);

    bool GetUseHttpLogging() const;

    void SetUseHttpLogging(bool useHttpLogging);

    std::string GetHttpLogFile() const;

    void SetHttpLogFile(std::string httpLogFile);

    bool GetUseDebugLogging() const;

    void SetUseDebugLogging(bool useDebugLogging);

    std::string GetDebugLogFile() const;

    void SetDebugLogFile(std::string debugLogFile);

    librets::RetsVersion GetRetsVersion() const;

    void SetRetsVersion(librets::RetsVersion retsVersion);

    bool GetUseBulkMetadata() const;

    void SetUseBulkMetadata(bool useBulkMetadata);

    bool GetIgnoreMetadataType() const;

    void SetIgnoreMetadataType(bool ignoreMetadataType);

    bool GetUseCompactFormat() const;

    void SetUseCompactFormat(bool useCompactFormat);

    bool GetDisableGetObjectMetadata() const;

    void SetDisableGetObjectMetadata(bool disableGetObjectMetadata);

    bool GetEnableUserAgentAuth() const;

    void SetEnableUserAgentAuth(bool enable);

    std::string GetUserAgentPassword() const;

    void SetUserAgentPassword(std::string passwd);

    librets::UserAgentAuthType GetUserAgentAuthType() const;

    void SetUserAgentAuthType(librets::UserAgentAuthType type);

    bool GetTreatDecimalAsString() const;

    void SetTreatDecimalAsString(bool enable);

    librets::EncodingType GetEncodingType() const;

    void SetEncodingType(librets::EncodingType encoding);

    void SetUseProxy(bool enable);

    bool GetUseProxy() const;

    std::string GetProxyUrl() const;

    void SetProxyUrl(std::string url);

    std::string GetProxyPassword() const;

    void SetProxyPassword(std::string password);

    bool GetSupportsQueryStar() const;

    void SetSupportsQueryStar(bool supports);

    bool GetHttpLogEverything() const;

    void SetHttpLogEverything(bool log);

    /**
     * Checks to see if all the required entries are filled in.
     */
    bool IsComplete() const;

    std::string GetConnectionString() const;

    std::ostream & Print(std::ostream & out) const;

    /**
     * Creates a new RETS session from the available data source
     * properties.
     *
     * @return new RetsSession
     */
    librets::RetsSessionPtr CreateRetsSession() const;

    /**
     * Logs into a RETS session using the data source's usesrname and
     * password.
     *
     * @param session RETS session
     * @return success or failure of the login
     */
    bool RetsLogin(librets::RetsSessionPtr session) const;

  private:
    static const char * INI_FILENAME;
    static const char * INI_DSN;
    static const char * INI_LOGIN_URL;
    static const char * INI_USERNAME;
    static const char * INI_PASSWORD;
    static const char * INI_STANDARD_NAMES;
    static const char * INI_USER_AGENT;
    static const char * INI_USE_HTTP_GET;
    static const char * INI_USE_HTTP_LOGGING;
    static const char * INI_HTTP_LOG_FILE;
    static const char * INI_USE_DEBUG_LOGGING;
    static const char * INI_DEBUG_LOG_FILE;
    static const char * INI_RETS_VERSION;
    static const char * INI_USE_BULK_METADATA;
    static const char * INI_DRIVER;
    static const char * INI_IGNORE_METADATA_TYPE;
    static const char * INI_USE_COMPACT_FORMAT;
    static const char * INI_DISABLE_GETOBJECT_METADATA;
    static const char * INI_ENABLE_USER_AGENT_AUTH;
    static const char * INI_USER_AGENT_PASSWORD;
    static const char * INI_USER_AGENT_AUTH_TYPE;
    static const char * INI_TREAT_DECIMAL_AS_STRING;
    static const char * INI_ENCODING_TYPE;
    static const char * INI_USE_PROXY;
    static const char * INI_PROXY_URL;
    static const char * INI_PROXY_PASSWORD;
    static const char * INI_SUPPORTS_QUERYSTAR;
    static const char * INI_USE_BULK_QUERY;
    static const char * INI_HTTP_LOG_EVERYTHING;

    static const std::string DEFAULT_USER_AGENT;
    static const librets::RetsVersion DEFAULT_RETS_VERSION;
    static const librets::UserAgentAuthType DEFAULT_UA_AUTH_TYPE;
    
    void init();

    std::string GetProfileString(std::string entry,
                                 std::string defaultValue = "");

    std::string GetSqlInstallerError();

    void WriteProfileString(UWORD configMode, std::string entry,
                            std::string value);
    void WriteProfileString(UWORD configMode, std::string entry, bool value);

    bool GetProfileBool(std::string entry, bool defaultValue = false);

    void MergeFromProfileString(std::string & aString, std::string entry);

    void WriteDSNToIni(std::string driver, UWORD configMode);
    void RemoveDSNFromIni();
    /**
     * Writes to the SQL INI file.  This assumes that the DSN has
     * been previously created.
     */
    void WriteToIni(UWORD configMode);

    void AssertNameNotEmpty(std::string message);
    void AssertSqlSuccess(BOOL sqlError, std::string message = "");

    static void AppendToConnectionString(std::string & connectString,
                                         std::string name, std::string value);

    static void AppendToConnectionString(std::string & connectString,
                                         std::string name, bool value);

    static bool stringToBool(std::string aString);
    static std::string boolToString(bool aBool);
    

    std::string mName;
    std::string mLoginUrl;
    std::string mUsername;
    std::string mPassword;
    bool mStandardNames;
    std::string mCustomUserAgent;
    bool mUseHttpGet;
    bool mUseHttpLogging;
    std::string mHttpLogFile;
    bool mUseDebugLogging;
    std::string mDebugLogFile;
    std::string mRetsVersionString;
    bool mUseBulkMetadata;
    std::string mDriver;
    bool mIgnoreMetadataType;
    bool mUseCompactFormat;
    bool mDisableGetObjectMetadata;
    bool mEnableUserAgentAuth;
    std::string mUserAgentPassword;
    std::string mUserAgentAuthTypeString;
    bool mTreatDecimalAsString;
    std::string mEncodingTypeString;
    bool mUseProxy;
    std::string mProxyUrl;
    std::string mProxyPassword;
    bool mSupportsQueryStar;
    bool mHttpLogEverything;
};

extern const char * RETS_1_0_STRING;
extern const char * RETS_1_5_STRING;
extern const char * RETS_1_7_STRING;
extern const char * RETS_1_7_2_STRING;

extern const char * RETS_XML_DEFAULT_ENCODING_STRING;
extern const char * RETS_XML_ISO_ENCODING_STRING;
extern const char * RETS_XML_UTF8_ENCODING_STRING;

extern const char * USER_AGENT_AUTH_RETS_1_7_STRING;
extern const char * USER_AGENT_AUTH_INTEREALTY_STRING;

std::string RetsVersionToString(librets::RetsVersion retsVersion);

librets::RetsVersion StringToRetsVersion(std::string versionString,
                                         librets::RetsVersion defaultVersion);

std::string EncodingTypeToString(librets::EncodingType encodingType);
librets::EncodingType StringToEncodingType(std::string encodingString,
                                           librets::EncodingType defaultEncoding);

std::string UserAgentAuthTypeToString(librets::UserAgentAuthType authType);
librets::UserAgentAuthType StringToUserAgentAuthType(
    std::string typeString, librets::UserAgentAuthType defaultType);

std::ostream & operator<<(std::ostream & out,
                          librets::RetsVersion retsVersion);

std::ostream & operator<<(std::ostream & out, const DataSource & dataSource);
std::ostream & operator<<(std::ostream & out, DataSource * dataSource);

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
