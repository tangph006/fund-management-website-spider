#include "StdAfx.h"
#include "ErrorCode.h"
#include "TotalDataManager.h"
#include "log4cplus/logger.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/loggingmacros.h"

using namespace log4cplus;

TotalDepthMarketDataManager::TotalDepthMarketDataManager()
{
    int iErr = Success0;
    m_totalData.MapToFile(iErr, m_filePath.c_str());
}


TotalDepthMarketDataManager::~TotalDepthMarketDataManager()
{
    m_totalData.UnMapFromFile();
}

void TotalDepthMarketDataManager::AddItem(int& iErr, CThostFtdcDepthMarketDataField* pDepthData)
{
    m_totalData.AddItem(iErr, pDepthData);
    if(iErr != Success0)
    {
        SharedAppenderPtr _append(new FileAppender("TotalDepthDataManagerLog.log"));
        _append->setName("TotalDepthDataManagerLogger");
        Logger _logger = Logger::getInstance("TotalDepthDataManager");
        _logger.addAppender(_append);
        if(iErr >= ErrorBegin)
            LOG4CPLUS_DEBUG(_logger, "Error code: " << iErr);
        else
            LOG4CPLUS_DEBUG(_logger, "Warning code: " << iErr);
    }
}

CThostFtdcDepthMarketDataField* TotalDepthMarketDataManager::GetDataByIndex(int iIndex)
{
    return m_totalData.GetDataByIndex(iIndex);
}
