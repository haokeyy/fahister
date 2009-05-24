#pragma once

class CListViewHelp
{
public:
    CListViewHelp();
    ~CListViewHelp();
    static void AddListItem(CListCtrl& listCtrl, CString itemText , CString itemValue);
    static void ChangeListItem(CListCtrl& listCtrl, int nItem, CString itemText , CString itemValue);
    static void ChangeListItemValue(CListCtrl& listCtrl, int nItem, CString itemValue);
    static void ResetItemId(CListCtrl& listCtrl);
    static void DeleteSelectedItem(CListCtrl& listCtrl);
    static int GetSelectedItem(CListCtrl& listCtrl);
    static void SetSelectedItem(CListCtrl& listCtrl, int pos);
    static CString GetSelectedItemText(CListCtrl& listCtrl);
    static CString GetSelectedItemValue(CListCtrl& listCtrl);
    static CString GetItemText(CListCtrl& listCtrl, int pos);
    static CString GetItemValue(CListCtrl& listCtrl, int pos);
    static void SelectedNextItem(CListCtrl& listCtrl);
    static int SelectedNextItemByValue(CListCtrl& listCtrl, CString szValue);
    static void ImportItems(CListCtrl& listCtrl, CString fileName);
};
