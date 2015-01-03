#pragma once
#include <string>
#include "MyFileMapManager.h"
#include "ThostFtdcUserApiStruct.h"
class TotalDepthMarketDataManager
{
public:
    TotalDepthMarketDataManager();
    virtual ~TotalDepthMarketDataManager();

    std::string GetFilePath(){ return m_filePath; }
    void SetFilePath(std::string filePath) { m_filePath = filePath; }
    int GetDataCount(){ return m_totalData.GetDataCount(); }
    CThostFtdcDepthMarketDataField* GetDataByIndex(int iIndex);
    void AddItem(int& iErr, CThostFtdcDepthMarketDataField* pDepthData);
protected:
    std::string m_filePath;
    MyFileMapper<CThostFtdcDepthMarketDataField> m_totalData;
};

