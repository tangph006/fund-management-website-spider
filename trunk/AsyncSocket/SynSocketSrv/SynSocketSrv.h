// SynSocketSrv.h : main header file for the SYNSOCKETSRV application
//

#if !defined(AFX_SYNSOCKETSRV_H__3C7DE2C5_A5DB_4BF7_AAFA_15406A99D12D__INCLUDED_)
#define AFX_SYNSOCKETSRV_H__3C7DE2C5_A5DB_4BF7_AAFA_15406A99D12D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSynSocketSrvApp:
// See SynSocketSrv.cpp for the implementation of this class
//

class CSynSocketSrvApp : public CWinApp
{
public:
	CSynSocketSrvApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynSocketSrvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSynSocketSrvApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNSOCKETSRV_H__3C7DE2C5_A5DB_4BF7_AAFA_15406A99D12D__INCLUDED_)
