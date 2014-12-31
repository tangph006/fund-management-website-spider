#include "stdafx.h"
#include "CTPDemo.h"
#include "CTPDemoDlg.h"

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
MyFileMapManager<CThostFtdcDepthMarketDataField> g_totalData;
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

    CCTPDemoDlg dlg;
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
    return FALSE;
}

