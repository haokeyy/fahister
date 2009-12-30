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
#include "Constants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchMessageDlg �Ի���

#define TIMER_ID 100

#define STATUS_UNSEND "������"
#define STATUS_SENDING "������..."
#define STATUS_SENDED "�ѷ���"
#define STATUS_FAIL   "����ʧ��"



CBatchMessageDlg::CBatchMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchMessageDlg::IDD, pParent)
    , m_Message(_T(""))
    , m_MemberLimit(100)
    , m_Keyword(_T(""))
    , m_bShutdown(FALSE)
    , m_bChkShowSendWindow(FALSE)
    , m_nSendLimit(50)
{
    nSendCount = 0;
    ValidUser = 20;
    szVersionAD = "\n����Ϣ�ɡ����ư���Ⱥ������������\n�ٷ���ַ��http://www.21cn2.com";

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
	DDX_Check(pDX, IDC_CHK_SHOWWINDOW, m_bChkShowSendWindow);
	DDX_Text(pDX, IDC_SEND_LIMIT, m_nSendLimit);
	DDX_Control(pDX, IDC_CMB_MEMBERGENDER, m_cmbMemberGender);
	DDX_Control(pDX, IDC_CHK_OBLYONLINE, m_ChkOnlyOnline);
	DDX_Control(pDX, IDC_CHK_ONLYBUYER, m_Chk_OnlyBuyer);
	DDX_Control(pDX, IDC_CHK_ONLYSENDONLINE, m_Chk_OnlySendOnline);
}

