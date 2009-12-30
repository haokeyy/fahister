// BatchMessageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "BatchMessageDlg.h"
#include "EditUserDlg.h"
#include "HelpDlg.h"
#include "shutdowndlg.h"
#include "WindowHelp.h"
#include "RegDlg.h"
#include "IMMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchMessageDlg �Ի���

#define TIMER_ID 100

#define STATUS_UNSEND "������"
#define STATUS_SENDING "������..."
#define STATUS_SENDED "�ѷ���"
#define STATUS_FAIL   "����ʧ��"
#define SZ_SPLIT_FLAG "                                 ---"



CBatchMessageDlg::CBatchMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchMessageDlg::IDD, pParent)
    , m_Message(_T(""))
    , m_MemberLimit(100)
    , m_Keyword(_T(""))
    , m_bShutdown(FALSE)
{
    nSendCount = 0;
    ValidUser = PRODUCT_ID;
    szVersionAD = "\n����Ϣ�ɡ����ư���Ⱥ������������\n�ٷ���ַ��http://www.21cn2.com";
    m_pWebSendDlg = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBatchMessageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
    DDX_Control(pDX, IDC_CMB_TARGET, m_CmbTarget);
    DDX_Text(pDX, IDC_EDIT_MSG, m_Message);
    DDX_Control(pDX, IDC_BTN_SEARCH, m_BtnSearch);
    DDX_Text(pDX, IDC_EDIT_LIMIT, m_MemberLimit);
    DDX_Control(pDX, IDC_CMB_CATEGORY, m_CmbCategory);
    DDX_Text(pDX, IDC_EDIT_KEYWORD, m_Keyword);
    DDX_Control(pDX, IDC_LIST_USER, m_UserList);
    DDX_Control(pDX, IDC_CMB_SPEED, m_CmbSendSpeed);
    DDX_Control(pDX, IDC_BTN_SENDMSG, m_BtnSendMsg);
    DDX_Check(pDX, IDC_CHK_SHUTDOWN, m_bShutdown);
    DDX_Control(pDX, IDC_BTN_ADDUSER, m_BtnAddUser);
    DDX_Control(pDX, IDC_BTN_DELUSER, m_BtnDelUser);
    DDX_Control(pDX, IDC_EDIT_MSG, m_EditMsg);
    DDX_Control(pDX, IDC_STATIC_LINK, m_CompanyLink);
    DDX_Control(pDX, IDC_CMB_LOCATION, m_CmbLocation);
    DDX_Control(pDX, IDC_EXPLORER_OCX, m_Browser);
}

BEGIN_MESSAGE_MAP(CBatchMessageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedBtnSearch)
    ON_MESSAGE(WM_FOUND_MEMBER, OnFoundMember) 
    ON_MESSAGE(WM_SENDMSGCOMPLETED, OnSendMsgCompleted) 
    ON_MESSAGE(WM_NOTIFY_CHANGEUSER, OnNotifyChangeUser) 
    ON_BN_CLICKED(IDC_BTN_IMPORT, OnBnClickedBtnImport)
    ON_BN_CLICKED(IDC_BTN_EXPORT, OnBnClickedBtnExport)
    ON_BN_CLICKED(IDC_BTN_DELETE, OnBnClickedBtnDelete)
    ON_BN_CLICKED(IDC_BTN_CLEAR, OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BTN_ADDUSER, OnBnClickedBtnAdduser)
    ON_BN_CLICKED(IDC_BTN_SENDMSG, OnBnClickedBtnSendmsg)
    ON_BN_CLICKED(IDC_BTN_DELUSER, OnBnClickedBtnDeluser)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBnClickedBtnHelp)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_EXPORT2, OnBnClickedBtnExport2)
    ON_BN_CLICKED(IDC_BTN_CLEAR2, OnBnClickedBtnClear2)
    ON_BN_CLICKED(IDC_BTN_REG, OnBnClickedBtnReg)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_MEMBERNAME, OnBnClickedBtnMembername)
    ON_CBN_SELCHANGE(IDC_CMB_TARGET, OnCbnSelchangeCmbTarget)
END_MESSAGE_MAP()


// CBatchMessageDlg ��Ϣ�������

