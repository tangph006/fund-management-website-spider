#pragma once
#include "afxcmn.h"
#include "FloatEdit.h"
#include "afxwin.h"
#include "MyMdSpi.h"

class CCTPDemoDlg : public CDialogEx
{
public:
    CCTPDemoDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_CTPDEMO_DIALOG };
protected:
    CFloatEdit m_editPort;
    CFloatEdit m_editUID;
    CListCtrl m_listResult;
    CIPAddressCtrl m_ipctrlIP;
    CToolBar m_tbarMain;
    CMyMdSpi* m_pMdSpi;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    void InitListResultHeader();
protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnButtonGo();
    afx_msg void OnButtonStop();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    DECLARE_MESSAGE_MAP()
private:
    int m_oldCx;
    int m_oldCy;
    typedef enum {TopLeft, TopRight, BottomLeft, BottomRight} LayoutRef;
    void LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy);
};
