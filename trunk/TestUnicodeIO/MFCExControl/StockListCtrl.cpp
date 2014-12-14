#include "stdafx.h"
#include "StockListCtrl.h"

IMPLEMENT_DYNAMIC(CStockListCtrl, CListCtrl)

CStockListCtrl::CStockListCtrl()
{
}

CStockListCtrl::~CStockListCtrl()
{
}


BEGIN_MESSAGE_MAP(CStockListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
    ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnLvnItemchanged)
END_MESSAGE_MAP()

void CStockListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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
    default:
        break;
    }
    *pResult = 0;
}

void CStockListCtrl::DrawSubItem(LPNMLVCUSTOMDRAW lpnmcd)
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
        brush.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
        dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
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

BOOL CStockListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if (pNMLV->uChanged & LVIF_STATE)
    {
        if (((pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED)) 
            || ((pNMLV->uOldState & LVIS_STATEIMAGEMASK) != (pNMLV->uNewState & LVIS_STATEIMAGEMASK)))
        {
            InvalidateItemRect(pNMLV->iItem);
        }
    }
    *pResult = 0;
    return FALSE;
}

void CStockListCtrl::InvalidateItemRect(int nItem)
{
    CRect rcClient;
    GetClientRect(&rcClient);
    CRect rcItem;
    GetItemRect(nItem, &rcItem, LVIR_BOUNDS);
    rcItem.left = rcClient.left;
    rcItem.right = rcClient.right;
    InvalidateRect(&rcItem, FALSE);
}

void CStockListCtrl::InitListFont()
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

void CStockListCtrl::PreSubclassWindow()
{
    InitListFont();
    CListCtrl::PreSubclassWindow();
}




