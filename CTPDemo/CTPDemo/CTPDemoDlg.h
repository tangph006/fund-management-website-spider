#pragma once
#include "afxcmn.h"
#include "FloatEdit.h"
#include "MyListCtrl.h"
#include "afxwin.h"
#include <string>
#include <vector>
#include "MyFileMapManager.h"
#include "..\6.3.0_20140811_traderapi_win32\ThostFtdcMdApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "MyMdSpi.h"

class CCTPDemoDlg : public CDialogEx, public CThostFtdcMdSpi
{
public:
    CCTPDemoDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_CTPDEMO_DIALOG };
protected:
    HICON m_hIcon;
    CFloatEdit m_editPort;
    CFloatEdit m_editUID;
    CMyListCtrl m_listResult;
    CIPAddressCtrl m_ipctrlIP;
    CToolBar m_toolbar;
    CMyMdSpi m_mdSpi;
    std::vector<CString> m_vecMsg;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnButtonStart();
    afx_msg void OnButtonStop();
    afx_msg void OnButtonLogin();
    afx_msg void OnBnClickedBtnLogout();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnLvnGetdispinfoListResult(NMHDR *pNMHDR, LRESULT *pResult);
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
private:
    void InitListResultHeader();
    void AppendMsg(CString strMsg);
    void DisableAllBtns();
    void EnableUsernameCtrls(BOOL bEnable);
private:
    int m_oldCx;
    int m_oldCy;
    typedef enum {TopLeft, TopRight, BottomLeft, BottomRight} LayoutRef;
    void LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy);
};
