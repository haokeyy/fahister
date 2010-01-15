// BatchMessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "BatchMessageDlg.h"
#include "dlg\EditUserDlg.h"
#include "dlg\HelpDlg.h"
#include "dlg\shutdowndlg.h"
#include "Utility\WindowHelp.h"
#include "dlg\RegDlg.h"
#include "IMMessage.h"
#include ".\batchmessagedlg.h"
#include "Utility\Constants.h"
#include "Ado\ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchMessageDlg 对话框

#define TIMER_ID 100

#define STATUS_UNSEND "待发送"
#define STATUS_SENDING "发送中..."
#define STATUS_SENDED "已发送"

#define SITE_0 "慧聪"
#define SITE_1 "阿里巴巴"

#define SZ_SPLIT_FLAG "                                 ---"

CBatchMessageDlg::CBatchMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchMessageDlg::IDD, pParent)
    , m_Message(_T(""))
    , m_MemberLimit(100)
    , m_Keyword(_T(""))
    , m_bShutdown(FALSE)
    , m_Message2(_T(""))
    , m_bChkAddFriend(FALSE)
    , m_bChkOnlyOnline(FALSE)
	, m_bChkIsVip(FALSE)
{
    m_nMsgType = 1;
    nSendCount = 0;
    //m_pWebSendDlg = NULL;
    szTaobaoSendUrl = "";

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBatchMessageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
    DDX_Text(pDX, IDC_EDIT_MSG, m_Message);
    DDX_Control(pDX, IDC_BTN_SEARCH, m_BtnSearch);
    DDX_Text(pDX, IDC_EDIT_LIMIT, m_MemberLimit);
    DDX_Control(pDX, IDC_CMB_CATEGORY, m_CmbCategory);
    DDX_Text(pDX, IDC_EDIT_KEYWORD, m_Keyword);
    DDX_Control(pDX, IDC_CMB_SPEED, m_CmbSendSpeed);
    DDX_Control(pDX, IDC_BTN_SENDMSG, m_BtnSendMsg);
    DDX_Check(pDX, IDC_CHK_SHUTDOWN, m_bShutdown);
    DDX_Control(pDX, IDC_BTN_ADDUSER, m_BtnAddUser);
    DDX_Control(pDX, IDC_BTN_DELUSER, m_BtnDelUser);
    DDX_Control(pDX, IDC_EDIT_MSG, m_EditMsg);
    DDX_Control(pDX, IDC_STATIC_LINK, m_CompanyLink);
    DDX_Control(pDX, IDC_CMB_LOCATION, m_CmbLocation);
    DDX_Control(pDX, IDC_TAB1, m_Tab1);
    DDX_Control(pDX, IDC_EDIT_LIMIT, m_Limit_Max);
    DDX_Control(pDX, IDC_EXPLORER1, m_SendExplorer);
    DDX_Check(pDX, IDC_CHK_ADDFRIEND, m_bChkAddFriend);
    DDX_Control(pDX, IDC_CMB_TARGET, m_CmbTarget);
    DDX_Check(pDX, IDC_CHK_ONLYONLINE, m_bChkOnlyOnline);
    DDX_Control(pDX, IDC_CHK_ONLYONLINE, m_btnOnlyOnline);
    DDX_Check(pDX, IDC_CHKVIP, m_bChkIsVip);
    DDX_Control(pDX, IDC_CMB_SALESLEVEL, m_cmbSalesLevel);
    DDX_Control(pDX, IDC_CMB_GENDER, m_cmbSalesGender);
    DDX_Control(pDX, IDC_LIST_MESSAGE, m_MessageList);
    DDX_Control(pDX, IDC_EXP_ADBAR, m_ExpADBar);
}