BOOL CBatchMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    m_GetMemberThread.Init(this->GetSafeHwnd());
    m_CompanyLink.SetURL("http://www.21cn2.com");
    m_Browser.Navigate("http://www.21cn2.com/aliww/banner.htm", 0,0,0,0);
   

    // ��ʼ��������
    //InitBizType();
    //InitProvince();
    InitCategory();
    InitLocation();

    int k=0;
    m_CmbSendSpeed.AddString("10��ÿ��");m_CmbSendSpeed.SetItemData(k++, 1400);
    m_CmbSendSpeed.AddString("12��ÿ��");m_CmbSendSpeed.SetItemData(k++, 11400);
    m_CmbSendSpeed.AddString("13��ÿ��");m_CmbSendSpeed.SetItemData(k++, 12400);
    m_CmbSendSpeed.AddString("14��ÿ��");m_CmbSendSpeed.SetItemData(k++, 13400);
    m_CmbSendSpeed.AddString("15��ÿ��");m_CmbSendSpeed.SetItemData(k++, 14400);
    //m_CmbSendSpeed.AddString("15��ÿ��");m_CmbSendSpeed.SetItemData(k++, 17000);
    m_CmbSendSpeed.SetCurSel(0);

    // �����б����ʽ
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// �����
    m_MemberList.InsertColumn(0, "��������", LVCFMT_LEFT, 105);
    m_MemberList.InsertColumn(1, "״̬", LVCFMT_LEFT, 60);

    // ����Ŀ����� 
    k=0;
    m_CmbTarget.AddString("�Ա����");
    m_CmbTarget.SetItemData(k++, 2);
    m_CmbTarget.AddString("�Ա�����");    
    m_CmbTarget.SetItemData(k++, 1);
    //m_CmbTarget.AddString("����Ͱ��̼�");
    //m_CmbTarget.SetItemData(k++, 3);
    m_CmbTarget.SetCurSel(1);

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    if (ValidateReg() == 0)
    {
        ValidUser = 0;
        CString szWndText;
        this->GetWindowText(szWndText);
        this->SetWindowText(szWndText + " (δע��)");
    }
    else
    {        
        CString szWndText;
        this->GetWindowText(szWndText);
        this->SetWindowText(szWndText + " (��ע��)");
    }

    // ����ȱʡֵ
    m_Message = AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME);
    CString szLoginUsers = AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_LOGINUSER);
    if (!szLoginUsers.IsEmpty())
    {
        int s = 0;
        int len = szLoginUsers.GetLength();
       
        while (s < len)
        {
            int e = szLoginUsers.Find("\n", s);
            if (e == -1) e = len;
            CString ss = szLoginUsers.Mid(s, e-s);
            m_UserList.AddString(ss);
            s = e + 1;
        }
    }
    // �����û�
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "recentuser.txt";
    ImportUser(szTempPath);

    UpdateData(FALSE);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBatchMessageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
HCURSOR CBatchMessageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBatchMessageDlg::OnBnClickedBtnSearch()
{ 
    UpdateData(TRUE);

    CString tempText;
    m_BtnSearch.GetWindowText(tempText);
    if (!tempText.CompareNoCase("ֹͣ"))
    {
        m_GetMemberThread.Stop();
        m_BtnSearch.EnableWindow(FALSE);
        return;
    }

  
    /*if (m_Keyword.IsEmpty())
    {
        MessageBox("�����������ؼ��֡�", "����");
        return;
    }*/
    // ���Ҹ���
    CString szLimit;
    this->GetDlgItemText(IDC_EDIT_LIMIT, szLimit);
    long n = atol(szLimit.GetBuffer());
    if (n <= 0)
    {
        MessageBox("������Ϸ��Ĳ��Ҹ�����", "����");
        return;
    }

    m_BtnSearch.SetWindowText("ֹͣ");

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());
    CString szLocation("");
    if (m_CmbLocation.GetCurSel())
    {
        m_CmbLocation.GetWindowText(szLocation);
    }

    m_GetMemberThread.GetMember(m_nMsgType, m_CmbCategory.GetItemData(m_CmbCategory.GetCurSel()), szLocation , m_Keyword, m_MemberLimit);
}

LRESULT CBatchMessageDlg::OnFoundMember(WPARAM wParam, LPARAM lParam)
{
    BOOL bIsFinish = (BOOL)wParam;
    if (!bIsFinish)
    {
        m_BtnSearch.SetWindowText("����");
        m_BtnSearch.EnableWindow(TRUE);
        return 0;
    }

    LPTSTR szMember = (LPTSTR)lParam;

    if (m_nMsgType == 2)
    {
        AddMemberNoRepeat(szMember);
    }
    else
    {
        AddMember(szMember);
    }

    return 0;
}

