#pragma once
#include <vector>
#include "StockListCtrl.h"
#include "afxcmn.h"
class CTabSubView5 : public CDialog
{
    DECLARE_DYNAMIC(CTabSubView5)

public:
    CTabSubView5(CWnd* pParent = NULL);
    virtual ~CTabSubView5();
    enum { IDD = IDD_TABSUBVIEW5 };

protected:
    CListCtrl m_listIntPP;
    int m_array[5][6];

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLvnGetdispinfoListIntPP(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void InitData();
    DECLARE_MESSAGE_MAP()
private:
    int m_oldCx;
    int m_oldCy;
    typedef enum {TopLeft, TopRight, BottomLeft, BottomRight} LayoutRef;
    void LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy);
};