BEGIN_MESSAGE_MAP(CBatchMessageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedBtnSearch)
    ON_MESSAGE(WM_FOUND_MEMBER, OnFoundMember) 
    ON_MESSAGE(WM_SENDMSGCOMPLETED, OnSendMsgCompleted) 
    ON_MESSAGE(WM_NOTIFY_DELETE_USER, OnNotifyDeleteUser) 
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
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
    //ON_CBN_SELCHANGE(IDC_CMB_TARGET, OnCbnSelchangeCmbTarget)
	ON_BN_CLICKED(IDC_BTN_ADDMSG, &CBatchMessageDlg::OnBnClickedBtnAddmsg)
	ON_BN_CLICKED(IDC_BTN_DELMSG, &CBatchMessageDlg::OnBnClickedBtnDelmsg)
END_MESSAGE_MAP()

// CBatchMessageDlg 消息处理程序

BOOL CBatchMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    CString szAdUrl = MAIN_WEB_SITE;
    szAdUrl += "/link.aspx?type=1&id=";
    szAdUrl += FILE_ID;
    m_ExpADBar.Navigate(szAdUrl, 0, 0, 0, 0);

    CString szSendUrl = MAIN_WEB_SITE;
    szSendUrl += "/c.html";
	m_SendExplorer.Navigate(szSendUrl, 0, 0, 0, 0);

    m_GetMemberThread.Init(this->GetSafeHwnd());
    m_CompanyLink.SetURL(MAIN_WEB_SITE);
   
        // 搜索目标类别 
    int k=0;
    //m_CmbTarget.AddString("淘宝买家");
    //m_CmbTarget.SetItemData(k++, 2);
    //m_CmbTarget.AddString("淘宝卖家");    
    //m_CmbTarget.SetItemData(k++, 1);
    ////m_CmbTarget.AddString("阿里巴巴商家");
    ////m_CmbTarget.SetItemData(k++, 3);
    //m_CmbTarget.SetCurSel(1);

    // 初始化下拉框
    InitBizType();
    InitSalesLevel();
    InitCategory();
    InitLocation();

    this->m_cmbSalesGender.SetCurSel(0);
    this->CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

    k=0;
	
    //m_CmbSendSpeed.AddString("1秒每条");m_CmbSendSpeed.SetItemData(k++, 1300);
    //m_CmbSendSpeed.AddString("2秒每条");m_CmbSendSpeed.SetItemData(k++, 2100);
    m_CmbSendSpeed.AddString("3秒每条");m_CmbSendSpeed.SetItemData(k++, 3000);
    m_CmbSendSpeed.AddString("4秒每条");m_CmbSendSpeed.SetItemData(k++, 4000);
    m_CmbSendSpeed.AddString("5秒每条");m_CmbSendSpeed.SetItemData(k++, 5000);
    m_CmbSendSpeed.AddString("8秒每条");m_CmbSendSpeed.SetItemData(k++, 8000);
    m_CmbSendSpeed.AddString("15秒每条");m_CmbSendSpeed.SetItemData(k++, 15000);
    m_CmbSendSpeed.SetCurSel(2);

    // 设置列表框样式
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// 添加列
    m_MemberList.InsertColumn(0, "QQ号码", LVCFMT_LEFT, 120);
    m_MemberList.InsertColumn(1, "状态", LVCFMT_LEFT, 80);    
    m_MemberList.InsertColumn(2, "脚本", LVCFMT_LEFT, 0);
    
	m_MessageList.InsertColumn(0, "消息", LVCFMT_LEFT, 300);
	m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    m_nMsgType = 1;
    nSendLimit = ValidateReg();

#ifdef TEST_VERSION        
	CString szWndText;
    this->GetWindowText(szWndText);
    this->SetWindowText(szWndText + " (试用版)");