LRESULT CBatchMessageDlg::OnNotifyChangeUser(WPARAM wParam, LPARAM lParam)
{
    if (m_pWebSendDlg != NULL)
    {
        m_pWebSendDlg->DestroyWindow();
    }

    return 0;
}

LRESULT CBatchMessageDlg::OnSendMsgCompleted(WPARAM wParam, LPARAM lParam)
{
    BOOL bResult = (BOOL)wParam;
    LPTSTR szUserID = (LPTSTR)lParam;

    if (szUserID)
    {
        SetMemberStatus(szUserID, bResult+2);
    }

    ////////////////////////////

    if (ValidUser == 0 && nSendCount++ > 20)
    {
        OnFinishSend();
        MessageBox("��û��ע�ᣬÿ�����ֻ�ܷ���20����Ϣ������ע�ᰴťע���Ϊ�����û���", "����");
        return 0;
    }  
    CString szTopMember;
    int i = GetUnSendedMember(szTopMember);
    if (i < 0)
    {
        OnFinishSend();
        m_MemberList.EnsureVisible(0, TRUE);

        UpdateData(TRUE);
        if (m_bShutdown)
        {
            CShutdownDlg dlg;
            dlg.DoModal();
        }
        else
        {        
            if (m_pWebSendDlg != NULL)
            {
                m_pWebSendDlg->DestroyWindow();
                m_pWebSendDlg = NULL;
                MessageBox("������ɡ�\t\t\t", "����");
            }
        }
        return 0;
    }

//    CString szSenderID;
//    GetSenderUser(szSenderID);

    CString tempMessage = m_Message;
    tempMessage += this->szVersionAD;
    tempMessage.Replace("[������]", szTopMember);

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());
    //CSendMsgThread *pSendMsgThread = new CSendMsgThread();
    //pSendMsgThread->Init(this->GetSafeHwnd());
    //pSendMsgThread->SendMsg(szSenderID, szTopMember, tempMessage, m_nMsgType);
    if (m_pWebSendDlg)
    {
        //CIMMessage msg;
        //msg.nMsgType = 1;
        //msg.szMessage= tempMessage;
        //msg.szSendTo = szTopMember;

        SetMemberStatus(szTopMember, 1);
        m_MemberList.SetHotItem(i);
        m_MemberList.EnsureVisible(i, TRUE);

        // ���ͼ��������
        int m = m_CmbSendSpeed.GetCurSel();
        int smSecond = m_CmbSendSpeed.GetItemData(m); // ������

        //::SendMessage(m_pWebSendDlg->GetSafeHwnd(), WM_SEND_WEB_MESSAGE, (WPARAM)smSecond, (LPARAM)&msg);   
    }

    return 0;
}

