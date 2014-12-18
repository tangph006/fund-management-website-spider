#if !defined(AFX_SYNSOCKETCLIENTDLG_H__00C4517C_5FE6_4BA8_9675_34CDC4377BDA__INCLUDED_)
#define AFX_SYNSOCKETCLIENTDLG_H__00C4517C_5FE6_4BA8_9675_34CDC4377BDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSynSocketClientDlg : public CDialog
{
public:
	void InitSocket();
	CSynSocketClientDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_SYNSOCKETCLIENT_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	HICON m_hIcon;
	SOCKET m_socket ;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	afx_msg LRESULT OnSocket(WPARAM wParam ,LPARAM lParam) ;
	afx_msg void OnBtnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_SYNSOCKETCLIENTDLG_H__00C4517C_5FE6_4BA8_9675_34CDC4377BDA__INCLUDED_)