#else
    if (nSendLimit != 1000000)
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
#endif
    TCITEM tcItem1;
    tcItem1.mask = TCIF_TEXT;
    tcItem1.pszText = _T("普通信息");
    m_Tab1.InsertItem(0, &tcItem1);

    TCITEM tcItem2;
    tcItem2.mask = TCIF_TEXT;
    tcItem2.pszText = _T("验证信息");
    m_Tab1.InsertItem(1, &tcItem2);

    // 加载缺省值
	CString strMessage = AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME);
	CString strFlag = "***\n\n\n***";
	for (int s = 0, e = 0; s < strMessage.GetLength() && e < strMessage.GetLength();)
	{
		e = strMessage.Find(strFlag, s);
		if (e > 0 && e > s)
		{
			m_MessageList.InsertItem(100, strMessage.Mid(s, e-s));
		}
		s = e + strFlag.GetLength();
	}
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME, strMessage);

    int speed = AfxGetApp()->GetProfileInt(REG_REGSECTION_NAME, "speed", 0);
    m_CmbSendSpeed.SetCurSel(speed);

    // 加载用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "ppuser.txt";
    ImportUser(szTempPath);

    UpdateData(FALSE);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CBatchMessageDlg::LoadUser()
{
    CAdoConnection m_adoConnection;
	CAdoRecordSet  m_adoRecordSet;

    if (m_adoConnection.ConnectAccess("member.mdb"))
	{
		m_adoRecordSet.SetAdoConnection(&m_adoConnection);
		m_adoRecordSet.SetCursorLocation(adUseClient);
		m_adoRecordSet.Open("select * from members");

		m_adoRecordSet.MoveFirst();

		int i=0;
		while (!m_adoRecordSet.IsEOF())
		{
			CString szUid, szType, szStatus;
			int bRequireValidate;
			m_adoRecordSet.GetCollect("uid", szUid);
			m_adoRecordSet.GetCollect("type", szType);
			m_adoRecordSet.GetCollect("status", szStatus);
			m_adoRecordSet.GetCollect("requireValidate", bRequireValidate);

			m_MemberList.InsertItem(i, szUid);
			m_MemberList.SetItemText(i, 1, szType);
			m_MemberList.SetItemText(i, 2, szStatus);
			m_MemberList.SetItemText(i, 3, (bRequireValidate==1)?"是":"否");

			m_adoRecordSet.MoveNext();
			i++;
		}

		m_adoRecordSet.Close();
		m_adoConnection.Close();
	}
	else
	{
		MessageBox("打开数据库文件失败!");
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

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
	m_BtnSearch.SetWindowText("停止");

    CString szKeywordType("");
    szKeywordType.Format("%d", this->m_CmbTarget.GetItemData(this->m_CmbTarget.GetCurSel()));

	CString szShopType("");
	if (this->m_CmbCategory.GetCurSel() > 0)
    {
		szShopType.Format("%d", this->m_CmbCategory.GetItemData(this->m_CmbCategory.GetCurSel()));
    }

	CString szMainSale("");
	this->GetDlgItem(IDC_EDIT_KEYWORD2)->GetWindowText(szMainSale);
	
    CString szLocation("");
    m_CmbLocation.GetWindowText(szLocation);

	CString szSalesLevel("");
	if (this->m_cmbSalesLevel.GetCurSel() > 0)
    {
		szSalesLevel.Format("%d", this->m_cmbSalesLevel.GetItemData(this->m_cmbSalesLevel.GetCurSel()));
    }
		
	CString szSalesGender("");
	szSalesGender.Format("%d", this->m_cmbSalesGender.GetCurSel());

	CString szSearchUrl = "";
	// 关键字
	CString szKeyword = URLEncode(m_Keyword);
	szKeyword.Replace("%", "_");
	szSearchUrl += szKeyword + "/";
	// 所在地
	szLocation = URLEncode(szLocation);
	szLocation.Replace("%", "_");
	szSearchUrl += szLocation + "/";
	// 主营项目
	szMainSale = URLEncode(szMainSale);
	szMainSale.Replace("%", "_");
	szSearchUrl += szMainSale + "/";


	szSearchUrl += szKeywordType;
	szSearchUrl += "-" + szShopType; // 类别
	szSearchUrl += "-" + szSalesLevel; //信用级别
	szSearchUrl += "-" + szSalesGender; //性别
	szSearchUrl += "-4-";

	int nMemberType = 0;
    int r = this->GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (r != IDC_RADIO1)
    {
		nMemberType = 1;
    }
    m_GetMemberThread.GetMember("", nMemberType, "" , szSearchUrl, false,false, 0, 1000000);
}

LRESULT CBatchMessageDlg::OnFoundMember(WPARAM wParam, LPARAM lParam)
{
    BOOL bIsFinish = (BOOL)wParam;
    if (!bIsFinish)
    {
        m_BtnSearch.SetWindowText("搜  索");
        m_BtnSearch.EnableWindow(TRUE);
        return 0;
    }

    LPTSTR szMember = (LPTSTR)lParam;

	//if (m_nMsgType == 2)
    //{
        AddMemberNoRepeat(szMember);
    //}
    //else
    //{
    //    AddMember(szMember);
    //}

    return 0;
}


LRESULT CBatchMessageDlg::OnNotifyDeleteUser(WPARAM wParam, LPARAM lParam)
{

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

    //if (ValidUser == 0 && nSendCount++ > 20)
    //{
    //    OnFinishSend();
    //    MessageBox("您没有注册，每次最多只能发送20个消息。请点击注册按钮注册成为正版用户。", "警告");
    //    return 0;
    //}  
    CString szTopMember;
    int nSite;
    int i = GetUnSendedMember(szTopMember, nSite);
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
            MessageBox("发送完成。\t\t\t", "发送");
        }
        return 0;
    }

