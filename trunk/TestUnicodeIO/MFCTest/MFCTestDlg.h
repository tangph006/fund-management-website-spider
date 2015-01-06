#pragma once

class CMFCTestDlg : public CDialogEx
{
public:
    CMFCTestDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_MFCTEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBegin();
    int m_nTotal;
    afx_msg void OnBnClickedBtnBegin2();
};
