#pragma once
#include <vector>
#include "..\6.3.0_20140811_traderapi_win32\ThostFtdcMdApi.h"

class CMyMdSpi : public CThostFtdcMdSpi
{
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
    void SetMdApi(CThostFtdcMdApi* val) { m_pMdApi = val; }
    CThostFtdcMdApi* GetMdApi(){ return m_pMdApi; }
    void StopSubscribe();
public:
    int m_nRequestID;
protected:
    CThostFtdcMdApi* m_pMdApi;
    TThostFtdcBrokerIDType m_brokerID;
    TThostFtdcInvestorIDType m_investorID;
    TThostFtdcPasswordType m_password;
    std::vector<std::string> m_vInstrumentID;
    //CThostFtdcDepthMarketDataField* m_p
};