BEGIN_MESSAGE_MAP(CBatchMessageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedBtnSearch)
    ON_MESSAGE(WM_FOUND_MEMBER, OnFoundMember) 
    ON_MESSAGE(WM_SENDMSGCOMPLETED, OnSendMsgCompleted) 
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
    ON_BN_CLICKED(IDC_IMPORT_SENDER, &CBatchMessageDlg::OnBnClickedImportSender)
    ON_BN_CLICKED(IDC_EXPORT_SENDER, &CBatchMessageDlg::OnBnClickedExportSender)
    ON_BN_CLICKED(IDC_CLEAR_SENDER, &CBatchMessageDlg::OnBnClickedClearSender)
	ON_CBN_SELCHANGE(IDC_CMB_TARGET, &CBatchMessageDlg::OnCbnSelchangeCmbTarget)
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
   

	m_cmbMemberGender.SetCurSel(0);
	m_Chk_OnlyBuyer.ShowWindow(SW_HIDE);

    // ��ʼ��������
    InitCategory();
    InitLocation();

    int k=0;
    m_CmbSendSpeed.AddString("1��ÿ��");m_CmbSendSpeed.SetItemData(k++, 1000);
    m_CmbSendSpeed.AddString("2��ÿ��");m_CmbSendSpeed.SetItemData(k++, 2000);
    m_CmbSendSpeed.AddString("3��ÿ��");m_CmbSendSpeed.SetItemData(k++, 3000);
    m_CmbSendSpeed.AddString("5��ÿ��");m_CmbSendSpeed.SetItemData(k++, 5000);
    m_CmbSendSpeed.AddString("10��ÿ��");m_CmbSendSpeed.SetItemData(k++, 10000);
    m_CmbSendSpeed.AddString("15��ÿ��");m_CmbSendSpeed.SetItemData(k++, 15000);

    // �����б����ʽ
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// �����
    m_MemberList.InsertColumn(0, "��������", LVCFMT_LEFT, 105);
    m_MemberList.InsertColumn(1, "״̬", LVCFMT_LEFT, 60);
    m_MemberList.InsertColumn(2, "�Ƿ�����", LVCFMT_LEFT, 60);

    m_UserList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_UserList.InsertColumn(0, "�û���", LVCFMT_LEFT, 85);
    m_UserList.InsertColumn(1, "����", LVCFMT_LEFT, 0);    
    m_UserList.InsertColumn(2, "���͸���", LVCFMT_LEFT, 65);
    // ����Ŀ����� 
    k=0;
    m_CmbTarget.AddString("�Ա����");
    m_CmbTarget.SetItemData(k++, 2);
    m_CmbTarget.AddString("�Ա�����");    
    m_CmbTarget.SetItemData(k++, 1);
    m_CmbTarget.SetCurSel(1);

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    ValidUser = ValidateReg();
    if (ValidUser != 1000000)
    {
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

	nSendedTotal = 0;
    // ����ȱʡֵ
    m_Message = AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME);
    int nSpeed = AfxGetApp()->GetProfileInt(REG_REGSECTION_NAME, REG_SENDSPEED, 2);
    m_CmbSendSpeed.SetCurSel(nSpeed);
    CString szLoginUsers = AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_LOGINUSER);
    if (!szLoginUsers.IsEmpty())
    {
        int s = 0;
        int len = szLoginUsers.GetLength();
        int k=0;
       
        while (s < len)
        {
            int e1 = szLoginUsers.Find("/", s);
            if (e1 < 0) break;
            int e2 = szLoginUsers.Find("/", e1+1);
            if (e2 == -1) e2 = len;
            int e3 = szLoginUsers.Find("\n", e1+1);
            if (e3 == -1) e3 = len;

            if (e2 > e3) break;

            CString ss1 = szLoginUsers.Mid(s, e1-s);
            CString ss2 = szLoginUsers.Mid(e1+1, e2-e1-1);
            CString ss3 = szLoginUsers.Mid(e2+1, e3-e2-1);
            m_UserList.InsertItem(k, ss1);
            m_UserList.SetItemText(k,1, ss2);
            m_UserList.SetItemText(k,2, ss3);
            s = e3 + 1;

            k++;
        }
    }
    // �����û�
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "websend.txt";
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


    CString szLocation;
    m_CmbLocation.GetWindowText(szLocation);
	if ("���е���" == szLocation)
	{
		szLocation = "";
	}

	//
	CString szRateSum = "";
	for (int i = 1; i <= 15; i++)
	{
		CString szWndText;
		szWndText.Format("RATE_%d", i);
		CButton *chkBtn = (CButton*)this->FindWindowEx(this->GetSafeHwnd(), NULL, "Button", szWndText);
		if (chkBtn->GetCheck() == BST_CHECKED)
		{
			CString rate;
			if (szRateSum.IsEmpty())
			{
				rate.Format("%d", i);
				szRateSum += rate;
			}
			else
			{				
				rate.Format(",%d", i);
				szRateSum += rate;
			}
		}
	}

    m_GetMemberThread.GetMember(m_nMsgType, m_CmbCategory.GetItemData(m_CmbCategory.GetCurSel()), szLocation , m_Keyword,szRateSum, m_MemberLimit);
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

			
	// �����ж�
	if (this->m_ChkOnlyOnline.GetCheck() == BST_CHECKED)
	{
		if (!IsOnline(szMember))
		{
			return 0;
		}
	}
	// �Ա��ж�
	if (m_cmbMemberGender.GetCurSel() > 0)
	{
		int gender = GetUserGender(szMember);
		if (gender != m_cmbMemberGender.GetCurSel())
		{
			return 0;
		}
	}

    if (m_nMsgType == 2)
    {
		// ��ҵ���		
		CString szLocation;
		m_CmbLocation.GetWindowText(szLocation);
		if (!(szLocation == "���е���" || szLocation.IsEmpty()))
		{
			if (!IsFromLocation(szMember, szLocation))
			{
				return 0;
			}
		}

		// ��ҵ�"��������"
		if (this->m_Chk_OnlyBuyer.GetCheck() == BST_CHECKED)
		{
			if (GetUserRate(szMember, 1) > 0)
			{
				return 0;
			}
		}

        AddMemberNoRepeat(szMember);
    }
    else
    {
        AddMember(szMember);
    }

    return 0;
}

