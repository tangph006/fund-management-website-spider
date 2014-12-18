#include "stdafx.h"
#include "SynSocketClient.h"
#include "SynSocketClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSynSocketClientApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CSynSocketClientApp::CSynSocketClientApp()
{
}


CSynSocketClientApp theApp;


BOOL CSynSocketClientApp::InitInstance()
{
	AfxEnableControlContainer();

	CSynSocketClientDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
