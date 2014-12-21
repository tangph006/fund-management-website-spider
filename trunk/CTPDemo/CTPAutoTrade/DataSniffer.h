#pragma once

#include <string>
#include <iomanip>
#include <vector>
#include <math.h>
#include <windows.h>
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MyTraderSpi.h"
#include "MyMdSpi.h"

#pragma warning(disable : 4996)

#define EPSILON_E4 (float)(1E-2) 
#define EPSILON_E5 (float)(1E-3)


extern int TickABS;
extern double TickAPrice[4]; //
extern int TickBNum;
extern double TickBPrice[4]; //
extern bool CloseAll; //���̱�־


void Sniffer() //����Tick�����Ѿ�ָ����� ʵ����
{

    if (RunMode && Q_BarTime_2>=0.1500 && CloseAll==false)
    { 
        std::cerr << "--->>> " <<TradingDay<<"׼������!" << std::endl;
        std::cerr << "--->>> " <<"WriteConfiguration!" << std::endl;
        WriteConfiguration("./AutoTrader.cfg"); //��������
        Sleep(3000);
        //ErasingTradeConfiguration();
        std::cerr << "--->>> " <<TradingDay<<"����!" << std::endl;
        CloseAll=true;
    }

    if (TickAPrice[0]>TickAPrice[1] && TickAPrice[1]>TickAPrice[2] && TickAPrice[2]>TickAPrice[3])
    {
        TickABS=1; //����3��TICK�ǣ�buy
    }
    else if (TickAPrice[0]<TickAPrice[1] && TickAPrice[1]<TickAPrice[2] && TickAPrice[2]<TickAPrice[3])
    {
        TickABS=2; //����3��TICK����Sell
    }
    else
    {
        TickABS=0;
    } 
}

