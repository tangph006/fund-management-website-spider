//http://bbs.csdn.net/topics/80299016
#include "MyFileMapManager.h"

#define BUFFSIZE 1024
#define FILE_MAP_START 64
TCHAR * lpcTheFile = TEXT("fmtest.txt");
const static short initItemCount = 2048;


class TempDataClass
{
public:
    int m_a;
    int m_b;
public:
    TempDataClass(void){}
    ~TempDataClass(void){}
};

int main()
{
    MyFileMapManager<TempDataClass> manager;
    int iErr=0;
    TCHAR* aaa= _T("1.txt");
    manager.MapToFile(iErr, aaa);
    for(int i=0; i<1000000; i++)
    {
        TempDataClass newA;
        newA.m_a = 10000;
        newA.m_b = 10000;
        manager.AddItem(iErr, &newA);
        if(iErr != ID_SUCCESS)
        {
            int i=0;
            i=0;
        }
    }
    return 0;
}