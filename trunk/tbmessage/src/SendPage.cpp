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
#define PAGE_SIZE 500

IMPLEMENT_DYNAMIC(CSendPage, CDialog)

CSendPage::CSendPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSendPage::IDD, pParent)
{
    m_IsStop = FALSE;
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
    m_AccountList.InsertColumn(0, "序号", LVCFMT_LEFT, 0);
    m_AccountList.InsertColumn(1, "用户名", LVCFMT_LEFT, 90);
    m_AccountList.InsertColumn(2, "密码", LVCFMT_LEFT, 0);

	// 设置列表框样式 && 添加列
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MemberList.InsertColumn(0, "序号", LVCFMT_LEFT, 0);
    m_MemberList.InsertColumn(1, "用户名", LVCFMT_LEFT, 105);
    m_MemberList.InsertColumn(2, "状态", LVCFMT_LEFT, 55);

    m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MessageList.InsertColumn(0, "序号", LVCFMT_LEFT, 36);
    m_MessageList.InsertColumn(1, "消息内容", LVCFMT_LEFT, 280);
    m_MessageList.InsertColumn(2, "消息HTML", LVCFMT_LEFT, 0);

    OnBnClickedBtnFirstPage();

    InitSpeed();

    LoadProfile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CSendPage, CDialog)
    ON_BN_CLICKED(IDC_BTN_SENDMSG, &CSendPage::OnBnClickedBtnSendmsg)
    ON_BN_CLICKED(IDC_BTN_ADD_ACCOUNT, &CSendPage::OnBnClickedBtnAddAccount)
    ON_BN_CLICKED(IDC_BTN_DEL_ACCOUNT, &CSendPage::OnBnClickedBtnDelAccount)
    ON_MESSAGE(WM_SENDMSG_COMPLETED, OnSendMsgCompleted)
    ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADD_MSG, &CSendPage::OnBnClickedBtnAddMsg)
	ON_BN_CLICKED(IDC_BTN_EDIT_MSG, &CSendPage::OnBnClickedBtnEditMsg)
	ON_BN_CLICKED(IDC_BTN_DEL_MSG, &CSendPage::OnBnClickedBtnDelMsg)
	ON_BN_CLICKED(IDC_BTN_IMPORT2, &CSendPage::OnBnClickedBtnImport2)
	ON_BN_CLICKED(IDC_BTN_EXPORT2, &CSendPage::OnBnClickedBtnExport2)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CSendPage::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_CLEAR2, &CSendPage::OnBnClickedBtnClear2)
	ON_BN_CLICKED(IDC_BTN_FIRST_PAGE, &CSendPage::OnBnClickedBtnFirstPage)
	ON_BN_CLICKED(IDC_BTN_PREV_PAGE, &CSendPage::OnBnClickedBtnPrevPage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CSendPage::OnBnClickedBtnNextPage)
	ON_BN_CLICKED(IDC_BTN_LAST_PAGE, &CSendPage::OnBnClickedBtnLastPage)
END_MESSAGE_MAP()

void CSendPage::LoadMembers(long startId, long stepCount)
{
    long count = CStoredMember::GetCount();
    long unsended = CStoredMember::GetUnSendCount();
    CString caption;
    caption.Format("买家/卖家(共:%d,已发送:%d)", count, count - unsended);
    this->SetDlgItemText(IDC_STATIC_MEMBER, caption);
	
    m_MemberList.DeleteAllItems();

    CList<TaobaoMember> *lpMemberList = new CList<TaobaoMember>(stepCount);    
	lastMemberId = CStoredMember::GetNextMember(startId, stepCount, lpMemberList);
    POSITION pos = lpMemberList->GetHeadPosition();
    while (pos)
    {
        TaobaoMember item = lpMemberList->GetNext(pos);

		CListViewHelp::AddListItem(m_MemberList, item.MemberName, item.Status ? STATUS_SENDED : STATUS_UNSEND);
    }
}

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
    m_IsStop = TRUE;
    this->SetDlgItemText(IDC_BTN_SENDMSG, "停止");

    int m = m_CmbSpeed.GetCurSel();
    int smSecond = m_CmbSpeed.GetItemData(m); // 毫秒数
    SendImMsg();
}

