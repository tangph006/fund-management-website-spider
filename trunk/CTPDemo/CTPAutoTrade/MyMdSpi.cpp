#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include "MyTraderSpi.h"
#include "MyMdSpi.h"
#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcMdApi* pMdApi;

// ���ò���
extern char FRONT_ADDR[]; 
extern TThostFtdcBrokerIDType BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType PASSWORD;
extern char* ppInstrumentID[]; 
extern int iInstrumentID;
extern TThostFtdcInstrumentIDType INSTRUMENT_ID;

// ������
extern int iRequestID;

// User��������
extern TThostFtdcDateExprType TradingDay;

extern bool ReceiveTick;

bool CloseAll= false; //���̱�־

char *InstrumentID_name; //
double Q_Dayopen=0; //
double Q_UpperLimit = 0; //
double Q_LowerLimit = 0; //
std::string Q_BarTime_s; //
int Q_BarTime_1; //
double Q_BarTime_2; //
int FirstVolume; //ǰһ�γɽ�������
int BNum = 0; //���ִ���
int SNum = 0; //���ִ���

bool MnKlinesig=false; //
double Mn_open[3] = {0,0,0}; // 
double Mn_high[3] = {0,0,0}; // 
double Mn_low[3] = {0,0,0}; // 
double Mn_close[3] = {0,0,0}; // 
double NewPrice = 0; //

double Mn_bbreak = 0; // 
double Mn_sbreak = 0; // 
double Mn_UpperBand = 100000; // 
double Mn_LowerBand = 0; // 

double M4 = 1.0; // 
double M5 = 1.0; // 

double BuyPrice = 0; //���ּ�
double SellPrice = 0; //���ּ�

double BuyPrice1 = 0; //��һ��
double SellPrice1 = 0; //��һ��
double BuyVol1 = 0; //��һ��
double SellVol1 = 0; //��һ��
double BuyVol10 = 0; //��һ����һ��
double SellVol10 = 0; //��һ����һ��

double TodayVolume = 0; //�ɽ���
double OpenInt = 0; //�ֲ���

double BuyStopline = 0; //����ֹ���
double SellStopline = 0; //����ֹ��� 
double BSVolume = 1; //������

bool BuySignal = false; //
bool SellSignal = false; //

int TickABS = 0;
double TickAPrice[4]; //
int TickBNum = 0;
double TickBPrice[4]; //
char TickFileWritepaths[80]=""; //
char LogFilePaths[80]=""; //


///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CMyMdSpi::OnFrontConnected()
{
    std::cerr << "--->>> ǰ�û�������" << std::endl;
    ///�û���¼����
    CThostFtdcReqUserLoginField m_req;
    memset(&m_req, 0, sizeof(m_req));
    strcpy(m_req.BrokerID, m_brokerID);
    strcpy(m_req.UserID, m_investorID);
    strcpy(m_req.Password, m_password);
    int iResult = pMdApi->ReqUserLogin(&m_req, ++iRequestID);
    std::cerr << "--->>> ���������û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << std::endl;
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
/// 0x1001 �����ʧ��
/// 0x1002 ����дʧ��
/// 0x2001 ����������ʱ
/// 0x2002 ��������ʧ��
/// 0x2003 �յ�������
void CMyMdSpi::OnFrontDisconnected(int nReason)
{
    std::cerr << "--->>> ǰ�û��Ͽ�����" << std::endl;
    std::cerr << "--->>> ԭ��: " << nReason << std::endl;
}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CMyMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
    std::cerr << "--->>> ��ʱ����" << std::endl;
    std::cerr << "--->>> �����ϴν��ձ��ĵ�ʱ��: " << nTimeLapse << std::endl;
}

