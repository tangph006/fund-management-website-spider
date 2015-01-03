#include "stdafx.h"
#include "CTPDemo.h"
#include "CTPDemoDlg.h"
#include "ErrorCode.h"
#include "TotalDataManager.h"
#include "DataStorage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCTPDemoApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CCTPDemoApp::CCTPDemoApp()
{
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CCTPDemoApp theApp;
HANDLE g_totalDataMutex = CreateMutex(NULL, FALSE, _T("g_totalData_Mutex"));

BOOL CCTPDemoApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();
    AfxEnableControlContainer();
    CShellManager *pShellManager = new CShellManager;
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    TotalDepthMarketDataManager totalData;
    DataStorage dataStorage;
    dataStorage.InitFromTotalData(&totalData);
    CMyMdSpi mdSpi(&totalData, &dataStorage);
    CCTPDemoDlg dlg(&mdSpi, NULL);
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
    }
    else if (nResponse == IDCANCEL)
    {
    }
    if (pShellManager != NULL)
    {
        delete pShellManager;
    }
    g_totalData.UnMapFromFile();
    return FALSE;
}

