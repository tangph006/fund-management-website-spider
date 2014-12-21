#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "MyTraderSpi.h"
#include "MyMdSpi.h"
#include "Common.h"
#include "DataSniffer.h"
#include "MyTrader.h"

// UserApi����
CThostFtdcTraderApi *pUserApi;
// MdApi����
CThostFtdcMdApi *pMdApi;

// ���ò���
char FRONT_ADDR_1A[] = "tcp://front111.ctp.gtjafutures.com:41205"; // ǰ�õ�ַ1����:ʵ��
char FRONT_ADDR_1B[] = "tcp://front111.ctp.gtjafutures.com:41213"; // ǰ�õ�ַ2����:ʵ��

char FRONT_ADDR_2A[] = "tcp://csv101.ctp.gtjafutures.com:41205"; // ǰ�õ�ַ1����:�̺�
char FRONT_ADDR_2B[] = "tcp://csv101.ctp.gtjafutures.com:41213"; // ǰ�õ�ַ2����:�̺�

char FRONT_ADDR_3A[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205"; // ǰ�õ�ַ3����:���� 17:00��ʼ
char FRONT_ADDR_3B[] = "tcp://27.115.78.154:31213"; // ǰ�õ�ַ3����:���� 17:00��ʼ
//char FRONT_ADDR_3A[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205"; // ǰ�õ�ַ3����:���� 17:00��ʼ
//char FRONT_ADDR_3B[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213"; // ǰ�õ�ַ3����:���� 17:00��ʼ

TThostFtdcBrokerIDType BROKER_ID = "8000"; // ʵ�̣����͹�˾���� ��̩����=7090
TThostFtdcInvestorIDType INVESTOR_ID = "test"; // ʵ�̣�Ͷ���ߴ���
TThostFtdcPasswordType PASSWORD = "123456"; // ʵ�̣��û�����
//TThostFtdcBrokerIDType BROKER_ID = "2030"; // ���͹�˾����:����
//TThostFtdcInvestorIDType INVESTOR_ID = "00092"; // Ͷ���ߴ���:����
//TThostFtdcPasswordType PASSWORD = "888888"; // �û�����:����

TThostFtdcInstrumentIDType INSTRUMENT_ID = "IF1412"; // ���׺�Լ����
TThostFtdcDirectionType DIRECTION; // ������������
TThostFtdcOffsetFlagType MARKETState; // ��ƽ��
TThostFtdcPriceType LIMIT_PRICE; // ���׼۸�

//char *ppInstrumentID[] = {"IF1406", "rb1410", "j1409", "ru1409","SR409", "m1409", "y1409", "p1409","ag1412", "cu1408"}; // ���鶩���б�
char *ppInstrumentID[] = {"IF1412"}; // ���鶩���б�
int iInstrumentID = 1; // ���鶩������

bool RunMode=0; // ʵ���µ�=1,����=0�� Ӱ��Common.h�е�SendOrder()����
bool ReceiveTick = false;

// ������
int iRequestID = 0;
// ����ʱ��
bool JustRun = false; //����������־

TThostFtdcDateExprType TradingDay;

// User��������

extern char *InstrumentID_name; //
extern std::string Q_BarTime_s; //ʱ���ַ���
extern int Q_BarTime_1; //ʱ��������
extern double Q_BarTime_2; //ʱ���ʽ0.145100
extern double Q_UpperLimit; //
extern double Q_LowerLimit; //

extern double NewPrice; //
extern int FirstVolume; //ǰһ�γɽ�������

extern double Mn_open[3]; // 
extern double Mn_high[3]; // 
extern double Mn_low[3]; // 
extern double Mn_close[3]; // 

extern double BuyPrice; //���ּ�
extern double SellPrice; //���ּ�
extern int BNum; //���ִ���
extern int SNum; //���ִ���

extern bool BuySignal; //
extern bool SellSignal; //

extern double BSVolume; //������

extern int TickABS;
extern double TickAPrice[4]; //
extern int TickBNum;
extern double TickBPrice[4]; //

extern char LogFilePaths[80]; //

// �Ự����
extern TThostFtdcFrontIDType FRONT_ID; //ǰ�ñ��
extern TThostFtdcSessionIDType SESSION_ID; //�Ự���
extern TThostFtdcOrderRefType ORDER_REF; //��������



void main()
{
    Erasefiles();
    Sleep(1000);

    std::cerr << "--->>> " << "Welcom MyAutoTrader System!" << std::endl;
    std::cerr << "--->>> " << "Version 1.0.0!" << std::endl;
    // ��ʼ��UserApi
    // pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll"); // ����UserApi//"./thosttraderapi.dll"
    // CTraderSpi* pUserSpi = new CTraderSpi();
    // pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi); // ע���¼���
    // pUserApi->SubscribePublicTopic(THOST_TERT_RESTART); // ע�ṫ����
    // pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART); // ע��˽����
    // pUserApi->RegisterFront(FRONT_ADDR_1A); // connect
    // 
    // pUserApi->Init();
    // std::cerr << "--->>> " << "Initialing UserApi" << std::endl;

    // ��ʼ��MdApi
    pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll"); // ����MdApi//"./thostmduserapi.dll"
    CMyMdSpi* pMdSpi = new CMyMdSpi();
    pMdSpi->SetBrokerID("8000");
    pMdSpi->SetInvestorID("test");
    pMdSpi->SetPassword("123456");
    pMdApi->RegisterSpi(pMdSpi); // ע���¼���
    pMdApi->RegisterFront("tcp://27.115.78.154:31213"); // connect ���������ַ
    pMdApi->RegisterFront("tcp://27.115.78.154:31213"); // connect ���������ַ��1B�Ͽ����Զ�����2B��ַ

    pMdApi->Init();
    std::cerr << "--->>> " << "Initialing MdApi" << std::endl;
    //pMdApi->Join();
    //pMdApi->Release();
    ReadConfiguration("./AutoTrader.dat"); //�Զ������ݣ���ֲ����ݵȾ���
    std::cerr << "--->>> " << "��ʼ�����!" << std::endl;

    while(1)
    {
        //ָ�����,����ֻ�Ǹ�������
        //���Խ����������и��Ӵ��� ��DataSniffer.h
        Sniffer();
        //�µ�����
        //���Խ��������������Ӵ��� ��MyTrader.h
        Trading();
    }
}



