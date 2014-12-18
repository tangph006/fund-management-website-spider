#include "stdafx.h"
#include "SynSocketClient.h"
#include "SynSocketClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CSynSocketClientDlg::CSynSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSynSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSynSocketClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_MESSAGE(WM_SOCKET,OnSocket)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
END_MESSAGE_MAP()

BOOL CSynSocketClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	InitSocket();
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText("8888");
	GetDlgItem(IDC_ADDRESS)->SetWindowText("127.0.0.1");
	return TRUE;
}

void CSynSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSynSocketClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSynSocketClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CSynSocketClientDlg::OnBtnConnect() 
{

	CString ip,p;
	int port ;
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(p);
	port = atoi(p);
	GetDlgItem(IDC_ADDRESS)->GetWindowText(ip);
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	::connect(m_socket, (SOCKADDR*)&addr, sizeof(SOCKADDR));
}

void CSynSocketClientDlg::InitSocket()
{
	WSADATA  wsa  ;
	::WSAStartup(MAKEWORD(2, 2), &wsa) ;
	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	::WSAAsyncSelect(m_socket, this->m_hWnd, WM_SOCKET, FD_READ);
}

LRESULT CSynSocketClientDlg::OnSocket(WPARAM wParam ,LPARAM lParam) 
{
	char buf[100];
	memset(buf, 0, 100) ;
	recv(m_socket, buf, 100, 0);
	CString s = buf;
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(s); 
	return 0;
}

void CSynSocketClientDlg::OnBtnSend() 
{   
	CString s = "";
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(s);
	int len = s.GetLength();
	if(SOCKET_ERROR == send(m_socket, s.GetBuffer(len), len, 0))
	{
		MessageBox("Error");
	}
}
