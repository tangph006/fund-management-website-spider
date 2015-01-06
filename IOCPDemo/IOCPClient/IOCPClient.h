#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIOCPClientApp:
// See IOCPClient.cpp for the implementation of this class
//
class CIOCPClientApp : public CWinApp
{
public:
    CIOCPClientApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};


