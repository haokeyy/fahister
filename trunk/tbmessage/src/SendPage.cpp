// SendPage.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "SendPage.h"
#include "EditAccDlg.h"
#include ".\Libraries\ListViewHelp.h"
#include ".\Libraries\WindowHelp.h"
#include "MsgSender.h"
#include "StoredMember.h"
#include "StoredAccount.h"
#include "StoredMessage.h"
#include "StoredProfile.h"
#include "EditMsgDlg.h"
#include "ShutdownDlg.h"

// CSendPage dialog
#define PAGE_SIZE 500
extern BOOL bHasMemberChanged;

IMPLEMENT_DYNAMIC(CSendPage, CDialog)

CSendPage::CSendPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSendPage::IDD, pParent)
    , m_nSendLimit(0)
{
    m_IsStop = FALSE;
    m_szLastSenderId = "";
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
    DDX_Text(pDX, IDC_SEND_LIMIT, m_nSendLimit);
}

BOOL CSendPage::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_ExprMsgHelp.Navigate("about:blank", 0, 0, 0, 0);

    szTaobaoSendUrl = m_szSendUrl; //"aliim:sendmsg?uid=cntaobao%s&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1";
    
    m_AccountList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_AccountList.InsertColumn(0, "序号", LVCFMT_LEFT, 0);
    m_AccountList.InsertColumn(1, "用户名", LVCFMT_LEFT, 80);
    //m_AccountList.InsertColumn(2, "密码", LVCFMT_LEFT, 0);
    m_AccountList.InsertColumn(2, "发送数量", LVCFMT_LEFT, 40);

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
    ON_BN_CLICKED(IDC_BTN_RESET, &CSendPage::OnBnClickedBtnReset)
    ON_WM_DESTROY()
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
    bHasMemberChanged = FALSE;
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
        CListViewHelp::AddListItem(m_AccountList, dlg.GetUserId(), "0");
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

        if (m_MessageList.GetItemCount() == 0)
        {            
            MessageBox("没有设置消息内容。", "错误", MB_ICONERROR);
            return;
        }

        if (m_AccountList.GetItemCount() == 0)
        {
            MessageBox("没有设置用来发送消息的淘宝用户。", "错误", MB_ICONERROR);
            return;
        }

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
    m_nSendedCount = 0;
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
    //CListViewHelp::ChangeListItemValue(m_MemberList, nItemId, STATUS_SENDED);

    CString szMessage;
    szMessage.Format("发送完成:%d", nItemId);
    this->SetDlgItemText(IDC_SEND_STATUS, szMessage);

    if (m_IsStop)
    {
        SendImMsg();
    }

    return 0;
}

BOOL CSendPage::IsOnline(CString szUserId)
{
	ConvertGBKToUtf8(szUserId);

	CString szURL = "http://amos1.taobao.com/online.ww?v=2&s=1&uid=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);
	return szHTML.GetLength() > 20;
}

int CSendPage::GetNextMember(CString& szNextReceiver)
{

NEXT_MEMBER:
    long i = CStoredMember::GetNextUnSendedMember(szNextReceiver);
    if (i > 0)
    {
        CStoredMember::SetMemberStatus(i, 1);

        // 当前页中的索引
        while (i > lastMemberId)
        {
            OnBnClickedBtnNextPage();
        }
        while (i < lastMemberId - PAGE_SIZE)
        {
            OnBnClickedBtnPrevPage();
        }
        int pageIndex = (lastMemberId / PAGE_SIZE) - 1;
        int itemIndex = i - pageIndex * PAGE_SIZE - 1;

        CButton *btnChkOnline = (CButton *)this->GetDlgItem(IDC_CHK_ONLINE);

        if (btnChkOnline->GetCheck() != BST_CHECKED || IsOnline(szNextReceiver))
        {
            CListViewHelp::ChangeListItemValue(m_MemberList, itemIndex, STATUS_SENDED);
        }
        else
        {
            CListViewHelp::ChangeListItemValue(m_MemberList, itemIndex, STATUS_OFFLINE);
            goto NEXT_MEMBER;
        }
    }

	return i;
}

int CSendPage::GetNextMessage(CString& szNextMessage)
{
    CListViewHelp::SelectedNextItem(m_MessageList);

    szNextMessage = CListViewHelp::GetSelectedItemText(m_MessageList);

    szNextMessage = " " + szNextMessage;

    if (!this->m_bHasReged)
    {
        szNextMessage += this->m_szAdText;
    }

    return 1;
}

