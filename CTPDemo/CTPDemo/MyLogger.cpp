#include "stdafx.h"
#include "MyLogger.h"
#include "ErrorCode.h"

std::map<std::string, MyFileMapper<LogItem32>, StringCompare> MyLogManager::m_map32;
std::map<std::string, MyFileMapper<LogItem64>, StringCompare> MyLogManager::m_map64;
std::map<std::string, MyFileMapper<LogItem128>, StringCompare> MyLogManager::m_map128;
std::map<std::string, MyFileMapper<LogItem256>, StringCompare> MyLogManager::m_map256;
std::map<std::string, MyFileMapper<LogItem512>, StringCompare> MyLogManager::m_map512;

MyLogManager::MyLogManager()
{
}

MyLogManager::~MyLogManager()
{
}

// get a logger which maps to a file named filePath.
// if a logger map to the specified filePath exists:
// 1. the existing logger's item length is 32, return pointer of the existing one, and give a warning.
// 2. the existing logger's item length is not 32, return 0, and give an error.
MyFileMapper<LogItem32>* MyLogManager::GetFileMapper32(int& iErr, const std::string& filePath)
{
    if(filePath.empty())
    {
        iErr = ErrorInvalidFilePath;
        return 0;
    }

    if(IsLoggerExists(iErr, filePath))
    {
        std::map<std::string, MyFileMapper<LogItem32>, StringCompare>::iterator itor = m_map32.find(filePath);
        if(itor != m_map32.end())
        {
            iErr = WarnSameLoggerExists;
            return &itor->second;
        }
        else
        {
            iErr = ErrorDiffLengthLoggerExists;
            return 0;
        }
    }
    else
    {
        m_map32[filePath].MapToFile(iErr, filePath);
        if(iErr != Success0)
        {
            m_map32.erase(filePath);
            return 0;
        }
        else
        {
            assert(iErr == Success0);
            return &m_map32[filePath];
        }
    }
}

// get a logger which maps to a file named filePath.
// if a logger map to the specified filePath exists:
// 1. the existing logger's item length is 64, return pointer of the existing one, and give a warning.
// 2. the existing logger's item length is not 64, return 0, and give an error.
MyFileMapper<LogItem64>* MyLogManager::GetFileMapper64(int& iErr, const std::string& filePath)
{
    if(filePath.empty())
    {
        iErr = ErrorInvalidFilePath;
        return 0;
    }

    if(IsLoggerExists(iErr, filePath))
    {
        std::map<std::string, MyFileMapper<LogItem64>, StringCompare>::iterator itor = m_map64.find(filePath);
        if(itor != m_map64.end())
        {
            iErr = WarnSameLoggerExists;
            return &itor->second;
        }
        else
        {
            iErr = ErrorDiffLengthLoggerExists;
            return 0;
        }
    }
    else
    {
        m_map64[filePath].MapToFile(iErr, filePath);
        if(iErr != Success0)
        {
            m_map64.erase(filePath);
            return 0;
        }
        else
        {
            assert(iErr == Success0);
            return &m_map64[filePath];
        }
    }
}

// get a logger which maps to a file named filePath.
// if a logger map to the specified filePath exists:
// 1. the existing logger's item length is 128, return pointer of the existing one, and give a warning.
// 2. the existing logger's item length is not 128, return 0, and give an error.
MyFileMapper<LogItem128>* MyLogManager::GetFileMapper128(int& iErr, const std::string& filePath)
{
    if(filePath.empty())
    {
        iErr = ErrorInvalidFilePath;
        return 0;
    }

    if(IsLoggerExists(iErr, filePath))
    {
        std::map<std::string, MyFileMapper<LogItem128>, StringCompare>::iterator itor = m_map128.find(filePath);
        if(itor != m_map128.end())
        {
            iErr = WarnSameLoggerExists;
            return &itor->second;
        }
        else
        {
            iErr = ErrorDiffLengthLoggerExists;
            return 0;
        }
    }
    else
    {
        m_map128[filePath].MapToFile(iErr, filePath);
        if(iErr != Success0)
        {
            m_map128.erase(filePath);
            return 0;
        }
        else
        {
            assert(iErr == Success0);
            return &m_map128[filePath];
        }
    }
}

// get a logger which maps to a file named filePath.
// if a logger map to the specified filePath exists:
// 1. the existing logger's item length is 256, return pointer of the existing one, and give a warning.
// 2. the existing logger's item length is not 256, return 0, and give an error.
MyFileMapper<LogItem256>* MyLogManager::GetFileMapper256(int& iErr, const std::string& filePath)
{
    if(filePath.empty())
    {
        iErr = ErrorInvalidFilePath;
        return 0;
    }

    if(IsLoggerExists(iErr, filePath))
    {
        std::map<std::string, MyFileMapper<LogItem256>, StringCompare>::iterator itor = m_map256.find(filePath);
        if(itor != m_map256.end())
        {
            iErr = WarnSameLoggerExists;
            return &itor->second;
        }
        else
        {
            iErr = ErrorDiffLengthLoggerExists;
            return 0;
        }
    }
    else
    {
        m_map256[filePath].MapToFile(iErr, filePath);
        if(iErr != Success0)
        {
            m_map256.erase(filePath);
            return 0;
        }
        else
        {
            assert(iErr == Success0);
            return &m_map256[filePath];
        }
    }
}

// get a logger which maps to a file named filePath.
// if a logger map to the specified filePath exists:
// 1. the existing logger's item length is 512, return pointer of the existing one, and give a warning.
// 2. the existing logger's item length is not 512, return 0, and give an error.
MyFileMapper<LogItem512>* MyLogManager::GetFileMapper512(int& iErr, const std::string& filePath)
{
    if(filePath.empty())
    {
        iErr = ErrorInvalidFilePath;
        return 0;
    }

    if(IsLoggerExists(iErr, filePath))
    {
        std::map<std::string, MyFileMapper<LogItem512>, StringCompare>::iterator itor = m_map512.find(filePath);
        if(itor != m_map512.end())
        {
            iErr = WarnSameLoggerExists;
            return &itor->second;
        }
        else
        {
            iErr = ErrorDiffLengthLoggerExists;
            return 0;
        }
    }
    else
    {
        m_map512[filePath].MapToFile(iErr, filePath);
        if(iErr != Success0)
        {
            m_map512.erase(filePath);
            return 0;
        }
        else
        {
            assert(iErr == Success0);
            return &m_map512[filePath];
        }
    }
}

bool MyLogManager::IsLoggerExists(int &iErr, const std::string& filePath)
{
    if( m_map32.find(filePath) != m_map32.end() ||
        m_map64.find(filePath) != m_map64.end() ||
        m_map128.find(filePath) != m_map128.end() ||
        m_map256.find(filePath) != m_map256.end() ||
        m_map512.find(filePath) != m_map512.end())
    {
        return true;
    }
    return false;
}

