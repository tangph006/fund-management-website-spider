#pragma once
//
#include <string>
#include <iomanip>
#include <vector>

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcTraderApi* pUserApi;
// ���ò��� 
extern TThostFtdcBrokerIDType BROKER_ID; //������
extern TThostFtdcInvestorIDType INVESTOR_ID; //
extern TThostFtdcPasswordType PASSWORD; //

extern TThostFtdcInstrumentIDType INSTRUMENT_ID; // ���׺�Լ����
extern TThostFtdcDirectionType DIRECTION ; // ������������
extern TThostFtdcOffsetFlagType MARKETState ; // ��ƽ��
extern TThostFtdcPriceType LIMIT_PRICE ; // ���׼۸�

// �Ự����
extern bool RunMode;
extern TThostFtdcFrontIDType FRONT_ID; //ǰ�ñ��
extern TThostFtdcSessionIDType SESSION_ID; //�Ự���
extern TThostFtdcOrderRefType ORDER_REF; //��������
extern TThostFtdcOrderActionRefType ORDERACTION_REF[20]; //��������

// ������
extern int iRequestID;
extern TThostFtdcDateExprType TradingDay;
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



void SendOrder(TThostFtdcInstrumentIDType FuturesId,int BuySell,int OpenClose)
{
    //INSTRUMENT_ID = FuturesId;
    DIRECTION = BuySell;
    MARKETState = OpenClose;

    if (BuySell==0 && OpenClose==0)
    {
        BuySignal = true;
        SellSignal= false;
    }
    if (BuySell==1 && OpenClose==0)
    {
        BuySignal = false;
        SellSignal = true;
    }
    if (BuySell==0 && OpenClose==3)
    {
        BuySignal = false;
        SellSignal = false; 
    }
    if (BuySell==1 && OpenClose==3)
    {
        BuySignal = false;
        SellSignal = false;
    }

    //Sleep(050);

    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    ///���͹�˾����
    strcpy_s(req.BrokerID, BROKER_ID);
    ///Ͷ���ߴ���
    strcpy_s(req.InvestorID, INVESTOR_ID);
    ///��Լ���� //INSTRUMENT_ID
    strcpy_s(req.InstrumentID, INSTRUMENT_ID);
    ///��������
    //strcpy_s(req.OrderRef, ORDER_REF);
    ///�û�����
    // TThostFtdcUserIDType UserID;
    ///�����۸�����: �޼�
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;

    ///��������: //THOST_FTDC_D_Buy,THOST_FTDC_D_Sell
    if (BuySell==0)
    {
        req.Direction = THOST_FTDC_D_Buy;
    }
    else if (BuySell==1)
    {
        req.Direction = THOST_FTDC_D_Sell;
    }

    ///��Ͽ�ƽ��־: ���� //THOST_FTDC_OF_Open,THOST_FTDC_OF_Close,THOST_FTDC_OF_CloseToday
    if (OpenClose==0)
    {
        req.CombOffsetFlag[0] = THOST_FTDC_OF_Open; //����
    }
    else if(OpenClose==1)
    {
        req.CombOffsetFlag[0] = THOST_FTDC_OF_Close; //ƽ��
    }
    else if(OpenClose==3)
    {
        req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday; //ƽ��
    }

    ///���Ͷ���ױ���־
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; //Ͷ��

    ///�۸�
    if (BuySell==0)
    {
        //LIMIT_PRICE = Q_UpperLimit; //��ͣ�� 
        LIMIT_PRICE = NewPrice; //���¼�
    }
    else if (BuySell==1)
    {
        //LIMIT_PRICE = Q_LowerLimit; //��ͣ��
        LIMIT_PRICE = NewPrice; //���¼�
    }
    req.LimitPrice = LIMIT_PRICE;

    ///����: 1 // ��ƽ������
    req.VolumeTotalOriginal = 1; 
    ///��Ч������: ������Ч
    req.TimeCondition = THOST_FTDC_TC_GFD;
    ///GTD����
    // TThostFtdcDateType GTDDate;
    ///�ɽ�������: �κ�����
    req.VolumeCondition = THOST_FTDC_VC_AV;
    ///��С�ɽ���: 1
    req.MinVolume = 1;
    ///��������: ����
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    ///ֹ���
    // TThostFtdcPriceType StopPrice;
    ///ǿƽԭ��: ��ǿƽ
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ///�Զ������־: ��
    req.IsAutoSuspend = 0;
    ///ҵ��Ԫ
    // TThostFtdcBusinessUnitType BusinessUnit;
    ///������
    // TThostFtdcRequestIDType RequestID;
    ///�û�ǿ����־: ��
    req.UserForceClose = 0;

    ///��������,��������ϴ�
    int iNextOrderRef = atoi(ORDER_REF);
    iNextOrderRef++;
    sprintf(ORDER_REF,"%d",iNextOrderRef);
    strcpy_s(req.OrderRef, ORDER_REF);

    //���ԣ��������µ�
    if(RunMode) // ����=0��ʵ��=1��
    {
        int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID); //ʵ�̣�����ʽ�µ�
        std::cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << std::endl;
    }
}

void _record0(double system_times, char *txt, double SPrice, double BPrice)
{
    std::ofstream o_file(LogFilePaths,std::ios::app);
    o_file << TradingDay << "_" << system_times << "_" << INSTRUMENT_ID << "_" << BSVolume << txt << "_" << SPrice << "_" << BPrice << std::endl; //������д�뵽�ı��ļ���
    o_file.close(); //�ر��ļ�
}

bool ReadConfiguration(char *filepaths)
{
    std::ifstream config(filepaths);

    if (!config)
    {
        std::cerr << "--->>> " << "Configuration File is missing!" << std::endl;
        return false;
    }
    else
    {
        std::cerr << "--->>> " << "Read Configuration File!" << std::endl;
    }

    std::vector < double > data(8);
    for (int i = 0; i < 8; i++)
    {
        config >> data[i];
        //cout << "Configuration:" << data[i] << std::endl;
    }

    Sleep(100);
    // //
    //Q_BarTime_2 = data[1]; //Q_BarTime_2
    BSVolume = data[2]; //BSVolume
    // //
    Mn_open[0] = data[4]; //Mn_open
    Mn_high[0] = data[5]; //Mn_high
    Mn_low[0] = data[6]; //Mn_low
    Mn_close[0] = data[7]; //Mn_close
    NewPrice = data[7];

    config.close();
    return true;
}

void WriteConfiguration(char *filepaths)
{
    std::ofstream o_file(filepaths, std::ios::trunc);
    o_file << "20140428" << "\t" <<Q_BarTime_2<< "\t" << BSVolume <<"\t"<<"1409" <<"\t"<< Mn_open << "\t" << Mn_high << "\t" << Mn_low << "\t" << Mn_close << std::endl; //������д�뵽�ı��ļ���
    o_file.close(); //�ر��ļ�

}


void Erasefiles()
{
    system("del .\\thostmduserapi.dllDialogRsp.con");
    system("del .\\thostmduserapi.dllQueryRsp.con");
    system("del .\\thostmduserapi.dllTradingDay.con");

    system("del .\\thosttraderapi.dllDialogRsp.con");
    system("del .\\thosttraderapi.dllPrivate.con");
    system("del .\\thosttraderapi.dllPublic.con");
    system("del .\\thosttraderapi.dllQueryRsp.con");
    system("del .\\thosttraderapi.dllTradingDay.con");

}