void CSendPage::SendImMsg()
{
    m_nSendedCount++;

    if (this->m_bHasReged && m_nSendedCount > 20)
    {        
        StopSendMsg();
        MessageBox("未注册用户每次最多只能发送20条消息，请注册成为正式用户。", "提示");
        return;
    }

    if (CListViewHelp::GetSelectedItem(m_AccountList) < 0)
    {
        CListViewHelp::SelectedNextItem(m_AccountList);
    }
    
    int cnt = m_AccountList.GetItemCount();

CHECK_LIMIT:
    // 获取目前用户已发送数量和发送限制
    CString szCurrentSendUserId = CListViewHelp::GetSelectedItemText(m_AccountList);
    CString szCurrentSendedNumber = CListViewHelp::GetSelectedItemValue(m_AccountList);
    char* strSendedNumber = szCurrentSendedNumber.GetBuffer();
    int nSendedNumber = atoi(strSendedNumber);
    UINT nSendLimit = this->GetDlgItemInt(IDC_SEND_LIMIT);

    if (nSendedNumber >= nSendLimit && cnt-- > 0) // 达到最大发送数才切换
    {
        // 切换到下一用户
        CListViewHelp::SelectedNextItem(m_AccountList);
        goto CHECK_LIMIT;
    }
    else if (cnt <= 0)
    {
        StopSendMsg();
        MessageBox("所有用户都已达到发送限制，请更换用户发送。", "提示", MB_ICONINFORMATION);
        return;
    }
    
    CInstantMessage msg;
    msg.SendUserId = CListViewHelp::GetSelectedItemText(m_AccountList);
    msg.nItemIndex = GetNextMember(msg.ReceiverId);
    if (msg.nItemIndex < 0)
    {
        StopSendMsg();

        CButton *btnChkShutdown = (CButton *)this->GetDlgItem(IDC_CHK_SHUTDOWN);
        if (btnChkShutdown->GetCheck() == BST_CHECKED)
        {
            CShutdownDlg dlg;
            dlg.DoModal();
        }
        else
        {
            MessageBox("发送完成。", "提示", MB_ICONINFORMATION);
        }
        return;
    }
    GetNextMessage(msg.MessageHtml);

    while (!CMessageSender::UserIsLogined(msg.SendUserId))
    {    
        // 关闭已打开的旺旺窗口
        DWORD dwProcId = GetProcessIDByProcessName("AliIM.exe");
        if (dwProcId > 0)
        {
            CString szProcName("");
            GetProcessNameByProcessID(dwProcId, szProcName);
            
            KillProcess(dwProcId);
            WinExec(szProcName, SW_SHOW);

            Sleep(2000);

            // 刷新托盘区图标
            RefreshTrayWnd();
        }

        // 执行登录
        HWND hWnd = FindTopWindow("阿里旺旺2009", "StandardFrame");
        
        HWND hTypeEdit = FindWndInChildren(hWnd, "EditComponent", 2);
        HWND hTypeCmb = ::GetWindow(hTypeEdit, GW_HWNDNEXT);
        HWND hUidEdit = ::GetWindow(hTypeCmb, GW_HWNDNEXT); //user name

        HWND hLoginBtn = FindWndInChildren(hWnd, "登 录", 1);

        ::SendMessage(hUidEdit, WM_SETTEXT, NULL, (LPARAM)msg.SendUserId.GetBuffer(0));
        //::SendMessage(hPwdEdit, WM_SETTEXT, NULL, (LPARAM)pwd.GetBuffer(0));
        ::SendMessage(hLoginBtn, WM_LBUTTONDOWN,0,0);
        ::SendMessage(hLoginBtn, WM_LBUTTONUP,0,0); 

        Sleep(5000);
    }

    this->m_szLastSenderId = msg.SendUserId;
    // 弹出窗口
    OpenSendWindow(msg.SendUserId, msg.ReceiverId);

    CString szSendedCount;
    szSendedCount.Format("%d", nSendedNumber + 1);
    CListViewHelp::ChangeListItemValue(m_AccountList, CListViewHelp::GetSelectedItem(m_AccountList), szSendedCount);
    
    CString szMessage;
    szMessage.Format("正在发送:%d", msg.nItemIndex);
    this->SetDlgItemText(IDC_SEND_STATUS, szMessage);

    CMessageSender *pSender = new CMessageSender(this->GetSafeHwnd());
    pSender->SendMsg(msg);
}

void CSendPage::SaveProfile()
{
    CStoredAccount::ClearAccount();
    int count = m_AccountList.GetItemCount();
    for (int i = 0; i < count; i++)
    {
        CString sendUserId = CListViewHelp::GetItemText(m_AccountList, i);
        CString sendedCnt = CListViewHelp::GetItemValue(m_AccountList, i);

        CStoredAccount::AddAccount(sendUserId, sendedCnt);
    }

    CStoredMessage::ClearMessage();
    int msgCount = m_MessageList.GetItemCount();
    for (int i = 0; i < msgCount; i++)
    {
        CString msgText = CListViewHelp::GetItemValue(m_MessageList, i);

        CStoredMessage::AddMessage(msgText);
    }

    
}

void CSendPage::LoadProfile()
{
    int lastId = 0;
    CString str1, str2;

    while (lastId != -1)
    {
        lastId = CStoredAccount::GetNextAccount(lastId, str1, str2);
        if (lastId > 0)
        {
            CListViewHelp::AddListItem(m_AccountList, str1, str2);
        }
    }

    lastId = 0;
    
    while (lastId != -1)
    {
        lastId = CStoredMessage::GetNextMessage(lastId, str1);
        if (lastId > 0)
        {
            CListViewHelp::AddListItem(m_MessageList, str1, str1);
        }
    }

    this->SetDlgItemInt(IDC_SEND_LIMIT ,30);
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

void CSendPage::OnBnClickedBtnReset()
{
    int cnt = m_AccountList.GetItemCount();
    for (int i = 0; i < cnt; i++)
    {
        CListViewHelp::ChangeListItemValue(m_AccountList, i, "0");
    } 
}

void CSendPage::OnDestroy()
{
    CDialog::OnDestroy();

    SaveProfile();
}
