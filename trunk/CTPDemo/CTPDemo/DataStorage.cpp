#include "StdAfx.h"
#include "DataStorage.h"

static const long nReserve = 8192;

DataStorage::DataStorage()
{
    m_vA.reserve(nReserve);
    m_vM.reserve(nReserve);
    m_vC.reserve(nReserve);
    m_vY.reserve(nReserve);
    m_vP.reserve(nReserve);
    m_vL.reserve(nReserve);
    m_vPVC.reserve(nReserve);
    m_vRU.reserve(nReserve);
    m_vCU.reserve(nReserve);
    m_vAL.reserve(nReserve);
    m_vZN.reserve(nReserve);
    m_vAU.reserve(nReserve);
    m_vFU.reserve(nReserve);
    m_vRB.reserve(nReserve);
    m_vWR.reserve(nReserve);
    m_vWT.reserve(nReserve);
    m_vWS.reserve(nReserve);
    m_vSR.reserve(nReserve);
    m_vPTA.reserve(nReserve);
    m_vCF.reserve(nReserve);
    m_vRO.reserve(nReserve);
    m_vER.reserve(nReserve);
    m_vIF.reserve(nReserve);
}


DataStorage::~DataStorage()
{
}

void DataStorage::AddDepthMarketData(const CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    // 
    if(pDepthMarketData->InstrumentID[1] < ':')
    {
        if(pDepthMarketData->InstrumentID[0] == 'a' || pDepthMarketData->InstrumentID[0] == 'A')
        {
            m_vA.push_back(*pDepthMarketData);
        }
        else if(pDepthMarketData->InstrumentID[0] == 'm' || pDepthMarketData->InstrumentID[0] == 'M')
        {
            m_vM.push_back(*pDepthMarketData);
        }
        else if(pDepthMarketData->InstrumentID[0] == 'c' || pDepthMarketData->InstrumentID[0] == 'C')
        {
            m_vC.push_back(*pDepthMarketData);
        }
        else if(pDepthMarketData->InstrumentID[0] == 'y' || pDepthMarketData->InstrumentID[0] == 'Y')
        {
            m_vY.push_back(*pDepthMarketData);
        }
        else if(pDepthMarketData->InstrumentID[0] == 'p' || pDepthMarketData->InstrumentID[0] == 'P')
        {
            m_vP.push_back(*pDepthMarketData);
        }
        else if(pDepthMarketData->InstrumentID[0] == 'l' || pDepthMarketData->InstrumentID[0] == 'L')
        {
            m_vL.push_back(*pDepthMarketData);
        }
    }
    else if((pDepthMarketData->InstrumentID[0] == 'p' || pDepthMarketData->InstrumentID[0] == 'P') &&
        (pDepthMarketData->InstrumentID[1] == 'v' || pDepthMarketData->InstrumentID[1] == 'V') &&
        (pDepthMarketData->InstrumentID[2] == 'c' || pDepthMarketData->InstrumentID[2] == 'C'))
    {
        m_vPVC.push_back(*pDepthMarketData);
    }
    // 
    else if((pDepthMarketData->InstrumentID[0] == 'r' || pDepthMarketData->InstrumentID[0] == 'R') &&
        (pDepthMarketData->InstrumentID[1] == 'u' || pDepthMarketData->InstrumentID[1] == 'U'))
    {
        m_vRU.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'c' || pDepthMarketData->InstrumentID[0] == 'C') &&
        (pDepthMarketData->InstrumentID[1] == 'u' || pDepthMarketData->InstrumentID[1] == 'U'))
    {
        m_vCU.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'a' || pDepthMarketData->InstrumentID[0] == 'l') &&
        (pDepthMarketData->InstrumentID[1] == 'A' || pDepthMarketData->InstrumentID[1] == 'L'))
    {
        m_vAL.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'z' || pDepthMarketData->InstrumentID[0] == 'Z') &&
        (pDepthMarketData->InstrumentID[1] == 'n' || pDepthMarketData->InstrumentID[1] == 'N'))
    {
        m_vZN.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'a' || pDepthMarketData->InstrumentID[0] == 'A') &&
        (pDepthMarketData->InstrumentID[1] == 'u' || pDepthMarketData->InstrumentID[1] == 'U'))
    {
        m_vAU.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'f' || pDepthMarketData->InstrumentID[0] == 'F') &&
        (pDepthMarketData->InstrumentID[1] == 'u' || pDepthMarketData->InstrumentID[1] == 'U'))
    {
        m_vFU.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'r' || pDepthMarketData->InstrumentID[0] == 'R') &&
        (pDepthMarketData->InstrumentID[1] == 'b' || pDepthMarketData->InstrumentID[1] == 'B'))
    {
        m_vRB.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'w' || pDepthMarketData->InstrumentID[0] == 'W') &&
        (pDepthMarketData->InstrumentID[1] == 'r' || pDepthMarketData->InstrumentID[1] == 'R'))
    {
        m_vWR.push_back(*pDepthMarketData);
    }
    // 
    else if((pDepthMarketData->InstrumentID[0] == 'w' || pDepthMarketData->InstrumentID[0] == 'W') &&
        (pDepthMarketData->InstrumentID[1] == 't' || pDepthMarketData->InstrumentID[1] == 'T'))
    {
        m_vWT.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'w' || pDepthMarketData->InstrumentID[0] == 'W') &&
        (pDepthMarketData->InstrumentID[1] == 's' || pDepthMarketData->InstrumentID[1] == 'S'))
    {
        m_vWS.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 's' || pDepthMarketData->InstrumentID[0] == 'S') &&
        (pDepthMarketData->InstrumentID[1] == 'r' || pDepthMarketData->InstrumentID[1] == 'R'))
    {
        m_vSR.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'p' || pDepthMarketData->InstrumentID[0] == 'P') &&
        (pDepthMarketData->InstrumentID[1] == 't' || pDepthMarketData->InstrumentID[1] == 'T') &&
        (pDepthMarketData->InstrumentID[1] == 'a' || pDepthMarketData->InstrumentID[1] == 'A'))
    {
        m_vPTA.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'c' || pDepthMarketData->InstrumentID[0] == 'C') &&
        (pDepthMarketData->InstrumentID[1] == 'f' || pDepthMarketData->InstrumentID[1] == 'F'))
    {
        m_vCF.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'r' || pDepthMarketData->InstrumentID[0] == 'R') &&
        (pDepthMarketData->InstrumentID[1] == 'o' || pDepthMarketData->InstrumentID[1] == 'O'))
    {
        m_vRO.push_back(*pDepthMarketData);
    }
    else if((pDepthMarketData->InstrumentID[0] == 'e' || pDepthMarketData->InstrumentID[0] == 'E') &&
        (pDepthMarketData->InstrumentID[1] == 'r' || pDepthMarketData->InstrumentID[1] == 'R'))
    {
        m_vER.push_back(*pDepthMarketData);
    }
    // 
    else if((pDepthMarketData->InstrumentID[0] == 'i' || pDepthMarketData->InstrumentID[0] == 'I') &&
        (pDepthMarketData->InstrumentID[1] == 'f' || pDepthMarketData->InstrumentID[1] == 'F'))
    {
        m_vIF.push_back(*pDepthMarketData);
    }
}
