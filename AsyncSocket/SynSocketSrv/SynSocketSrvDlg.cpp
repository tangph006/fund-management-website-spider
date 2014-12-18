#include "stdafx.h"
#include "SynSocketSrv.h"
#include "SynSocketSrvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSynSocketSrvDlg::CSynSocketSrvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynSocketSrvDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSynSocketSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSynSocketSrvDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET_EVENT, OnSocketEvent) 
	ON_BN_CLICKED(IDC_BTN_SEND, OnButtonSend)
END_MESSAGE_MAP()


BOOL CSynSocketSrvDlg::OnInitDialog()
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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitSocket();
	return TRUE;
}

void CSynSocketSrvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

void CSynSocketSrvDlg::OnPaint() 
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

HCURSOR CSynSocketSrvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSynSocketSrvDlg::InitSocket()
{  
	WSADATA wsa ;
	::WSAStartup(MAKEWORD(2, 2), &wsa) ;
	m_socket = ::socket(AF_INET, SOCK_STREAM, 0) ;  
	::WSAAsyncSelect(m_socket, this->m_hWnd, WM_SOCKET_EVENT, FD_ACCEPT|FD_READ) ;
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888) ;
	bind(m_socket, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	listen(m_socket, 2) ;
}

LRESULT CSynSocketSrvDlg::OnSocketEvent(WPARAM wParam ,LPARAM lParam) 
{
	SOCKET sock = (SOCKET)wParam;
	CString strContent;
	if(WSAGETSELECTERROR(lParam))
	{
		closesocket(sock);
		return -1;
	}
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
		{
			SOCKET client = accept(sock, NULL, NULL);
			WSAAsyncSelect(client, m_hWnd, WM_SOCKET_EVENT, FD_READ|FD_WRITE|FD_CLOSE);
			GetDlgItemText(IDC_EDIT_LISTALL, strContent);
			if(!strContent.IsEmpty())
			{
				strContent+="\r\n";
			}			
			strContent+="New client connected";
			SetDlgItemText(IDC_EDIT_LISTALL, strContent);
		}
		break;
	case FD_WRITE:
		{
		}
		break;
	case FD_READ:
		{
			char szText[1024] = {0};
			if(recv(sock, szText, 1024, 0) == -1)
			{
				closesocket(sock);
			}
			else
			{				
				GetDlgItemText(IDC_EDIT_LISTALL, strContent);
				if(!strContent.IsEmpty())
				{
					strContent+="\r\n";
				}
				CString strTemp;
				strTemp.Format("%s", szText);
				strContent += strTemp;
				SetDlgItemText(IDC_EDIT_LISTALL, strContent);
			}
		}
		break;
	case FD_CLOSE:
		{
			closesocket(sock);
			GetDlgItemText(IDC_EDIT_LISTALL, strContent);
			if(!strContent.IsEmpty())
			{
				strContent += "\r\n";
			}			
			strContent += "One client disconnected.";
			SetDlgItemText(IDC_EDIT_LISTALL, strContent);
		}
		break;
	}
	return 0;
}

void CSynSocketSrvDlg::OnButtonSend() 
{
	CString str;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(str) ;
	int len = str.GetLength() ;
	send(m_clientSocket[0], str.GetBuffer(len), len, 0) ;
}
