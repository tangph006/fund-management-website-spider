#include "stdafx.h"
#include "CTPDemo.h"
#include "CTPDemoDlg.h"
#include "afxdialogex.h"
#include "MyClass.h"
#include "MyTradeClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCTPDemoDlg::CCTPDemoDlg(CWnd* pParent)
    : CDialogEx(CCTPDemoDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTPDemoDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 380;
    lpMMI->ptMinTrackSize.y = 360;
    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

int CCTPDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_tbarMain.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_tbarMain.LoadToolBar(IDR_STOP_START))
    {
        return -1;
    }

    return 0;
}

void CCTPDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
    DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
    DDX_Control(pDX, IDC_IPCTRL_IP, m_ipctrlIP);
    DDX_Control(pDX, IDC_EDIT_USERID, m_editUID);
}

BEGIN_MESSAGE_MAP(CCTPDemoDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_WM_QUERYDRAGICON()
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BUTTON1, &CCTPDemoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CCTPDemoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_listResult.SetParent(this);
    DWORD dwStyle = m_listResult.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES;
    m_listResult.SetExtendedStyle(dwStyle);
    InitListResultHeader();

    CRect rect;
    m_listResult.GetWindowRect(rect);
    ScreenToClient(rect);
    m_tbarMain.SetWindowPos(NULL, rect.left, rect.top-24, rect.Width(), 24, NULL);

    m_editPort.SetPrecision(0);
    m_editUID.SetPrecision(0);
    return TRUE;
}

void CCTPDemoDlg::InitListResultHeader()
{
    m_listResult.InsertColumn(0, _T("Column1"), LVCFMT_LEFT, 100);
    m_listResult.InsertColumn(1, _T("Column2"), LVCFMT_LEFT, 100);
    m_listResult.InsertColumn(2, _T("Column3"), LVCFMT_LEFT, 100);
    m_listResult.InsertColumn(3, _T("Column4"), LVCFMT_LEFT, 100);
    m_listResult.InsertColumn(4, _T("Time Elapsed"), LVCFMT_LEFT, 100);
}

void CCTPDemoDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
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
        CDialogEx::OnPaint();
    }
}

HCURSOR CCTPDemoDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CCTPDemoDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if(m_listResult.GetSafeHwnd() == NULL)
    {
        if(nType != SIZE_MINIMIZED)
        {
            m_oldCx = cx;
            m_oldCy = cy;
        }
        return;
    }
    LayoutControl(GetDlgItem(IDC_STATIC_IP), TopLeft, TopLeft, cx, cy);
    LayoutControl(GetDlgItem(IDC_STATIC_PORT), TopLeft, TopLeft, cx, cy);
    LayoutControl(GetDlgItem(IDOK), BottomRight, BottomRight, cx, cy);
    LayoutControl(GetDlgItem(IDCANCEL), BottomRight, BottomRight, cx, cy);
    LayoutControl(&m_ipctrlIP, TopLeft, TopLeft, cx, cy);
    LayoutControl(&m_editPort, TopLeft, TopLeft, cx, cy);
    LayoutControl(&m_listResult, TopLeft, BottomRight, cx, cy);
    LayoutControl(&m_tbarMain, TopLeft, TopRight, cx, cy);
    if(nType != SIZE_MINIMIZED)
    {
        m_oldCx = cx;
        m_oldCy = cy;
    }
    InvalidateRect(NULL);
}

void CCTPDemoDlg::LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy)
{
    CRect rcS;
    pCtrl->GetWindowRect(&rcS);
    ScreenToClient(&rcS);
    int deltaX = cx - m_oldCx;
    int deltaY = cy - m_oldCy;
    if(refTopLeft == TopLeft && refBottomRight == TopLeft)
    {
        pCtrl->MoveWindow(&rcS);
    }
    else if(refTopLeft == TopLeft && refBottomRight == TopRight)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top, rcS.Width()+deltaX, rcS.Height());
    }
    else if(refTopLeft == TopLeft && refBottomRight == BottomLeft)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top, rcS.Width(), rcS.Height()+deltaY);
    }
    else if(refTopLeft == TopLeft && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top, rcS.Width()+deltaX, rcS.Height()+deltaY);
    }
    else if(refTopLeft == TopRight && refBottomRight == TopRight)
    {
        pCtrl->MoveWindow(rcS.left+deltaX, rcS.top, rcS.Width(), rcS.Height());
    }
    else if(refTopLeft == TopRight && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left+deltaX, rcS.top, rcS.Width(), rcS.Height()+deltaY);
    }
    else if(refTopLeft == BottomLeft && refBottomRight == BottomLeft)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top+deltaY, rcS.Width(), rcS.Height());
    }
    else if(refTopLeft == BottomLeft && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top+deltaY, rcS.Width()+deltaX, rcS.Height());
    }
    else if(refTopLeft == BottomRight && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left+deltaX, rcS.top+deltaY, rcS.Width(), rcS.Height());
    }
}

BOOL CCTPDemoDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) {
        switch(pMsg->wParam)
        {
        case VK_RETURN:
            return TRUE;
        case VK_ESCAPE:
            return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}







void CCTPDemoDlg::OnBnClickedButton1()
{
//     char moudlePath[256] = {0};
//     ::GetCurrentDirectory(255, moudlePath);
//     // 创建实例
//     CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(moudlePath);
//     // 创建spi实例
//     CTraderApiSample* pSpi = new CTraderApiSample(pUserApi, "31000853", "80002", "123456", "cu1411");
//     // 注册spi实例
//     pUserApi->RegisterSpi(pSpi);
//     //订阅私有流
//     pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
//     //订阅公有流
//     pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
//     // 注册前置机
//     pUserApi->RegisterFront("tcp://10.253.117.107:13153");
//     pUserApi->RegisterNameServer("tcp://10.253.117.110:11000");
//     CThostFtdcFensUserInfoField FensUserInfo = {0};
//     strncpy(FensUserInfo.UserID, "201301", sizeof(FensUserInfo.UserID)-1);
//     FensUserInfo.LoginMode = 'E';
//     pUserApi->RegisterFensUserInfo(&FensUserInfo);
//     // 初始化
//     pUserApi->Init();
//     printf ("\npress return to quit...\n");
//     //pUserApi->Join();
//     pUserApi->Release();

    ///////////////////
    // 创建实例
    CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi("");
    // 创建spi实例
    CMdUserApiSample* pSpi = new CMdUserApiSample(pUserApi, "8000", "test", "123456");
    // 注册spi实例
    pUserApi->RegisterSpi(pSpi);
    // 注册前置机 
    //pUserApi->RegisterFront("tcp://10.253.117.107:13153");
    //pUserApi->RegisterFront("udp://10.253.117.107:20518");
    //pUserApi->RegisterNameServer("tcp://10.253.117.110:11000");
    pUserApi->RegisterNameServer("tcp://27.115.78.154:31213");
    CThostFtdcFensUserInfoField FensUserInfo = {0};
    strncpy(FensUserInfo.UserID, "201301", sizeof(FensUserInfo.UserID)-1);
    FensUserInfo.LoginMode = 'E';
    pUserApi->RegisterFensUserInfo(&FensUserInfo);
    // 初始化
    pUserApi->Init();
    // 等待信号
    //WaitForSingleObject(g_hEvent, INFINITE);
    // 初始化
    pUserApi->Release();
}