void CBatchMessageDlg::ImportUser(CString fileName)
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

            AddMember(szUserName);
            if (szStatus == "1")
            {
                SetMemberStatus(szUserName, 3);
            }
        }
        file.Close();
    }
    
    CString szMemberCount;
    szMemberCount.Format("�����б�(%d)", m_MemberList.GetItemCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnImport()
{        
    CFileDialog fileOpen(TRUE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        ImportUser(fileOpen.GetPathName());
    }
}

void CBatchMessageDlg::ExportUser(CString fileName)
{
    CStdioFile file;
    CFileException ex;

    if (file.Open(fileName, CFile::modeCreate|CFile::modeReadWrite, &ex))
    {
        for (int i = 0; i < m_MemberList.GetItemCount(); i++)
        {
            CString szUserID = m_MemberList.GetItemText(i, 0);
            CString szStatus = m_MemberList.GetItemText(i, 1);
            if (szStatus == STATUS_UNSEND)
            {
                szStatus = "0";
            }
            else
            {
                szStatus = "1";
            }
            file.WriteString(m_MemberList.GetItemText(i, 0)+" " + szStatus + "\n");
        }
        file.Close();
    }
}

void CBatchMessageDlg::OnBnClickedBtnExport()
{    
    if (ValidUser == 0)
    {
        MessageBox("��û��ע�ᣬ����ʹ�ô˹��ܡ�����ע�ᰴťע���Ϊ�����û���", "����");
        return;
    }

    CFileDialog fileOpen(FALSE, ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CString fileName = fileOpen.GetPathName();

        ExportUser(fileName);
    }
}

void CBatchMessageDlg::OnBnClickedBtnDelete()
{
    POSITION pos = m_MemberList.GetFirstSelectedItemPosition();
    int i = m_MemberList.GetNextSelectedItem(pos);
    m_MemberList.DeleteItem(i);

            CString szMemberCount;
        szMemberCount.Format("�����б�(%d)", m_MemberList.GetItemCount());
        this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnClear()
{
    m_MemberList.DeleteAllItems();
            
    CString szMemberCount;
    szMemberCount.Format("�����б�(%d)", 0);
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnAdduser()
{
    CEditUserDlg dlg;
    if (dlg.DoModal() == IDOK)
    {        
        CString s = dlg.m_UserId;
        s += SZ_SPLIT_FLAG;
        s += dlg.m_Password;
        m_UserList.AddString(s);
    }

    /*int n = m_UserList.GetCount();
    switch (n)
    {
    case 1:
        m_CmbSendSpeed.SetCurSel(5);
        break;
    case 2:
        m_CmbSendSpeed.SetCurSel(4);
        break;    
    case 3:
        m_CmbSendSpeed.SetCurSel(3);
        break;
    case 4:
        m_CmbSendSpeed.SetCurSel(2);
        break;
    case 5:
        m_CmbSendSpeed.SetCurSel(1);
        break;
    case 8:
        m_CmbSendSpeed.SetCurSel(0);
        break;
    }*/
}



void CBatchMessageDlg::OnBnClickedBtnSendmsg()
{
    CString szText;
    m_BtnSendMsg.GetWindowText(szText);
    if (szText == "����")
    {
        UpdateData(TRUE);

        // �ж��Ƿ��пɷ��͵ĺ���
        CString temp;
        int k = this->GetUnSendedMember(temp);
        if (k < 0)
        {
            MessageBox("û�д����͵ĺ��ѡ�", "����");
            return;
        }

        if (m_Message.IsEmpty())
        {            
            MessageBox("��Ϣ����Ϊ�ա�", "����");
            return;
        }

        // �ж��Ƿ��пɷ��͵��û�
        if (m_UserList.GetCount() == 0)
        {
            MessageBox("���������������Ϣ���Ա��û���", "����");
            return;
        }

        // �ж��û��Ƿ��Ѿ���¼
        /*for (int i=0; i < m_UserList.GetCount(); i++)
        {
            CString strSenderID;
            m_UserList.GetText(i, strSenderID);
            CString strWndTitle("�������� - ");
            strWndTitle += strSenderID;

            HWND hwnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
            if (!hwnd)
            {
                CString szPrompt;
                szPrompt.Format("�û�%sû�е�¼�����ȵ�¼���û���\n\n�����ʹ�ø��û����ͣ���ӷ����û��б��н���ɾ����", strSenderID);
                MessageBox(szPrompt, "����");
                return;
            }
        }*/        

        // �жϷ����ٶȺ��û������Ƿ�ƥ��
        int m = m_CmbSendSpeed.GetCurSel();
        int smSecond = m_CmbSendSpeed.GetItemData(m); // ������

        if (m_pWebSendDlg == NULL)
        {
            //m_pWebSendDlg = new CWebSendDlg(this);
            //CString szSenderID, szPassword;
            //GetSenderUser(szSenderID, szPassword);

            //m_pWebSendDlg->szSendFrom = szSenderID;
            //m_pWebSendDlg->szPassword = szPassword;

            //m_pWebSendDlg->Create(CWebSendDlg::IDD);
            //m_pWebSendDlg->ShowWindow(SW_SHOW);
        }

        //OnTimer(TIMER_ID);
        //this->SetTimer(TIMER_ID, smSecond, NULL);
        OnStartSend();
    }
    else
    {
        //this->KillTimer(TIMER_ID);
        if (m_pWebSendDlg != NULL)
        {
            //CString szUserID = m_pWebSendDlg->szSendTo;
            //m_pWebSendDlg->DestroyWindow();
            //delete m_pWebSendDlg;
            //m_pWebSendDlg = NULL;
            //this->SetMemberStatus(szUserID, 0);
        }

        OnFinishSend();
    }
}

void CBatchMessageDlg::OnBnClickedBtnDeluser()
{
    int k = m_UserList.GetCurSel();
    m_UserList.DeleteString(k);

    //POSITION pos = userList.GetHeadPosition();
    //for (int i = 0; i < k; i++)
    //{
    //    userList.GetNext(pos);
    //}
    //userList.RemoveAt(pos);    
    
    /*int n = m_UserList.GetCount();
    switch (n)
    {
    case 1:
        m_CmbSendSpeed.SetCurSel(5);
        break;
    case 2:
        m_CmbSendSpeed.SetCurSel(4);
        break;    
    case 3:
        m_CmbSendSpeed.SetCurSel(3);
        break;
    case 4:
        m_CmbSendSpeed.SetCurSel(2);
        break;
    case 5:
        m_CmbSendSpeed.SetCurSel(1);
        break;
    case 8:
        m_CmbSendSpeed.SetCurSel(0);
        break;
    }*/
}

void CBatchMessageDlg::OnBnClickedBtnHelp()
{
    /*CHelpDlg dlg;
    dlg.DoModal();*/
    CHelpDlg *pDlg = new CHelpDlg();
    pDlg->Create(IDD_DLG_HELP, this);
    pDlg->ShowWindow(SW_SHOW);

}


void CBatchMessageDlg::AddMember(CString szMember)
{
    int cnt = m_MemberList.GetItemCount();
   
    // ����û�
    m_MemberList.InsertItem(cnt, szMember);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();
            
    CString szMemberCount;
    szMemberCount.Format("�����б�(%d)", ++cnt);
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}


void CBatchMessageDlg::AddMemberNoRepeat(CString szMember)
{
    int cnt = m_MemberList.GetItemCount();
    for(int i=0; i < cnt; i++)
    {
        CString s=m_MemberList.GetItemText(i,0);
        if (s==szMember)
        {
            return;
        }
    }

    // ����û�
    m_MemberList.InsertItem(cnt, szMember);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();

    CString szMemberCount;
    szMemberCount.Format("�����б�(%d)", ++cnt);
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

//nStatus 0�������ͣ�1�������У�2������ʧ�ܣ�3�����ͳɹ�
void CBatchMessageDlg::SetMemberStatus(CString szMember, int nStatus)
{
    int n = m_MemberList.GetItemCount();
    int k = -1;
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 0);
        if (temp == szMember)
        {
            k = i;
            break;
        }
    }
    if (k < 0)
    {
        return;
    }

    switch(nStatus)
    {
        case 0:
            m_MemberList.SetItemText(k, 1, STATUS_UNSEND);
            break;
        case 1:
            m_MemberList.SetItemText(k, 1, STATUS_SENDING);
            break;
        case 2:
            m_MemberList.SetItemText(k, 1, STATUS_FAIL);
            break;
        case 3:
            m_MemberList.SetItemText(k, 1, STATUS_SENDED);
            break;
        default:
            break;
    }
}

void CBatchMessageDlg::OnTimer(UINT nIDEvent)
{
    if (ValidUser == 0 && nSendCount++ > 20)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        MessageBox("��û��ע�ᣬÿ�����ֻ�ܷ���20����Ϣ������ע�ᰴťע���Ϊ�����û���", "����");
        return;
    }  
    CString szTopMember;
    int i = GetUnSendedMember(szTopMember);
    if (i < 0)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        m_MemberList.EnsureVisible(0, TRUE);

        UpdateData(TRUE);
        if (m_bShutdown)
        {
            CShutdownDlg dlg;
            dlg.DoModal();
        }
        return;
    }

    //CString szSenderID, szPassword;
    //GetSenderUser(szSenderID, szPassword);

    CString tempMessage = m_Message;
    tempMessage += szVersionAD;
    tempMessage.Replace("[������]", szTopMember);

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());
    //CSendMsgThread *pSendMsgThread = new CSendMsgThread();
    //pSendMsgThread->Init(this->GetSafeHwnd());
    //pSendMsgThread->SendMsg(szSenderID, szTopMember, tempMessage, m_nMsgType);
    /*if (m_pWebSendDlg)
    {
        CIMMessage msg;
        msg.nMsgType = 1;
        msg.szSendFrom = "abc42356879";
        msg.szPassword = "abc42356879454r654";
        msg.szMessage= "���";
        msg.szSendTo = "wenghaiyu";

        this->KillTimer(TIMER_ID);
        ::SendMessage(m_pWebSendDlg->GetSafeHwnd(), WM_SEND_WEB_MESSAGE, (WPARAM)1, (LPARAM)&msg);
        
    }*/

    SetMemberStatus(szTopMember, 1);
    m_MemberList.SetHotItem(i);
    m_MemberList.EnsureVisible(i, TRUE);

    CDialog::OnTimer(nIDEvent);
}

