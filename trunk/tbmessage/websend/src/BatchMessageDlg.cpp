// BatchMessageDlg.cpp : 实现文件
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


// CBatchMessageDlg 对话框

#define TIMER_ID 100

#define STATUS_UNSEND "待发送"
#define STATUS_SENDING "发送中..."
#define STATUS_SENDED "已发送"
#define STATUS_FAIL   "发送失败"



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
    szVersionAD = "\n本信息由《王牌阿里群发王》发布，\n官方网址：http://www.21cn2.com";

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


// CBatchMessageDlg 消息处理程序

BOOL CBatchMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    m_GetMemberThread.Init(this->GetSafeHwnd());
    m_CompanyLink.SetURL("http://www.21cn2.com");
    m_Browser.Navigate("http://www.21cn2.com/aliww/banner.htm", 0,0,0,0);
   

	m_cmbMemberGender.SetCurSel(0);
	m_Chk_OnlyBuyer.ShowWindow(SW_HIDE);

    // 初始化下拉框
    InitCategory();
    InitLocation();

    int k=0;
    m_CmbSendSpeed.AddString("1秒每条");m_CmbSendSpeed.SetItemData(k++, 1000);
    m_CmbSendSpeed.AddString("2秒每条");m_CmbSendSpeed.SetItemData(k++, 2000);
    m_CmbSendSpeed.AddString("3秒每条");m_CmbSendSpeed.SetItemData(k++, 3000);
    m_CmbSendSpeed.AddString("5秒每条");m_CmbSendSpeed.SetItemData(k++, 5000);
    m_CmbSendSpeed.AddString("10秒每条");m_CmbSendSpeed.SetItemData(k++, 10000);
    m_CmbSendSpeed.AddString("15秒每条");m_CmbSendSpeed.SetItemData(k++, 15000);

    // 设置列表框样式
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// 添加列
    m_MemberList.InsertColumn(0, "好友名称", LVCFMT_LEFT, 105);
    m_MemberList.InsertColumn(1, "状态", LVCFMT_LEFT, 60);
    m_MemberList.InsertColumn(2, "是否在线", LVCFMT_LEFT, 60);

    m_UserList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_UserList.InsertColumn(0, "用户名", LVCFMT_LEFT, 85);
    m_UserList.InsertColumn(1, "密码", LVCFMT_LEFT, 0);    
    m_UserList.InsertColumn(2, "发送个数", LVCFMT_LEFT, 65);
    // 搜索目标类别 
    k=0;
    m_CmbTarget.AddString("淘宝买家");
    m_CmbTarget.SetItemData(k++, 2);
    m_CmbTarget.AddString("淘宝卖家");    
    m_CmbTarget.SetItemData(k++, 1);
    m_CmbTarget.SetCurSel(1);

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    ValidUser = ValidateReg();
    if (ValidUser != 1000000)
    {
        CString szWndText;
        this->GetWindowText(szWndText);
        this->SetWindowText(szWndText + " (未注册)");
    }
    else
    {        
        CString szWndText;
        this->GetWindowText(szWndText);
        this->SetWindowText(szWndText + " (已注册)");
    }

	nSendedTotal = 0;
    // 加载缺省值
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
    // 加载用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "websend.txt";
    ImportUser(szTempPath);

    UpdateData(FALSE);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBatchMessageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CBatchMessageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBatchMessageDlg::OnBnClickedBtnSearch()
{ 
    UpdateData(TRUE);

    CString tempText;
    m_BtnSearch.GetWindowText(tempText);
    if (!tempText.CompareNoCase("停止"))
    {
        m_GetMemberThread.Stop();
        m_BtnSearch.EnableWindow(FALSE);
        return;
    }

    // 查找个数
    CString szLimit;
    this->GetDlgItemText(IDC_EDIT_LIMIT, szLimit);
    long n = atol(szLimit.GetBuffer());
    if (n <= 0)
    {
        MessageBox("请输入合法的查找个数。", "错误");
        return;
    }

    m_BtnSearch.SetWindowText("停止");

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());


    CString szLocation;
    m_CmbLocation.GetWindowText(szLocation);
	if ("所有地区" == szLocation)
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
        m_BtnSearch.SetWindowText("查找");
        m_BtnSearch.EnableWindow(TRUE);
        return 0;
    }

    LPTSTR szMember = (LPTSTR)lParam;

			
	// 在线判断
	if (this->m_ChkOnlyOnline.GetCheck() == BST_CHECKED)
	{
		if (!IsOnline(szMember))
		{
			return 0;
		}
	}
	// 性别判断
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
		// 买家地区		
		CString szLocation;
		m_CmbLocation.GetWindowText(szLocation);
		if (!(szLocation == "所有地区" || szLocation.IsEmpty()))
		{
			if (!IsFromLocation(szMember, szLocation))
			{
				return 0;
			}
		}

		// 买家的"卖家信誉"
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
    int bResult = (int)lParam;// -1:登录失败，0:失败，1:成功

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
   
            // 添加用户
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
        MessageBox("您没有注册，不能使用此功能。请点击注册按钮注册成为正版用户。", "警告");
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
    if (szText == "发送")
    {
        UpdateData(TRUE);

        CString szLimit;
        this->GetDlgItemText(IDC_SEND_LIMIT, szLimit);
        long n = atol(szLimit.GetBuffer());
        if (n <= 0)
        {
            MessageBox("请输入合法的发送数量。", "错误");
            return;
        }
        // 判断是否有可发送的好友
        CString temp;
        int k = this->GetUnSendedMember(temp);
        if (k < 0)
        {
            MessageBox("没有待发送的好友。", "错误");
            return;
        }

        if (m_Message.IsEmpty())
        {            
            MessageBox("消息内容为空。", "错误");
            return;
        }

        // 判断是否有可发送的用户
        if (m_UserList.GetItemCount() == 0)
        {
            MessageBox("请添加用来发送消息的淘宝用户。", "错误");
            return;
        }

        // 判断发送速度和用户个数是否匹配
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
   
    // 添加用户
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

    // 添加用户
    m_MemberList.InsertItem(cnt, szMember);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();

	SetMemberStatInfo();;
}

