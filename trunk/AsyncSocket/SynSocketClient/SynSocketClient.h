#if !defined(AFX_SYNSOCKETCLIENT_H__906F4696_4210_4A81_98D2_989E3FEE68E4__INCLUDED_)
#define AFX_SYNSOCKETCLIENT_H__906F4696_4210_4A81_98D2_989E3FEE68E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
class CSynSocketClientApp : public CWinApp
{
public:
	CSynSocketClientApp();
	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SYNSOCKETCLIENT_H__906F4696_4210_4A81_98D2_989E3FEE68E4__INCLUDED_)