///��¼������Ӧ
void CMyMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (bIsLast && pRspInfo->ErrorID==0)
    {
        ///��ȡ��ǰ������
        std::cerr << "--->>> ��ǰ������: " << pMdApi->GetTradingDay() << std::endl;
        // ����������
        int iResult0 = pMdApi->UnSubscribeMarketData(ppInstrumentID, iInstrumentID);
        std::cerr << "--->>> ȡ�����鶩������: " << ((iResult0 == 0) ? "�ɹ�" : "ʧ��") << std::endl;
        Sleep(1000);
        int iResult = pMdApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
        std::cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << std::endl;

        //�ļ���������OrderInfo_date.txt��
        char perf[100];
        char tmp[20];
        int tmp1 = 20140424;
        strcpy(perf,"./AutoTrade/");
        strcpy(perf,"TraderInfo_");
        sprintf(tmp,"%d",tmp1);
        strcat(perf,tmp);
        strcat(perf,".txt");
        std::cerr << "--->>> ���·��:" << perf << std::endl;

        //����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
        std::ifstream inf;
        inf.open(perf, std::ios::out);
    }
}

///�ǳ�������Ӧ
void CMyMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///����Ӧ��
void CMyMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cerr << "--->>> �������� "<< std::endl;
    std::cerr << "--->>> ����ID: " << pRspInfo->ErrorID << ", ������Ϣ: " << pRspInfo->ErrorMsg << std::endl;
}

///��������Ӧ��
void CMyMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cerr << "--->>> �ɹ����ĺ�Լ: " << pSpecificInstrument->InstrumentID << std::endl;

}

///ȡ����������Ӧ��
void CMyMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cerr << "--->>> ��ȡ�����ĺ�Լ: " << pSpecificInstrument->InstrumentID << std::endl;
}