//nStatus 0：待发送；1：发送中，2：发送失败，3：发送成功
void CBatchMessageDlg::SetMemberStatus(CString szMember, int nStatus)
{
    int n = m_MemberList.GetItemCount();
    int k = -1;
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 0); // 用户名
        CString temp2 = m_MemberList.GetItemText(i, 1);
        if (temp == szMember)
        {
            if (nStatus == 1) // 如果要设置为发送中，必须以前的状态为待发送
            {
                if (temp2 == STATUS_UNSEND)
                {
                    k = i;
                    break;
                }
            }
            else if (nStatus == 3) // 如果要设置为已发送，必须以前的状态为发送中
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
        MessageBox("您没有注册，发送数量已达到限制。请点击注册按钮注册成为正版用户。", "警告");
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
            MessageBox("发送完成!", "提示");
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

        MessageBox("所有帐号都已达到发送限制，请更换帐号登录。\n\n(发送数量为\"-1\"的帐号表示登录失败)!", "提示");
        return;
    }

    CString tempMessage = m_Message;
    tempMessage += szVersionAD;
    tempMessage.Replace("[好友名]", szTopMember);
    
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
        MessageBox("没有找到文件" + szPath, "错误");

        return;
    }

    UpdateData(TRUE);
    // 开始发送
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
        // 结束发送

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
				if (onlineStatus == "否")
				{
					continue;
				} 
				if (IsOnline(szMemberName) == FALSE)
				{
					m_MemberList.SetItemText(i, 2, "否");
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

    m_BtnSendMsg.SetWindowText("停止");
}

void CBatchMessageDlg::OnFinishSend()
{
    m_CmbSendSpeed.EnableWindow(TRUE);
    m_BtnAddUser.EnableWindow(TRUE);
    m_BtnDelUser.EnableWindow(TRUE);
    m_EditMsg.EnableWindow(TRUE);

    m_BtnSendMsg.SetWindowText("发送");
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
    // 验证用户是否注册
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
    CString szData4 = "发送淘宝消息-";

#else
    szAuthURL.Format("http://www.21cn2.com/AuthPageWeb.aspx?machinecode=%s&userid=%s&fileid=%s&checksum=%s", szMachineCode, m_RegUserID, FILE_ID, szFileChecksum);

    CString szResult = GetPageDirect(szAuthURL);

    int p1, p2;
    CString szFlag;

    szFlag = "<Span id=\"result\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szRet = szResult.Mid(p1,p2-p1);
        
    // 淘宝地址
    szFlag = "<Span id=\"data1\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData1 = szResult.Mid(p1,p2-p1);
    
    // 阿里巴巴地址
    szFlag = "<Span id=\"data2\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData2 = szResult.Mid(p1,p2-p1);

    // 没注册用户的信息尾巴
    szFlag = "<Span id=\"data3\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData3 = szResult.Mid(p1,p2-p1);

    // 阿里旺旺发送用的URL地址
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
        MessageBox("您没有注册，不能使用此功能。请点击注册按钮注册成为正版用户。", "警告");
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
            MessageBox(szError, "提示");
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
    // 保存消息内容
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
    
    // 导出最近的用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "websend.txt";
    ExportUser(szTempPath);

    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
}

void CBatchMessageDlg::OnBnClickedBtnMembername()
{
    UpdateData(TRUE);
    m_Message = "[好友名]" + m_Message;
    UpdateData(FALSE);
}


void CBatchMessageDlg::OnBnClickedImportSender()
{
    if (ValidUser != 1000000)
    {
        MessageBox("您没有注册，不能使用此功能。请点击注册按钮注册成为正版用户。", "警告");
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
            MessageBox(szError, "提示");
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

//2:女, 1:男, 0:未知
int CBatchMessageDlg::GetUserGender(CString szUserId)
{
	CString szURL = "http://wwpartner.taobao.com/wangwang/ming_pian.htm?user_id=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);

	if (szHTML.Find("性别:男", 10000) > 0)
		return 1;
	else if (szHTML.Find("性别:女", 0) > 0)
		return 2;
	else
		return 0;
}

//是否来自指定的地方
BOOL CBatchMessageDlg::IsFromLocation(CString szUserId, CString szLocation)
{
	CString szURL = "http://wwpartner.taobao.com/wangwang/ming_pian.htm?user_id=" + URLEncode(szUserId);
	CString szHTML = GetPageDirect(szURL);
	CString flag = "来自:" + szLocation;

	if (szHTML.Find(flag, 10000) > 0)
		return TRUE;

	return FALSE;
}

//type=1:卖家信誉,type=0:买家信誉
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
    szMemberCount.Format("好友(共%d，已发送%d，未发送：%d)", m_MemberList.GetItemCount(), nSendedTotal, m_MemberList.GetItemCount()-nSendedTotal);
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