int CBatchMessageDlg::GetUnSendedMember(CString& szMemberName)
{
    int n = m_MemberList.GetItemCount();
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 1);
        if (temp == STATUS_UNSEND)
        {
            szMemberName = m_MemberList.GetItemText(i, 0);
            return i;
        }
    }
    return -1;
}

void CBatchMessageDlg::OnStartSend()
{
    m_CmbSendSpeed.EnableWindow(FALSE);
    m_BtnAddUser.EnableWindow(FALSE);
    m_BtnDelUser.EnableWindow(FALSE);   
    m_EditMsg.EnableWindow(FALSE);

    m_BtnSendMsg.SetWindowText("ֹͣ");
}

void CBatchMessageDlg::OnFinishSend()
{
    m_CmbSendSpeed.EnableWindow(TRUE);
    m_BtnAddUser.EnableWindow(TRUE);
    m_BtnDelUser.EnableWindow(TRUE);
    m_EditMsg.EnableWindow(TRUE);

    m_BtnSendMsg.SetWindowText("����");
}

int CBatchMessageDlg::GetSenderUser(CString& szUserID, CString& szPassword)
{
    int n = m_UserList.GetCount();
    if (n == 0)
    {
        return -1;
    }

    int i = m_UserList.GetCurSel();
    CString s, ss(SZ_SPLIT_FLAG);
    if (i < 0 || i == n-1)
    {
        m_UserList.SetCurSel(0);
        m_UserList.GetText(0, s);
        int i = s.Find(SZ_SPLIT_FLAG);
        szUserID = s.Left(i);
        int j = s.GetLength()-szUserID.GetLength()-ss.GetLength();
        szPassword = s.Right(j);
        return 0;
    }
    else
    {
        m_UserList.SetCurSel(i+1);
        m_UserList.GetText(i+1, szUserID);
        return i+1;
    }
}