///����ѯ��Ӧ��
void CMyMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///ȡ������ѯ��Ӧ��
void CMyMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///�������֪ͨ
void CMyMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    //std::cerr << "--->>> " << __FUNCTION__ << std::endl;
    ///���ݻ��档
    ReceiveTick = true;
    void WriteConfiguration(char *filepaths); 
    //SaveData();

    int Volume_t=pDepthMarketData->Volume;
    //std::cerr << "--->>> " <<"NewTick!" << std::endl;

    //���������䶯���޳ɽ���tick����,Ҳ�ɱ���
    if (1)
        //if (Volume_t>FirstVolume)
    {

        FirstVolume=Volume_t;

        char names[10];
        char times[10];

        strcpy(names, pDepthMarketData->InstrumentID);
        strcpy(times, pDepthMarketData->UpdateTime);

        std::string str0=names;
        std::string str1=times;
        std::string str2=times;
        std::string str3=times;

        str1=str1.substr(0,2); //
        str2=str2.substr(3,2); //
        str3=str3.substr(6,2); //
        int hours = atoi(str1.c_str());
        int minutes = atoi(str2.c_str());
        int seconds = atoi(str3.c_str());
        int Millisecs = pDepthMarketData->UpdateMillisec;

        Q_BarTime_s = times; //ʱ���ַ���
        Q_BarTime_1 = hours*60*60+minutes*60+seconds; //ʱ��������
        Q_BarTime_2 = (1/10e1)*hours + (1/10e3)*minutes + (1/10e5)*seconds; //ʱ���ʽ0.145100 = 14��51��00

        Q_UpperLimit=pDepthMarketData->UpperLimitPrice;
        Q_LowerLimit=pDepthMarketData->LowerLimitPrice;

        InstrumentID_name=names;

        NewPrice = pDepthMarketData->LastPrice;
        SellPrice1 = pDepthMarketData->AskPrice1;
        SellVol1 = pDepthMarketData->AskVolume1;
        BuyPrice1 = pDepthMarketData->BidPrice1;
        BuyVol1 = pDepthMarketData->BidVolume1;

        TodayVolume = pDepthMarketData->Volume;
        OpenInt = pDepthMarketData->OpenInterest;

        TickAPrice[3]=TickAPrice[2];
        TickAPrice[2]=TickAPrice[1];
        TickAPrice[1]=TickAPrice[0];
        TickAPrice[0]=pDepthMarketData->LastPrice;

        //TICK����
        //cerr << "--->>> " <<InstrumentID_name<<"_"<<Q_BarTime_2<<"_"<<SellPrice1<<"_"<<SellVol1<<"_"<<BuyPrice1<<"_"<<BuyVol1<<"_"<<NewPrice<<"_"<<TodayVolume<<"_"<<OpenInt<< endl;

        //*************************************************** 
        //����1����K��

        bool Timemore0=Q_BarTime_2!=0.0859 && Q_BarTime_2!=0.0900 && Q_BarTime_2!=0.1015 && Q_BarTime_2!=0.1130 && Q_BarTime_2!=0.1500;
        bool Timemore1=Q_BarTime_2!=0.2059 && Q_BarTime_2!=0.2100 && Q_BarTime_2!=0.0230;
        bool Timemore2=(Q_BarTime_2>0.0900 && Q_BarTime_2<0.1500) || (Q_BarTime_2>0.2100 || Q_BarTime_2<0.0230);

        if (Timemore0 && Timemore1 && Timemore2 && seconds>=0 && seconds<40 && MnKlinesig==false)
        {
            MnKlinesig=true;

            Mn_open[1] =Mn_open[0]; 
            Mn_high[1] =Mn_high[0];
            Mn_low[1] =Mn_low[0];
            Mn_close[1] =Mn_close[0];

            Mn_open[0] =NewPrice; 
            Mn_high[0] =NewPrice;
            Mn_low[0] =NewPrice;
            Mn_close[0] =NewPrice;

            //��ӡ1����K������
            std::cerr << "--->>> " <<InstrumentID_name<< "_" <<Q_BarTime_s<< "_" << Mn_open[1]<< "_" << Mn_high[1]<< "_" << Mn_low[1]<< "_" << Mn_close[1] << std::endl;
        }
        else
        {
            Mn_high[0] =max(Mn_high[0],NewPrice);
            Mn_low[0] =min(Mn_low[0],NewPrice);
            Mn_close[0] =NewPrice;
        }

        if (seconds>45 && seconds<55 && MnKlinesig==true)
        {
            MnKlinesig=false;
        }
        //***************************************************

        if (LogFilePaths[0]== '\0')
        {
            strcpy_s(LogFilePaths,"./AutoTrade/");
            strcat_s(LogFilePaths,pDepthMarketData->TradingDay);
            strcat_s(LogFilePaths,".txt");

            //����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
            std::ifstream inf;
            inf.open(LogFilePaths, std::ios::out);
        }

        if (TickFileWritepaths[0]== '\0')
        {
            strcpy_s(TickFileWritepaths,"./TickData/");
            strcat_s(TickFileWritepaths,pDepthMarketData->InstrumentID);
            strcat_s(TickFileWritepaths,"_");
            strcat_s(TickFileWritepaths,pDepthMarketData->TradingDay); 
            strcat_s(TickFileWritepaths,".txt");

            //����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
            std::ifstream inf;
            inf.open(TickFileWritepaths, std::ios::out);
        }

        //����ģ���޷���ȡ���������ݣ�ʵ�������ʹ�� TradingDay
        std::ofstream o_file(TickFileWritepaths, std::ios::app);
        o_file << "20140508" << " " <<Q_BarTime_2<< "\t" << Millisecs <<"\t"<<SellPrice1 <<"\t"<< SellVol1 << "\t" << BuyPrice1 << "\t" << BuyVol1 << "\t" << NewPrice <<"\t"<<TodayVolume<<"\t"<<OpenInt<< std::endl; //������д�뵽�ı��ļ���
        o_file.close(); //�ر��ļ�

    }

    ReceiveTick = false;
}

///ѯ��֪ͨ
void CMyMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
}



