/*
*  HistoryEdit.h
*
*  Description:
*    CHistoryEdit interface
*
*    A CEdit subclass that allows you to display a scrolling history
*    of text entries.
*
*  Author:
*    Ravi Bhavnani (ravib@datablast.net)
*
*  Revision History:
*    15 Mar 1998   rab   Original version
*/

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CHistoryEdit window

class CHistoryEdit : public CEdit
{
    // Construction
public:
    CHistoryEdit();

    // Attributes
public:

    // Operations
public:
    void  AppendString (CString str);
    BOOL  IsSelectable() { return m_bSelectable; }
    void  AllowSelection (BOOL bAllowSelect) { m_bSelectable = bAllowSelect; }

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHistoryEdit)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CHistoryEdit();

    // Generated message map functions
protected:
    //{{AFX_MSG(CHistoryEdit)
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    BOOL  m_bSelectable;                          // flag: user can select text in control
};

