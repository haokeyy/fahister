
// tbmsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tbmessage.h"
#include "tbmsgDlg.h"
#include "EditMsgDlg.h"
#include "ListViewHelp.h"
#include "editaccdlg.h"
#include "MsgSender.h"
#include "Constants.h"
#include ".\Libraries\WindowHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtbmsgDlg �Ի���
CtbmsgDlg::CtbmsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtbmsgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtbmsgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXPR_MSG_HELP, m_ExprMsgHelp);
    DDX_Control(pDX, IDC_MESSAGE_LIST, m_MessageList);
    DDX_Control(pDX, IDC_ACCOUNT_LIST, m_AccountList);
    DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
    DDX_Control(pDX, IDC_CMB_TARGET, m_CmbTarget);
    DDX_Control(pDX, IDC_CMB_CATEGORY, m_CmbCategory);
    DDX_Control(pDX, IDC_CMB_LOCATION, m_CmbLocation);
    DDX_Control(pDX, IDC_CMB_SPEED, m_CmbSpeed);
}

BEGIN_MESSAGE_MAP(CtbmsgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_ADD_MSG, &CtbmsgDlg::OnBnClickedBtnAddMsg)
    ON_BN_CLICKED(IDC_BTN_EDIT_MSG, &CtbmsgDlg::OnBnClickedBtnEditMsg)
    ON_BN_CLICKED(IDC_BTN_DEL_MSG, &CtbmsgDlg::OnBnClickedBtnDelMsg)
    ON_BN_CLICKED(IDC_BTN_ADD_ACCOUNT, &CtbmsgDlg::OnBnClickedBtnAddAccount)
    ON_BN_CLICKED(IDC_BTN_DEL_ACCOUNT, &CtbmsgDlg::OnBnClickedBtnDelAccount)
    ON_BN_CLICKED(IDC_BTN_SENDMSG, &CtbmsgDlg::OnBnClickedBtnSendmsg)
    ON_BN_CLICKED(IDC_BTN_IMPORT, &CtbmsgDlg::OnBnClickedBtnImport)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CtbmsgDlg::OnBnClickedBtnClear)
    ON_WM_TIMER()
    ON_MESSAGE(WM_SENDMSG_COMPLETED, OnSendMsgCompleted) 
    ON_MESSAGE(WM_FOUND_MEMBER, OnFoundMember) 
END_MESSAGE_MAP()


// CtbmsgDlg ��Ϣ�������

BOOL CtbmsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_ExprMsgHelp.Navigate("about:blank", 0, 0, 0, 0);

    // �����б����ʽ && �����
    m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MessageList.InsertColumn(0, "���", LVCFMT_LEFT, 36);
    m_MessageList.InsertColumn(1, "��Ϣ����", LVCFMT_LEFT, 420);
    m_MessageList.InsertColumn(2, "��ϢHTML", LVCFMT_LEFT, 0);

    m_AccountList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_AccountList.InsertColumn(0, "���", LVCFMT_LEFT, 0);
    m_AccountList.InsertColumn(1, "�û���", LVCFMT_LEFT, 120);
    m_AccountList.InsertColumn(2, "����", LVCFMT_LEFT, 0);

    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
    m_MemberList.InsertColumn(0, "���", LVCFMT_LEFT, 0);
    m_MemberList.InsertColumn(1, "�û���", LVCFMT_LEFT, 150);
    m_MemberList.InsertColumn(2, "״̬", LVCFMT_LEFT, 60);

    m_CmbTarget.SetCurSel(0);
    InitCategory();
    InitLocation();
    InitSpeed();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CtbmsgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CtbmsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtbmsgDlg::OnBnClickedBtnAddMsg()
{
    CEditMsgDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        CString szBodyHtml = dlg.GetMsgHtml();
        CString szBodyText = dlg.GetMsgText();

        CListViewHelp::AddListItem(m_MessageList, szBodyText, szBodyHtml);
    }
}

void CtbmsgDlg::OnBnClickedBtnEditMsg()
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

void CtbmsgDlg::OnBnClickedBtnDelMsg()
{
    CListViewHelp::DeleteSelectedItem(m_MessageList);
}

void CtbmsgDlg::OnBnClickedBtnAddAccount()
{
    CEditAccountDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        CListViewHelp::AddListItem(m_AccountList, dlg.GetUserId(), dlg.GetPassword());
    }
}

void CtbmsgDlg::OnBnClickedBtnDelAccount()
{
    CListViewHelp::DeleteSelectedItem(m_AccountList);
}

void CtbmsgDlg::OnBnClickedBtnImport()
{
    CFileDialog fileOpen(TRUE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CListViewHelp::ImportItems(m_MemberList, fileOpen.GetPathName());
    }
}

void CtbmsgDlg::OnBnClickedBtnClear()
{
    m_MemberList.DeleteAllItems();
}


void CtbmsgDlg::InitCategory()
{
    m_CmbCategory.ResetContent();
    int i=0;
    while (ShopCategorys[i].Value != -1)
    {
        m_CmbCategory.AddString(ShopCategorys[i].Name);
        m_CmbCategory.SetItemData(i, ShopCategorys[i].Value);
        i++;
    }

    m_CmbCategory.SetCurSel(0);
}

