
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
    m_MemberList.InsertColumn(0, "���", LVCFMT_LEFT, 45);
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
        i++;
    }

    m_CmbSpeed.SetCurSel(3);
}


void CtbmsgDlg::OnBnClickedBtnSendmsg()
{
    this->SetTimer(100, 5000, NULL);
}


void CtbmsgDlg::OnTimer(UINT_PTR nIDEvent)
{    
    CListViewHelp::SelectedNextItem(m_MessageList);
    CString currentMessage = CListViewHelp::GetSelectedItemValue(m_MessageList);
    CListViewHelp::SelectedNextItem(m_AccountList);
    CString currentUserId = CListViewHelp::GetSelectedItemText(m_AccountList);
    CString currentUserPwd = CListViewHelp::GetSelectedItemValue(m_AccountList);
    CListViewHelp::SelectedNextItemByValue(m_MemberList, STATUS_UNSEND);
    CString currentReceiver = CListViewHelp::GetSelectedItemValue(m_MemberList);

    CInstantMessage msg;
    msg.SendUserId = currentUserId;
    msg.SendUserPassword = currentUserPwd;
    msg.ReceiverId = currentReceiver;
    msg.MessageHtml = currentMessage;

    CMessageSender::SendMsg(msg);

    CDialog::OnTimer(nIDEvent);
}