LRESULT CBatchMessageDlg::OnSendMsgCompleted(WPARAM wParam, LPARAM lParam)
{
    int bResult = (int)lParam;// -1:��¼ʧ�ܣ�0:ʧ�ܣ�1:�ɹ�

    CString szSendFrom, szSendTo;
    this->GetDlgItemText(IDC_SEND_FROM, szSendFrom);
    this->GetDlgItemText(IDC_SEND_TO, szSendTo);

    int index = -1;
    for (int i = 0; i < m_UserList.GetItemCount(); i++)
    {
        CString ss = m_UserList.GetItemText(i, 0);
        if (ss == szSendFrom)
        {
            index = i;
        }
    }

    if (bResult == 1)
    {
        CString ss = m_UserList.GetItemText(index, 2);

        int c = atoi(ss.GetBuffer(0));
        CString sss;
        sss.Format("%d", ++c);

        m_UserList.SetItemText(index, 2, sss);        
        SetMemberStatus(szSendTo, 3);

    }
    else if (bResult == 0)
    {
        SetMemberStatus(szSendTo, 2);
    }
    else
    {
        m_UserList.SetItemText(index, 2, "-1");
        SetMemberStatus(szSendTo, 2);
    }

	nSendedTotal++;
	SetMemberStatInfo();

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

            int cnt = m_MemberList.GetItemCount();
   
            // ����û�
            m_MemberList.InsertItem(cnt, szUserName);
            if (szStatus == "1")
            {
                m_MemberList.SetItemText(cnt, 1, STATUS_SENDED);
            }
            else
            {
                m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
            }
        }
        file.Close();
    }
    
    nSendedTotal = GetSendedCount();
	SetMemberStatInfo();
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
    if (ValidUser != 1000000)
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

}

void CBatchMessageDlg::OnBnClickedBtnClear()
{
    m_MemberList.DeleteAllItems();
            
    nSendedTotal = 0;
	SetMemberStatInfo();
}

void CBatchMessageDlg::OnBnClickedBtnAdduser()
{
    CEditUserDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        int cnt = m_UserList.GetItemCount();
        m_UserList.InsertItem(cnt, dlg.m_UserId);
        m_UserList.SetItemText(cnt, 1, dlg.m_Password);
        m_UserList.SetItemText(cnt, 2, "0");
    }
}

void CBatchMessageDlg::OnBnClickedBtnSendmsg()
{
    CString szText;
    m_BtnSendMsg.GetWindowText(szText);
    if (szText == "����")
    {
        UpdateData(TRUE);

        CString szLimit;
        this->GetDlgItemText(IDC_SEND_LIMIT, szLimit);
        long n = atol(szLimit.GetBuffer());
        if (n <= 0)
        {
            MessageBox("������Ϸ��ķ���������", "����");
            return;
        }
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
        if (m_UserList.GetItemCount() == 0)
        {
            MessageBox("���������������Ϣ���Ա��û���", "����");
            return;
        }

        // �жϷ����ٶȺ��û������Ƿ�ƥ��
        int m = m_CmbSendSpeed.GetCurSel();
        int smSecond = m_CmbSendSpeed.GetItemData(m);

        OnTimer(TIMER_ID);
        this->SetTimer(TIMER_ID, smSecond, NULL);
        OnStartSend();
    }
    else
    {
        this->KillTimer(TIMER_ID);

        OnFinishSend();
    }
}

void CBatchMessageDlg::OnBnClickedBtnDeluser()
{
    POSITION pos = m_UserList.GetFirstSelectedItemPosition();

    m_UserList.DeleteItem(m_UserList.GetNextSelectedItem(pos));
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
            
	SetMemberStatInfo();;
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

	SetMemberStatInfo();;
}

