#include "stdafx.h"
#include "TestUnicodeIO.h"
#include "TabSubView5.h"
#include <algorithm>

IMPLEMENT_DYNAMIC(CTabSubView5, CDialog)

CTabSubView5::CTabSubView5(CWnd* pParent) : CDialog(CTabSubView5::IDD, pParent)
{

}

CTabSubView5::~CTabSubView5()
{
}

BEGIN_MESSAGE_MAP(CTabSubView5, CDialog)
    ON_WM_SIZE()
    ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_INTPP, OnLvnGetdispinfoListIntPP)
END_MESSAGE_MAP()

void CTabSubView5::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_INTPP, m_listIntPP);
}


void CTabSubView5::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if(m_listIntPP.GetSafeHwnd() == NULL)
    {
        if(nType != SIZE_MINIMIZED)
        {
            m_oldCx = cx;
            m_oldCy = cy;
        }
        return;
    }
    LayoutControl(&m_listIntPP, TopLeft, BottomRight, cx, cy);
    if(nType != SIZE_MINIMIZED)
    {
        m_oldCx = cx;
        m_oldCy = cy;
    }
    InvalidateRect(NULL);
}

BOOL CTabSubView5::OnInitDialog()
{
    CDialog::OnInitDialog();
    InitData();

    DWORD dwStyle = m_listIntPP.GetExtendedStyle();
    m_listIntPP.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
    LV_COLUMN lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;
    lvc.fmt = LVCFMT_CENTER;
    lvc.pszText = _T("");
    lvc.cx = 20;
    m_listIntPP.InsertColumn(0, &lvc);
    lvc.cx = 66;
    int nCol = sizeof(m_array[0])/sizeof(m_array[0][0]);
    CString strTitle;
    for(int i=0; i<nCol; i++)
    {
        strTitle.Format(_T("%d"), i);
        lvc.pszText = strTitle.GetBuffer();
        m_listIntPP.InsertColumn(i+1, &lvc);
    }
    return TRUE;
}

BOOL CTabSubView5::PreTranslateMessage(MSG* pMsg)
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

void CTabSubView5::LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy)
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

void CTabSubView5::InitData()
{
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    srand(sysTime.wMilliseconds%100);

    memset(m_array, sizeof(m_array), 0);
    int nRow = sizeof(m_array)/sizeof(m_array[0]);
    int nCol = sizeof(m_array[0])/sizeof(m_array[0][0]);
    for(int i=0; i<nRow; i++)
    {
        for(int j=0; j<nCol; j++)
        {
            m_array[i][j] = rand()%nRow*nCol;
        }
    }
}

void CTabSubView5::OnLvnGetdispinfoListIntPP(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    int iItem = pDispInfo->item.iItem;
    int iSubItem = pDispInfo->item.iSubItem;
    if(pDispInfo->item.mask & LVIF_TEXT)
    {
        CString strItem;
        strItem.Format(_T("%d"), m_array[iItem][iSubItem]);
        lstrcpy(pDispInfo->item.pszText, strItem);
    }

    if(pDispInfo->item.mask & LVIF_IMAGE)
    {

    }
    *pResult = 0;
}
