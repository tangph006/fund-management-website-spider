#pragma once
#include <tchar.h>
#include <assert.h>
#include <typeinfo>
#include <windows.h>
#include "resource.h"
#include "ErrorCode.h"

template<class T> 
class MyFileMapper
{
private:
    typedef struct 
    {
        int m_nDataCount;
        char m_strStructName[64];
    } tagDataInfo;

public:
    MyFileMapper(): m_pData(0),m_hFile(0),m_hMap(0), m_nMappedDataCount(0)
    {
        m_dataInfo.m_nDataCount = 0;
        memset(m_dataInfo.m_strStructName, 0, 64);
        memset(m_strPath, 0, 256);
        const type_info& typeinfo = typeid(T);
        const char* className = typeinfo.name();
        strcpy(&m_dataInfo.m_strStructName[0], className);
    }

    virtual ~MyFileMapper()
    {
        UnMapFromFile();
    }

    void MapToFile(int& iErr, const std::string& filePath)
    {
        UnMapFromFile();
        if(filePath.empty())
        {
            iErr = ErrorInvalidFilePath;
            return;
        }

        CreateFileIfNotExist(iErr, filePath);
        if(iErr != Success0)
            return;

        strcpy(m_strPath, filePath.c_str());

        ReadDataInformation(iErr, filePath);
        if(iErr != Success0)
            return;

        m_hFile = CreateFile(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hFile == INVALID_HANDLE_VALUE)
        {
            iErr = ErrorOpenFileFailed;
            return;
        }

        if(m_dataInfo.m_nDataCount >= m_nMappedDataCount)
            m_nMappedDataCount = GetNextSize(m_dataInfo.m_nDataCount);
        CreateAndMap(iErr);
        return;
    }

    void UnMapFromFile() 
    {
        if(m_pMapViewBegin)
        {
            UnmapViewOfFile(m_pMapViewBegin);
            m_pMapViewBegin = NULL;
        }
        if(m_hMap)
        {
            CloseHandle(m_hMap);
            m_hMap = NULL;
        }

        if(m_hFile)
        {
            if(m_nMappedDataCount > m_dataInfo.m_nDataCount)
            {
                //SetEndOfFile(m_hFile);
            }
            CloseHandle(m_hFile);
            m_hFile = NULL;
        }
    }

    int AddItem(int& iErr, const T* pItem)
    {
        assert(pItem);
        memcpy(&m_pData[m_dataInfo.m_nDataCount], pItem, sizeof(T));
        m_dataInfo.m_nDataCount++;
        memcpy(m_pMapViewBegin, &m_dataInfo.m_nDataCount, 4);
        if(m_dataInfo.m_nDataCount >= m_nMappedDataCount)
        {
            UnmapViewOfFile(m_pMapViewBegin);
            CloseHandle(m_hMap);
            m_hMap = NULL;
            m_nMappedDataCount = GetNextSize(m_nMappedDataCount);
            CreateAndMap(iErr);
            return m_dataInfo.m_nDataCount;
        }
        iErr = Success0;
        return m_dataInfo.m_nDataCount;
    }

    int GetDataCount()
    {
        return m_dataInfo.m_nDataCount;
    }

    T* GetDataByIndex(int iIndex)
    {
        return m_pData+iIndex;
    }

protected:
    void CreateAndMap(int &iErr)
    {
        int sizeOfT = sizeof(T);
        m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, sizeof(m_dataInfo)+sizeof(T)*m_nMappedDataCount, NULL);
        if (m_hMap == NULL)
        {
            iErr = ErrorCreateFileMapFailed;
            CloseHandle(m_hFile);
            m_hFile = NULL;
            return;
        }

        m_pMapViewBegin = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(m_dataInfo)+sizeof(T)*m_nMappedDataCount);
        if (m_pMapViewBegin == NULL)
        {
            int iErr = ErrorMapViewOfFileFailed;
            CloseHandle(m_hMap);
            m_hMap = NULL;
            CloseHandle(m_hFile);
            m_hFile = NULL;
            return;
        }

        m_pData = (T*)((DWORD)m_pMapViewBegin+sizeof(m_dataInfo));
        iErr = Success0;
        return;
    }

    void CreateFileIfNotExist(int &iErr, std::string filePath)
    {
        HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            int iWin32Err = GetLastError();
            if(iWin32Err == 2)
            {
                hFile = CreateFile(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 
                    NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE)
                {
                    iErr = ErrorOpenFileFailed;
                    return;
                }
                DWORD tempDWord;
                WriteFile(hFile, &m_dataInfo, sizeof(m_dataInfo), &tempDWord, NULL);
                CloseHandle(hFile);
                iErr = Success0;
                return;
            }
            else
            {
                iErr = ErrorOpenFileFailed;
                return;
            }
        }
        CloseHandle(hFile);
        iErr = Success0;
    }

    void ReadDataInformation(int &iErr, const std::string& filePath)
    {
        HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            iErr = ErrorOpenFileFailed;
            return;
        }
        DWORD tempDWord;
        ReadFile(hFile, &m_dataInfo, sizeof(m_dataInfo), &tempDWord, NULL);
        CloseHandle(hFile);
        iErr = Success0;
        return;
    }

    static int GetNextSize(int nCurSize)
    {
        static const int primeList[72] = {
        3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
        1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
        17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
        187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
        1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369};
        int nCount = sizeof(primeList) / sizeof(primeList[0]);
        for(int i=0; i<nCount; i++)
        {
            if(primeList[i] > nCurSize)
            {
                return primeList[i];
            }
        }
        return -1;
    }
protected:
    tagDataInfo m_dataInfo;
    LPVOID m_pMapViewBegin;
    int m_nMappedDataCount;
    T* m_pData;
    TCHAR m_strPath[256];
    HANDLE m_hFile;
    HANDLE m_hMap;
};




