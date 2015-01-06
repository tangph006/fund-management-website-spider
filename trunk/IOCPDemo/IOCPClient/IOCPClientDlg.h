#pragma once
#include "MyListCtrl.h"
#include "HistoryEdit.h"
#include "MyIOCP.h"
#include "IOCPSettingsDlg.h"
class CIOCPClientDlg : public CDialog
{
public:
    CIOCPClientDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_IOCPCLIENT_DIALOG };
    CHistoryEdit m_CtrlLogg;
    MyListCtrl m_CtrlClientList;
    CString m_Adress;
    CString m_sReceivedTxt;
    CString m_sSendTxt;
    BOOL m_bFlood;
    int  m_MsgPerSec;
    CString m_sDownloadFilePath;
    BOOL m_bAutoReconnect;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
protected:
    HICON m_hIcon;
    MyIOCP m_iocp;
    IOCPSettingsDlg m_ConfigDlg;
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    afx_msg void OnConnect();
    afx_msg void OnDestroy();
    afx_msg void OnDisconnectall();
    afx_msg void OnSend();
    afx_msg void OnDisconnect();
    afx_msg void OnClickClientlist(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnFlood();
    afx_msg void OnStopstart();
    afx_msg void OnSettings();
    afx_msg void OnAutoreconnect();
    DECLARE_MESSAGE_MAP()
private:
    void UpdateClientData();
    void UpdateList();
    void EnableClientPanel();
    void DisableClientPanel();
    void DisableAllPanels();
    void EnableAllPanels();
    void StartTheSystem();
    LRESULT OnNewClient(WPARAM wParam, LPARAM lParam);
    LRESULT OnAppendLog(WPARAM wParam, LPARAM lParam);
    LRESULT OnClientDisconnected(WPARAM wParam, LPARAM lParam);
    int m_ihTimer;
    int m_iCurrentClientID;
    // We put all the Context String2Pointer HashMap copy of ContextMapLock
    // used to update user info about the clients. 
    CCriticalSection m_GUIListLock;
    //CMapStringToPtr m_GUIContextMap;
    // The path which we download the file into. 

public:
    // The port number to use when connecting 
    int m_iPort;
};