//    CString szSenderID;
//    GetSenderUser(szSenderID);

    //CString tempMessage = m_Message;
    //tempMessage += this->szVersionAD;
    //tempMessage.Replace("[好友名]", szTopMember);

    //m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());
    //CSendMsgThread *pSendMsgThread = new CSendMsgThread();
    //pSendMsgThread->Init(this->GetSafeHwnd());
    //pSendMsgThread->SendMsg(szSenderID, szTopMember, tempMessage, m_nMsgType);
    //if (m_pWebSendDlg)
    //{
    //    CIMMessage msg;
    //    msg.nMsgType = 1;
    //    msg.szMessage= tempMessage;
    //    msg.szSendTo = szTopMember;

    //    SetMemberStatus(szTopMember, 1);
    //    m_MemberList.SetHotItem(i);
    //    m_MemberList.EnsureVisible(i, TRUE);

    //    // 发送间隔毫秒数
    //    int m = m_CmbSendSpeed.GetCurSel();
    //    int smSecond = m_CmbSendSpeed.GetItemData(m); // 毫秒数

    //    ::SendMessage(m_pWebSendDlg->GetSafeHwnd(), WM_SEND_WEB_MESSAGE, (WPARAM)smSecond, (LPARAM)&msg);   
    //}

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
            // 格式：好友名 网站(0:淘宝,1:阿里巴巴) 需要验证(0:否,1:是) 状态(0:未发送,1:已发送)
            CString szUserName(strLine), szStatus("0");
            int i = strLine.Find(" ", 0);
            if (i > 0)
            {
                szUserName = strLine.Left(i);

                szStatus  = strLine.Right(1);
            }

            int cnt = m_MemberList.GetItemCount();   
            // 添加用户
            m_MemberList.InsertItem(cnt, ExtractQQNumber(szUserName));
            m_MemberList.SetItemText(cnt, 1, szStatus=="1"?STATUS_SENDED:STATUS_UNSEND);            
            m_MemberList.SetItemText(cnt, 2, szUserName);
        }
        file.Close();
    }
    
    CString szMemberCount;
    szMemberCount.Format("QQ用户(共%d,已发送%d)", m_MemberList.GetItemCount(), GetUnSendedCount());
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
            CString szStatus = m_MemberList.GetItemText(i, 1)==STATUS_UNSEND ? "0":"1";

            file.WriteString(m_MemberList.GetItemText(i, 2)+" " + szStatus + "\n");
        }
        file.Close();
    }
}

