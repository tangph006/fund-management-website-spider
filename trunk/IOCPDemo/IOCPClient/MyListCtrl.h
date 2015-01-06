#pragma once
#include "HistoryEdit.h"
#ifndef __LIST_ITEMINFO
#define __LIST_ITEMINFO

/*
*	Structure to contain the list data. 
*/
typedef struct tagITEMINFO { 
    unsigned int m_ID;
    CString  m_ClientAddress;
    int m_iListIndex; // to Know what index the item has in the list.  
    int m_iNumberOfReceivedMsg;
    unsigned int m_iMaxFileBytes;
    unsigned int m_iFileBytes; 
    BOOL m_bFileSendMode;
    BOOL m_bFileReceivedMode;

}ITEMINFO;

class MyListCtrl : public CListCtrl
{
public:
    MyListCtrl();
public:
    BOOL Select(int index);
    BOOL FreeItem(int Index);
    void ReSort();
    BOOL SetHeaderIcon(int iIndex,int iPictureIndex);
    ITEMINFO* Find(CString ID);
    ITEMINFO* Select(CString ID);
    void SetIconSize(int x,int y);
    //BOOL DBLoadIcon();
    //BOOL DBLoadData(CString kategori="");
    //BOOL SqlInit(CString username,CString password,CString  hostadress,int Listtype, CHistoryEdit *logg=NULL);
    BOOL DeleteSelectedItem();


    BOOL init();

    ITEMINFO* GetSelectedItem();
    BOOL DeleteAllItems();
    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    static int CALLBACK CompareFunc2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    BOOL FreeListItems();
    int AddItemToList(int nIndex,int iImage=0, ITEMINFO *pItem=NULL);
    int AddItemToList(ITEMINFO *pItem, int iImage=0);
    virtual ~MyListCtrl();
    //
    // Data base functions
    //
    //BYTE* DBLoadImage(Connection *con,long* size,CString stable,CString sblobfield,CString sKeyField,CString sKey);

    // Generated message map functions
protected:

    // Header and Column managment 
    // Current column nr which we sort by.
    int m_iCurrentSortColumn;
    int m_iNumberOfColumns;

    int m_iColumnWidthArray[10];
    // The Image List for the columns.
    CImageList m_HeaderImages;
    // The dirrection of the sort. 
    int m_sortdirection;

    // Other configs 
    int m_ListType;
    CImageList m_ImageListThumb;

    // The size of the icons
    int m_IconWidth;
    int m_IconHeight;
    BOOL m_ShowIcons;
    afx_msg void OnDestroy();
    afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCustomdraw( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
private:
    void ResizeColumns(int cx=-1);
};

#endif//__LIST_ITEMINFO
