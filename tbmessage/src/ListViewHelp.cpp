#include "StdAfx.h"
#include "ListViewHelp.h"

CListViewHelp::CListViewHelp()
{
}

CListViewHelp::~CListViewHelp()
{
}

void CListViewHelp::AddListItem(CListCtrl& listCtrl, CString itemText , CString itemValue)
{
    int nIndex = listCtrl.GetItemCount();
    CString szIndex;
    szIndex.Format("%d", nIndex + 1);
    listCtrl.InsertItem(nIndex, szIndex);
    listCtrl.SetItemText(nIndex, 1, itemText);
    listCtrl.SetItemText(nIndex, 2, itemValue);
}

void CListViewHelp::ChangeListItem(CListCtrl& listCtrl, int nItem, CString itemText , CString itemValue)
{
    listCtrl.SetItemText(nItem, 1, itemText);
    listCtrl.SetItemText(nItem, 2, itemValue);
}

void CListViewHelp::ResetItemId(CListCtrl& listCtrl)
{
    int count = listCtrl.GetItemCount();
    for (int i = 0; i < count; i++)
    {
        CString szIndex;
        szIndex.Format("%d", i + 1);
        listCtrl.SetItemText(i, 0, szIndex);
    }
}

void CListViewHelp::DeleteSelectedItem(CListCtrl& listCtrl)
{
    POSITION firstPosition = listCtrl.GetFirstSelectedItemPosition();
    int pos = listCtrl.GetNextSelectedItem(firstPosition);
    listCtrl.DeleteItem(pos);
    ResetItemId(listCtrl);
}

int CListViewHelp::GetSelectedItem(CListCtrl& listCtrl)
{
    POSITION firstPosition = listCtrl.GetFirstSelectedItemPosition();
    int pos = listCtrl.GetNextSelectedItem(firstPosition);
    return pos;
}

void CListViewHelp::SetSelectedItem(CListCtrl& listCtrl, int pos)
{
    listCtrl.SetItemState(pos, LVNI_SELECTED, LVNI_SELECTED);
}

CString CListViewHelp::GetSelectedItemText(CListCtrl& listCtrl)
{
    POSITION firstPosition = listCtrl.GetFirstSelectedItemPosition();
    int pos = listCtrl.GetNextSelectedItem(firstPosition);
    return listCtrl.GetItemText(pos, 1);
}

CString CListViewHelp::GetSelectedItemValue(CListCtrl& listCtrl)
{
    POSITION firstPosition = listCtrl.GetFirstSelectedItemPosition();
    int pos = listCtrl.GetNextSelectedItem(firstPosition);
    return listCtrl.GetItemText(pos, 2);
}

void CListViewHelp::SelectedNextItem(CListCtrl& listCtrl)
{
    POSITION firstPosition = listCtrl.GetFirstSelectedItemPosition();
    int pos = listCtrl.GetNextSelectedItem(firstPosition);
    if (pos < 0 || ++pos >= listCtrl.GetItemCount())
    {
        pos = 0;
    }
    listCtrl.SetItemState(pos, LVNI_SELECTED, LVNI_SELECTED);
}

int CListViewHelp::SelectedNextItemByValue(CListCtrl& listCtrl, CString szValue)
{
    int count = listCtrl.GetItemCount();
    int offset = 0;
    
    CString itemValue;

    do
    {
        SelectedNextItem(listCtrl);
        itemValue = GetSelectedItemValue(listCtrl);
    }
    while (itemValue != szValue && offset++ < count);
 
    return offset;
}


void CListViewHelp::ImportItems(CListCtrl& listCtrl, CString fileName)
{     
    CString strLine;

    CStdioFile file;
    CFileException ex;

    if (file.Open(fileName, CFile::modeRead, &ex))
    {
        while (file.ReadString(strLine))
        {
            CString szUserName(strLine), szStatus("0");
            int i = strLine.Find(" ", 0);
            if (i > 0)
            {
                szUserName = strLine.Left(i);
                szStatus = strLine.Right(1);
            }
            
            AddListItem(listCtrl, szUserName, szStatus == "1" ? STATUS_SENDED : STATUS_UNSEND);
        }
        file.Close();
    }
}