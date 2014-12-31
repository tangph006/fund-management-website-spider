//http://bbs.csdn.net/topics/80299016
#include "MyFileMapManager.h"
#include "ThostFtdcUserApiStruct.h"
#include <stdio.h>
class TempDataClass
{
public:
    TCHAR m_str1[16];
    TCHAR m_str2[16];
public:
    TempDataClass(){ memset(m_str1, 0, 16); memset(m_str2, 0, 16); }
    ~TempDataClass(){}
};

class TempDataClass2
{
public:
    TCHAR m_int1;
    TCHAR m_int2;
public:
    TempDataClass2() : m_int1(0), m_int2(0){}
    ~TempDataClass2(){}
};

int main()
{
    TCHAR* strPath = _T("1.dat");
    HANDLE hFile = CreateFile(strPath, GENERIC_READ | GENERIC_WRITE, 0, 
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD sizeToAlloc = 1024*1024*64*8;
    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeToAlloc, NULL);
    if (hMap == NULL)
    {
        CloseHandle(hFile);
        hFile = NULL;
        return 0;
    }
    int iWinErr = GetLastError();
    LPVOID pMapViewBegin1 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin2 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin3 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin4 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin5 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin6 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin7 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    LPVOID pMapViewBegin8 = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeToAlloc);
    if (pMapViewBegin1 == NULL)
    {
        iWinErr = GetLastError();
        CloseHandle(hMap);
        hMap = NULL;
        CloseHandle(hFile);
        hFile = NULL;
        return 0;
    }

    UnmapViewOfFile(pMapViewBegin1);
    CloseHandle(hMap);
    hMap = NULL;
    CloseHandle(hFile);
    hFile = NULL;
    return 0;
}

int main12121()
{
    MyFileMapManager<CThostFtdcDepthMarketDataField> manager;

    TCHAR* strPath = _T("1.dat");
    DeleteFile(strPath);

    int iErr=0;
    manager.MapToFile(iErr, strPath);
    if(iErr != ID_SUCCESS)
        return -1;

    LARGE_INTEGER time_start;
    QueryPerformanceCounter(&time_start);
    for(int i=0; i<10000000; i++)
    {
        CThostFtdcDepthMarketDataField a;
        memset(&a, 0, sizeof(a));
        manager.AddItem(iErr, &a);
        if(iErr != ID_SUCCESS)
        {
            printf(_T("error occured: %d"), iErr);
            getchar();
            break;
        }
    }
    LARGE_INTEGER time_end;
    QueryPerformanceCounter(&time_end);

    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    double nResult = (time_end.QuadPart-time_start.QuadPart)/(double)f.QuadPart;
    return 0;
}