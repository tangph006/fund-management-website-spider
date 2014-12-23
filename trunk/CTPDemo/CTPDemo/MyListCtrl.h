#pragma once
#include "afxcmn.h"
class CMyListCtrl : public CListCtrl
{
public:
    CMyListCtrl();
    ~CMyListCtrl();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual void PreSubclassWindow();
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

private:
    void Init();
    void InvalidateItemRect(int nItem);
    void DrawSubItem(LPNMLVCUSTOMDRAW lpnmcd);
    void DrawRemainSpace(LPNMLVCUSTOMDRAW lpnmcd);
};

