#include "StdAfx.h"
#include ".\cooltreedatetimectrl.h"
#include "CoolTree.h"

CCoolTreeDateTimeCtrl::CCoolTreeDateTimeCtrl(HTREEITEM hItem)
{
	m_hItem = hItem;
}

CCoolTreeDateTimeCtrl::~CCoolTreeDateTimeCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CCoolTreeDateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CCoolTreeDateTimeCtrl)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCoolTreeDateTimeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	COleDateTime oleDateTime;
	GetTime(oleDateTime);

	long lSecondTime = oleDateTime.GetHour()*3600l + oleDateTime.GetMinute()*60l + oleDateTime.GetSecond();

	GetParent()->SendMessage(UM_CEW_DATETIME_END,(WPARAM)m_hItem,(LPARAM)lSecondTime);

	ShowWindow(SW_HIDE);

	CDateTimeCtrl::OnKillFocus(pNewWnd);
}