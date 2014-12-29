#pragma once
#include <vector>
#include "StockListCtrl.h"

class VecIntSorter
{
public:
    VecIntSorter(const std::vector<int> vec)
    {
        m_vec.resize(vec.size());
        m_vec = vec;
    }
    ~VecIntSorter(){}

    typedef struct 
    {
        size_t m_first;
        size_t m_last;
    } IteratorRange;

protected:
    std::vector<int> m_vec;
public:
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


    std::vector<int>& get_sorted_vec()
    {
        _Sort_my(0, m_vec.size()-1);
        return m_vec;
    }

protected:
    // sort [m_vec[first_], m_vec[end]]
    void _Sort_my(size_t first_, size_t last_)
    {
        int count_ = last_-first_+1;
        while(count_ > 32)
        {
            IteratorRange mid_ = _Unguarded_partition_my(first_, last_);
            if(mid_.m_first - first_ < last_ - mid_.m_last)
            {
                _Sort_my(first_, mid_.m_first-1);
                first_ = mid_.m_last+1;
            }
            else
            {
                _Sort_my(mid_.m_last+1, last_);
                last_ = mid_.m_first-1;
            }
            count_ = last_-first_+1;
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
    void iter_swap_my(size_t first, size_t second)
    {
        m_vec[first] += m_vec[second];
        m_vec[second] = m_vec[first] - m_vec[second];
        m_vec[first] -= m_vec[second];
    }
    void _Med3_my(size_t first_, size_t mid_, size_t last_)
    {
        if(m_vec[mid_] < m_vec[first_])
            iter_swap_my(mid_, first_);

        if(m_vec[last_] < m_vec[mid_])
            iter_swap_my(last_, mid_);

        if(m_vec[mid_] < m_vec[first_])
            iter_swap_my(mid_, first_);
    }
    void _Median_my(size_t first_, size_t mid_, size_t last_)
    {
        int length_ = last_ - first_ +1;
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
    IteratorRange _Unguarded_partition_my(size_t first_, size_t last_)
    {
        size_t mid_ = first_ + (last_-first_)/2;
        _Median_my(first_, mid_, last_);
        size_t mid_begin_ = mid_;
        size_t mid_end_ = mid_begin_ + 1;

        while(first_<mid_begin_ && m_vec[mid_begin_-1] == m_vec[mid_begin_])
            --mid_begin_;

        while(mid_end_<last_ && m_vec[mid_end_] == m_vec[mid_begin_])
            ++mid_end_;

        size_t right_begin_ = mid_end_;
        size_t left_end_ = mid_begin_;

        for( ; ; )
        {
            for(; right_begin_ < last_; right_begin_++)
            {
                if(m_vec[right_begin_] > m_vec[mid_begin_])
                {

                }
                else if(m_vec[right_begin_] < m_vec[mid_begin_])
                {
                    break;
                }
                else
                {
                    iter_swap_my(right_begin_, mid_end_);
                    ++mid_end_;
                }
            }

            for(; left_end_>first_; left_end_--)
            {
                if(m_vec[left_end_-1] < m_vec[mid_begin_])
                {

                }
                else if(m_vec[left_end_-1] > m_vec[mid_begin_])
                {
                    break;
                }
                else
                {
                    --mid_begin_;
                    iter_swap_my(left_end_-1, mid_begin_);
                }
            }

            if(left_end_ == first_ && right_begin_ == last_)
            {
                IteratorRange pair_;
                pair_.m_first = mid_begin_;
                pair_.m_last = mid_end_-1;
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
    void _Push_heap_my(size_t first_, int hole_, int top_, int val_)
    {
        for(int idx_ = (hole_-1)/2; top_<hole_ && m_vec[first_+idx_]<val_; idx_ = (hole_-1)/2)
        {
            m_vec[first_+hole_] = m_vec[first_+idx_];
            hole_ = idx_;
        }
        m_vec[first_+hole_] = val_;
    }
    void _Adjust_heap_my(size_t first_, int hhh121, int bottom_, int val_)
    {
        int top_ = hhh121;
        int idx_ = 2*hhh121 + 2;
        for(; idx_<bottom_; idx_ = idx_*2 +2)
        {
            if(m_vec[first_+idx_] < m_vec[first_+idx_-1])
            {
                --idx_;
            }
            m_vec[first_+hhh121] = m_vec[first_+idx_];
            hhh121 = idx_;
        }

        if(idx_ == bottom_)
        {
            m_vec[first_+hhh121] = m_vec[first_+bottom_-1];
            hhh121 = bottom_ - 1;
        }

        for(int idx_ = (hhh121-1)/2; top_<hhh121 && m_vec[first_+idx_]<val_; idx_ = (hhh121-1)/2)
        {
            m_vec[first_+hhh121] = m_vec[first_+idx_];
            hhh121 = idx_;
        }
        m_vec[first_+hhh121] = val_;
    }
    void _Make_heap_my(size_t first_, size_t last_)
    {
        int count_ = last_-first_+1;
        for(int cur_=count_/2; cur_>0; cur_--)
        {
            int parent = first_+cur_-1;
            int leftchild = first_+cur_*2-1;
            int rightchild = first_+cur_*2;
            if(m_vec[parent] > m_vec[leftchild])
            {
                iter_swap_my(parent, leftchild);
            }
            if(m_vec[parent] > m_vec[rightchild])
            {
                iter_swap_my(parent, rightchild);
            }
        }
    }
    void make_heap_my(size_t first_, size_t last_)
    {
        if(last_ - first_ > 1)
        {
            _Make_heap_my(first_, last_);
        }
    }
    void _Pop_heap_my(size_t first_, size_t last_, size_t dest_, int val_)
    {
        m_vec[dest_] = m_vec[first_];
        _Adjust_heap_my(first_, 0, last_-first_, val_);
    }
    void _Pop_heap_0_my(size_t first_, size_t last_)
    {
        int val_ = m_vec[last_-1];
        _Pop_heap_my(first_, last_-1, last_-1, val_);
    }
    void _Pop_heap_my(size_t first_, size_t last_)
    {
        _Pop_heap_0_my(first_, last_);
    }
    void _Sort_heap_my(size_t first_, size_t last_)
    {
        for( ; last_-first_ > 1; --last_)
        {
            _Pop_heap_my(first_, last_);
        }
    }
    void sort_heap_my(size_t first_, size_t last_)
    {
        _Sort_heap_my(first_, last_);
    }
};

class CTabSubView4 : public CDialog
{
    DECLARE_DYNAMIC(CTabSubView4)

public:
    CTabSubView4(CWnd* pParent = NULL);
    virtual ~CTabSubView4();
    enum { IDD = IDD_TABSUBVIEW4 };

protected:
    CStockListCtrl m_lstVirtual;
    std::vector<int> m_vInt;

protected:
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void RefreshData();
    DECLARE_MESSAGE_MAP()
private:
    int m_oldCx;
    int m_oldCy;
    typedef enum {TopLeft, TopRight, BottomLeft, BottomRight} LayoutRef;
    void LayoutControl(CWnd* pCtrl, LayoutRef refTopLeft, LayoutRef refBottomRight, int cx, int cy);
public:
    afx_msg void OnLvnGetdispinfoListVirtual(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnRefresh();
    afx_msg void OnBnClickedBtnSort();
};