void CtbmsgDlg::InitLocation()
{
    m_CmbLocation.ResetContent();
   
    int i=0;
    while (Citys[i].Value != -1)
    {
        m_CmbLocation.AddString(Citys[i].Name);
        i++;
    }

    m_CmbLocation.SetCurSel(0);
}

void CtbmsgDlg::InitSpeed()
{
    m_CmbSpeed.ResetContent();
   
    int i=0;
    while (Speeds[i].Value != -1)
    {
        m_CmbSpeed.AddString(Speeds[i].Name);
        m_CmbSpeed.SetItemData(i, Speeds[i].Value);
        i++;
    }

    m_CmbSpeed.SetCurSel(3);
}


void CtbmsgDlg::OnBnClickedBtnSendmsg()
{
    CString szText;
    this->GetDlgItemText(IDC_BTN_SENDMSG, szText);
    if (szText == "����")
    {
        if (m_MemberList.GetItemCount() == 0)
        {
            MessageBox("û�д����͵ĺ��ѡ�", "����", MB_ICONERROR);
            return;
        }

        if (m_MessageList.GetItemCount() == 0)
        {            
            MessageBox("û��������Ϣ���ݡ�", "����", MB_ICONERROR);
            return;
        }

        if (m_AccountList.GetItemCount() == 0)
        {
            MessageBox("û����������������Ϣ���Ա��û���", "����", MB_ICONERROR);
            return;
        }

        CListViewHelp::SetSelectedItem(m_MemberList, -1);

        StartSendMsg();
    }
    else
    {
        StopSendMsg();
    }
}


void CtbmsgDlg::OnTimer(UINT_PTR nIDEvent)
{
    CInstantMessage msg;

    CButton *chkAutoLogin = (CButton*)this->GetDlgItem(IDC_CHK_ADDTO_FRIEND);
    CButton *chkAddToFriend = (CButton*)this->GetDlgItem(IDC_CHK_AUTO_LOGIN);
    msg.AutoLogin = chkAutoLogin->GetCheck();
    msg.AddToFriend = chkAddToFriend->GetCheck();

    CListViewHelp::SelectedNextItem(m_AccountList);
    msg.SendUserId = CListViewHelp::GetSelectedItemText(m_AccountList);
    msg.SendUserPassword = CListViewHelp::GetSelectedItemValue(m_AccountList);

    if (!CMessageSender::UserIsLogined(msg.SendUserId) && !msg.AutoLogin) // �û�û�е�¼��������Ϊ���Զ���¼������Դ˴η��ͣ�������һ���û�����
    {
        return;
    }

    CListViewHelp::SelectedNextItem(m_MessageList);
    msg.MessageHtml = CListViewHelp::GetSelectedItemValue(m_MessageList);

    int offset = CListViewHelp::SelectedNextItemByValue(m_MemberList, STATUS_UNSEND);
    if (offset >= m_MemberList.GetItemCount())
    {
        StopSendMsg();
        MessageBox("������ɡ�", "��ʾ", MB_ICONINFORMATION);
        return;
    }

    msg.nItemIndex = CListViewHelp::GetSelectedItem(m_MemberList);
    msg.ReceiverId = CListViewHelp::GetSelectedItemText(m_MemberList);

    OpenSendWindow(msg.SendUserId, msg.ReceiverId);
    CListViewHelp::ChangeListItemValue(m_MemberList, msg.nItemIndex, STATUS_SENDING);

    CMessageSender *pSender = new CMessageSender(this->GetSafeHwnd());
    pSender->SendMsg(msg);

    CDialog::OnTimer(nIDEvent);
}


void CtbmsgDlg::StartSendMsg()
{
    this->SetDlgItemText(IDC_BTN_SENDMSG, "ֹͣ");

    int m = m_CmbSpeed.GetCurSel();
    int smSecond = m_CmbSpeed.GetItemData(m); // ������
    this->SetTimer(TIMER_ID, smSecond, NULL);
    OnTimer(TIMER_ID);
}

void CtbmsgDlg::StopSendMsg()
{
    this->KillTimer(TIMER_ID);

    this->SetDlgItemText(IDC_BTN_SENDMSG, "����");
}

BOOL CtbmsgDlg::OpenSendWindow(CString szSenderID, CString szReceiverID)
{    
    CString szTaobaoSendUrl = "aliim:sendmsg?uid=cntaobao%s&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1";
    
    CString szURL;
    szURL.Format(szTaobaoSendUrl, szSenderID.GetBuffer(), szReceiverID.GetBuffer());        
    m_ExprMsgHelp.Navigate(szURL, NULL, NULL, NULL, NULL);

    return TRUE;
}

LRESULT CtbmsgDlg::OnFoundMember(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CtbmsgDlg::OnSendMsgCompleted(WPARAM wParam, LPARAM lParam)
{
    LPTSTR szItemId = (LPTSTR)lParam;

    int nItemId = atoi(szItemId);
    CListViewHelp::ChangeListItemValue(m_MemberList, nItemId, STATUS_SENDED);

    return 0;
}