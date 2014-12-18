#pragma once

class CSynSocketSrvDlg : public CDialog
{
public:
	void InitSocket();
	CSynSocketSrvDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_SYNSOCKETSRV_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	HICON m_hIcon;
	SOCKET m_socket;
	SOCKET m_clientSocket[5];

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSocketEvent(WPARAM  wParam ,LPARAM lParam) ;
	afx_msg void OnButtonSend();
	DECLARE_MESSAGE_MAP()
};
