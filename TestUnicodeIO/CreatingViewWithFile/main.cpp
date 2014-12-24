//http://bbs.csdn.net/topics/80299016
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include <assert.h>

#define BUFFSIZE 1024
#define FILE_MAP_START 0
TCHAR * lpcTheFile = TEXT("fmtest.txt");

class MyFileMapManager
{
public:
    MyFileMapManager(){}
    virtual ~MyFileMapManager(){}

    void MapFile(int& iErr, TCHAR* pPath, DWORD dwFileMapStart)
    {
        HANDLE hFile = CreateFile(lpcTheFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
        {
            iErr = ID_INVALID_HANDLE_VALUE;
            return;
        }

        SYSTEM_INFO SysInfo;
        GetSystemInfo(&SysInfo);
        DWORD dwSysGran = SysInfo.dwAllocationGranularity;
        DWORD dwFileMapStart1 = 0/*dwFileMapStart*/;
        _tprintf (TEXT("The file map view starts at %ld bytes into the file.\n"), dwFileMapStart1);
        DWORD dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFSIZE;
        _tprintf (TEXT("The file map view is %ld bytes large.\n"), dwMapViewSize);
        DWORD dwFileMapSize = FILE_MAP_START + BUFFSIZE;
        _tprintf (TEXT("The file mapping object is %ld bytes large.\n"), dwFileMapSize);
        int iViewDelta = FILE_MAP_START - dwFileMapStart1;
        _tprintf (TEXT("The data is %d bytes into the view.\n"), iViewDelta);
        DWORD dBytesWritten;
        for (int i=0; i<(int)dwSysGran; i++)
        {
            WriteFile (hFile, &i, sizeof (i), &dBytesWritten, NULL);
        }

        DWORD dwFileSize = GetFileSize(hFile,  NULL);
        _tprintf(TEXT("hFile size: %10d\n"), dwFileSize);

        HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwFileMapSize, NULL);
        if (hMapFile == NULL)
        {
            _tprintf(TEXT("hMapFile is NULL: last error: %d\n"), GetLastError());
            return;
        }

        LPVOID lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, dwFileMapStart1, dwMapViewSize);
        if (lpMapAddress == NULL)
        {
            _tprintf(TEXT("lpMapAddress is NULL: last error: %d\n"), GetLastError());
            return;
        }

        char* p = (char*)lpMapAddress;
        for(int i=0; i<100; i++)
        {
            *p = 'c';
            p++;
        }
        int iRet = FlushViewOfFile(lpMapAddress, 100);
        char * pData = (char *) lpMapAddress + iViewDelta;
        int iData = *(int *)pData;
        _tprintf (TEXT("The value at the pointer is %d,\nwhich %s one quarter of the desired file offset.\n"),
            iData, iData*4 == FILE_MAP_START ? TEXT("is") : TEXT("is not"));
        BOOL bRet = UnmapViewOfFile(lpMapAddress);
        bRet = CloseHandle(hMapFile);
        if(!bRet)
        {
            _tprintf(TEXT("Error %ld occurred closing the mapping object!\n"), GetLastError());
        }

        bRet = CloseHandle(hFile);
        if(!bRet)
        {
            _tprintf(TEXT("Error %ld occurred closing the file!\n"), GetLastError());
        }
        return;


    }
    int GetDataCount() const { return m_nDataCount; }
    void Increase(int iDelta=1) { m_nDataCount += iDelta; }

protected:
    int m_nDataCount;
    void* m_pData;
};

int main()
{
    HANDLE hFile = CreateFile(lpcTheFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("hFile is NULL\n"));
        _tprintf(TEXT("Target file is %s\n"),
            lpcTheFile);
        return 4;
    }

    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    DWORD dwSysGran = SysInfo.dwAllocationGranularity;
    DWORD dwFileMapStart = FILE_MAP_START;
    _tprintf (TEXT("The file map view starts at %ld bytes into the file.\n"), dwFileMapStart);
    DWORD dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFSIZE;
    _tprintf (TEXT("The file map view is %ld bytes large.\n"), dwMapViewSize);
    DWORD dwFileMapSize = FILE_MAP_START + BUFFSIZE;
    _tprintf (TEXT("The file mapping object is %ld bytes large.\n"), dwFileMapSize);
    int iViewDelta = FILE_MAP_START - dwFileMapStart;
    _tprintf (TEXT("The data is %d bytes into the view.\n"), iViewDelta);
    DWORD dBytesWritten;
    for (int i=0; i<(int)dwSysGran; i++)
    {
        WriteFile (hFile, &i, sizeof (i), &dBytesWritten, NULL);
    }

    _tprintf(TEXT("hFile size: %d\n"), GetFileSize(hFile,  NULL));

    HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwFileMapSize, NULL);
    if (hMapFile == NULL)
    {
        _tprintf(TEXT("hMapFile is NULL: last error: %d\n"), GetLastError());
        return (2);
    }

    LPVOID lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, dwFileMapStart, dwMapViewSize);
    if (lpMapAddress == NULL)
    {
        _tprintf(TEXT("lpMapAddress is NULL: last error: %d\n"), GetLastError());
        return 3;
    }

    char* p = (char*)lpMapAddress;
    for(int i=0; i<100; i++)
    {
        *p = 'c';
        p++;
    }
    int iRet = FlushViewOfFile(lpMapAddress, 100);
    char * pData = (char *) lpMapAddress + iViewDelta;
    int iData = *(int *)pData;
    _tprintf (TEXT("The value at the pointer is %d,\nwhich %s one quarter of the desired file offset.\n"),
        iData, iData*4 == FILE_MAP_START ? TEXT("is") : TEXT("is not"));
    BOOL bRet = UnmapViewOfFile(lpMapAddress);
    bRet = CloseHandle(hMapFile);
    if(!bRet)
    {
        _tprintf(TEXT("Error %ld occurred closing the mapping object!\n"), GetLastError());
    }

    bRet = CloseHandle(hFile);
    if(!bRet)
    {
        _tprintf(TEXT("Error %ld occurred closing the file!\n"), GetLastError());
    }
    return 0;
}