long CBatchMessageDlg::ValidateReg()
{
    // ��֤�û��Ƿ�ע��
    CString szMachineCode, szFileChecksum;
    GetMachineCode(szMachineCode);
    szFileChecksum = GetFileMD5Checksum();

    CString m_RegUserID = URLEncode(AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_REGUSERID));

    CString szAuthURL;
#ifdef DEBUG
    szAuthURL.Format("http://localhost/AliwwAdmin/Aliww/AuthPage.aspx?machinecode=%s&userid=%s&fileid=%s&checksum=%s", szMachineCode, m_RegUserID, FILE_ID, szFileChecksum);

    CString szRet   = "1000000";
    CString szData1 = "http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    CString szData2 = "http://search.china.alibaba.com/search/company_search.htm?categoryId=0";
    CString szData3 = "";
    CString szData4 = "wangwang:SendIM?s=1&v=3&mul=1&uid=%s&uid_t=%s&suid=%s&desc=";
#else
    szAuthURL.Format("http://www.21cn2.com/AuthPage.aspx?machinecode=%s&userid=%s&fileid=%s&checksum=%s", szMachineCode, m_RegUserID, FILE_ID, szFileChecksum);

    CString szResult = GetPageDirect(szAuthURL);

    int p1, p2;
    CString szFlag;

    szFlag = "<Span id=\"result\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szRet = szResult.Mid(p1,p2-p1);
        
    // �Ա���ַ
    szFlag = "<Span id=\"data1\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData1 = szResult.Mid(p1,p2-p1);
    
    // ����Ͱ͵�ַ
    szFlag = "<Span id=\"data2\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData2 = szResult.Mid(p1,p2-p1);

    // ûע���û�����Ϣβ��
    szFlag = "<Span id=\"data3\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData3 = szResult.Mid(p1,p2-p1);
#endif
    m_GetMemberThread.szTaobaoAddress = szData1.Trim();
    m_GetMemberThread.szAlibabaAddress = szData2.Trim();
    this->szVersionAD = szData3.Trim();

    int nRetValue = 0;
    try
    {
        nRetValue = atoi(szRet.Trim().GetBuffer(0));
    }
    catch(CSimpleException* )
    {

    }
    return nRetValue;
}

