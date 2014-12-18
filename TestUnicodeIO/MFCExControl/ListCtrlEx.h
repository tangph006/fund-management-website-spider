#pragma once
#include "EditSpinWnd.h"
#include "InPlaceList.h"
#include "InplaceDatetimeCtrl.h"

#define WM_COLLUM_INDEX (WM_USER + 45)
#define LVCFMT_DROPDOWN                 0x0040
#define LVCFMT_SPINEDIT                 0x0080
#define LVCFMT_NOEDIT                   0x0100
#define LVCFMT_NUMBER                   0x0200
#define LVCFMT_DATETIME                 0x0400
#define LVCFMT_POPUP_GATE_SELECTION     0x0800
#define LVCFMT_EDIT                     0x1000
#define LVCFMT_CHECK                    0x2000
#define LVCFMT_BMP                      0x4000

#define DROP_DOWN               1
#define SPIN_EDIT               2
#define NO_EDIT                 3
#define NUMBER_ONLY             4
#define DATE_TIME               5
#define POPUP_GATE_SELECTION    6
#define TEXT_EDIT               7
#define CHECKBOX                8
#define BMP_EDIT                9

class LV_COLUMNEX
{
public:
    LV_COLUMNEX()
    {
        csList = NULL;
        pszDateFormat = NULL;
        pszShowDateFormat = NULL;
        nParam1 = -1;
        nParam2 = -1;
    }

public:
    UINT mask;
    int fmt;
    int cx;
    TCHAR* pszText;
    int cchTextMax;
    int iSubItem;
    int iImage;
    int iOrder;
    CStringList* csList;
    LPCSTR pszDateFormat;
    LPCSTR pszShowDateFormat;
    int nParam1;
    int nParam2;
};

struct LVCOLDROPLIST
{
    WORD Style;
    CStringList List;
    CString strDateFormat;
    CString strShowDateFormat;
    int nParam1;
    int nParam2;
};

class MFCEXCONTROL_API CListCtrlEx : public CListCtrl
{
public:
    CListCtrlEx();
    virtual ~CListCtrlEx();

protected:
    CPtrArray ddStyleList;
    int m_iSpinDisplayType;
    CEditSpinWnd m_SpinEdit;
    CInPlaceDateTimeCtrl m_inPlaceDateTimeCtrl;
    int m_nSpinSelItem;
    int m_nSpinSelCol;
    int m_nTimeSelItem;
    int m_nTimeSelCol;
    int CurrentSelection;
    int LastTimeSelection;
    int m_nLastDblClkRow;
    int m_nLastDlbClkColumn;
    BOOL m_bHHmmFormat;

public:
    void EditNew();
    CEdit* EditSubLabel( int nItem, int nCol );
    int HitTestEx(CPoint &point, int *col);
    void DeleteItemEx( int p_index );
    BOOL DeleteAllItems();
    int GetCurSel(void) const;
    int GetLastSel(void) const { return LastTimeSelection; }
    void SetCurSel(int nItem);
    int InsertColumn(int nCol, const LV_COLUMNEX* pColumn);
    void SetSpinDisplayType( int _iType );
    void SetHHmmTimeFormat();
    void SetEditSpinRange(int _low,int _Upp);
    void DblClickItem(int index, int column);
    int GetLastDblClk(int& column);
    LVCOLDROPLIST* GetColumnStyle(int nColumn);

protected:
    virtual void PreSubclassWindow();
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    BOOL IsItemEmpty(int nItem);
    void PopUpGateSelectionDlg();
    CEdit* EditSpinLabel(int nItem, int nCol);
    CComboBox* ShowInPlaceList( int nItem, int nCol);
    COleDateTime* EditDateTimeLabel(int nItem, int nCol);
    BOOL IsPreItemEmpty(int nItem);

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg BOOL OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);

private:
    void Init();
    void InvalidateItemRect(int nItem);
    void DrawSubItem(LPNMLVCUSTOMDRAW lpnmcd);
    void DrawRemainSpace(LPNMLVCUSTOMDRAW lpnmcd);

};
