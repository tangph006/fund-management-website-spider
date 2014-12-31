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

    m_mdSpi.m_observers.push_back(this->GetSafeHwnd());

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

    MoveWindow(0, 0, 660, 200);

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
    if(pMsg->message == WM_KEYDOWN)
    {
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
    int iErr = ID_SUCCESS;
    m_mdSpi.StartSubscribe(iErr);
}

void CCTPDemoDlg::OnButtonStop()
{
    DisableAllBtns();
    GetDlgItem(IDC_BTN_GO)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
    int iErr = ID_SUCCESS;
    m_mdSpi.StopSubscribe(iErr);
}


void CCTPDemoDlg::OnButtonLogin()
{
    //m_mdSpi.ClearMyParts();
    CString strItem;
    GetDlgItemText(IDC_EDIT_BROKERID, strItem);
    m_mdSpi.SetBrokerID(strItem.GetBuffer());
    GetDlgItemText(IDC_EDIT_USERID, strItem);
    m_mdSpi.SetInvestorID(strItem.GetBuffer());
    GetDlgItemText(IDC_EDIT_PASSWORD, strItem);
    m_mdSpi.SetPassword(strItem.GetBuffer());
    TThostFtdcInstrumentIDType iID;
    GetDlgItemText(IDC_EDIT_CODE, iID, sizeof(iID)-1);
    int iErr = ID_SUCCESS;
    m_mdSpi.AddIntrusmentType(iErr, std::string(iID));
    if(iErr != ID_SUCCESS)
        return;

    CString strIP, strPort;
    BYTE b1,b2,b3,b4;
    m_ipctrlIP.GetAddress(b1,b2,b3,b4);
    strIP.Format(_T("%u.%u.%u.%u"), b1, b2, b3, b4);
    m_editPort.GetWindowText(strPort);
    CString strAddr1;
    strAddr1.Format(_T("tcp://%s:%s"), strIP, strPort);
    m_mdSpi.SetAddr1(strAddr1.GetBuffer());
    CString strAddr2;
    strAddr2.Format(_T("tcp://%s:%s"), strIP, strPort);
    m_mdSpi.SetAddr2(strAddr2.GetBuffer());
    m_mdSpi.Login(iErr);
    if(iErr != ID_SUCCESS)
        return;
}

LRESULT CCTPDemoDlg::DefWindowProc(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
    case WM_OnFrontConnected:
        AppendMsg(_T("WM_OnFrontConnected"));
        break;
    case WM_OnFrontDisconnected:
        AppendMsg(_T("WM_OnFrontDisconnected"));
        break;
    case WM_OnHeartBeatWarning:
        AppendMsg(_T("WM_OnHeartBeatWarning"));
        break;
    case WM_OnRspUserLogin:
        {
            AppendMsg(_T("WM_OnRspUserLogin"));
            EnableUsernameCtrls(FALSE);
            DisableAllBtns();
            GetDlgItem(IDC_BTN_GO)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
        }
        break;
    case WM_OnRspUserLogout:
        {
            AppendMsg(_T("WM_OnRspUserLogout"));
            EnableUsernameCtrls(TRUE);
            DisableAllBtns();
            GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
        }
        break;
    case WM_OnRspError:
        {
            CThostFtdcRspInfoField *pRspInfo = (CThostFtdcRspInfoField*)wParam;
            CString strMsg;
            strMsg.Format(_T("WM_OnRspError: id: %d, msg: %s."), pRspInfo->ErrorID, pRspInfo->ErrorMsg);
            AppendMsg(strMsg);
            delete pRspInfo;
        }
        break;
    case WM_OnRspSubMarketData:
        {
            AppendMsg(_T("WM_OnRspSubMarketData"));
            DisableAllBtns();
            GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
        }
        break;
    case WM_OnRspUnSubMarketData:
        {
            AppendMsg(_T("WM_OnRspUnSubMarketData"));
            DisableAllBtns();
            GetDlgItem(IDC_BTN_GO)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
        }
        break;
    case WM_OnRspSubForQuoteRsp:
        AppendMsg(_T("WM_OnRspSubForQuoteRsp"));
        break;
    case WM_OnRspUnSubForQuoteRsp:
        AppendMsg(_T("WM_OnRspUnSubForQuoteRsp"));
        break;
    case WM_OnRtnDepthMarketData:
        {
            MyOnRtnDepthMarketData((int)wParam);
        }
        break;
    case WM_OnRtnForQuoteRsp:
        AppendMsg(_T("WM_OnRtnForQuoteRsp"));
        break;
    default:
        break;
    }
    return CDialog::DefWindowProc(Msg, wParam, lParam);
}

void CCTPDemoDlg::OnBnClickedBtnLogout()
{
    int iErr = ID_SUCCESS;
    m_mdSpi.StopSubscribe(iErr);
    if(iErr != ID_SUCCESS)
        return;
    m_mdSpi.Logout(iErr);
    if(iErr != ID_SUCCESS)
        return;
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

void CCTPDemoDlg::AppendMsg(CString strMsg)
{
    m_vecMsg.push_back(strMsg);
    int nCount = (int)m_vecMsg.size();
    int minPos, maxPos, curPos;
    curPos = m_listResult.GetScrollPos(SB_VERT);
    m_listResult.GetScrollRange(SB_VERT, &minPos, &maxPos);
    m_listResult.SetItemCount(nCount);
    if((maxPos-minPos) > 0 &&
        (float)((maxPos-curPos)/(float)(maxPos-minPos)) < 0.05f)
    {
        m_listResult.EnsureVisible(nCount-1, TRUE);
    }
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

void CCTPDemoDlg::MyOnRtnDepthMarketData(int nDataCount)
{
    extern MyFileMapper<CThostFtdcDepthMarketDataField> g_totalData;
    extern HANDLE g_totalDataMutex;
    int iErr = ID_SUCCESS;
    WaitForSingleObject(g_totalDataMutex, INFINITE);
    CThostFtdcDepthMarketDataField* pMarketData = g_totalData.GetDataByIndex(nDataCount-1);
    CString strMsg;
    strMsg.Format(_T("WM_OnRtnDepthMarketData: %s, %.2f, %.2f"), 
        pMarketData->ActionDay,
        pMarketData->AskPrice1,
        pMarketData->BidPrice1);
    AppendMsg(strMsg);
    ReleaseMutex(g_totalDataMutex);
}
