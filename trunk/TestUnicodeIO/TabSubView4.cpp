#include "stdafx.h"
#include "TestUnicodeIO.h"
#include "TabSubView4.h"
#include <algorithm>

static void bs(std::vector<int>& pArray)
{
    int nCount = (int)pArray.size();
    for(int i=nCount-1; i>=0; i--)
    {
        for(int j=0; j<i; j++)
        {
            int nTemp;
            if(pArray[j] > pArray[j+1])
            {
                nTemp = pArray[j];
                pArray[j] = pArray[j+1];
                pArray[j+1] = nTemp;
            }
        }
    }
}

typedef std::vector<int>::iterator intItor;

typedef struct 
{
    intItor m_first;
    intItor m_last;
} IteratorRange;


static void iter_swap_my(intItor first, intItor second)
{
    *first += *second;
    *second = *first - *second;
    *first -= *second;
}

static void _Med3_my(intItor first_, intItor mid_, intItor last_)
{
    if(*mid_ < *first_)
        iter_swap_my(mid_, first_);

    if(*last_ < *mid_)
        iter_swap_my(last_, mid_);

    if(*mid_ < *first_)
        iter_swap_my(mid_, first_);
}

static void _Median_my(intItor first_, intItor mid_, intItor last_)
{
    int length_ = last_ - first_;
    if(length_ > 40)
    {
        int nStep = (length_+1)/8;
        _Med3_my(first_,         first_+nStep,    first_+2*nStep);
        _Med3_my(mid_-nStep,     mid_,            mid_+nStep);
        _Med3_my(last_-2*nStep,   last_-nStep,    last_);
        _Med3_my(first_+nStep,   mid_,            last_-nStep);
    }
    else
    {
        _Med3_my(first_,         mid_,            last_);
    }
}

static IteratorRange _Unguarded_partition_my(intItor first_, intItor last_)
{
    intItor mid_ = first_ + (last_-first_)/2;
    _Median_my(first_, mid_, last_);
    intItor mid_begin_ = mid_;
    intItor mid_end_ = mid_begin_ + 1;

    while(first_<mid_begin_ && *(mid_begin_-1) == *mid_begin_)
        --mid_begin_;

    while(mid_end_<last_ && *mid_end_ == *mid_begin_)
        ++mid_end_;

    intItor right_begin_ = mid_end_;
    intItor left_end_ = mid_begin_;

    for( ; ; )
    {
        int ndiff = last_-right_begin_;
        for(; right_begin_ < last_; ++right_begin_)
        {
            if(*mid_begin_ < *right_begin_)
            {

            }
            else if(*right_begin_ < *mid_begin_)
            {
                break;
            }
            else
            {
                 iter_swap_my(mid_end_, right_begin_);
                 ++mid_end_;
            }
        }

        for(; first_<left_end_; --left_end_)
        {
            if(*(left_end_-1) < *mid_begin_)
            {

            }
            else if(*mid_begin_ < *(left_end_-1))
            {
                break;
            }
            else
            {
                --mid_begin_;
                iter_swap_my(mid_begin_, left_end_-1);
            }
        }

        if(left_end_ == first_ && right_begin_ == last_)
        {
            IteratorRange pair_;
            pair_.m_first = mid_begin_;
            pair_.m_last = mid_end_;
            return pair_;
        }

        if(left_end_ == first_)
        {
            if(mid_end_ != right_begin_)
            {
                iter_swap_my(mid_begin_, mid_end_);
            }
            ++mid_end_;
            iter_swap_my(mid_begin_, right_begin_);
            ++mid_begin_;
            ++right_begin_;
        }
        else if(right_begin_ == last_)
        {
            --left_end_;
            --mid_begin_;
            if(left_end_ != mid_begin_)
            {
                iter_swap_my(left_end_, mid_begin_);
            }
            --mid_end_;
            iter_swap_my(mid_begin_, mid_end_);
        }
        else
        {
            --left_end_;
            iter_swap_my(right_begin_, left_end_);
            ++right_begin_;
        }
    }
}

static void _Push_heap_my(intItor first_, int hole_, int top_, int val_)
{
    for(int idx_ = (hole_-1)/2; top_<hole_ && *(first_+idx_)<val_; idx_ = (hole_-1)/2)
    {
        *(first_+hole_) = *(first_+idx_);
        hole_ = idx_;
    }
    *(first_+hole_) = val_;
}

static void _Adjust_heap_my(intItor first_, int hole_, int bottom_, int val_)
{
    int top_ = hole_;
    int idx_ = 2*hole_ + 2;
    for(; idx_<bottom_; idx_ = idx_*2 +2)
    {
        if(*(first_+idx_) < *(first_+idx_-1))
        {
            --idx_;
        }
        *(first_+hole_) = *(first_+idx_);
        hole_ = idx_;
    }

    if(idx_ == bottom_)
    {
        *(first_+hole_) = *(first_+bottom_-1);
        hole_ = bottom_ - 1;
    }

    _Push_heap_my(first_, hole_, top_, val_);
}

