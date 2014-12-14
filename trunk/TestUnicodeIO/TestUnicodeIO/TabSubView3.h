#pragma once
#include "afxcmn.h"
#include "CoolTree.h"

class CTabSubView3 : public CDialog
{
    DECLARE_DYNAMIC(CTabSubView3)

public:
    CTabSubView3(CWnd* pParent = NULL);
    virtual ~CTabSubView3();
    enum { IDD = IDD_TABSUBVIEW3 };
protected:
    CCoolTree m_cooltree;
    CTreeCtrl m_normaltree;

protected:
    void InertTempDataToCooltree();
    void InertTempDataToNormaltree();
    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()
private:
    int m_oldCx;
    int m_oldCy;
    typedef enum {TopLeft, TopRight, BottomLeft, BottomRight} LayoutRef;
    void LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy);
};
