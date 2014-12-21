#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include "MyMdSpi.h"

void CMyMdSpi::OnFrontConnected()
{
    CThostFtdcReqUserLoginField m_req;
    memset(&m_req, 0, sizeof(m_req));
    strcpy(m_req.BrokerID, m_brokerID);
    strcpy(m_req.UserID, m_investorID);
    strcpy(m_req.Password, m_password);
    m_pMdApi->ReqUserLogin(&m_req, m_nRequestID);
    m_nRequestID++;
}

void CMyMdSpi::OnFrontDisconnected(int nReason)
{
}

void CMyMdSpi::OnHeartBeatWarning(int nTimeElapse)
{
}

void CMyMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    size_t nCount = m_vInstrumentID.size();
    char** ppInstrumentID = new char*[nCount];
    for(size_t i=0; i<nCount; i++)
    {
        std::string str = m_vInstrumentID.at(i);
        ppInstrumentID[i] = new char[str.length()];
    }
    if (bIsLast && pRspInfo->ErrorID==0)
    {
        m_pMdApi->UnSubscribeMarketData(ppInstrumentID, nCount);
        m_pMdApi->SubscribeMarketData(ppInstrumentID, nCount);
    }
}

void CMyMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CMyMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CMyMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CMyMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CMyMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CMyMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CMyMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
}

void CMyMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
}

void CMyMdSpi::StopSubscribe()
{
    size_t nCount = m_vInstrumentID.size();
    char** ppInstrumentID = new char*[nCount];
    for(size_t i=0; i<nCount; i++)
    {
        std::string str = m_vInstrumentID.at(i);
        ppInstrumentID[i] = new char[str.length()];
    }
    m_pMdApi->UnSubscribeMarketData(ppInstrumentID, nCount);
}

