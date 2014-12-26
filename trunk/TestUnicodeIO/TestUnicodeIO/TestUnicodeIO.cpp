#include "stdafx.h"
#include "TestUnicodeIO.h"
#include "TestUnicodeIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTestUnicodeIOApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CTestUnicodeIOApp::CTestUnicodeIOApp()
{
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CTestUnicodeIOApp theApp;

BOOL CTestUnicodeIOApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();
    AfxEnableControlContainer();
    CShellManager *pShellManager = new CShellManager;
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    CTestUnicodeIODlg dlg;
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

