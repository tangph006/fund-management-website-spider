//http://bbs.csdn.net/topics/80299016
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "aaaaaaaa.h"
#include <assert.h>
#include <typeinfo>

#define BUFFSIZE 1024
#define FILE_MAP_START 64
TCHAR * lpcTheFile = TEXT("fmtest.txt");
const static short initItemCount = 2048;

struct structBase
{
    int m_a;
};

struct structDerive : public structBase
{
    int m_b;
};

template<class T> class MyFileMapManager
{
public:
    MyFileMapManager():m_nDataCount(0),m_pData(0),m_hFile(0),m_hMap(0)
    {
        memset(m_strStructName, 0, 64);
        const type_info& typeinfo = typeid(T);
        const char* className = typeinfo.name();
        strcpy(&m_strStructName[0], className);
    }
    virtual ~MyFileMapManager()
    {
        if(m_pMapViewBegin)
        {
            UnmapViewOfFile(m_pMapViewBegin);
            m_pMapViewBegin = NULL;
        }
        if(m_hMap)
            CloseHandle(m_hMap);
        if(m_hFile)
            CloseHandle(m_hFile);
    }

    void MapToFile(int& iErr, TCHAR* pPath)
    {
        if(pPath == NULL)
        {
            iErr = ID_ERROR_NULL_POINTER;
            return;
        }

        CreateFileIfNotExist(iErr, pPath);
        if(iErr != ID_SUCCESS)
            return;

        m_nDataCount = GetDataCountFromFile(iErr, pPath);
        if(iErr != ID_SUCCESS)
            return;
        m_n2 = m_nDataCount + 1024;

        m_hFile = CreateFile(pPath, GENERIC_READ | GENERIC_WRITE, 0, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hFile == INVALID_HANDLE_VALUE)
        {
            iErr = ID_ERROR_OPEN_FILE_FAILED;
            return;
        }

        m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 68+sizeof(T)*m_n2, NULL);
        if (m_hMap == NULL)
        {
            iErr = ID_ERROR_CREATE_FILE_MAPPING_FAILED;
            CloseHandle(m_hFile);
            return;
        }

        m_pMapViewBegin = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 68+sizeof(T)*m_n2);
        if (m_pMapViewBegin == NULL)
        {
            int iErr = ID_ERROR_MAP_VIEW_OF_FILE_FAILED;
            CloseHandle(m_hMap);
            CloseHandle(m_hFile);
            return;
        }

        int* pTemp = (int*)m_pMapViewBegin;
        m_nDataCount = *pTemp;
        pTemp++;
        if(strcmp(m_strStructName, (char*)pTemp) != 0)
        {
            int iErr = ID_ERROR_NOT_CORRECT_DATATYPE;
            return;
        }
        m_pData = ((T*)pTemp) + 16;
        iErr = ID_SUCCESS;
        return;
    }


    void AddItem(int& iErr, const T* pItem)
    {
        assert(pItem);
        {
            int sizeOfT = sizeof(T);
            memcpy(&m_pData[m_nDataCount], pItem, sizeOfT);
        }
        int iWin32Err = GetLastError();
        m_nDataCount++;
        memcpy(m_pMapViewBegin, &m_nDataCount, 4);
        FlushViewOfFile(m_pMapViewBegin, 4);
        FlushViewOfFile((LPVOID)(m_pData+m_nDataCount), sizeof(T));
        if(m_nDataCount == m_n2-2)
        {
            UnmapViewOfFile(m_pMapViewBegin);

            m_n2 += 1024;
            CloseHandle(m_hMap);
            m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 68+sizeof(T)*m_n2, NULL);
            if (m_hMap == NULL)
            {
                iErr = ID_ERROR_CREATE_FILE_MAPPING_FAILED;
                CloseHandle(m_hFile);
                return;
            }

            m_pMapViewBegin = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 68+sizeof(T)*m_n2);
            if (m_pMapViewBegin == NULL)
            {
                int iErr = ID_ERROR_MAP_VIEW_OF_FILE_FAILED;
                CloseHandle(m_hMap);
                CloseHandle(m_hFile);
                return;
            }
        }
    }

protected:
    void CreateFileIfNotExist(int &iErr, TCHAR* pPath)
    {
        HANDLE hFile = CreateFile(pPath, GENERIC_READ | GENERIC_WRITE, 0, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            int iWin32Err = GetLastError();
            if(iWin32Err == 2)
            {
                hFile = CreateFile(pPath, GENERIC_READ | GENERIC_WRITE, 0, 
                    NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE)
                {
                    iErr = ID_ERROR_OPEN_FILE_FAILED;
                    return;
                }
                DWORD tempDWord;
                WriteFile(hFile, &m_nDataCount, 4, &tempDWord, NULL);
                WriteFile(hFile, m_strStructName, 64, &tempDWord, NULL);
                CloseHandle(hFile);
                iErr = ID_SUCCESS;
                return;
            }
            else
            {
                iErr = ID_ERROR_OPEN_FILE_FAILED;
                return;
            }
        }
        CloseHandle(hFile);
        iErr = ID_SUCCESS;
    }

    int GetDataCountFromFile(int &iErr, TCHAR* pPath)
    {
        HANDLE hFile = CreateFile(pPath, GENERIC_READ | GENERIC_WRITE, 0, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            iErr = ID_ERROR_OPEN_FILE_FAILED;
            return -1;
        }

        LARGE_INTEGER fileSize;
        ::GetFileSizeEx(hFile, &fileSize);

        HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 68, NULL);
        if (hMap == NULL)
        {
            iErr = ID_ERROR_CREATE_FILE_MAPPING_FAILED;
            CloseHandle(hFile);
            return -1;
        }

        LPVOID pMapViewBegin = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 68);
        if (pMapViewBegin == NULL)
        {
            int iErr = ID_ERROR_MAP_VIEW_OF_FILE_FAILED;
            CloseHandle(hMap);
            CloseHandle(hFile);
            return -1;
        }

        int* pTemp = (int*)pMapViewBegin;
        int nDataCount = *pTemp;
        pTemp++;
        if(strcmp(m_strStructName, (char*)pTemp) != 0)
        {
            int iErr = ID_ERROR_NOT_CORRECT_DATATYPE;
            return -1;
        }

        UnmapViewOfFile(pMapViewBegin);
        CloseHandle(hMap);
        CloseHandle(hFile);
        iErr = ID_SUCCESS;
        return nDataCount;
    }

protected:
    LPVOID m_pMapViewBegin;
    T* m_pData;
private:
    int m_nDataCount;
    char m_strStructName[64];
    HANDLE m_hFile;
    HANDLE m_hMap;
    int m_n2;
};

int main()
{
    structBase ss;
    MyFileMapManager<aaaaaaaa> manager;
    int iErr=0;
    TCHAR* aaa= _T("1.txt");
    manager.MapToFile(iErr, aaa);
    for(int i=0; i<1000000; i++)
    {
        aaaaaaaa newA;
        newA.m_a = 10000;
        newA.m_b = 10000;
        manager.AddItem(iErr, &newA);
    }
    return 0;
}