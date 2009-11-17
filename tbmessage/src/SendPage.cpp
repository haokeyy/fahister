// SendPage.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "SendPage.h"
#include "EditAccDlg.h"
#include ".\Libraries\ListViewHelp.h"
#include "MsgSender.h"
#include "StoredMember.h"
#include "StoredAccount.h"
#include "EditMsgDlg.h"

// CSendPage dialog

IMPLEMENT_DYNAMIC(CSendPage, CDialog)

CSendPage::CSendPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSendPage::IDD, pParent)
{

}

CSendPage::~CSendPage()
{
}

void CSendPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCOUNT_LIST, m_AccountList);
	DDX_Control(pDX, IDC_CMB_SPEED, m_CmbSpeed);
	DDX_Control(pDX, IDC_EXPR_MSG_HELP, m_ExprMsgHelp);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_MessageList);
	DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
}

BOOL CSendPage::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_ExprMsgHelp.Navigate("about:blank", 0, 0, 0, 0);

    szTaobaoSendUrl = "aliim:sendmsg?uid=cntaobao%s&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1";
    
    m_AccountList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_AccountList.InsertColumn(0, "���", LVCFMT_LEFT, 0);
    m_AccountList.InsertColumn(1, "�û���", LVCFMT_LEFT, 120);
    m_AccountList.InsertColumn(2, "����", LVCFMT_LEFT, 0);

	// �����б����ʽ && �����
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MemberList.InsertColumn(0, "���", LVCFMT_LEFT, 0);
    m_MemberList.InsertColumn(1, "�û���", LVCFMT_LEFT, 90);
    m_MemberList.InsertColumn(2, "״̬", LVCFMT_LEFT, 45);

    m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MessageList.InsertColumn(0, "���", LVCFMT_LEFT, 36);
    m_MessageList.InsertColumn(1, "��Ϣ����", LVCFMT_LEFT, 200);
    m_MessageList.InsertColumn(2, "��ϢHTML", LVCFMT_LEFT, 0);

    m_MemberList.DeleteAllItems();
    LoadMembers(0, 100);

    InitSpeed();

    LoadProfile();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CSendPage, CDialog)
    ON_BN_CLICKED(IDC_BTN_SENDMSG, &CSendPage::OnBnClickedBtnSendmsg)
    ON_BN_CLICKED(IDC_BTN_ADD_ACCOUNT, &CSendPage::OnBnClickedBtnAddAccount)
    ON_BN_CLICKED(IDC_BTN_DEL_ACCOUNT, &CSendPage::OnBnClickedBtnDelAccount)
    ON_MESSAGE(WM_SENDMSG_COMPLETED, OnSendMsgCompleted) 
    ON_WM_TIMER()
    ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADD_MSG, &CSendPage::OnBnClickedBtnAddMsg)
	ON_BN_CLICKED(IDC_BTN_EDIT_MSG, &CSendPage::OnBnClickedBtnEditMsg)
	ON_BN_CLICKED(IDC_BTN_DEL_MSG, &CSendPage::OnBnClickedBtnDelMsg)
	ON_BN_CLICKED(IDC_BTN_IMPORT2, &CSendPage::OnBnClickedBtnImport2)
	ON_BN_CLICKED(IDC_BTN_EXPORT2, &CSendPage::OnBnClickedBtnExport2)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CSendPage::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_CLEAR2, &CSendPage::OnBnClickedBtnClear2)
END_MESSAGE_MAP()

void CSendPage::LoadMembers(long startId, long stepCount)
{
    long count = CStoredMember::GetCount();
    long unsended = CStoredMember::GetUnSendCount();
    CString caption;
    caption.Format("���/����(��:%d,�ѷ���:%d)", count, count - unsended);
    this->SetDlgItemText(IDC_STATIC_MEMBER, caption);

    CStringList *lpMemberList = new CStringList(stepCount);
    CStoredMember::GetNextMember(startId, stepCount, lpMemberList);
    POSITION pos = lpMemberList->GetHeadPosition();
    while (pos)
    {
        CString szMemberName = lpMemberList->GetNext(pos);

        CListViewHelp::AddListItem(m_MemberList, szMemberName, STATUS_UNSEND);
    }
}