void CBatchMessageDlg::OnBnClickedBtnExport()
{    
    if (nSendLimit != 1000000)
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
    szMemberCount.Format("QQ用户(共%d,已发送%d)",m_MemberList.GetItemCount(), GetUnSendedCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnClear()
{
    m_MemberList.DeleteAllItems();
            
    CString szMemberCount;
    szMemberCount.Format("QQ用户(共%d,已发送%d)", m_MemberList.GetItemCount(), GetUnSendedCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnAdduser()
{
}



void CBatchMessageDlg::OnBnClickedBtnSendmsg()
{
    //if (m_RadioSeller.GetCheck() > 0)
    //{
    //    this->m_nMsgType = 1;
    //}
    //else
    //{
    //    this->m_nMsgType = 2;
    //}

    CString szText;
    m_BtnSendMsg.GetWindowText(szText);
    if (szText == "发送")
    {
        UpdateData(TRUE);

        // 判断是否有可发送的好友
        CString temp;
        int nSite;
        int k = this->GetUnSendedMember(temp, nSite);
        if (k < 0)
        {
            MessageBox("没有待发送的好友。", "错误");
            return;
        }

		if (m_MessageList.GetItemCount() <= 0)
        {            
            MessageBox("消息内容为空。", "错误");
            return;
        }
 
		/*
        // 判断是否有可发送的用户
        if (m_UserList.GetCount() == 0)
        {
            MessageBox("请添加用来发送消息的淘宝用户。", "错误");
            return;
        }

        // 判断用户是否已经登录
       
        for (int i=0; i < m_UserList.GetCount(); i++)
        {
            CString strSenderID;
            DWORD nStatus;
            GetSenderUser(strSenderID, &nStatus);
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
        }
        */

        // 判断发送速度和用户个数是否匹配
        int m = m_CmbSendSpeed.GetCurSel();
        int smSecond = m_CmbSendSpeed.GetItemData(m); // 毫秒数

       /* if (m_pWebSendDlg == NULL)
        {
            m_pWebSendDlg = new CWebSendDlg(this);
            CString szSenderID, szPassword;
            GetSenderUser(szSenderID, szPassword);

            m_pWebSendDlg->szSendFrom = szSenderID;
            m_pWebSendDlg->szPassword = szPassword;

            m_pWebSendDlg->Create(CWebSendDlg::IDD);
            m_pWebSendDlg->ShowWindow(SW_SHOW);
        }*/

        this->SetTimer(TIMER_ID, smSecond, NULL);
        OnTimer(TIMER_ID);
        OnStartSend();
    }
    else
    {
        this->KillTimer(TIMER_ID);
        /*if (m_pWebSendDlg != NULL)
        {
            CString szUserID = m_pWebSendDlg->szSendTo;
            m_pWebSendDlg->DestroyWindow();
            delete m_pWebSendDlg;
            m_pWebSendDlg = NULL;
            this->SetMemberStatus(szUserID, 0);
        }*/

        OnFinishSend();
    }
}

void CBatchMessageDlg::OnBnClickedBtnDeluser()
{
}

void CBatchMessageDlg::OnBnClickedBtnHelp()
{
    CHelpDlg *pDlg = new CHelpDlg();
    pDlg->Create(IDD_DLG_HELP, this);
    pDlg->ShowWindow(SW_SHOW);
}

int CBatchMessageDlg::GetUnSendedCount()
{
    int cnt = m_MemberList.GetItemCount();
	int n = 0;
    for (int i = 0; i < cnt; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 1);
        if (temp == STATUS_SENDED)
        {
            n++;
        }
		else
		{
			break;
		}
    }
    return n;
}

void CBatchMessageDlg::AddMember(CString szMember)
{
    int cnt = m_MemberList.GetItemCount();
   
    // 添加用户
    m_MemberList.InsertItem(cnt, ExtractQQNumber(szMember));
    //m_MemberList.SetItemText(cnt, 1, (this->m_nMsgType>2)?SITE_1:SITE_0);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);
    m_MemberList.SetItemText(cnt, 2, szMember);
    m_GetMemberThread.AddFoundCount();
            
    CString szMemberCount;
    szMemberCount.Format("QQ用户(共%d,已发送%d)", ++cnt, GetUnSendedCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}


void CBatchMessageDlg::AddMemberNoRepeat(CString szMember)
{
    m_GetMemberThread.AddFoundCount();

	CString strQQID = ExtractQQNumber(szMember);
    int cnt = m_MemberList.GetItemCount();
    for(int i=0; i < cnt; i++)
    {
        CString s=m_MemberList.GetItemText(i,0);
        if (s==strQQID)
        {
            return;
        }
    }

    // 添加用户
    m_MemberList.InsertItem(cnt, strQQID);
    //m_MemberList.SetItemText(cnt, 1, (this->m_nMsgType>2)?SITE_1:SITE_0);
    m_MemberList.SetItemText(cnt, 1, STATUS_UNSEND);    
    m_MemberList.SetItemText(cnt, 2, szMember);
    

    CString szMemberCount;
    szMemberCount.Format("QQ用户(共%d,已发送%d)", ++cnt, GetUnSendedCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

//nStatus 0：待发送；1：发送中，3：发送成功
void CBatchMessageDlg::SetMemberStatus(CString szMember, int nStatus)
{
    int n = m_MemberList.GetItemCount();
    int k = -1;
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 2); // 用户名
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
        case 3:
            m_MemberList.SetItemText(k, 1, STATUS_SENDED);
            break;
        default:
            break;
    }
	
	CString szMemberCount;
    szMemberCount.Format("QQ用户(共%d,已发送%d)", n, GetUnSendedCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnTimer(UINT nIDEvent)
{
    if (nSendCount++ > nSendLimit)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        CString strMsg;
        strMsg.Format("您没有注册，每次最多只能发送%d条消息。请点击注册按钮注册成为正版用户。", nSendLimit);
        MessageBox(strMsg, "警告");
        return;
    }
    CString szTopMember;
    int nSite;
    int i = GetUnSendedMember(szTopMember, nSite);
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

    CString szSenderID, szPassword;
    DWORD nStatus;
    GetSenderUser(szSenderID, szPassword, &nStatus);
    if (nStatus == 1)
    {
        return;
    }

	CString tempMessage = m_MessageList.GetItemText(this->GetNextMessage(), 0);

    tempMessage += szVersionAD;
    //tempMessage2 += szVersionAD;
    //tempMessage.Replace("[好友名]", szTopMember);
    //tempMessage2.Replace("[好友名]", szTopMember);

    if (this->OpenSendWindow(szSenderID, szPassword, szTopMember, nSite) == FALSE) return;    

    CSendMsgThread *pSendMsgThread = new CSendMsgThread();
    pSendMsgThread->Init(this->GetSafeHwnd(), this->m_bChkAddFriend);
    pSendMsgThread->SendMsg(szSenderID, szTopMember, tempMessage, "", nSite);

    SetMemberStatus(szTopMember, 1);
    m_MemberList.SetHotItem(i);
    m_MemberList.EnsureVisible(i, TRUE);

    CDialog::OnTimer(nIDEvent);
}

int CBatchMessageDlg::GetUnSendedMember(CString& szMemberName, int& nSite)
{
    nSite = 1;
    int n = m_MemberList.GetItemCount();
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 1);
        if (temp == STATUS_UNSEND)
        {
            szMemberName = m_MemberList.GetItemText(i, 2);
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

int CBatchMessageDlg::GetSenderUser(CString& szUserID, CString& szPassword, DWORD* nStatus)
{
	return 0;
}

UINT CBatchMessageDlg::ValidateReg()
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
    CString szData1 = "http://search1.paipai.com/shop/";
    CString szData2 = "";
    CString szData3 = "";
    CString szData4 = "javascript:var x="; // 发送选项

#else
    szAuthURL.Format("%s/AuthPagePP.aspx?machinecode=%s&userid=%s&fileid=%s&checksum=%s", MAIN_WEB_SITE, szMachineCode, m_RegUserID, FILE_ID, szFileChecksum);

    CString szResult = GetPageDirect(szAuthURL);

	ConvertUtf8ToGBK(szResult);

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
	
#ifdef TEST_VERSION
    m_GetMemberThread.szTaobaoAddress = "http://search1.paipai.com/shop/";
    this->szTaobaoSendUrl = "javascript:var x=";
	if (this->szVersionAD == "")
	{
		this->szVersionAD = "\n本信息由《易发拍拍QQ群发》发布，\n官方网址：";
		this->szVersionAD += MAIN_WEB_SITE;
	}	
	if (nRetValue == 0)
	{
		nRetValue = 100;
	}

#endif

    return nRetValue;
}

void CBatchMessageDlg::OnBnClickedBtnExport2()
{
    if (nSendLimit == 1000000)
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
    szMemberCount.Format("QQ用户(共%d,已发送%d)", m_MemberList.GetItemCount(), GetUnSendedCount());
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
}

void CBatchMessageDlg::OnBnClickedBtnReg()
{
    CRegDlg dlg;
    dlg.DoModal();
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

void CBatchMessageDlg::InitCategory()
{
    m_CmbCategory.ResetContent();
    int i=0;
    while (Industries[i].Value != -1)
    {
        m_CmbCategory.AddString(Industries[i].Name);
        m_CmbCategory.SetItemData(i, Industries[i].Value);
        i++;
    }

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitBizType()
{    
    m_CmbTarget.ResetContent();

    int i=0;
    while (BizTypes[i].Value != -1)
    {
        m_CmbTarget.AddString(BizTypes[i].Name);
        m_CmbTarget.SetItemData(i, BizTypes[i].Value);
        i++;
    }

    m_CmbTarget.SetCurSel(0);
}

void CBatchMessageDlg::InitSalesLevel()
{
    m_cmbSalesLevel.ResetContent();
   
    int i=0;
    while (SalesLevels[i].Value != -1)
    {
        m_cmbSalesLevel.AddString(SalesLevels[i].Name);
        m_cmbSalesLevel.SetItemData(i, SalesLevels[i].Value);
        i++;
    }

    m_cmbSalesLevel.SetCurSel(0);
}


void CBatchMessageDlg::OnDestroy()
{
    // 保存消息内容
    UpdateData(TRUE);

	CString strMessage = "";
	int cnt = m_MessageList.GetItemCount();
	for (int i = 0; i < cnt; i++)
	{
		strMessage += m_MessageList.GetItemText(i, 0);
		strMessage += "***\n\n\n***";
	}
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME, strMessage);

    int speed = m_CmbSendSpeed.GetCurSel();
    AfxGetApp()->WriteProfileInt(REG_REGSECTION_NAME, "speed", speed);

    // 导出最近的用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "ppuser.txt";
    ExportUser(szTempPath);

    CDialog::OnDestroy();
}

void CBatchMessageDlg::OnBnClickedBtnMembername()
{    
    int curSel = m_Tab1.GetCurSel();
    UpdateData(TRUE);
    if (curSel == 0)
    {    
        m_Message = "[好友名]" + m_Message;
    }
    else
    {
        m_Message2 = "[好友名]" + m_Message2;
    }
    UpdateData(FALSE);
}

void CBatchMessageDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{    
    int curSel = m_Tab1.GetCurSel();
    if (curSel == 0)
    {
        m_EditMsg.ShowWindow(SW_SHOW);
        m_EditMsg2.ShowWindow(SW_HIDE);
    }
    else
    {        
        m_EditMsg2.ShowWindow(SW_SHOW);
        m_EditMsg.ShowWindow(SW_HIDE);
    }
    *pResult = 0;
}


HRESULT FindElementByID(IHTMLElementCollection *pColl, CString szID, IHTMLElement** pElem)
{
    BSTR bstrID = szID.AllocSysString();

    VARIANT varName, varIndex;
    varName.vt = VT_BSTR;
    varName.bstrVal = bstrID;
    varIndex.vt = VT_I4;
    varIndex.intVal = 0;

    IDispatch* pDisp; 
    HRESULT hr = pColl->item(varName, varIndex, &pDisp);
    if (pDisp)
    {
        hr = pDisp->QueryInterface(IID_IHTMLElement, (void**)pElem );
    }
    else
    {
        *pElem = NULL;
    }
    
    ::SysFreeString(bstrID);
    return hr;
}

BOOL CBatchMessageDlg::OpenSendWindow(CString szSenderID, CString szPassword, CString szReceiverID, int nMsgType)
{
    //CString url = "tencent://message/?uin=" + szReceiverID + "&fromuserid=no&touserid=no&unionid=72000106&WebSiteName=拍拍网";

    //ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
   
    HRESULT hr = 1;    
    IHTMLElementCollection *pColl = NULL;
    IHTMLDocument2 *pDoc = NULL;
    IHTMLElement *pLinkTalk = NULL;

    CString szLink = "<a id=\"linkTalk\" href=\"" + szTaobaoSendUrl + szReceiverID + "\">click</a>";
   
    pDoc = (IHTMLDocument2 *)m_SendExplorer.get_Document();
    hr = pDoc->get_all(&pColl);

    hr = FindElementByID(pColl, "linkTalk", &pLinkTalk);

    if (hr == S_OK && pLinkTalk)
    {
        BSTR bstr = szLink.AllocSysString();
        pLinkTalk->put_outerHTML(bstr);
        ::SysFreeString(bstr);

        pDoc = (IHTMLDocument2 *)m_SendExplorer.get_Document();
        hr = pDoc->get_all(&pColl);

        hr = FindElementByID(pColl, "linkTalk", &pLinkTalk);

        pLinkTalk->click();
        pLinkTalk->Release();
    }
    
    pColl->Release();
    pDoc->Release();

    return TRUE;
}

void CBatchMessageDlg::OnCbnSelchangeCmbTarget()
{
    //m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    //if (m_nMsgType == 1 || m_nMsgType == 2)
    //{
    //    InitCategory();
    //    InitLocation();
    //    m_btnOnlyOnline.EnableWindow(FALSE);
    //}
    //else
    //{
    //    InitBizType();
    //    InitSalesLevel();
    //    m_btnOnlyOnline.EnableWindow(TRUE);
    //}
}


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

CString ExtractQQNumber(CString szStattupScript)
{
    int s = szStattupScript.Find("('") + 2;
    int e = szStattupScript.Find("','");
    if (s < 0 || e < 0)
    {
        return szStattupScript;
    }
    return szStattupScript.Mid(s, e - s);
}

void CBatchMessageDlg::OnBnClickedBtnAddmsg()
{
	CEditUserDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_MessageList.InsertItem(100, dlg.m_UserId);
	}
}

void CBatchMessageDlg::OnBnClickedBtnDelmsg()
{
	POSITION pos = m_MessageList.GetFirstSelectedItemPosition();
    int i = m_MessageList.GetNextSelectedItem(pos);
    m_MessageList.DeleteItem(i);
}

int CBatchMessageDlg::GetNextMessage()
{	
	int n = m_MessageList.GetItemCount();
    if (n == 0)
    {
        return -1;
    }

	int i = m_MessageList.GetHotItem();
    if (i < 0 || i == n-1)
    {        
		m_MessageList.SetHotItem(0);;
        return 0;
    }
    else
    {
		m_MessageList.SetHotItem(i+1);;
        return i+1;
    }
}