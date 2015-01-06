#include "stdafx.h"
#include "MFCTest.h"
#include "MFCTestDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCTestDlg::CMFCTestDlg(CWnd* pParent)
    : CDialogEx(CMFCTestDlg::IDD, pParent)
    , m_nTotal(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TOTAL, m_nTotal);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_BEGIN, &CMFCTestDlg::OnBnClickedBegin)
    ON_BN_CLICKED(IDC_BTN_BEGIN2, &CMFCTestDlg::OnBnClickedBtnBegin2)
END_MESSAGE_MAP()

BOOL CMFCTestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    m_nTotal = 11;
    UpdateData(FALSE);
    return TRUE;
}

void CMFCTestDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CMFCTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMFCTestDlg::OnBnClickedBegin()
{
    UpdateData(TRUE);
    std::vector<int> vec_1_2(m_nTotal+1, -1);
    for(int i=0; i<=m_nTotal; i++)
    {
        vec_1_2[i] = 1+i/2;
    }

    std::vector<int> vec_1_2_5(vec_1_2.begin(), vec_1_2.end());
    for(int i=5; i<=m_nTotal; i++)
    {
        for(int j=1; j<=i/5; j++)
        {
            vec_1_2_5[i] += vec_1_2[i-j*5];
        }
    }

    std::vector<int> vec_1_2_5_10(vec_1_2_5.begin(), vec_1_2_5.end());
    for(int i=10; i<=m_nTotal; i++)
    {
        for(int j=1; j<=i/10; j++)
        {
            vec_1_2_5_10[i] += vec_1_2_5[i-j*10];
        }
    }

    std::vector<int> vec_1_2_5_10_20(vec_1_2_5_10.begin(), vec_1_2_5_10.end());
    for(int i=20; i<=m_nTotal; i++)
    {
        for(int j=1; j<=i/20; j++)
        {
            vec_1_2_5_10_20[i] += vec_1_2_5_10[i-j*20];
        }
    }
}


void CMFCTestDlg::OnBnClickedBtnBegin2()
{
    UpdateData(TRUE);
    int nCountOf5 = 0;
    for(int j=1; j<=m_nTotal; j++)
    {
        int temp = j;
        while(temp%5 == 0 && temp >0)
        {
            temp = temp/5;
            nCountOf5++;
        }
    }
}
