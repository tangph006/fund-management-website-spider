#pragma once
#include <map>
#include <vector>
#include "MyFileMapManager.h"
#include "ThostFtdcMdApi.h"
#include "TotalDataManager.h"
#include "DataStorage.h"

static const int MaxPathLen = 256;
static const int WM_MY_BASE = WM_USER + 1;
static const int WM_OnFrontConnected = WM_MY_BASE + 1;
static const int WM_OnFrontDisconnected = WM_MY_BASE + 2;
static const int WM_OnHeartBeatWarning = WM_MY_BASE + 3;
static const int WM_OnRspUserLogin = WM_MY_BASE + 4;
static const int WM_OnRspUserLogout = WM_MY_BASE + 5;
static const int WM_OnRspError = WM_MY_BASE + 6;
static const int WM_OnRspSubMarketData = WM_MY_BASE + 7;
static const int WM_OnRspUnSubMarketData = WM_MY_BASE + 8;
static const int WM_OnRspSubForQuoteRsp = WM_MY_BASE + 9;
static const int WM_OnRspUnSubForQuoteRsp = WM_MY_BASE + 10;
static const int WM_OnRtnDepthMarketData = WM_MY_BASE + 11;
static const int WM_OnRtnForQuoteRsp = WM_MY_BASE + 12;

class CMyMdSpi : public CThostFtdcMdSpi
{
public:
    CMyMdSpi(TotalDepthMarketDataManager* pDataManager, DataStorage* pDataStorage);
    virtual ~CMyMdSpi();
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
    void SetBrokerID(TThostFtdcBrokerIDType val) { strcpy(m_brokerID, val); }
    void SetInvestorID(TThostFtdcInvestorIDType val) { strcpy(m_investorID, val); }
    void SetPassword(TThostFtdcPasswordType val) { strcpy(m_password, val); }

    void SetAddr1(char* strAddr) { assert(strlen(strAddr)<MaxPathLen); strcpy(m_addr1, strAddr); }
    void SetAddr2(char* strAddr) { assert(strlen(strAddr)<MaxPathLen); strcpy(m_addr2, strAddr); }
    void Login(int& iErr);
    void Logout(int& iErr);
    void StartSubscribe(int& iErr);
    void StopSubscribe(int& iErr);
    void AddIntrusmentType(int& iErr, std::string strIntrusmentID);
    TotalDepthMarketDataManager* GetDataManager(){ return m_pDataManager; }
    void SetDataManager(TotalDepthMarketDataManager* pManager) { m_pDataManager = pManager; }
    DataStorage* GetDataSotrage(){ return m_pDataSotrage; }
    void SetDataSotrage(DataStorage* pDataStorage) { m_pDataSotrage = pDataStorage; }
private:
    void ClearMyParts();
    void PostSimpleMsgToObservers(UINT Msg, WPARAM wParam, LPARAM lParam);
public:
    int m_nRequestID;
    std::vector<HWND> m_observers;
protected:
    CThostFtdcMdApi* m_pMdApi;
    TThostFtdcBrokerIDType m_brokerID;
    TThostFtdcInvestorIDType m_investorID;
    TThostFtdcPasswordType m_password;
    char m_addr1[MaxPathLen];
    char m_addr2[MaxPathLen];
    std::vector<std::string> m_vIntrusmentID;
    TotalDepthMarketDataManager* m_pDataManager;
    DataStorage* m_pDataSotrage;
};