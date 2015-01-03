#pragma once
#include <vector>
#include "ThostFtdcUserApiStruct.h"
#include "TotalDataManager.h"

class DataStorage
{
public:
    DataStorage();
    ~DataStorage();
public:
    void InitFromTotalData(TotalDepthMarketDataManager* pDataManager);
    void AddDepthMarketData(const CThostFtdcDepthMarketDataField* pDepthMarketData);

protected:
    // ������Ʒ������
    std::vector<CThostFtdcDepthMarketDataField> m_vA;
    std::vector<CThostFtdcDepthMarketDataField> m_vM;
    std::vector<CThostFtdcDepthMarketDataField> m_vC;
    std::vector<CThostFtdcDepthMarketDataField> m_vY;
    std::vector<CThostFtdcDepthMarketDataField> m_vP;
    std::vector<CThostFtdcDepthMarketDataField> m_vL;
    std::vector<CThostFtdcDepthMarketDataField> m_vPVC;
    // �Ϻ��ڻ�������
    std::vector<CThostFtdcDepthMarketDataField> m_vRU;
    std::vector<CThostFtdcDepthMarketDataField> m_vCU;
    std::vector<CThostFtdcDepthMarketDataField> m_vAL;
    std::vector<CThostFtdcDepthMarketDataField> m_vZN;
    std::vector<CThostFtdcDepthMarketDataField> m_vAU;
    std::vector<CThostFtdcDepthMarketDataField> m_vFU;
    std::vector<CThostFtdcDepthMarketDataField> m_vRB;
    std::vector<CThostFtdcDepthMarketDataField> m_vWR;
    // ֣����Ʒ������
    std::vector<CThostFtdcDepthMarketDataField> m_vWT;
    std::vector<CThostFtdcDepthMarketDataField> m_vWS;
    std::vector<CThostFtdcDepthMarketDataField> m_vSR;
    std::vector<CThostFtdcDepthMarketDataField> m_vPTA;
    std::vector<CThostFtdcDepthMarketDataField> m_vCF;
    std::vector<CThostFtdcDepthMarketDataField> m_vRO;
    std::vector<CThostFtdcDepthMarketDataField> m_vER;
    // �й����ڽ�����
    std::vector<CThostFtdcDepthMarketDataField> m_vIF;
};