static void _Make_heap_my(intItor first_, intItor last_)
{
    int bottom_ = last_ - first_;
    for(int hole_ = bottom_/2; hole_>0; )
    {
        --hole_;
        int val_ = *(first_ + hole_);
        _Adjust_heap_my(first_, 0, last_-first_, val_);
    }
}

static void make_heap_my(intItor first_, intItor last_)
{
    if(last_ - first_ > 1)
    {
        _Make_heap_my(first_, last_);
    }
}

void _Pop_heap_my(intItor first_, intItor last_, intItor dest_, int val_)
{
    *dest_ = *first_;
    _Adjust_heap_my(first_, 0, last_-first_, val_);
}

static void _Pop_heap_0_my(intItor first_, intItor last_)
{
    int val_ = *(last_-1);
    _Pop_heap_my(first_, last_-1, last_-1, val_);
}

static void _Pop_heap_my(intItor first_, intItor last_)
{
    _Pop_heap_0_my(first_, last_);
}

static void _Sort_heap_my(intItor first_, intItor last_)
{
    for( ; last_-first_ > 1; --last_)
    {
        _Pop_heap_my(first_, last_);
    }
}

static void sort_heap_my(intItor first_, intItor last_)
{
    _Sort_heap_my(first_, last_);
}

// sort [first_, last_)
static void _Sort_my(intItor first_, intItor last_)
{
    int count_;
    for(; (count_= last_ - first_)>32; )
    {
        IteratorRange mid_ = _Unguarded_partition_my(first_, last_);
        if(mid_.m_first - first_ < last_ - mid_.m_last)
        {
            _Sort_my(first_, mid_.m_first);
            first_ = mid_.m_last;
        }
        else
        {
            _Sort_my(mid_.m_last, last_);
            last_ = mid_.m_first;
        }
    }

    if(1<count_ && count_<=32)
    {
        make_heap_my(first_, last_);
        sort_heap_my(first_, last_);
    }
    else if(count_ > 32)
    {
        make_heap_my(first_, last_);
        sort_heap_my(first_, last_);
    }
}


IMPLEMENT_DYNAMIC(CTabSubView4, CDialog)

CTabSubView4::CTabSubView4(CWnd* pParent) : CDialog(CTabSubView4::IDD, pParent)
{

}

CTabSubView4::~CTabSubView4()
{
}

BEGIN_MESSAGE_MAP(CTabSubView4, CDialog)
    ON_WM_SIZE()
    ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_VIRTUAL, OnLvnGetdispinfoListVirtual)
    ON_BN_CLICKED(IDC_BTN_REFRESH, OnBnClickedBtnRefresh)
    ON_BN_CLICKED(IDC_BTN_SORT, OnBnClickedBtnSort)
END_MESSAGE_MAP()

void CTabSubView4::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_VIRTUAL, m_lstVirtual);
}


void CTabSubView4::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if(m_lstVirtual.GetSafeHwnd() == NULL)
    {
        if(nType != SIZE_MINIMIZED)
        {
            m_oldCx = cx;
            m_oldCy = cy;
        }
        return;
    }
    LayoutControl(&m_lstVirtual, TopLeft, BottomRight, cx, cy);
    LayoutControl(GetDlgItem(IDC_EDIT_DATACOUNT), TopRight, TopRight, cx, cy);
    LayoutControl(GetDlgItem(IDC_BTN_REFRESH), TopRight, TopRight, cx, cy);
    LayoutControl(GetDlgItem(IDC_BTN_SORT), TopRight, TopRight, cx, cy);
    LayoutControl(GetDlgItem(IDC_COMBO_SORTTYPE), TopRight, TopRight, cx, cy);
    LayoutControl(GetDlgItem(IDC_STATIC_ELAPSED), TopRight, TopRight, cx, cy);
    if(nType != SIZE_MINIMIZED)
    {
        m_oldCx = cx;
        m_oldCy = cy;
    }
    InvalidateRect(NULL);
}

