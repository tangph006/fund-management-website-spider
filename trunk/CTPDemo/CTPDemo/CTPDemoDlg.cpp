#include "stdafx.h"
#include "CTPDemo.h"
#include "CTPDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCTPDemoDlg::CCTPDemoDlg(CWnd* pParent)
    : CDialogEx(CCTPDemoDlg::IDD, pParent)
    , CThostFtdcMdSpi()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    ClearMyParts();
    m_vecMsg.clear();
}

void CCTPDemoDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 380;
    lpMMI->ptMinTrackSize.y = 200;
    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

int CCTPDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_toolbar.LoadToolBar(IDR_STOP_START))
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
    ON_BN_CLICKED(IDC_BTN_GO, &CCTPDemoDlg::OnButtonStart)
    ON_BN_CLICKED(IDC_BTN_STOP, &CCTPDemoDlg::OnButtonStop)
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CCTPDemoDlg::OnButtonLogin)
    ON_BN_CLICKED(IDC_BTN_LOGOUT, &CCTPDemoDlg::OnBnClickedBtnLogout)
    ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RESULT, &CCTPDemoDlg::OnLvnGetdispinfoListResult)
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
    m_toolbar.SetWindowPos(NULL, rect.left, rect.top-24, rect.Width(), 24, NULL);

    m_editPort.SetPrecision(0);
    m_editUID.SetPrecision(0);
    m_toolbar.ShowWindow(FALSE);

    m_ipctrlIP.SetAddress(27, 115, 78, 154);
    m_editPort.SetWindowText(_T("31213"));
    SetDlgItemText(IDC_EDIT_BROKERID, _T("8000"));
    SetDlgItemText(IDC_EDIT_USERID, _T("test"));
    SetDlgItemText(IDC_EDIT_PASSWORD, _T("123456"));
    SetDlgItemText(IDC_EDIT_CODE, _T("IF1501"));

    DisableAllBtns();
    GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);

    MoveWindow(0, 0, 380, 200);

    return TRUE;
}

void CCTPDemoDlg::InitListResultHeader()
{
    m_listResult.InsertColumn(0, _T("Column1"), LVCFMT_LEFT, 360);
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
    LayoutControl(&m_toolbar, TopLeft, TopRight, cx, cy);
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

void CCTPDemoDlg::OnButtonStart()
{
    DisableAllBtns();
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);

    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    CString strFile;
    strFile.Format(_T("%4d-%2d-%2d.txt"), sysTime.wYear, sysTime.wMonth, sysTime.wDay);
    int iErr = ID_SUCCESS;
    m_data.MapToFile(iErr, strFile.GetBuffer());
    StartSubscribe();
}

void CCTPDemoDlg::OnButtonStop()
{
    DisableAllBtns();
    GetDlgItem(IDC_BTN_GO)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
    StopSubscribe();
    m_data.UnMapFromFile();
}


void CCTPDemoDlg::OnButtonLogin()
{
    ClearMyParts();
    CString strItem;
    GetDlgItemText(IDC_EDIT_BROKERID, strItem);
    SetBrokerID(strItem.GetBuffer());
    GetDlgItemText(IDC_EDIT_USERID, strItem);
    SetInvestorID(strItem.GetBuffer());
    GetDlgItemText(IDC_EDIT_PASSWORD, strItem);
    SetPassword(strItem.GetBuffer());
    GetDlgItemText(IDC_EDIT_CODE, strItem);
    m_vInstrumentID.clear();
    m_vInstrumentID.push_back(strItem);
//     m_vInstrumentID.push_back(_T("CU1501"));
//     m_vInstrumentID.push_back(_T("TA1501"));
//     m_vInstrumentID.push_back(_T("WT1501"));

    CString strIP, strPort;
    BYTE b1,b2,b3,b4;
    m_ipctrlIP.GetAddress(b1,b2,b3,b4);
    strIP.Format(_T("%u.%u.%u.%u"), b1, b2, b3, b4);
    m_editPort.GetWindowText(strPort);
    m_addr1.Format(_T("tcp://%s:%s"), strIP, strPort);
    m_addr2.Format(_T("tcp://%s:%s"), strIP, strPort);
    Login();
}

void CCTPDemoDlg::OnBnClickedBtnLogout()
{
    StopSubscribe();
    Logout();
    EnableUsernameCtrls(TRUE);
}

void CCTPDemoDlg::DisableAllBtns()
{
    GetDlgItem(IDC_BTN_GO)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
}

void CCTPDemoDlg::EnableUsernameCtrls(BOOL bEnable)
{
    GetDlgItem(IDC_STATIC_IP)->EnableWindow(bEnable);
    GetDlgItem(IDC_IPCTRL_IP)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_PORT)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_PORT)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_BROKERID)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_BROKERID)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_USERID)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_USERID)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_PASSWORD)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(bEnable);
}

/////////////////////////////////////////////engine start/////////////////////////////////////////////

void CCTPDemoDlg::OnFrontConnected()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, m_brokerID);
    strcpy(req.UserID, m_investorID);
    strcpy(req.Password, m_password);
    m_pMdApi->ReqUserLogin(&req, m_nRequestID);
    m_nRequestID++;
    AppendMsg(_T("OnFrontConnected()"));
}