void CSendPage::InitSpeed()
{
    m_CmbSpeed.ResetContent();
    int i=0;
    m_CmbSpeed.AddString("1��ÿ��"); m_CmbSpeed.SetItemData(i++,1000);
    m_CmbSpeed.AddString("2��ÿ��"); m_CmbSpeed.SetItemData(i++, 2000);
    m_CmbSpeed.AddString("3��ÿ��"); m_CmbSpeed.SetItemData(i++, 3000);
    m_CmbSpeed.AddString("5��ÿ��"); m_CmbSpeed.SetItemData(i++, 5000);
    m_CmbSpeed.AddString("8��ÿ��"); m_CmbSpeed.SetItemData(i++, 8000);
    m_CmbSpeed.AddString("10��ÿ��"); m_CmbSpeed.SetItemData(i++, 10000);
    m_CmbSpeed.AddString("15��ÿ��"); m_CmbSpeed.SetItemData(i++, 15000);

    m_CmbSpeed.SetCurSel(3);
}
// CSendPage message handlers


void CSendPage::OnBnClickedBtnAddAccount()
{
    CEditAccountDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        CListViewHelp::AddListItem(m_AccountList, dlg.GetUserId(), dlg.GetPassword());
    }
}

void CSendPage::OnBnClickedBtnDelAccount()
{
    CListViewHelp::DeleteSelectedItem(m_AccountList);
}

void CSendPage::OnBnClickedBtnSendmsg()
{    
    CString szText;
    this->GetDlgItemText(IDC_BTN_SENDMSG, szText);
    if (szText == "��ʼ����")
    {
        if (CStoredMember::GetUnSendCount() == 0)
        {
            MessageBox("û�д����͵����/���ҡ�", "����", MB_ICONERROR);
            return;
        }

        //if (m_MessageList.GetItemCount() == 0)
        //{            
        //    MessageBox("û��������Ϣ���ݡ�", "����", MB_ICONERROR);
        //    return;
        //}

        if (m_AccountList.GetItemCount() == 0)
        {
            MessageBox("û����������������Ϣ���Ա��û���", "����", MB_ICONERROR);
            return;
        }

        //CListViewHelp::SetSelectedItem(m_MemberList, -1);

        StartSendMsg();
    }
    else
    {
        StopSendMsg();
    }
}



void CSendPage::StartSendMsg()
{
    this->SetDlgItemText(IDC_BTN_SENDMSG, "ֹͣ");

    int m = m_CmbSpeed.GetCurSel();
    int smSecond = m_CmbSpeed.GetItemData(m); // ������
    //this->SetTimer(TIMER_ID, smSecond, NULL);
    OnTimer(TIMER_ID);
}

void CSendPage::StopSendMsg()
{
    this->KillTimer(TIMER_ID);

    this->SetDlgItemText(IDC_BTN_SENDMSG, "��ʼ����");
}

BOOL CSendPage::OpenSendWindow(CString szSenderID, CString szReceiverID)
{  
    CString szURL;
    szURL.Format(szTaobaoSendUrl, szSenderID.GetBuffer(), szReceiverID.GetBuffer());        
    m_ExprMsgHelp.Navigate(szURL, NULL, NULL, NULL, NULL);

    return TRUE;
}

LRESULT CSendPage::OnSendMsgCompleted(WPARAM wParam, LPARAM lParam)
{
    LPTSTR szItemId = (LPTSTR)lParam;

    int nItemId = atoi(szItemId);
    CListViewHelp::ChangeListItemValue(m_MemberList, nItemId, STATUS_SENDED);

    CString szMessage;
    szMessage.Format("�������:%d", nItemId);
    this->SetDlgItemText(IDC_SEND_STATUS, szMessage);

    return 0;
}

