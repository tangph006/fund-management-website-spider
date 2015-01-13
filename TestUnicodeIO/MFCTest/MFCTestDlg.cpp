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
    LARGE_INTEGER beginTime;
    QueryPerformanceCounter(&beginTime);

    int* pInt1 = new int[m_nTotal+1];
    int* pInt2 = new int[m_nTotal+1];

    memset(pInt1, 0, sizeof(int)*(m_nTotal+1));
    for(int i=0; i<=m_nTotal; i++)
    {
        pInt1[i] = 1+i/2;
    }

    memset(pInt2, 0, sizeof(int)*(m_nTotal+1));
    for(int i=0; i<=m_nTotal; i++)
    {
        for(int j=0; j<=i/5; j++)
        {
            pInt2[i] += pInt1[i-j*5];
        }
    }

    memset(pInt1, 0, sizeof(int)*(m_nTotal+1));
    for(int i=0; i<=m_nTotal; i++)
    {
        for(int j=0; j<=i/10; j++)
        {
            pInt1[i] += pInt2[i-j*10];
        }
    }

    memset(pInt2, 0, sizeof(int)*(m_nTotal+1));
    for(int i=0; i<=m_nTotal; i++)
    {
        for(int j=0; j<=i/20; j++)
        {
            pInt2[i] += pInt1[i-j*20];
        }
    }

    LARGE_INTEGER endTime;
    QueryPerformanceCounter(&endTime);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    float fTime = (endTime.QuadPart-beginTime.QuadPart)/(float)(frequency.QuadPart);
    delete pInt1;
    delete pInt2;
}


void CMFCTestDlg::OnBnClickedBtnBegin2()
{
    UpdateData(TRUE);
    LARGE_INTEGER beginTime;
    QueryPerformanceCounter(&beginTime);

    int nCount =0;
    for(int i1=0; i1<=m_nTotal/20; i1++)
    {
        int nLeft1 = m_nTotal - i1*20;
        for(int i2=0; i2<=nLeft1/10; i2++)
        {
            int nLeft2 = nLeft1 - i2*10;
            for(int i3=0; i3<=nLeft2/5; i3++)
            {
                int nLeft3 = nLeft2 - i3*5;
                for(int i4=0; i4<=nLeft3/2; i4++)
                {
                    nCount++;
                }
            }
        }
    }

    LARGE_INTEGER endTime;
    QueryPerformanceCounter(&endTime);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    float fTime = (endTime.QuadPart-beginTime.QuadPart)/(float)(frequency.QuadPart);
    fTime = 0;
}
