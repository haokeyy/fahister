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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchMessageDlg 对话框

#define TIMER_ID 100

#define STATUS_UNSEND "待发送"
#define STATUS_SENDING "发送中..."
#define STATUS_SENDED "已发送"
#define STATUS_FAIL   "发送失败"
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
    szVersionAD = "\n本信息由《王牌阿里群发王》发布，\n官方网址：http://www.21cn2.com";
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
   

    // 初始化下拉框
    //InitBizType();
    //InitProvince();
    InitCategory();
    InitLocation();

    int k=0;
    m_CmbSendSpeed.AddString("10秒每条");m_CmbSendSpeed.SetItemData(k++, 1400);
    m_CmbSendSpeed.AddString("12秒每条");m_CmbSendSpeed.SetItemData(k++, 11400);
    m_CmbSendSpeed.AddString("13秒每条");m_CmbSendSpeed.SetItemData(k++, 12400);
    m_CmbSendSpeed.AddString("14秒每条");m_CmbSendSpeed.SetItemData(k++, 13400);
    m_CmbSendSpeed.AddString("15秒每条");m_CmbSendSpeed.SetItemData(k++, 14400);
    //m_CmbSendSpeed.AddString("15秒每条");m_CmbSendSpeed.SetItemData(k++, 17000);
    m_CmbSendSpeed.SetCurSel(0);

    // 设置列表框样式
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// 添加列
    m_MemberList.InsertColumn(0, "好友名称", LVCFMT_LEFT, 105);
    m_MemberList.InsertColumn(1, "状态", LVCFMT_LEFT, 60);

    // 搜索目标类别 
    k=0;
    m_CmbTarget.AddString("淘宝买家");
    m_CmbTarget.SetItemData(k++, 2);
    m_CmbTarget.AddString("淘宝卖家");    
    m_CmbTarget.SetItemData(k++, 1);
    //m_CmbTarget.AddString("阿里巴巴商家");
    //m_CmbTarget.SetItemData(k++, 3);
    m_CmbTarget.SetCurSel(1);

    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    if (ValidateReg() == 0)
    {
        ValidUser = 0;
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

    // 加载缺省值
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
    // 加载用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "recentuser.txt";
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

  
    /*if (m_Keyword.IsEmpty())
    {
        MessageBox("请输入搜索关键字。", "错误");
        return;
    }*/
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
        m_BtnSearch.SetWindowText("查找");
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
        MessageBox("您没有注册，每次最多只能发送20个消息。请点击注册按钮注册成为正版用户。", "警告");
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
                MessageBox("发送完成。\t\t\t", "发送");
            }
        }
        return 0;
    }

