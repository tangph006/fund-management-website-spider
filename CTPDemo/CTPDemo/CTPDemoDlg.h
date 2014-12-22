#pragma once
#include "afxcmn.h"
#include "FloatEdit.h"
#include "afxwin.h"
#include <string>
#include <vector>
#include "..\6.3.0_20140811_traderapi_win32\ThostFtdcMdApi.h"
#include "..\CTPAutoTrade\ThostTraderApi\ThostFtdcMdApi.h"

class CCTPDemoDlg : public CDialogEx, public CThostFtdcMdSpi
{
public:
    CCTPDemoDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_CTPDEMO_DIALOG };
protected:
    CFloatEdit m_editPort;
    CFloatEdit m_editUID;
    CListCtrl m_listResult;
    CIPAddressCtrl m_ipctrlIP;
    CToolBar m_toolbar;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    void InitListResultHeader();
protected:
    HICON m_hIcon;
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
    DECLARE_MESSAGE_MAP()
private:
    void DisableBtns();
    void EnableUsernameCtrls(BOOL bEnable);
private:
    int m_oldCx;
    int m_oldCy;
    typedef enum {TopLeft, TopRight, BottomLeft, BottomRight} LayoutRef;
    void LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy);

/////////////////////////////////////////////engine start/////////////////////////////////////////////
public:
    virtual void OnFrontConnected();
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
public:
    void ClearMyParts();
    void SetBrokerID(TThostFtdcBrokerIDType val) { strcpy(m_brokerID, val); }
    void SetInvestorID(TThostFtdcInvestorIDType val) { strcpy(m_investorID, val); }
    void SetPassword(TThostFtdcPasswordType val) { strcpy(m_password, val); }
    void Login();
    void Logout();
    void StartSubscribe();
    void StopSubscribe();
protected:
private:
    void AddMsg(CString strMsg);
public:
    int m_nRequestID;
protected:
    CThostFtdcMdApi* m_pMdApi;
    TThostFtdcBrokerIDType m_brokerID;
    TThostFtdcInvestorIDType m_investorID;
    TThostFtdcPasswordType m_password;
    std::vector<CString> m_vInstrumentID;
    CString m_addr1;
    CString m_addr2;
private:
    std::vector<CString> m_vecMsg;
public:
    afx_msg void OnLvnGetdispinfoListResult(NMHDR *pNMHDR, LRESULT *pResult);
};
