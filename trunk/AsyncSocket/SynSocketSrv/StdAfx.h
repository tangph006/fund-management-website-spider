#if !defined(AFX_STDAFX_H__68A0A0B6_E182_45E9_8E51_CA0187399D94__INCLUDED_)
#define AFX_STDAFX_H__68A0A0B6_E182_45E9_8E51_CA0187399D94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif
#define WM_SOCKET_EVENT WM_USER+1
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib") 

#endif // !defined(AFX_STDAFX_H__68A0A0B6_E182_45E9_8E51_CA0187399D94__INCLUDED_)
