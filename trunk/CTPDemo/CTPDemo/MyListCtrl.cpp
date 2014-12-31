#include "StdAfx.h"
#include "MyListCtrl.h"


CMyListCtrl::CMyListCtrl(void)
{
}


CMyListCtrl::~CMyListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONUP()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

void CMyListCtrl::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    CRect headerRect;
    CDC MenDC;
    CBitmap MemMap;

    GetClientRect(&rect);
    GetDlgItem(0)->GetWindowRect(&headerRect);
    MenDC.CreateCompatibleDC(&dc);
    MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    MenDC.SelectObject(&MemMap);
    MenDC.FillSolidRect(&rect, RGB(255,255,255));

    // default paint
    DefWindowProc(WM_PAINT,(WPARAM)MenDC.m_hDC,(LPARAM)0);
    dc.BitBlt(0, headerRect.Height(), rect.Width(), rect.Height(), &MenDC, 0, headerRect.Height(), SRCCOPY);
    MenDC.DeleteDC();
    MemMap.DeleteObject();
}

BOOL CMyListCtrl::OnEraseBkgnd(CDC* pDC)
{
    return FALSE;
    //return CListCtrl::OnEraseBkgnd(pDC);
}

void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVCUSTOMDRAW lpnmcd = (LPNMLVCUSTOMDRAW) pNMHDR;
    switch(lpnmcd ->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        {
            *pResult =  CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
            return;
        }
        break;
    case CDDS_ITEMPREPAINT:
        {
            *pResult =  CDRF_NOTIFYSUBITEMDRAW;
            return;
        }
        break;
    case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
        {
            if (lpnmcd->nmcd.dwItemSpec >= 0 && lpnmcd->iSubItem >= 0)
            {
                DrawSubItem(lpnmcd);
                *pResult =  CDRF_SKIPDEFAULT;
                return;
            }
        }
        break;
    case CDDS_POSTPAINT:
        {
            //DrawRemainSpace(lpnmcd);
            *pResult =  CDRF_DODEFAULT;
            return;
        }
        break;
    default:
        break;
    }
    *pResult = 0;
}

void CMyListCtrl::DrawRemainSpace(LPNMLVCUSTOMDRAW lpnmcd)
{
    int nTop = lpnmcd->nmcd.rc.top;
    int nCount = GetItemCount();
    if (nCount > 0)
    {
        CRect rcItem;
        GetItemRect(nCount - 1, &rcItem, LVIR_LABEL);
        nTop = rcItem.bottom;
    }
    CRect rectClient;
    GetClientRect(&rectClient);
    if (nTop < lpnmcd->nmcd.rc.bottom)
    {
        CRect rcRemain = lpnmcd->nmcd.rc;
        rcRemain.top = nTop;
        rcRemain.right = rectClient.right;
        CDC dc;
        dc.Attach(lpnmcd->nmcd.hdc);
        CBrush brush;
        brush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
        dc.FillRect(&rcRemain, &brush);
        dc.Detach();
    }
}

void CMyListCtrl::DrawSubItem(LPNMLVCUSTOMDRAW lpnmcd)
{
    CRect rSubItem;
    int iItem = lpnmcd->nmcd.dwItemSpec;
    int iSubItem = lpnmcd->iSubItem;
    GetSubItemRect(iItem, iSubItem, LVIR_LABEL,rSubItem);
    if(iSubItem == 0 && rSubItem.left > 0)
        rSubItem.left = 0;
    rSubItem.NormalizeRect();

    CDC dc;
    dc.Attach(lpnmcd->nmcd.hdc);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

    CWnd* pParentWnd = GetParent();
    if(pParentWnd != NULL)
        dc.SelectObject(pParentWnd->GetFont());
    else
        dc.SelectObject(::GetStockObject(SYSTEM_FONT));

    bool bSelected = false;
    if(GetItemState(iItem, LVIS_SELECTED))
        bSelected = true;

    bool bFocus = false;
    CWnd *pWndFocus = GetFocus();
    if (pWndFocus == this || IsChild(pWndFocus))
        bFocus = true;

    int nSave = dc.SaveDC();
    CBrush brush;
    if(bSelected)
    {
        if(bFocus)
        {
            brush.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
            dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
        }
        else
        {
            brush.CreateSolidBrush(RGB(206, 206, 206));
        }
    }
    else
    {
        brush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
    }
    dc.FillRect(&rSubItem, &brush); // draw background
    CString strText = GetItemText(iItem, iSubItem);
    GetSubItemRect(iItem, iSubItem, LVIR_LABEL,rSubItem);
    if(iSubItem != 0)
        rSubItem.left += 4;
    rSubItem.right -= 2;
    // draw text
    LVCOLUMN columnInfo;
    columnInfo.mask = LVCF_FMT;
    GetColumn(iSubItem, &columnInfo);
    UINT pos = DT_LEFT;
    if(columnInfo.fmt & LVCFMT_CENTER)
        pos = DT_CENTER;
    else if(columnInfo.fmt & LVCFMT_RIGHT)
        pos = DT_RIGHT;
    dc.DrawText(strText, strText.GetLength(), &rSubItem, DT_SINGLELINE | pos | DT_VCENTER | DT_END_ELLIPSIS);
    dc.RestoreDC(nSave);
    dc.Detach();
}

void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    int iItem = HitTest(point, NULL);
    CListCtrl::OnLButtonUp(nFlags, point);
}

void CMyListCtrl::InvalidateItemRect(int nItem)
{
    CRect rcClient;
    GetClientRect(&rcClient);
    CRect rcItem;
    GetItemRect(nItem, &rcItem, LVIR_BOUNDS);
    rcItem.left = rcClient.left;
    rcItem.right = rcClient.right;
    InvalidateRect(&rcItem, FALSE);
}

void CMyListCtrl::Init()
{
    CFont* pFont = NULL;
    CWnd* pWnd = GetParent();
    if(pWnd)
        pFont = pWnd->GetFont();
    else
        pFont = CFont::FromHandle((HFONT)::GetStockObject(SYSTEM_FONT));

    SetFont(pFont);
    pFont->Detach();
}

void CMyListCtrl::PreSubclassWindow()
{
    Init();
    CListCtrl::PreSubclassWindow();
}

