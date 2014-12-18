#pragma once
#include "MFCExControlAPI.h"

typedef struct tagSPINTEXT
{
    TCHAR szTitle[1024];
    int iPercent;
    int nItem;
    int nColumn;
} SPINTEXT, FAR* LPSPINTEXT;


class MFCEXCONTROL_API CEditSpinWnd : public CWnd
{
public:
    CEditSpinWnd();
    virtual ~CEditSpinWnd();

public:
    int m_nSpinEditLower;
    int m_nSpinEditUpper;

protected:
    CEdit m_Edit;
    CSpinButtonCtrl m_Spin;
    CString m_strTitle;
    int m_iPercent;
    int m_iDisplayType;
    bool m_bSetPosFlag;

public:
    void SetPercent(int iPercent);
    void SetTitle(CString strTitle);
    void SetDisplayType(int _iDisplayType);
    void SetSpinRange(int _nLower, int _nUpper);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnKillfocusMyEdit();
    afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};