int CSendPage::GetNextMember(CString& szNextReceiver)
{
    long i = CStoredMember::GetNextUnSendedMember(szNextReceiver);
	CStoredMember::SetMemberStatus(i, 1);
	return i;
}

int CSendPage::GetNextMessage(CString& szNextMessage)
{
    szNextMessage = " HiHi";
    return 1;
}

void CSendPage::OnTimer(UINT_PTR nIDEvent)
{    
    CInstantMessage msg;

    CButton *chkAutoLogin = (CButton*)this->GetDlgItem(IDC_CHK_AUTO_LOGIN);
    CButton *chkAddToFriend = (CButton*)this->GetDlgItem(IDC_CHK_ADDTO_FRIEND);
    msg.AutoLogin = chkAutoLogin->GetCheck();
    msg.AddToFriend = chkAddToFriend->GetCheck();

    CListViewHelp::SelectedNextItem(m_AccountList);
    msg.SendUserId = CListViewHelp::GetSelectedItemText(m_AccountList);
    msg.SendUserPassword = CListViewHelp::GetSelectedItemValue(m_AccountList);

    if (!CMessageSender::UserIsLogined(msg.SendUserId) && !msg.AutoLogin) // �û�û�е�¼��������Ϊ���Զ���¼������Դ˴η��ͣ�������һ���û�����
    {
        return;
    }

    msg.nItemIndex = GetNextMember(msg.ReceiverId);
    if (msg.nItemIndex < 0)
    {
        StopSendMsg();
        MessageBox("������ɡ�", "��ʾ", MB_ICONINFORMATION);
        return;
    }
    GetNextMessage(msg.MessageHtml);

    OpenSendWindow(msg.SendUserId, msg.ReceiverId);
    
    CString szMessage;
    szMessage.Format("���ڷ���:%d", msg.nItemIndex);
    this->SetDlgItemText(IDC_SEND_STATUS, szMessage);

    CMessageSender *pSender = new CMessageSender(this->GetSafeHwnd());
    pSender->SendMsg(msg);

    CDialog::OnTimer(nIDEvent);
}

void CSendPage::SaveProfile()
{
    int count = m_AccountList.GetItemCount();

    for (int i = 0; i < count; i++)
    {
        CString sendUserId = CListViewHelp::GetItemText(m_AccountList, i);
        CString sendPassword = CListViewHelp::GetItemValue(m_AccountList, i);

        CStoredAccount::AddAccount(sendUserId, sendPassword);
    }
}

void CSendPage::LoadProfile()
{
}

void CSendPage::OnClose()
{
    SaveProfile();

    CDialog::OnClose();
}

void CSendPage::OnBnClickedBtnAddMsg()
{
	CEditMsgDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        CString szBodyHtml = dlg.GetMsgHtml();
        CString szBodyText = dlg.GetMsgText();

        CListViewHelp::AddListItem(m_MessageList, szBodyText, szBodyHtml);
    }
}

void CSendPage::OnBnClickedBtnEditMsg()
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

void CSendPage::OnBnClickedBtnDelMsg()
{
    CListViewHelp::DeleteSelectedItem(m_MessageList);
}

void CSendPage::OnBnClickedBtnImport2()
{
    CFileDialog fileOpen(TRUE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CListViewHelp::ImportItems(m_MemberList, fileOpen.GetPathName());
    }
}

void CSendPage::OnBnClickedBtnExport2()
{
    CFileDialog fileOpen(FALSE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CListViewHelp::ImportItems(m_MemberList, fileOpen.GetPathName());
    }    
}

void CSendPage::OnBnClickedBtnClear()
{
    CStoredMember::DeleteAllMembers();
}

void CSendPage::OnBnClickedBtnClear2()
{
    CStoredMember::DeleteSendedMembers();
}