void CSendPage::StopSendMsg()
{
    m_IsStop = FALSE;
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
    CListViewHelp::ChangeListItemValue(m_MemberList, nItemId, STATUS_SENDED);

    CString szMessage;
    szMessage.Format("发送完成:%d", nItemId);
    this->SetDlgItemText(IDC_SEND_STATUS, szMessage);

    if (m_IsStop)
    {
        SendImMsg();
    }

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

void CSendPage::SendImMsg()
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
		CString strLine;

		CStdioFile file;
		CFileException ex;

		if (file.Open(fileOpen.GetPathName(), CFile::modeRead, &ex))
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
	            
				CStoredMember::AddMember(szUserName);
			}
			file.Close();
		}

		OnBnClickedBtnFirstPage();
    }
}

void CSendPage::OnBnClickedBtnExport2()
{
    CFileDialog fileOpen(FALSE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {     
		CString strLine;

		CStdioFile file;
		CFileException ex;

		if (file.Open(fileOpen.GetPathName(), CFile::modeCreate|CFile::modeWrite, &ex))
		{
			long count = CStoredMember::GetCount();
			CList<TaobaoMember> *lpMemberList = new CList<TaobaoMember>();
			CStoredMember::GetNextMember(0, count, lpMemberList);
			POSITION pos = lpMemberList->GetHeadPosition();
			while (pos)
			{
				TaobaoMember item = lpMemberList->GetNext(pos);

				strLine.Format("%s %d\r\n", item.MemberName, item.Status);
				file.WriteString(strLine);
			}
			
			file.Close();
		}
    }
}

void CSendPage::OnBnClickedBtnClear()
{
	if (MessageBox("确实要删除所有买家/卖家吗？", "提示", MB_YESNO) == IDYES)
	{
		CStoredMember::DeleteAllMembers();
		OnBnClickedBtnFirstPage();
	}
}

void CSendPage::OnBnClickedBtnClear2()
{
	if (MessageBox("确实要删除已发送买家/卖家吗？", "提示", MB_YESNO) == IDYES)
	{
		CStoredMember::DeleteSendedMembers();
		OnBnClickedBtnFirstPage();
	}
}

void CSendPage::SetPagging(long start, long pageSize)
{
	this->GetDlgItem(IDC_BTN_FIRST_PAGE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_PREV_PAGE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_LAST_PAGE)->EnableWindow(TRUE);

	long count = CStoredMember::GetCount();
	if (start == 0)
	{
		this->GetDlgItem(IDC_BTN_FIRST_PAGE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_PREV_PAGE)->EnableWindow(FALSE);
	}
	if (start + pageSize >= count)
	{
		this->GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_LAST_PAGE)->EnableWindow(FALSE);
	}
}

void CSendPage::OnBnClickedBtnFirstPage()
{
    LoadMembers(0, PAGE_SIZE);
	SetPagging(0, PAGE_SIZE);
}

void CSendPage::OnBnClickedBtnPrevPage()
{
	int prevPageIndex = lastMemberId - PAGE_SIZE*2;
	if (prevPageIndex < 0)
	{
		OnBnClickedBtnFirstPage();
	}
	else
	{
		LoadMembers(prevPageIndex, PAGE_SIZE);
		SetPagging(prevPageIndex, PAGE_SIZE);
	}
}

void CSendPage::OnBnClickedBtnNextPage()
{
    long count = CStoredMember::GetCount();
	int nextPageIndex = lastMemberId;
	if (nextPageIndex < count)
	{
		LoadMembers(lastMemberId, PAGE_SIZE);
		SetPagging(lastMemberId, PAGE_SIZE);
	}
	else
	{
		OnBnClickedBtnLastPage();
	}
}

void CSendPage::OnBnClickedBtnLastPage()
{	
    long count = CStoredMember::GetCount();
	int pageCount = (int)(count / PAGE_SIZE) + ((count % PAGE_SIZE) > 0 ? 1 : 0) - 1;
	LoadMembers(pageCount * PAGE_SIZE, PAGE_SIZE);
	
	SetPagging(pageCount * PAGE_SIZE, PAGE_SIZE);
}