//nStatus 0�������ͣ�1�������У�2������ʧ�ܣ�3�����ͳɹ�
void CBatchMessageDlg::SetMemberStatus(CString szMember, int nStatus)
{
    int n = m_MemberList.GetItemCount();
    int k = -1;
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 0); // �û���
        CString temp2 = m_MemberList.GetItemText(i, 1);
        if (temp == szMember)
        {
            if (nStatus == 1) // ���Ҫ����Ϊ�����У�������ǰ��״̬Ϊ������
            {
                if (temp2 == STATUS_UNSEND)
                {
                    k = i;
                    break;
                }
            }
            else if (nStatus == 3) // ���Ҫ����Ϊ�ѷ��ͣ�������ǰ��״̬Ϊ������
            {
                if (temp2 == STATUS_SENDING)
                {
                    k = i;
                    break;
                }
            }
            else
            {
                k = i;
                break;
            }
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
    if (nSendCount > ValidUser)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        MessageBox("��û��ע�ᣬ���������Ѵﵽ���ơ�����ע�ᰴťע���Ϊ�����û���", "����");
        return;
    }  
    CString szTopMember;
    int i = GetUnSendedMember(szTopMember);
    if (i < 0)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        m_MemberList.EnsureVisible(0, TRUE);

        if (m_bShutdown)
        {
            CShutdownDlg dlg;
            dlg.DoModal();
        }
        else
        {
            MessageBox("�������!", "��ʾ");
        }
        return;
    }

    CString szSenderID, szPassword;
    int j = GetSenderUser(szSenderID, szPassword);
    if (j < 0)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        m_UserList.EnsureVisible(0, TRUE);

        MessageBox("�����ʺŶ��Ѵﵽ�������ƣ�������ʺŵ�¼��\n\n(��������Ϊ\"-1\"���ʺű�ʾ��¼ʧ��)!", "��ʾ");
        return;
    }

    CString tempMessage = m_Message;
    tempMessage += szVersionAD;
    tempMessage.Replace("[������]", szTopMember);
    
    CString szWndTitle;
    this->GetWindowText(szWndTitle);

    CString szFileName;
    DWORD nProcID = ::GetCurrentProcessId();

    GetProcessNameByProcessID(nProcID, szFileName);

    int index = szFileName.ReverseFind('\\');
    CString szPath = szFileName.Left(index);
    szPath += "\\sender.exe";

    CFileFind finder;
    if (finder.FindFile(szPath) == FALSE)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        MessageBox("û���ҵ��ļ�" + szPath, "����");

        return;
    }

    UpdateData(TRUE);
    // ��ʼ����
    UINT SHOW_FLAG = SW_HIDE;
    if (m_bChkShowSendWindow)
    {
        SHOW_FLAG = SW_SHOW;
    }
    ::WinExec(szPath, SHOW_FLAG);
    Sleep(500);
    
    HWND hWnd = ::FindTopWindowExactly(szTaobaoSendUrl.GetBuffer(0), "#32770");
    if (hWnd)
    {
        HWND hSendFrom = ::FindWindowEx(hWnd, NULL, "Edit", "");
        HWND hPassword = ::GetWindow(hSendFrom, GW_HWNDNEXT);
        HWND hSendTo = ::GetWindow(hPassword, GW_HWNDNEXT);
        HWND hMsg = ::GetWindow(hSendTo, GW_HWNDNEXT);
        HWND hMainWndTitle = ::GetWindow(hMsg, GW_HWNDNEXT);

        ::SendMessage(hSendFrom, WM_SETTEXT, 0, (LPARAM)szSenderID.GetBuffer(0));
        ::SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)szPassword.GetBuffer(0));    
        ::SendMessage(hSendTo, WM_SETTEXT, 0, (LPARAM)szTopMember.GetBuffer(0));    
        ::SendMessage(hMsg, WM_SETTEXT, 0, (LPARAM)tempMessage.GetBuffer(0));
        ::SendMessage(hMainWndTitle, WM_SETTEXT, 0, (LPARAM)szWndTitle.GetBuffer(0));   
        
        ::SendMessage(hWnd, WM_SEND_WEB_MESSAGE, (WPARAM)0, (LPARAM)1);
        // ��������

        nSendCount++;
        SetMemberStatus(szTopMember, 1);
        m_MemberList.SetHotItem(i);
        m_MemberList.EnsureVisible(i, TRUE);
    }

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

			if (m_Chk_OnlySendOnline.GetCheck() == BST_CHECKED)
			{
				CString onlineStatus = m_MemberList.GetItemText(i, 2);
				if (onlineStatus == "��")
				{
					continue;
				} 
				if (IsOnline(szMemberName) == FALSE)
				{
					m_MemberList.SetItemText(i, 2, "��");
					continue;
				}
			}
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
    int n = m_UserList.GetItemCount();
    if (n == 0)
    {
        return -1;
    }

    int i = m_UserList.GetHotItem();
   
    int k = 0;
    while (k++ < n)
    {
        if (i < 0 || i >= n-1)
        {
            i = 0;
        }
        else
        {
            i++;
        }
        
        CString cnt = m_UserList.GetItemText(i, 2);
        long c = atol(cnt.GetBuffer(0));
        if (c >= 0  && c < m_nSendLimit)
        {
            m_UserList.SetHotItem(i);
            szUserID = m_UserList.GetItemText(i, 0);
            szPassword = m_UserList.GetItemText(i, 1); 
            return i;
        }
    }

    return -1;
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
    CString szData4 = "�����Ա���Ϣ-";

