// MemberPage.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MemberPage.h"
#include "EditMsgDlg.h"
#include ".\Libraries\ListViewHelp.h"
#include "StoredMember.h"
#include ".\Libraries\WindowHelp.h"


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
	//DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
	//DDX_Control(pDX, IDC_MESSAGE_LIST, m_MessageList);
	DDX_Control(pDX, IDC_EXPR_WELCOME, m_ExpWelcome);
}



BOOL CMemberPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ExpWelcome.Navigate(URL_WELCOME, 0, 0, 0, 0);

    CString szMachineCode;
    GetMachineCode(szMachineCode);
    this->SetDlgItemText(IDC_EDIT_MCODE, szMachineCode);

    szMachineCode = "�����룺" + szMachineCode;
    if (this->m_bHasReged)
    {
        szMachineCode += "  (��ע��)";
    }
    else
    {
        szMachineCode += "  (δע��)";
    }
    this->SetDlgItemText(IDC_STATIC_STATUS, szMachineCode);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMemberPage::LoadMembers()
{
    long cnt = CStoredMember::GetCount();
    long ucnt = CStoredMember::GetUnSendCount();
    CString strCaption;
    strCaption.Format("���/�����б�(�ѷ���:%d,��:%d)", cnt - ucnt, cnt);
    this->SetDlgItemText(IDC_STATIC_MEMBER, strCaption);
    
}

BEGIN_MESSAGE_MAP(CMemberPage, CDialog)
    //ON_BN_CLICKED(IDC_BTN_ADD_MSG, &CMemberPage::OnBnClickedBtnAddMsg)
    //ON_BN_CLICKED(IDC_BTN_EDIT_MSG, &CMemberPage::OnBnClickedBtnEditMsg)
    //ON_BN_CLICKED(IDC_BTN_DEL_MSG, &CMemberPage::OnBnClickedBtnDelMsg)
    //ON_BN_CLICKED(IDC_BTN_IMPORT2, &CMemberPage::OnBnClickedBtnImport2)
    //ON_BN_CLICKED(IDC_BTN_CLEAR, &CMemberPage::OnBnClickedBtnClear)
    //ON_BN_CLICKED(IDC_BTN_EXPORT2, &CMemberPage::OnBnClickedBtnExport2)
    //ON_BN_CLICKED(IDC_BTN_CLEAR2, &CMemberPage::OnBnClickedBtnClear2)
    ON_BN_CLICKED(IDC_BTN_COPY_MCODE, &CMemberPage::OnBnClickedBtnCopyMcode)
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

void CMemberPage::OnBnClickedBtnCopyMcode()
{
    CEdit *txtEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_MCODE);
    txtEdit->SetSel(32);
    txtEdit->Copy();

    MessageBox("�������Ѿ����Ƶ�������", "��Ϣ");
}