BOOL CTabSubView4::OnInitDialog()
{
    CDialog::OnInitDialog();
    GetDlgItem(IDC_EDIT_DATACOUNT)->SetWindowText(_T("1000"));
    InitData();

    DWORD dwStyle = m_lstVirtual.GetExtendedStyle();
    m_lstVirtual.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
    LV_COLUMN lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT;
    lvc.pszText = _T("index");
    lvc.cx = 140;
    m_lstVirtual.InsertColumn(0, &lvc);
    lvc.pszText = _T("int1");
    m_lstVirtual.InsertColumn(1, &lvc);
    lvc.pszText = _T("int2");
    m_lstVirtual.InsertColumn(2, &lvc);

    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("bs"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("qs"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("std::sort"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("bs"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("bs"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("bs"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->AddString(_T("bs"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_SORTTYPE))->SetCurSel(0);

    return TRUE;
}

BOOL CTabSubView4::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN)
    {
        switch(pMsg->wParam)
        {
        case VK_RETURN:
            return TRUE;
        case VK_ESCAPE:
            return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CTabSubView4::LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy)
{
    CRect rcS;
    pCtrl->GetWindowRect(&rcS);
    ScreenToClient(&rcS);
    int deltaX = cx - m_oldCx;
    int deltaY = cy - m_oldCy;
    if(refTopLeft == TopLeft && refBottomRight == TopLeft)
    {
        pCtrl->MoveWindow(&rcS);
    }
    else if(refTopLeft == TopLeft && refBottomRight == TopRight)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top, rcS.Width()+deltaX, rcS.Height());
    }
    else if(refTopLeft == TopLeft && refBottomRight == BottomLeft)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top, rcS.Width(), rcS.Height()+deltaY);
    }
    else if(refTopLeft == TopLeft && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top, rcS.Width()+deltaX, rcS.Height()+deltaY);
    }
    else if(refTopLeft == TopRight && refBottomRight == TopRight)
    {
        pCtrl->MoveWindow(rcS.left+deltaX, rcS.top, rcS.Width(), rcS.Height());
    }
    else if(refTopLeft == TopRight && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left+deltaX, rcS.top, rcS.Width(), rcS.Height()+deltaY);
    }
    else if(refTopLeft == BottomLeft && refBottomRight == BottomLeft)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top+deltaY, rcS.Width(), rcS.Height());
    }
    else if(refTopLeft == BottomLeft && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left, rcS.top+deltaY, rcS.Width()+deltaX, rcS.Height());
    }
    else if(refTopLeft == BottomRight && refBottomRight == BottomRight)
    {
        pCtrl->MoveWindow(rcS.left+deltaX, rcS.top+deltaY, rcS.Width(), rcS.Height());
    }
}

void CTabSubView4::InitData()
{
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    srand(sysTime.wMilliseconds%100);

    CString strEdit;
    GetDlgItemText(IDC_EDIT_DATACOUNT, strEdit);
    int nCount = wcstol(strEdit, NULL, 10);
    m_vInt.clear();
    for(int i=0; i<nCount; i++)
    {
        m_vInt.push_back(rand()%nCount);
    }
    m_lstVirtual.SetItemCount(nCount);
}

void CTabSubView4::OnLvnGetdispinfoListVirtual(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    int iItem = pDispInfo->item.iItem;
    int iSubItem = pDispInfo->item.iSubItem;
    if(pDispInfo->item.mask & LVIF_TEXT)
    {
        switch(iSubItem)
        {
        case 0:
            {
                CString strItem;
                strItem.Format(_T("%d"), iItem+1);
                lstrcpy(pDispInfo->item.pszText, strItem);
            }
            break;
        case 1:
            {
                CString strItem;
                strItem.Format(_T("%d"), m_vInt[iItem]);
                lstrcpy(pDispInfo->item.pszText, strItem);
            }
            break;
        case 2:
            {
                CString strItem;
                strItem.Format(_T("%d"), m_vInt[iItem]);
                lstrcpy(pDispInfo->item.pszText, strItem);
            }
            break;
        default:
            break;
        }
    }

    if(pDispInfo->item.mask & LVIF_IMAGE)
    {

    }

    *pResult = 0;
}


void CTabSubView4::OnBnClickedBtnRefresh()
{
    InitData();
}


void CTabSubView4::OnBnClickedBtnSort()
{
    CString strSortType;
    GetDlgItemText(IDC_COMBO_SORTTYPE, strSortType);
    ULONGLONG timeTick = GetTickCount64();

    LARGE_INTEGER time_start;
    LARGE_INTEGER time_end;
    double dqFreq;
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    dqFreq=(double)f.QuadPart;
    QueryPerformanceCounter(&time_start);
    if(strSortType.CompareNoCase(_T("bs")) == 0)
    {
        bs(m_vInt);
    }
    else if(strSortType.CompareNoCase(_T("qs")) == 0)
    {
        _Sort_my(m_vInt.begin(), m_vInt.end()-1);
    }
    else if(strSortType.CompareNoCase(_T("std::sort")) == 0)
    {
        std::sort(m_vInt.begin(), m_vInt.end());
    }
    QueryPerformanceCounter(&time_end);
    CString strElapsed;
    strElapsed.Format(_T("%.06fs"), (time_end.QuadPart-time_start.QuadPart)/dqFreq);
    GetDlgItem(IDC_STATIC_ELAPSED)->SetWindowText(strElapsed);

    CRect rcListVirtual;
    m_lstVirtual.GetWindowRect(rcListVirtual);
    ScreenToClient(rcListVirtual);
    InvalidateRect(rcListVirtual);
}
