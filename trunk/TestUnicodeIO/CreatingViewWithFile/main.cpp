//http://bbs.csdn.net/topics/80299016
#include "MyFileMapManager.h"
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
    MyFileMapManager<TempDataClass2> manager;

    TCHAR* strPath = _T("1.txt");
    DeleteFile(strPath);

    int iErr=0;
    manager.MapToFile(iErr, strPath);
    if(iErr != ID_SUCCESS)
        return -1;

    LARGE_INTEGER time_start;
    QueryPerformanceCounter(&time_start);
    for(int i=0; i<10000000; i++)
    {
        TempDataClass2 a;
        a.m_int1 = i;
        a.m_int2 = 2;
        manager.AddItem(iErr, &a);
    }
    LARGE_INTEGER time_end;
    QueryPerformanceCounter(&time_end);

    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    double nResult = (time_end.QuadPart-time_start.QuadPart)/(double)f.QuadPart;
    return 0;
}