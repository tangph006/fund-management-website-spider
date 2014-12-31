#pragma once
#include <map>
#include <string>
#include "MyFileMapManager.h"

struct LogItem32
{
    LogItem32(){ Clear(); }
    LogItem32(char* strLog){ assert(strlen(strLog) < 32); Clear(); strncpy(logline, strLog, 31); }
    SetLogLine(char* strLog){ assert(strlen(strLog) < 32); Clear(); strncpy(logline, strLog, 31); }
    void Clear(){ memset(logline, 0, 32); }
    char logline[32];
};
struct LogItem64
{
    LogItem64(){ Clear(); }
    LogItem64(char* strLog){ assert(strlen(strLog) < 64); Clear(); strncpy(logline, strLog, 63); }
    SetLogLine(char* strLog){ assert(strlen(strLog) < 64); Clear(); strncpy(logline, strLog, 63); }
    void Clear(){ memset(logline, 0, 64); }
    char logline[64];
};
struct LogItem128
{
    LogItem128(){ Clear(); }
    LogItem128(char* strLog){ assert(strlen(strLog) < 128); Clear(); strncpy(logline, strLog, 127); }
    SetLogLine(char* strLog){ assert(strlen(strLog) < 128); Clear(); strncpy(logline, strLog, 127); }
    void Clear(){ memset(logline, 0, 128); }
    char logline[128];
};
struct LogItem256
{
    LogItem256(){ Clear(); }
    LogItem256(char* strLog){ assert(strlen(strLog) < 256); Clear(); strncpy(logline, strLog, 255); }
    SetLogLine(char* strLog){ assert(strlen(strLog) < 256); Clear(); strncpy(logline, strLog, 255); }
    void Clear(){ memset(logline, 0, 256); }
    char logline[256];
};
struct LogItem512
{
    LogItem512(){ Clear(); }
    LogItem512(char* strLog){ assert(strlen(strLog) < 512); Clear(); strncpy(logline, strLog, 511); }
    SetLogLine(char* strLog){ assert(strlen(strLog) < 512); Clear(); strncpy(logline, strLog, 511); }
    void Clear(){ memset(logline, 0, 512); }
    char logline[512];
};

class StringCompare
{
public:
    bool operator() (const std::string& str1, const std::string& str2) const
    {
        return str1.compare(str2) < 0;
    }
};

class MyLogManager
{
private:
    MyLogManager();
    ~MyLogManager();
public:
    static MyFileMapper<LogItem32>* GetFileMapper32(int& iErr, const std::string& filePath);
    static MyFileMapper<LogItem64>* GetFileMapper64(int& iErr, const std::string& filePath);
    static MyFileMapper<LogItem128>* GetFileMapper128(int& iErr, const std::string& filePath);
    static MyFileMapper<LogItem256>* GetFileMapper256(int& iErr, const std::string& filePath);
    static MyFileMapper<LogItem512>* GetFileMapper512(int& iErr, const std::string& filePath);

private:
    static bool IsLoggerExists(int &iErr, const std::string& filePath);

private:
    // map<filePath, fileMapper>
    static std::map<std::string, MyFileMapper<LogItem32>, StringCompare> m_map32;
    static std::map<std::string, MyFileMapper<LogItem64>, StringCompare> m_map64;
    static std::map<std::string, MyFileMapper<LogItem128>, StringCompare> m_map128;
    static std::map<std::string, MyFileMapper<LogItem256>, StringCompare> m_map256;
    static std::map<std::string, MyFileMapper<LogItem512>, StringCompare> m_map512;
};

