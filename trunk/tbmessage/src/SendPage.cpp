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
}

BOOL CSendPage::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_ExprMsgHelp.Navigate("about:blank", 0, 0, 0, 0);

    szTaobaoSendUrl = "aliim:sendmsg?uid=cntaobao%s&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1";
    
    m_AccountList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_AccountList.InsertColumn(0, "序号", LVCFMT_LEFT, 0);
    m_AccountList.InsertColumn(1, "用户名", LVCFMT_LEFT, 120);
    m_AccountList.InsertColumn(2, "密码", LVCFMT_LEFT, 0);

    InitSpeed();

    LoadProfile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CSendPage, CDialog)
    ON_BN_CLICKED(IDC_BTN_SENDMSG, &CSendPage::OnBnClickedBtnSendmsg)
    ON_BN_CLICKED(IDC_BTN_ADD_ACCOUNT, &CSendPage::OnBnClickedBtnAddAccount)
    ON_BN_CLICKED(IDC_BTN_DEL_ACCOUNT, &CSendPage::OnBnClickedBtnDelAccount)
    ON_MESSAGE(WM_SENDMSG_COMPLETED, OnSendMsgCompleted) 
    ON_WM_TIMER()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


void CSendPage::InitSpeed()
{
    m_CmbSpeed.ResetContent();
    int i=0;
    m_CmbSpeed.AddString("1秒每条"); m_CmbSpeed.SetItemData(i++,1000);
    m_CmbSpeed.AddString("2秒每条"); m_CmbSpeed.SetItemData(i++, 2000);
    m_CmbSpeed.AddString("3秒每条"); m_CmbSpeed.SetItemData(i++, 3000);
    m_CmbSpeed.AddString("5秒每条"); m_CmbSpeed.SetItemData(i++, 5000);
    m_CmbSpeed.AddString("8秒每条"); m_CmbSpeed.SetItemData(i++, 8000);
    m_CmbSpeed.AddString("10秒每条"); m_CmbSpeed.SetItemData(i++, 10000);
    m_CmbSpeed.AddString("15秒每条"); m_CmbSpeed.SetItemData(i++, 15000);

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
    if (szText == "开始发送")
    {
        if (CStoredMember::GetUnSendCount() == 0)
        {
            MessageBox("没有待发送的买家/卖家。", "错误", MB_ICONERROR);
            return;
        }

        //if (m_MessageList.GetItemCount() == 0)
        //{            
        //    MessageBox("没有设置消息内容。", "错误", MB_ICONERROR);
        //    return;
        //}

        if (m_AccountList.GetItemCount() == 0)
        {
            MessageBox("没有设置用来发送消息的淘宝用户。", "错误", MB_ICONERROR);
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
    this->SetDlgItemText(IDC_BTN_SENDMSG, "停止");

    int m = m_CmbSpeed.GetCurSel();
    int smSecond = m_CmbSpeed.GetItemData(m); // 毫秒数
    this->SetTimer(TIMER_ID, smSecond, NULL);
    OnTimer(TIMER_ID);
}

void CSendPage::StopSendMsg()
{
    this->KillTimer(TIMER_ID);

    this->SetDlgItemText(IDC_BTN_SENDMSG, "开始发送");
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
    //CListViewHelp::ChangeListItemValue(m_MemberList, nItemId, STATUS_SENDED);

    CString szMessage;
    szMessage.Format("发送完成:%d", nItemId);
    this->SetDlgItemText(IDC_SEND_STATUS, szMessage);

    return 0;
}

int CSendPage::GetNextMember(CString& szNextReceiver)
{
    return CStoredMember::GetNextUnSenderMember(szNextReceiver);
}

int CSendPage::GetNextMessage(CString& szNextMessage)
{
    szNextMessage = "Hi";
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

    if (!CMessageSender::UserIsLogined(msg.SendUserId) && !msg.AutoLogin) // 用户没有登录并且设置为不自动登录，则忽略此次发送，即用下一个用户发送
    {
        return;
    }

    msg.nItemIndex = GetNextMember(msg.ReceiverId);
    if (msg.nItemIndex < 0)
    {
        StopSendMsg();
        MessageBox("发送完成。", "提示", MB_ICONINFORMATION);
        return;
    }
    GetNextMessage(msg.MessageHtml);

    OpenSendWindow(msg.SendUserId, msg.ReceiverId);
    
    CString szMessage;
    szMessage.Format("正在发送:%d", msg.nItemIndex);
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