#else
    szAuthURL.Format("http://www.21cn2.com/AuthPageWeb.aspx?machinecode=%s&userid=%s&fileid=%s&checksum=%s", szMachineCode, m_RegUserID, FILE_ID, szFileChecksum);

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

    // �������������õ�URL��ַ
    szFlag = "<Span id=\"data4\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData4 = szResult.Mid(p1,p2-p1);

#endif

    m_GetMemberThread.szTaobaoAddress = szData1.Trim();
    m_GetMemberThread.szAlibabaAddress = szData2.Trim();
    this->szVersionAD = szData3.Trim();
    this->szTaobaoSendUrl = szData4.Trim();

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
    if (ValidUser != 1000000)
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

    nSendedTotal = 0;
	SetMemberStatInfo();
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
    while (ShopCategorys[i].Value != -1)
    {
        m_CmbCategory.AddString(ShopCategorys[i].Name);
        m_CmbCategory.SetItemData(i, ShopCategorys[i].Value);
        i++;
    }

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitLocation()
{
    m_CmbLocation.ResetContent();
   
    int i=0;
    while (Citys[i].Value != -1)
    {
        m_CmbLocation.AddString(Citys[i].Name);
        //m_CmbLocation.SetItemData(i, Citys[i].Value);
        i++;
    }

    m_CmbLocation.SetCurSel(0);
}

void CBatchMessageDlg::OnDestroy()
{
    // ������Ϣ����
    UpdateData(TRUE);
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME, m_Message);
    CString szLoginUser = "";
    for (int i = 0; i < m_UserList.GetItemCount(); i++)
    {
        CString szTemp1 = m_UserList.GetItemText(i, 0) + "/";
        CString szTemp2 = m_UserList.GetItemText(i, 1) + "/";
        CString szTemp3 = m_UserList.GetItemText(i, 2);

        if ( i != m_UserList.GetItemCount()-1) szTemp3 += "\n";
        szLoginUser += szTemp1 + szTemp2 + szTemp3;
    }
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_LOGINUSER, szLoginUser);
    
    int nSpeed = m_CmbSendSpeed.GetCurSel();
    AfxGetApp()->WriteProfileInt(REG_REGSECTION_NAME, REG_SENDSPEED, nSpeed);
    
    // ����������û�
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "websend.txt";
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


void CBatchMessageDlg::OnBnClickedImportSender()
{
    if (ValidUser != 1000000)
    {
        MessageBox("��û��ע�ᣬ����ʹ�ô˹��ܡ�����ע�ᰴťע���Ϊ�����û���", "����");
        return;
    }  

    CFileDialog fileOpen(TRUE,  ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CString strLine;

        CStdioFile file;
        CFileException ex;

        if (file.Open(fileOpen.GetPathName(), CFile::modeRead, &ex))
        {
            int k=0;
            while (file.ReadString(strLine))
            {
                int len = strLine.GetLength();
                int e1 = strLine.Find("/", 0);
                int e2 = strLine.Find("\n", e1+1);
                if (e2 == -1) e2 = len;
                CString ss1 = strLine.Left(e1);
                CString ss2 = strLine.Mid(e1+1, e2-e1-1);
                m_UserList.InsertItem(k, ss1);
                m_UserList.SetItemText(k,1, ss2);

                CString ss3 = "0";
                //if (len > e2+1)
                //    ss3 = strLine.Mid(e2+1, len);

                m_UserList.SetItemText(k,2, ss3);

                k++;
            }
            file.Close();
        }
    }
}