void CBatchMessageDlg::OnBnClickedBtnExport2()
{
    if (ValidUser == 0)
    {
        MessageBox("��û��ע�ᣬ����ʹ�ô˹��ܡ�����ע�ᰴťע���Ϊ�����û���", "����");
        return;
    }  
    CFileDialog fileOpen(FALSE, ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CString fileName = fileOpen.GetPathName();

        CStdioFile file;
        CFileException ex;

        if (!file.Open(fileName, CFile::modeCreate|CFile::modeReadWrite, &ex))
        {
            TCHAR szError[1024];
            ex.GetErrorMessage(szError, 1024);
            MessageBox(szError, "��ʾ");
        }

        for (int i = 0; i < m_MemberList.GetItemCount(); i++)
        {
            CString szStatus=m_MemberList.GetItemText(i, 1);
            if (!szStatus.CompareNoCase(STATUS_UNSEND))
            {
                file.WriteString(m_MemberList.GetItemText(i, 0)+"\n");
            }
        }
        
        file.Close();
    }
}

void CBatchMessageDlg::OnBnClickedBtnClear2()
{
    int n = m_MemberList.GetItemCount();
    for (int i = n-1; i >= 0 ; i--)
    {
        CString szStatusName = m_MemberList.GetItemText(i, 1);
        if (szStatusName.CompareNoCase(STATUS_UNSEND))
        {
            m_MemberList.DeleteItem(i);
        }
    }

    CString szMemberCount;
    szMemberCount.Format("�����б�(%d)", m_MemberList.GetItemCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnReg()
{
    CRegDlg dlg;
    dlg.DoModal();
}

void CBatchMessageDlg::InitCategory()
{
    m_CmbCategory.ResetContent();
    int i=0;
    m_CmbCategory.AddString("�������");
    m_CmbCategory.SetItemData(i++, 0);

    m_CmbCategory.AddString("�ֻ�/ͨѶ/��");
    m_CmbCategory.SetItemData(i++, 13);
    m_CmbCategory.AddString("�������/������Ӱ");
    m_CmbCategory.SetItemData(i++, 17);
    m_CmbCategory.AddString("MP3/��������/����/�ҵ�");
    m_CmbCategory.SetItemData(i++, 12);
    m_CmbCategory.AddString("���ԡ����缰����豸");
    m_CmbCategory.SetItemData(i++, 11);
    m_CmbCategory.AddString("�칫�豸���Ĳ�");
    m_CmbCategory.SetItemData(i++, 21);
    m_CmbCategory.AddString("��ֵ����/IP��;/����绰");
    m_CmbCategory.SetItemData(i++, 1041);

    m_CmbCategory.AddString("Ůװ/Ůʿ��Ʒ");
    m_CmbCategory.SetItemData(i++, 14);
    m_CmbCategory.AddString("Ůʿ���/Ьñ/Ůʿ���");
    m_CmbCategory.SetItemData(i++, 31);
    m_CmbCategory.AddString("�鱦����/�ֱ�/�۾�");
    m_CmbCategory.SetItemData(i++, 23);
    m_CmbCategory.AddString("��ױ/��ˮ/����/����");
    m_CmbCategory.SetItemData(i++, 15);

    m_CmbCategory.AddString("�Ӽ�����/װ��/�ľ�/����");
    m_CmbCategory.SetItemData(i++, 24);
    m_CmbCategory.AddString("���õ���/������е");
    m_CmbCategory.SetItemData(i++, 36);
    m_CmbCategory.AddString("���/ģ��/��ͨ��Ʒ");
    m_CmbCategory.SetItemData(i++, 30);
    m_CmbCategory.AddString("װ��/��𹤾�/����");
    m_CmbCategory.SetItemData(i++, 26);
    m_CmbCategory.AddString("ͯװ/Ӥ��/�и���Ʒ");
    m_CmbCategory.SetItemData(i++, 1020);
    m_CmbCategory.AddString("ʳƷ/����Ʒ");
    m_CmbCategory.SetItemData(i++, 29);
    m_CmbCategory.AddString("����/Ʊ��/����/�廨����");
    m_CmbCategory.SetItemData(i++, 27);

    m_CmbCategory.AddString("�ղ�Ʒ/�ʱ�/�Ŷ�/�ֻ�");
    m_CmbCategory.SetItemData(i++, 20);
    m_CmbCategory.AddString("����/����Ʒ/��Ʒ/�ʻ�");
    m_CmbCategory.SetItemData(i++, 32);

    m_CmbCategory.AddString("�鼮/��־/��ֽ");
    m_CmbCategory.SetItemData(i++, 34);
    m_CmbCategory.AddString("����/Ӱ��/����/����");
    m_CmbCategory.SetItemData(i++, 33);

    m_CmbCategory.AddString("����/��Ϸ/�ܱ�");
    m_CmbCategory.SetItemData(i++, 16);
    m_CmbCategory.AddString("������Ʒ/��Ϸװ��");
    m_CmbCategory.SetItemData(i++, 35);

    m_CmbCategory.AddString("��װ/�������");
    m_CmbCategory.SetItemData(i++, 37);
    m_CmbCategory.AddString("����/Ħ��/���г�");
    m_CmbCategory.SetItemData(i++, 22);
    m_CmbCategory.AddString("ZIPPO/��ʿ����/��ʿ��Ʒ");
    m_CmbCategory.SetItemData(i++, 1040);

    m_CmbCategory.AddString("�˶�/����/����");
    m_CmbCategory.SetItemData(i++, 18);

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitLocation()
{
    m_CmbLocation.ResetContent();
    m_CmbLocation.AddString("���е���");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�Ϻ�");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�Ͼ�");
    m_CmbLocation.AddString("�人");
    m_CmbLocation.AddString("���");
    m_CmbLocation.AddString("���");
    m_CmbLocation.AddString("�ɶ�");
    m_CmbLocation.AddString("������");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�ൺ");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("��ɳ");
    m_CmbLocation.AddString("�Ϸ�");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�ϲ�");
    m_CmbLocation.AddString("֣��");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("ʯ��ׯ");
    m_CmbLocation.AddString("��ݸ");
    m_CmbLocation.AddString("̨��");
    m_CmbLocation.SetCurSel(0);
}

void CBatchMessageDlg::InitBizType()
{
    m_CmbCategory.ResetContent();
    int i=0;
    m_CmbCategory.AddString("���о�Ӫģʽ");
    m_CmbCategory.SetItemData(i++, 0);
    m_CmbCategory.AddString("������");
    m_CmbCategory.SetItemData(i++, 1);
    m_CmbCategory.AddString("ó����");
    m_CmbCategory.SetItemData(i++, 2);
    m_CmbCategory.AddString("������");
    m_CmbCategory.SetItemData(i++, 4);
    m_CmbCategory.AddString("��������������");
    m_CmbCategory.SetItemData(i++, 8);

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitProvince()
{
    m_CmbLocation.ResetContent();
    m_CmbLocation.AddString("����ʡ��");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�㶫");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�ӱ�");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("������");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("���ɹ�");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�ຣ");
    m_CmbLocation.AddString("ɽ��");
    m_CmbLocation.AddString("ɽ��");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�Ϻ�");
    m_CmbLocation.AddString("�Ĵ�");
    m_CmbLocation.AddString("���");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�½�");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("�㽭");
    m_CmbLocation.AddString("����");
    m_CmbLocation.AddString("HK");
    m_CmbLocation.AddString("MO");
    m_CmbLocation.AddString("TW");

    m_CmbLocation.SetCurSel(0);
}


void CBatchMessageDlg::OnDestroy()
{
    // ������Ϣ����
    UpdateData(TRUE);
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME, m_Message);
    CString szLoginUser = "";
    for (int i = 0; i < m_UserList.GetCount(); i++)
    {
        CString szTemp;
        m_UserList.GetText(i, szTemp);
        if ( i != m_UserList.GetCount()-1) szTemp += "\n";
        szLoginUser += szTemp;
    }
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_LOGINUSER, szLoginUser);
    
    // ����������û�
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "recentuser.txt";
    ExportUser(szTempPath);

    CDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
}

void CBatchMessageDlg::OnBnClickedBtnMembername()
{
    UpdateData(TRUE);
    m_Message = "[������]" + m_Message;
    UpdateData(FALSE);
}

void CBatchMessageDlg::OnCbnSelchangeCmbTarget()
{
    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    if (m_nMsgType == 1 || m_nMsgType == 2)
    {
        InitCategory();
        InitLocation();
    }
    else
    {
        InitBizType();
        InitProvince();
    }
}
