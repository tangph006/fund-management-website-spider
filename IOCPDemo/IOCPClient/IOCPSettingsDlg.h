#pragma once

class IOCPSettingsDlg : public CDialog
{
public:
    IOCPSettingsDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_SETTINGS };
    int  m_iMaxNumberOfConnections;
    int  m_iMaxNrOfFreeBuff;
    int  m_iMaxNrOfFreeContext;
    int  m_iNrOfIOWorkers;
    int  m_iNrOfLogicalWorkers;
    int  m_iPortNr;
    BOOL m_bReadInOrder;
    BOOL m_iSendInOrder;
    int  m_iNrPendlingReads;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
protected:
    DECLARE_MESSAGE_MAP()
};