void CCTPDemoDlg::OnFrontDisconnected(int nReason)
{
    AppendMsg(_T("OnFrontDisconnected()"));
}

void CCTPDemoDlg::OnHeartBeatWarning(int nTimeElapse)
{
    AppendMsg(_T("OnHeartBeatWarning()"));
}

void CCTPDemoDlg::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    AppendMsg(_T("OnRspUserLogin()"));
    if(bIsLast && pRspInfo->ErrorID==0)
    {
        EnableUsernameCtrls(FALSE);
        DisableAllBtns();
        GetDlgItem(IDC_BTN_GO)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
    }
}

void CCTPDemoDlg::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    AppendMsg(_T("OnRspUserLogout()"));
    if(bIsLast && pRspInfo->ErrorID==0)
    {
        EnableUsernameCtrls(TRUE);
        DisableAllBtns();
        GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
    }
}

void CCTPDemoDlg::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CString strMsg;
    strMsg.Format(_T("OnRspError(), code: %d, message: %s"), pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    AppendMsg(strMsg);
}

void CCTPDemoDlg::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    AppendMsg(_T("OnRspSubMarketData()"));
    if(bIsLast && pRspInfo->ErrorID==0)
    {
        DisableAllBtns();
        GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
    }
}

void CCTPDemoDlg::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    AppendMsg(_T("OnRspUnSubMarketData()"));
    if(bIsLast && pRspInfo->ErrorID==0)
    {
        DisableAllBtns();
        GetDlgItem(IDC_BTN_GO)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
    }
}

void CCTPDemoDlg::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    AppendMsg(_T("OnRspSubForQuoteRsp()"));
}

void CCTPDemoDlg::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    AppendMsg(_T("OnRspUnSubForQuoteRsp()"));
}

void CCTPDemoDlg::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    AppendMsg(_T("OnRtnDepthMarketData()"));
    int iErr = ID_SUCCESS;
    m_data.AddItem(iErr, pDepthMarketData);
}

void CCTPDemoDlg::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
    AppendMsg(_T("OnRtnForQuoteRsp()"));
}

void CCTPDemoDlg::StartSubscribe()
{
    size_t nCount = m_vInstrumentID.size();
    char** ppInstrumentID = new char*[nCount];
    for(size_t i=0; i<nCount; i++)
    {
        CString str = m_vInstrumentID.at(i);
        ppInstrumentID[i] = new char[str.GetLength()+1];
        strcpy(ppInstrumentID[i], str.GetBuffer());
        ppInstrumentID[i][str.GetLength()] = 0;
    }

    m_pMdApi->UnSubscribeMarketData(ppInstrumentID, nCount);
    m_pMdApi->SubscribeMarketData(ppInstrumentID, nCount);
}

void CCTPDemoDlg::StopSubscribe()
{
    size_t nCount = m_vInstrumentID.size();
    char** ppInstrumentID = new char*[nCount];
    for(size_t i=0; i<nCount; i++)
    {
        CString str = m_vInstrumentID.at(i);
        ppInstrumentID[i] = new char[str.GetLength()+1];
        strcpy(ppInstrumentID[i], str.GetBuffer());
        ppInstrumentID[i][str.GetLength()] = 0;
    }
    m_pMdApi->UnSubscribeMarketData(ppInstrumentID, nCount);
}

void CCTPDemoDlg::Login()
{
    m_pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");
    m_pMdApi->RegisterSpi(this);
    m_pMdApi->RegisterFront(m_addr1.GetBuffer());
    m_pMdApi->RegisterFront(m_addr2.GetBuffer());
    m_pMdApi->Init();
}

void CCTPDemoDlg::Logout()
{
    CThostFtdcUserLogoutField logout;
    memset(&logout, 0, sizeof(logout));
    strcpy(logout.BrokerID, m_brokerID);
    strcpy(logout.UserID, m_investorID);
    m_pMdApi->ReqUserLogout(&logout, m_nRequestID);
    m_nRequestID++;
}

void CCTPDemoDlg::ClearMyParts()
{
    memset(m_brokerID, 0, sizeof(m_brokerID));
    memset(m_investorID, 0, sizeof(m_investorID));
    memset(m_password, 0, sizeof(m_password));
    m_vInstrumentID.clear();
}

void CCTPDemoDlg::AppendMsg(CString strMsg)
{
    m_vecMsg.push_back(strMsg);
    m_listResult.SetItemCount(m_vecMsg.size());
    m_listResult.EnsureVisible(m_vecMsg.size()-1, FALSE);
}

void CCTPDemoDlg::OnLvnGetdispinfoListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    int nItem = pDispInfo->item.iItem;
    int nSubItem = pDispInfo->item.iSubItem;
    if(pDispInfo->item.mask & LVIF_TEXT)
    {
        assert(nSubItem == 0);
        lstrcpy(pDispInfo->item.pszText, m_vecMsg[nItem]);
    }
    *pResult = 0;
}
