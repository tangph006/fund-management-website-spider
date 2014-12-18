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


// User��������

extern	char	*InstrumentID_name;	//
extern	string	Q_BarTime_s;		//ʱ���ַ���
extern	int		Q_BarTime_1;		//ʱ��������
extern	double	Q_BarTime_2;		//ʱ���ʽ0.145100
extern	double	Q_UpperLimit;	//
extern	double	Q_LowerLimit;	//

extern	double	NewPrice;		//
extern	int		FirstVolume;	//ǰһ�γɽ�������

extern	double  Mn_open[3];		// 
extern	double  Mn_high[3];		// 
extern	double  Mn_low[3];		// 
extern	double  Mn_close[3];	// 

extern	double  BuyPrice;		//���ּ�
extern	double  SellPrice;		//���ּ�
extern	int		BNum;			//���ִ���
extern	int		SNum;			//���ִ���

extern	bool	BuySignal;		//
extern	bool	SellSignal;		//

extern	double	BSVolume;		//������

extern	int		TickABS;
extern	double  TickAPrice[4];		//
extern	int		TickBNum;
extern	double  TickBPrice[4];		//

extern	char    LogFilePaths[80];	//


void Trading()	//�µ��Լ���������
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int BuySell,int OpenClose);
	void _record0(double system_times, char *txt, double SPrice, double BPrice);
	void StopLoss(double system_times);

		bool TradingTime=(Q_BarTime_2>0.0910 && Q_BarTime_2<0.2355);	//����ʱ��
		bool fanshou=false;	//����ʹ��

		//����ֹ���ӳ���	************************�����ж��Ƿ����ֹ��ֹӯ�������ڽ��п��ִ���
		StopLoss(Q_BarTime_2);

		if (TickABS==1 && TradingTime)
		{

			if (SellSignal == true && fanshou)	//����гֲ֣���ƽ��
			{
			strcpy(INSTRUMENT_ID,"rb1410");
			SendOrder(INSTRUMENT_ID, 0, 3);		//��ƽ
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ��" << endl;
			_record0(Q_BarTime_2, "_��_ƽ��", NewPrice, NewPrice);
			}

			strcpy(INSTRUMENT_ID,"rb1410");		//��
			SendOrder(INSTRUMENT_ID, 0, 0);	

			BuyPrice=NewPrice;
			SellPrice=0;
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_����" << endl;
			_record0(Q_BarTime_2, "_��_����", BuyPrice, BuyPrice);

		}
		if (TickABS==2 && TradingTime)
		{
			if (BuySignal == true && fanshou)	//����гֲ֣���ƽ��
			{
			strcpy(INSTRUMENT_ID,"rb1410");		//��ƽ
			SendOrder(INSTRUMENT_ID, 1, 3);	
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ��" << endl;
			_record0(Q_BarTime_2, "_��_ƽ��", NewPrice, NewPrice);
			}

			strcpy(INSTRUMENT_ID,"rb1410");		//����
			SendOrder(INSTRUMENT_ID, 1, 0);	

			BuyPrice=0;
			SellPrice=NewPrice;
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_����" << endl;
			_record0(Q_BarTime_2, "_��_����", SellPrice, SellPrice);
		}

}





void StopLoss(double system_times)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b);
	void _record0(double system_times, char *txt, double SPrice, double BPrice);
	
	bool stopwin =true;	//ʹ��ֹӯ
	bool stoploss=true;	//ʹ��ֹ��
	bool stoptime=true;	//ʹ������ƽ��

	double surplus	=20;	//ֹӮ����
	double Stopline	=30;	//ֹ�����

		//ֹӮƽ��
	if (BuySignal == true && (NewPrice-BuyPrice) >= surplus && stopwin)
		{
			SendOrder(INSTRUMENT_ID, 1, 3);
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ��ֹӮ_" << BuyPrice << endl;
			_record0(system_times, "_��_ƽ��ֹӮ_", BuyPrice, BuyPrice);
		}
	if (SellSignal == true && (SellPrice-NewPrice) >= surplus && stopwin)
		{
			SendOrder(INSTRUMENT_ID, 0, 3);
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ��ֹӮ_" << SellPrice << endl;
			_record0(system_times, "_��_ƽ��ֹӮ_", SellPrice, SellPrice);
		}

		//ֹ��ƽ��
	if (BuySignal == true && (BuyPrice-NewPrice)>Stopline && stoploss)
		{
			SendOrder(INSTRUMENT_ID,1,3);
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ��ֹ��_" << BuyPrice << endl;
			_record0(system_times, "_��_ƽ��ֹ��_", BuyPrice, BuyPrice);
		}
	if (SellSignal == true && (NewPrice-SellPrice)>Stopline && stoploss)
		{
			SendOrder(INSTRUMENT_ID,0,3);
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ��ֹ��_" << SellPrice << endl;
			_record0(system_times, "_��_ƽ��ֹ��_", SellPrice, SellPrice);
		}

		//����ƽ��
	if (BuySignal == true && Q_BarTime_2>0.1455 && stoptime)
		{
			SendOrder(INSTRUMENT_ID,1,3);
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ������_" << BuyPrice << endl;
			_record0(system_times, "_��_ƽ������_", BuyPrice, BuyPrice);
		}
	if (SellSignal == true && Q_BarTime_2>0.1455 && stoptime)
		{
			SendOrder(INSTRUMENT_ID,0,3);
			cerr << "--->>> " << TradingDay << "_" << Q_BarTime_s << "_" << INSTRUMENT_ID << "_��_ƽ������_" << SellPrice << endl;
			_record0(system_times, "_��_ƽ������_", SellPrice, SellPrice);
		}

}