void CBatchMessageDlg::OnBnClickedExportSender()
{
    
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

        for (int i = 0; i < m_UserList.GetItemCount(); i++)
        {
            CString szTemp1 = m_UserList.GetItemText(i, 0) + "/";
            CString szTemp2 = m_UserList.GetItemText(i, 1) + "\n";
            CString szTemp3 = m_UserList.GetItemText(i, 2);

            if (szTemp3 != "-1")
            {
                file.WriteString(szTemp1 + szTemp2);
            }
        }
        
        file.Close();
    }    
}

void CBatchMessageDlg::OnBnClickedClearSender()
{
    m_UserList.DeleteAllItems();
}

void CBatchMessageDlg::OnCbnSelchangeCmbTarget()
{
	if (m_CmbTarget.GetCurSel() == 1)
	{		
		m_Chk_OnlyBuyer.ShowWindow(SW_HIDE);
	}
	else
	{
		m_Chk_OnlyBuyer.ShowWindow(SW_SHOW);
	}
}


BOOL CBatchMessageDlg::IsOnline(CString szUserId)
{
	ConvertGBKToUtf8(szUserId);

	CString szURL = "http://amos1.taobao.com/online.ww?v=2&s=1&uid=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);
	return szHTML.GetLength() > 20;
}

//2:Ů, 1:��, 0:δ֪
int CBatchMessageDlg::GetUserGender(CString szUserId)
{
	CString szURL = "http://wwpartner.taobao.com/wangwang/ming_pian.htm?user_id=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);

	if (szHTML.Find("�Ա�:��", 10000) > 0)
		return 1;
	else if (szHTML.Find("�Ա�:Ů", 0) > 0)
		return 2;
	else
		return 0;
}

//�Ƿ�����ָ���ĵط�
BOOL CBatchMessageDlg::IsFromLocation(CString szUserId, CString szLocation)
{
	CString szURL = "http://wwpartner.taobao.com/wangwang/ming_pian.htm?user_id=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);
	CString flag = "����:" + szLocation;

	if (szHTML.Find(flag, 10000) > 0)
		return TRUE;

	return FALSE;
}

//type=1:��������,type=0:�������
int CBatchMessageDlg::GetUserRate(CString szUserId, int type)
{
	CString szURL = "http://wwpartner.taobao.com/wangwang/ming_pian.htm?user_id=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);

	CString szFlag = "getRank(";
	if (type == 1)
	{
		int i1 = szHTML.Find(szFlag, 10000);
		int i2 = szHTML.Find(",\"SELLER\"", i1+1);
		if (i2 > i1 && i2-i1 < 20)
		{
			CString num = szHTML.Mid(i1+szFlag.GetLength(), i2-(i1+szFlag.GetLength()));
			return atoi(num.GetBuffer());
		}
	}
	if (type == 0)
	{
		int i1 = szHTML.Find(szFlag, 10000);
		i1 = szHTML.Find(szFlag, i1+1);

		int i2 = szHTML.Find(",\"BUYER\"", i1+1);
		if (i2 > i1 && i2-i1 < 20)
		{
			CString num = szHTML.Mid(i1+szFlag.GetLength(), i2-(i1+szFlag.GetLength()));
			return atoi(num.GetBuffer());
		}
	}

	return 0;
}

void CBatchMessageDlg::SetMemberStatInfo()
{	
	CString szMemberCount;
    szMemberCount.Format("����(��%d���ѷ���%d��δ���ͣ�%d)", m_MemberList.GetItemCount(), nSendedTotal, m_MemberList.GetItemCount()-nSendedTotal);
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

int CBatchMessageDlg::GetSendedCount()
{
	int n=0;
	for (int i = 0; i < m_MemberList.GetItemCount(); i++)
	{
		if (m_MemberList.GetItemText(i, 1) == STATUS_SENDED)
		{
			n++;
		}
	}
	return n;
}