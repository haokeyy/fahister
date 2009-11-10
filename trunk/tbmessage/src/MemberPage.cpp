// MemberPage.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MemberPage.h"
#include "EditMsgDlg.h"
#include ".\Libraries\ListViewHelp.h"
#include "StoredMember.h"


// CMemberPage dialog

IMPLEMENT_DYNAMIC(CMemberPage, CDialog)

CMemberPage::CMemberPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMemberPage::IDD, pParent)
{

}

CMemberPage::~CMemberPage()
{
}

void CMemberPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
    DDX_Control(pDX, IDC_MESSAGE_LIST, m_MessageList);
}

BOOL CMemberPage::OnInitDialog()
{
	CDialog::OnInitDialog();
    
    // 设置列表框样式 && 添加列
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MemberList.InsertColumn(0, "序号", LVCFMT_LEFT, 0);
    m_MemberList.InsertColumn(1, "用户名", LVCFMT_LEFT, 150);
    m_MemberList.InsertColumn(2, "状态", LVCFMT_LEFT, 60);

    m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MessageList.InsertColumn(0, "序号", LVCFMT_LEFT, 36);
    m_MessageList.InsertColumn(1, "消息内容", LVCFMT_LEFT, 200);
    m_MessageList.InsertColumn(2, "消息HTML", LVCFMT_LEFT, 0);

    //m_MemberList.DeleteAllItems();
    //LoadMembers();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMemberPage::LoadMembers()
{
    long cnt = CStoredMember::GetCount();
    long ucnt = CStoredMember::GetUnSendCount();
    CString strCaption;
    strCaption.Format("买家/卖家列表(已发送:%d,共:%d)", cnt - ucnt, cnt);
    this->SetDlgItemText(IDC_STATIC_MEMBER, strCaption);
    
}

BEGIN_MESSAGE_MAP(CMemberPage, CDialog)
    ON_BN_CLICKED(IDC_BTN_ADD_MSG, &CMemberPage::OnBnClickedBtnAddMsg)
    ON_BN_CLICKED(IDC_BTN_EDIT_MSG, &CMemberPage::OnBnClickedBtnEditMsg)
    ON_BN_CLICKED(IDC_BTN_DEL_MSG, &CMemberPage::OnBnClickedBtnDelMsg)
    ON_BN_CLICKED(IDC_BTN_IMPORT2, &CMemberPage::OnBnClickedBtnImport2)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CMemberPage::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BTN_EXPORT2, &CMemberPage::OnBnClickedBtnExport2)
    ON_BN_CLICKED(IDC_BTN_CLEAR2, &CMemberPage::OnBnClickedBtnClear2)
END_MESSAGE_MAP()


// CMemberPage message handlers

void CMemberPage::OnBnClickedBtnAddMsg()
{
    CEditMsgDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        CString szBodyHtml = dlg.GetMsgHtml();
        CString szBodyText = dlg.GetMsgText();

        CListViewHelp::AddListItem(m_MessageList, szBodyText, szBodyHtml);
    }
}

void CMemberPage::OnBnClickedBtnEditMsg()
{    
    CEditMsgDlg dlg;
    int nItem = CListViewHelp::GetSelectedItem(m_MessageList);
    CString szItemValue = CListViewHelp::GetSelectedItemValue(m_MessageList);
    if (nItem >= 0 && !szItemValue.IsEmpty())
    {
        dlg.SetMsgHtml(szItemValue);
        if (dlg.DoModal() == IDOK)
        {
            CString szBodyHtml = dlg.GetMsgHtml();
            CString szBodyText = dlg.GetMsgText();

            CListViewHelp::ChangeListItem(m_MessageList, nItem, szBodyText, szBodyHtml);
        }
    }
}

void CMemberPage::OnBnClickedBtnDelMsg()
{
    CListViewHelp::DeleteSelectedItem(m_MessageList);
}

void CMemberPage::OnBnClickedBtnImport2()
{
    CFileDialog fileOpen(TRUE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CListViewHelp::ImportItems(m_MemberList, fileOpen.GetPathName());
    }
}

void CMemberPage::OnBnClickedBtnClear()
{
    CStoredMember::DeleteAllMembers();
}

void CMemberPage::OnBnClickedBtnExport2()
{
    CFileDialog fileOpen(FALSE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        //CListViewHelp::ImportItems(m_MemberList, fileOpen.GetPathName());
    }    
}

void CMemberPage::OnBnClickedBtnClear2()
{
    CStoredMember::DeleteSendedMembers();
}