//    CString szSenderID;
//    GetSenderUser(szSenderID);

    CString tempMessage = m_Message;
    tempMessage += this->szVersionAD;
    tempMessage.Replace("[好友名]", szTopMember);

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

        // 发送间隔毫秒数
        int m = m_CmbSendSpeed.GetCurSel();
        int smSecond = m_CmbSendSpeed.GetItemData(m); // 毫秒数

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
    szMemberCount.Format("好友列表(%d)", m_MemberList.GetItemCount());
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
    POSITION pos = m_MemberList.GetFirstSelectedItemPosition();
    int i = m_MemberList.GetNextSelectedItem(pos);
    m_MemberList.DeleteItem(i);

            CString szMemberCount;
        szMemberCount.Format("好友列表(%d)", m_MemberList.GetItemCount());
        this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnClear()
{
    m_MemberList.DeleteAllItems();
            
    CString szMemberCount;
    szMemberCount.Format("好友列表(%d)", 0);
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
    if (szText == "发送")
    {
        UpdateData(TRUE);

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
        if (m_UserList.GetCount() == 0)
        {
            MessageBox("请添加用来发送消息的淘宝用户。", "错误");
            return;
        }

        // 判断用户是否已经登录
        /*for (int i=0; i < m_UserList.GetCount(); i++)
        {
            CString strSenderID;
            m_UserList.GetText(i, strSenderID);
            CString strWndTitle("阿里旺旺 - ");
            strWndTitle += strSenderID;

            HWND hwnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
            if (!hwnd)
            {
                CString szPrompt;
                szPrompt.Format("用户%s没有登录，请先登录该用户。\n\n如果不使用该用户发送，请从发送用户列表中将其删除。", strSenderID);
                MessageBox(szPrompt, "错误");
                return;
            }
        }*/        

        // 判断发送速度和用户个数是否匹配
        int m = m_CmbSendSpeed.GetCurSel();
        int smSecond = m_CmbSendSpeed.GetItemData(m); // 毫秒数

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
   
    // 添加用户
    m_MemberList.InsertItem(cnt, szMember);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();
            
    CString szMemberCount;
    szMemberCount.Format("好友列表(%d)", ++cnt);
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

    // 添加用户
    m_MemberList.InsertItem(cnt, szMember);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();

    CString szMemberCount;
    szMemberCount.Format("好友列表(%d)", ++cnt);
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

//nStatus 0：待发送；1：发送中，2：发送失败，3：发送成功
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
        MessageBox("您没有注册，每次最多只能发送20个消息。请点击注册按钮注册成为正版用户。", "警告");
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
    tempMessage.Replace("[好友名]", szTopMember);

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
        msg.szMessage= "你好";
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

    CString szMemberCount;
    szMemberCount.Format("好友列表(%d)", m_MemberList.GetItemCount());
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
    m_CmbCategory.AddString("所有类别");
    m_CmbCategory.SetItemData(i++, 0);

    m_CmbCategory.AddString("手机/通讯/卡");
    m_CmbCategory.SetItemData(i++, 13);
    m_CmbCategory.AddString("数码相机/摄像摄影");
    m_CmbCategory.SetItemData(i++, 17);
    m_CmbCategory.AddString("MP3/随身视听/音响/家电");
    m_CmbCategory.SetItemData(i++, 12);
    m_CmbCategory.AddString("电脑、网络及相关设备");
    m_CmbCategory.SetItemData(i++, 11);
    m_CmbCategory.AddString("办公设备及耗材");
    m_CmbCategory.SetItemData(i++, 21);
    m_CmbCategory.AddString("充值中心/IP长途/网络电话");
    m_CmbCategory.SetItemData(i++, 1041);

    m_CmbCategory.AddString("女装/女士精品");
    m_CmbCategory.SetItemData(i++, 14);
    m_CmbCategory.AddString("女士箱包/鞋帽/女士配件");
    m_CmbCategory.SetItemData(i++, 31);
    m_CmbCategory.AddString("珠宝首饰/手表/眼镜");
    m_CmbCategory.SetItemData(i++, 23);
    m_CmbCategory.AddString("彩妆/香水/护肤/美体");
    m_CmbCategory.SetItemData(i++, 15);

    m_CmbCategory.AddString("居家日用/装饰/文具/服务");
    m_CmbCategory.SetItemData(i++, 24);
    m_CmbCategory.AddString("家用电器/保健器械");
    m_CmbCategory.SetItemData(i++, 36);
    m_CmbCategory.AddString("玩具/模型/卡通产品");
    m_CmbCategory.SetItemData(i++, 30);
    m_CmbCategory.AddString("装潢/五金工具/房产");
    m_CmbCategory.SetItemData(i++, 26);
    m_CmbCategory.AddString("童装/婴幼/孕妇用品");
    m_CmbCategory.SetItemData(i++, 1020);
    m_CmbCategory.AddString("食品/保健品");
    m_CmbCategory.SetItemData(i++, 29);
    m_CmbCategory.AddString("旅游/票务/成人/五花八门");
    m_CmbCategory.SetItemData(i++, 27);

    m_CmbCategory.AddString("收藏品/邮币/古董/字画");
    m_CmbCategory.SetItemData(i++, 20);
    m_CmbCategory.AddString("宠物/工艺品/礼品/鲜花");
    m_CmbCategory.SetItemData(i++, 32);

    m_CmbCategory.AddString("书籍/杂志/报纸");
    m_CmbCategory.SetItemData(i++, 34);
    m_CmbCategory.AddString("音乐/影视/明星/娱乐");
    m_CmbCategory.SetItemData(i++, 33);

    m_CmbCategory.AddString("动漫/游戏/周边");
    m_CmbCategory.SetItemData(i++, 16);
    m_CmbCategory.AddString("虚拟物品/游戏装备");
    m_CmbCategory.SetItemData(i++, 35);

    m_CmbCategory.AddString("男装/服饰配件");
    m_CmbCategory.SetItemData(i++, 37);
    m_CmbCategory.AddString("汽车/摩托/自行车");
    m_CmbCategory.SetItemData(i++, 22);
    m_CmbCategory.AddString("ZIPPO/瑞士军刀/男士精品");
    m_CmbCategory.SetItemData(i++, 1040);

    m_CmbCategory.AddString("运动/休闲/户外");
    m_CmbCategory.SetItemData(i++, 18);

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitLocation()
{
    m_CmbLocation.ResetContent();
    m_CmbLocation.AddString("所有地区");
    m_CmbLocation.AddString("北京");
    m_CmbLocation.AddString("上海");
    m_CmbLocation.AddString("杭州");
    m_CmbLocation.AddString("广州");
    m_CmbLocation.AddString("深圳");
    m_CmbLocation.AddString("南京");
    m_CmbLocation.AddString("武汉");
    m_CmbLocation.AddString("天津");
    m_CmbLocation.AddString("香港");
    m_CmbLocation.AddString("成都");
    m_CmbLocation.AddString("哈尔滨");
    m_CmbLocation.AddString("重庆");
    m_CmbLocation.AddString("宁波");
    m_CmbLocation.AddString("无锡");
    m_CmbLocation.AddString("济南");
    m_CmbLocation.AddString("苏州");
    m_CmbLocation.AddString("温州");
    m_CmbLocation.AddString("青岛");
    m_CmbLocation.AddString("沈阳");
    m_CmbLocation.AddString("福州");
    m_CmbLocation.AddString("西安");
    m_CmbLocation.AddString("长沙");
    m_CmbLocation.AddString("合肥");
    m_CmbLocation.AddString("南宁");
    m_CmbLocation.AddString("南昌");
    m_CmbLocation.AddString("郑州");
    m_CmbLocation.AddString("厦门");
    m_CmbLocation.AddString("大连");
    m_CmbLocation.AddString("常州");
    m_CmbLocation.AddString("石家庄");
    m_CmbLocation.AddString("东莞");
    m_CmbLocation.AddString("台湾");
    m_CmbLocation.SetCurSel(0);
}

void CBatchMessageDlg::InitBizType()
{
    m_CmbCategory.ResetContent();
    int i=0;
    m_CmbCategory.AddString("所有经营模式");
    m_CmbCategory.SetItemData(i++, 0);
    m_CmbCategory.AddString("生产型");
    m_CmbCategory.SetItemData(i++, 1);
    m_CmbCategory.AddString("贸易型");
    m_CmbCategory.SetItemData(i++, 2);
    m_CmbCategory.AddString("服务型");
    m_CmbCategory.SetItemData(i++, 4);
    m_CmbCategory.AddString("政府或其他机构");
    m_CmbCategory.SetItemData(i++, 8);

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitProvince()
{
    m_CmbLocation.ResetContent();
    m_CmbLocation.AddString("所有省份");
    m_CmbLocation.AddString("安徽");
    m_CmbLocation.AddString("北京");
    m_CmbLocation.AddString("甘肃");
    m_CmbLocation.AddString("广东");
    m_CmbLocation.AddString("广西");
    m_CmbLocation.AddString("贵州");
    m_CmbLocation.AddString("海南");
    m_CmbLocation.AddString("河北");
    m_CmbLocation.AddString("河南");
    m_CmbLocation.AddString("黑龙江");
    m_CmbLocation.AddString("湖北");
    m_CmbLocation.AddString("湖南");
    m_CmbLocation.AddString("吉林");
    m_CmbLocation.AddString("江苏");
    m_CmbLocation.AddString("江西");
    m_CmbLocation.AddString("辽宁");
    m_CmbLocation.AddString("内蒙古");
    m_CmbLocation.AddString("宁夏");
    m_CmbLocation.AddString("青海");
    m_CmbLocation.AddString("山东");
    m_CmbLocation.AddString("山西");
    m_CmbLocation.AddString("福建");
    m_CmbLocation.AddString("陕西");
    m_CmbLocation.AddString("上海");
    m_CmbLocation.AddString("四川");
    m_CmbLocation.AddString("天津");
    m_CmbLocation.AddString("西藏");
    m_CmbLocation.AddString("新疆");
    m_CmbLocation.AddString("云南");
    m_CmbLocation.AddString("浙江");
    m_CmbLocation.AddString("重庆");
    m_CmbLocation.AddString("HK");
    m_CmbLocation.AddString("MO");
    m_CmbLocation.AddString("TW");

    m_CmbLocation.SetCurSel(0);
}


void CBatchMessageDlg::OnDestroy()
{
    // 保存消息内容
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
    
    // 导出最近的用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "recentuser.txt";
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
