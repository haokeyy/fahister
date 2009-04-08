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
#include "EditMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchMessageDlg 对话框

#define TIMER_ID 100

#define STATUS_UNSEND "待发送"
#define STATUS_SENDING "发送中..."
#define STATUS_SENDED "已发送"

#define SITE_0 "淘宝"
#define SITE_1 "阿里巴巴"

#define SZ_SPLIT_FLAG "                                 ---"

CBatchMessageDlg::CBatchMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchMessageDlg::IDD, pParent)
    , m_MemberLimit(100)
    , m_Keyword(_T(""))
    , m_bShutdown(FALSE)
    , m_bChkAddFriend(FALSE)
    , m_bChkOnlyOnline(FALSE)
    , m_bChkAutoLogin(FALSE)
	, m_ChkSendOnline(FALSE)
{
    m_nMsgType = 1;
    nSendCount = 0;
	nSendedTotal = 0;
    szVersionAD = "\n本信息由《及讯淘宝群发》发布，\n官方网址：http://www.jxqunfa.com";
//    m_pWebSendDlg = NULL;
    szTaobaoSendUrl = "";

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBatchMessageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
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
    DDX_Control(pDX, IDC_STATIC_LINK, m_CompanyLink);
    DDX_Control(pDX, IDC_CMB_LOCATION, m_CmbLocation);
    DDX_Control(pDX, IDC_EDIT_LIMIT, m_Limit_Max);
    DDX_Control(pDX, IDC_EXPLORER1, m_SendExplorer);
    DDX_Check(pDX, IDC_CHK_ADDFRIEND, m_bChkAddFriend);
    DDX_Control(pDX, IDC_CMB_TARGET, m_CmbTarget);
    DDX_Control(pDX, IDC_EXPLORER2, m_AdBar);
    DDX_Check(pDX, IDC_CHK_ONLYONLINE, m_bChkOnlyOnline);
    DDX_Control(pDX, IDC_CHK_ONLYONLINE, m_btnOnlyOnline);
    DDX_Check(pDX, IDC_CHK_AUTOLOGIN, m_bChkAutoLogin);
    DDX_Check(pDX, IDC_CHK_SENDONLINE, m_ChkSendOnline);
    DDX_Control(pDX, IDC_CHECK18, m_ChkOnlyBuyer);
    DDX_Control(pDX, IDC_COMBO3, m_cmbMemberGender);
    DDX_Control(pDX, IDC_DHTMLEDIT1, m_DhtmlEditMsg);
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
    ON_CBN_SELCHANGE(IDC_CMB_TARGET, OnCbnSelchangeCmbTarget)
    ON_BN_CLICKED(IDC_IMPORT_SENDER, &CBatchMessageDlg::OnBnClickedImportSender)
    ON_BN_CLICKED(IDC_CLEAR_SENDER, &CBatchMessageDlg::OnBnClickedClearSender)
    ON_BN_CLICKED(IDC_EXPORT_SENDER, &CBatchMessageDlg::OnBnClickedExportSender)
    ON_BN_CLICKED(IDC_BUTTON2, &CBatchMessageDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

// CBatchMessageDlg 消息处理程序

BOOL CBatchMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    CString szAdBarUrl = "http://www.21cn2.com/link.aspx?type=titlebar&id=";
    szAdBarUrl += FILE_ID;
    m_AdBar.Navigate(szAdBarUrl, 0, 0, 0, 0);
    m_GetMemberThread.Init(this->GetSafeHwnd());
    m_CompanyLink.SetURL("http://www.jxqunfa.com");
   
        // 搜索目标类别 
    int k=0;

#ifdef PRODUCT_A
    this->SetWindowText("王牌阿里旺旺群发器(增强版) V5.6");

    m_CmbTarget.AddString("淘宝买家");
    m_CmbTarget.SetItemData(k++, 2);
    m_CmbTarget.AddString("淘宝卖家");    
    m_CmbTarget.SetItemData(k++, 1);
    m_CmbTarget.AddString("阿里巴巴商家");
    m_CmbTarget.SetItemData(k++, 3);
    m_CmbTarget.SetCurSel(1);

    // 初始化下拉框
    InitCategory();
    InitLocation();

    m_btnOnlyOnline.EnableWindow(FALSE);
#endif

#ifdef PRODUCT_B  
    this->SetWindowText("王牌阿里旺旺群发器(淘宝版) V5.75");

    m_CmbTarget.AddString("淘宝买家");
    m_CmbTarget.SetItemData(k++, 2);
    m_CmbTarget.AddString("淘宝卖家");    
    m_CmbTarget.SetItemData(k++, 1);
    m_CmbTarget.AddString("指定店铺买家");    
    m_CmbTarget.SetItemData(k++, 4);
    m_CmbTarget.AddString("指定商品买家");
    m_CmbTarget.SetItemData(k++, 5);
    m_CmbTarget.SetCurSel(1);

    // 初始化下拉框
    InitCategory();
    InitLocation();

    m_btnOnlyOnline.ShowWindow(SW_SHOW);
	m_ChkOnlyBuyer.ShowWindow(SW_HIDE);
#endif

#ifdef PRODUCT_C
    this->SetWindowText("王牌阿里旺旺群发器(阿里版) V5.6");

    m_CmbTarget.AddString("阿里巴巴商家");
    m_CmbTarget.SetItemData(k++, 3);
    m_CmbTarget.SetCurSel(0);

    // 初始化下拉框
    InitBizType();
    InitProvince();
    m_btnOnlyOnline.EnableWindow(TRUE);
#endif


    k=0;
    m_CmbSendSpeed.AddString("1秒每条");m_CmbSendSpeed.SetItemData(k++, 1100);
    m_CmbSendSpeed.AddString("2秒每条");m_CmbSendSpeed.SetItemData(k++, 2400);
    m_CmbSendSpeed.AddString("3秒每条");m_CmbSendSpeed.SetItemData(k++, 3400);
    m_CmbSendSpeed.AddString("4秒每条");m_CmbSendSpeed.SetItemData(k++, 4400);
    m_CmbSendSpeed.AddString("5秒每条");m_CmbSendSpeed.SetItemData(k++, 5800);
    m_CmbSendSpeed.AddString("8秒每条");m_CmbSendSpeed.SetItemData(k++, 8400);
    m_CmbSendSpeed.AddString("15秒每条");m_CmbSendSpeed.SetItemData(k++, 17000);
    //m_CmbSendSpeed.SetCurSel(0);

    // 设置列表框样式
    m_MemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// 添加列
    m_MemberList.InsertColumn(0, "好友名称", LVCFMT_LEFT, 85);
    m_MemberList.InsertColumn(1, "所属网站", LVCFMT_LEFT, 65);
    m_MemberList.InsertColumn(2, "状态", LVCFMT_LEFT, 60);
    m_MemberList.InsertColumn(3, "是否在线", LVCFMT_LEFT, 60);
    

    m_nMsgType = 1;
    nSendLimit = ValidateReg();

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
    
	m_cmbMemberGender.SetCurSel(0);

    // 加载缺省值
    m_DhtmlEditMsg.put_DocumentHTML(AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME));
    int speed = AfxGetApp()->GetProfileInt(REG_REGSECTION_NAME, "speed", 0);
    m_CmbSendSpeed.SetCurSel(speed);

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
    
    //LoadUser();

    // 加载用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "{0110CD3E-069D-4b92-B5FD-B20435A1BC27}.txt";
    ImportUser(szTempPath);

	// 初始已发送数量
	nSendedTotal = GetSendedCount();

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

    // 查找类别
    this->m_nMsgType = this->m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    CString szLimit2;
    this->GetDlgItemText(IDC_EDIT_LIMIT, szLimit2);
    long n2 = atol(szLimit2.GetBuffer());
    if (n2 <= 0 || n2 > 200000)
    {
        MessageBox("请输入合法的查找范围：0 - 200000。", "错误");
        return;
    }

    m_BtnSearch.SetWindowText("停止");

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

    m_GetMemberThread.GetMember(m_nMsgType, m_CmbCategory.GetItemData(m_CmbCategory.GetCurSel()), szLocation , m_Keyword, this->m_bChkOnlyOnline, szRateSum, 0, this->m_MemberLimit);
//
//    CGetMemberThread m_GetMemberThread2;
//    m_GetMemberThread2.GetMember(m_nMsgType, m_CmbCategory.GetItemData(m_CmbCategory.GetCurSel()), szLocation , m_Keyword, this->m_bChkOnlyOnline, szRateSum, 200, 400);
//
//    CGetMemberThread m_GetMemberThread3;
//    m_GetMemberThread3.GetMember(m_nMsgType, m_CmbCategory.GetItemData(m_CmbCategory.GetCurSel()), szLocation , m_Keyword, this->m_bChkOnlyOnline, szRateSum, 400, 600);
//
//    CGetMemberThread m_GetMemberThread4;
//    m_GetMemberThread4.GetMember(m_nMsgType, m_CmbCategory.GetItemData(m_CmbCategory.GetCurSel()), szLocation , m_Keyword, this->m_bChkOnlyOnline, szRateSum, 600, 800);
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

	
	if (m_nMsgType == 2 || m_nMsgType == 1)
	{
		// 在线判断
		if (this->m_bChkOnlyOnline == TRUE)
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
	}

    if (m_nMsgType == 2) //买家
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
		if (m_ChkOnlyBuyer.GetCheck() == BST_CHECKED)
		{
			if (GetUserRate(szMember, 1) > 0)
			{
				return 0;
			}
		}

        AddMemberNoRepeat(szMember);
    }
    else if (m_nMsgType == 1)
    {
        AddMember(szMember);
    }
	else
	{
		this->AddMemberNoRepeat(szMember);
	}

    return 0;
}


LRESULT CBatchMessageDlg::OnNotifyDeleteUser(WPARAM wParam, LPARAM lParam)
{
    BOOL bIsDelete = (BOOL)wParam;
    LPTSTR strSender = (LPTSTR)lParam;
    if (bIsDelete)
    {
        // 设置状态为正在删除
        int n = m_UserList.GetCount();
        for (int i = 0; i < n; i++)
        {
            CString szUser;
            m_UserList.GetText(i, szUser);
            if (szUser == strSender)
            {
                m_UserList.SetItemData(i, 1);
                break;
            }
        }
        // 发送删除命令        
        CSendMsgThread *pSendMsgThread = new CSendMsgThread();
        pSendMsgThread->Init(this->GetSafeHwnd(), this->m_bChkAddFriend);
        pSendMsgThread->DeleteFriend(strSender);
    }
    else
    {
        // 设置状态为等待
        int n = m_UserList.GetCount();
        for (int i = 0; i < n; i++)
        {
            CString szUser;
            m_UserList.GetText(i, szUser);
            if (szUser == strSender)
            {
                m_UserList.SetItemData(i, 0);
                break;
            }
        }
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
            //if (m_pWebSendDlg != NULL)
            //{
            //    m_pWebSendDlg->DestroyWindow();
            //    m_pWebSendDlg = NULL;
            //}
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
            CString szUserName(strLine), szSite("0"), szStatus("0"), szRequireValidate("0");
            int i = strLine.Find(" ", 0);
            if (i > 0)
            {
                szUserName = strLine.Left(i);

                int k = strLine.Find(" ", i+1);
                if (k > 0)
                {
                    szStatus = strLine.Mid(i+1, k-i-1);
                
                    int j = strLine.Find(" ", k+1);
                    if (j > 0)
                    {
                        szSite = strLine.Mid(k+1, j-k-1);
                    }
                    else
                    {
                        szSite  = strLine.Right(1);
                    }
                }
                else
                {
                    szStatus = strLine.Right(1);
                }

                //szRequireValidate = strLine.Right(1);
            }

            int cnt = m_MemberList.GetItemCount();   
            // 添加用户
            m_MemberList.InsertItem(cnt, szUserName);
            m_MemberList.SetItemText(cnt, 1, szSite=="0"?SITE_0:SITE_1);
            m_MemberList.SetItemText(cnt, 2, szStatus=="0"?STATUS_UNSEND:STATUS_SENDED);
            //m_MemberList.SetItemText(cnt, 3, szRequireValidate=="0"?"否":"是");
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
            CString szSite = m_MemberList.GetItemText(i, 1)==SITE_0?"0":"1";
            CString szStatus = m_MemberList.GetItemText(i, 2)==STATUS_UNSEND ? "0":"1";
            CString szRequireValidate = m_MemberList.GetItemText(i, 3)=="是"?"1":"0";

            CString ss = szUserID+" " + szStatus + " " + szSite +" " + szRequireValidate;
            ss.Trim();
            ss +=  + "\n";
            file.WriteString(ss);
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
        CString s = dlg.m_UserId;
        s += SZ_SPLIT_FLAG;
        s += dlg.m_Password;
        m_UserList.AddString(s);
    }

    int n = m_UserList.GetCount();
    switch (n)
    {
    case 1:
        m_CmbSendSpeed.SetCurSel(6);
        break;
    case 2:
        m_CmbSendSpeed.SetCurSel(5);
        break;    
    case 3:
        m_CmbSendSpeed.SetCurSel(4);
        break;
    case 4:
        m_CmbSendSpeed.SetCurSel(3);
        break;
    case 5:
    case 6:
    case 7:
        m_CmbSendSpeed.SetCurSel(2);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        m_CmbSendSpeed.SetCurSel(1);
        break;
    case 15:
        m_CmbSendSpeed.SetCurSel(0);
        break;
    }
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

        if (m_DhtmlEditMsg.get_DocumentHTML().GetLength() == 0)
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
        /*
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
    int k = m_UserList.GetCurSel();
    m_UserList.DeleteString(k);
    
    int n = m_UserList.GetCount();
    switch (n)
    {
    case 1:
        m_CmbSendSpeed.SetCurSel(6);
        break;
    case 2:
        m_CmbSendSpeed.SetCurSel(5);
        break;    
    case 3:
        m_CmbSendSpeed.SetCurSel(4);
        break;
    case 4:
        m_CmbSendSpeed.SetCurSel(3);
        break;
    case 5:
    case 6:
    case 7:
        m_CmbSendSpeed.SetCurSel(2);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        m_CmbSendSpeed.SetCurSel(1);
        break;
    case 15:
        m_CmbSendSpeed.SetCurSel(0);
        break;
    }
}

void CBatchMessageDlg::OnBnClickedBtnHelp()
{
    CHelpDlg *pDlg = new CHelpDlg();
    pDlg->Create(IDD_DLG_HELP, this);
    pDlg->ShowWindow(SW_SHOW);
}


void CBatchMessageDlg::AddMember(CString szMember)
{
    int cnt = m_MemberList.GetItemCount();
   
    // 添加用户
    m_MemberList.InsertItem(cnt, szMember);
    m_MemberList.SetItemText(cnt, 1, (this->m_nMsgType==3)?SITE_1:SITE_0);
    m_MemberList.SetItemText(cnt, 2, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();
    
            
    SetMemberStatInfo();
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
    m_MemberList.SetItemText(cnt, 1, (this->m_nMsgType==3)?SITE_1:SITE_0);
    m_MemberList.SetItemText(cnt, 2, STATUS_UNSEND);
    m_GetMemberThread.AddFoundCount();

    SetMemberStatInfo();
}

//nStatus 0：待发送；1：发送中，3：发送成功
void CBatchMessageDlg::SetMemberStatus(CString szMember, int nStatus)
{
    int n = m_MemberList.GetItemCount();
    int k = -1;
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 0); // 用户名
        CString temp2 = m_MemberList.GetItemText(i, 2);
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
            m_MemberList.SetItemText(k, 2, STATUS_UNSEND);
            break;
        case 1:
            m_MemberList.SetItemText(k, 2, STATUS_SENDING);
            break;
        case 3:
            m_MemberList.SetItemText(k, 2, STATUS_SENDED);
			nSendedTotal++;
			SetMemberStatInfo();
            break;
        default:
            break;
    }
}

CString CBatchMessageDlg::GetNextMessage()
{
    return m_DhtmlEditMsg.get_DocumentHTML();
}

void CBatchMessageDlg::OnTimer(UINT nIDEvent)
{
    if (nSendCount++ > nSendLimit)
    {
        this->KillTimer(TIMER_ID);
        OnFinishSend();
        CString strMsg;
        strMsg.Format("您没有注册，每次最多只能发送%条消息。请点击注册按钮注册成为正版用户。", nSendLimit);
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

	CString tempMessage = GetNextMessage();
	CString tempMessage2 = GetNextMessage();

	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	ft.lpstrText = "[好友名]";

	//long n = m_RichMessage.FindText(FR_DOWN, &ft);
	//if (n >= 0)
	//{
	//	m_RichMessage.SetSel(ft.chrgText);
	//	m_RichMessage.ReplaceSel(szTopMember, 1);
	//	m_RichMessage.SetSel(m_RichMessage.GetTextLength(),-1);
	//	m_RichMessage.ReplaceSel(szVersionAD, 1);

	//	tempMessage = GetRichEditRtfText(&m_RichMessage);

	//	m_RichMessage.Undo();		
	//	m_RichMessage.Undo();
	//}
	//else
	//{		
	//	m_RichMessage.SetSel(ft.chrgText);
	//	m_RichMessage.ReplaceSel(szVersionAD, 1);

	//	tempMessage = GetRichEditRtfText(&m_RichMessage);

	//	m_RichMessage.Undo();	
	//}
	
	//m_RichMessage2.GetWindowText(tempMessage2);
	//tempMessage2.Replace("[好友名]", szTopMember);

	//tempMessage2 = tempMessage2.Mid(0, 29);

    if (this->OpenSendWindow(szSenderID, szPassword, szTopMember, nSite) == FALSE) return;    

    CSendMsgThread *pSendMsgThread = new CSendMsgThread();
    pSendMsgThread->Init(this->GetSafeHwnd(), this->m_bChkAddFriend);
    pSendMsgThread->SendMsg(szSenderID, szTopMember, tempMessage, tempMessage2, nSite);

    SetMemberStatus(szTopMember, 1);
    m_MemberList.SetHotItem(i);
    m_MemberList.EnsureVisible(i, TRUE);

    CDialog::OnTimer(nIDEvent);
}

int CBatchMessageDlg::GetUnSendedMember(CString& szMemberName, int& nSite)
{
    int n = m_MemberList.GetItemCount();
    for (int i = 0; i < n; i++)
    {
        CString temp = m_MemberList.GetItemText(i, 2);
        if (temp == STATUS_UNSEND)
        {
            szMemberName = m_MemberList.GetItemText(i, 0);
			if (m_ChkSendOnline == TRUE)
			{
				CString onlineStatus = m_MemberList.GetItemText(i, 3);
				if (onlineStatus == "否")
				{
					continue;
				} 
				if (IsOnline(szMemberName) == FALSE)
				{
					m_MemberList.SetItemText(i, 3, "否");
					continue;
				}
			}
            CString temp2 = m_MemberList.GetItemText(i, 1);
            nSite = (temp2 == SITE_0) ? 1:3;

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

    m_BtnSendMsg.SetWindowText("停止");
}

void CBatchMessageDlg::OnFinishSend()
{
    m_CmbSendSpeed.EnableWindow(TRUE);
    m_BtnAddUser.EnableWindow(TRUE);
    m_BtnDelUser.EnableWindow(TRUE);

    m_BtnSendMsg.SetWindowText("发送");
}

int CBatchMessageDlg::GetSenderUser(CString& szUserID, CString& szPassword, DWORD* nStatus)
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
        int k = s.Find(SZ_SPLIT_FLAG);
        szUserID = s.Left(k);
        int j = s.GetLength()-szUserID.GetLength()-ss.GetLength();
        szPassword = s.Right(j);

        *nStatus = m_UserList.GetItemData(0);
        return 0;
    }
    else
    {
        m_UserList.SetCurSel(i+1);
        m_UserList.GetText(i+1, s);
        int k = s.Find(SZ_SPLIT_FLAG);
        szUserID = s.Left(k);
        int j = s.GetLength()-szUserID.GetLength()-ss.GetLength();
        szPassword = s.Right(j);

        *nStatus = m_UserList.GetItemData(i+1);
        return i+1;
    }
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
    CString szData1 = "http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    CString szData2 = "http://search.china.alibaba.com/search/company_search.htm?categoryId=0";
    CString szData3 = "呵呵";
    CString szData4 = "aliim:sendmsg?uid=cntaobao&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1"; //wangwang:SendIM?s=1&v=3&mul=1&uid=%s&uid_t=%s&suid=%s&desc=";

#else
    szAuthURL.Format("http://www.21cn2.com/AuthPage6.aspx?machinecode=%s&userid=%s&fileid=%s&checksum=%s", szMachineCode, m_RegUserID, FILE_ID, szFileChecksum);

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
        CString szStatusName = m_MemberList.GetItemText(i, 2);
        if (szStatusName.CompareNoCase(STATUS_UNSEND))
        {
            m_MemberList.DeleteItem(i);
        }
    }
	nSendedTotal=0;

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

void CBatchMessageDlg::InitBizType()
{    
    m_CmbCategory.ResetContent();

    int i=0;
    while (BizTypes[i].Value != -1)
    {
        m_CmbCategory.AddString(BizTypes[i].Name);
        m_CmbCategory.SetItemData(i, BizTypes[i].Value);
        i++;
    }

    m_CmbCategory.SetCurSel(0);
}

void CBatchMessageDlg::InitProvince()
{
    m_CmbLocation.ResetContent();
   
    int i=0;
    while (Provinces[i].Value != -1)
    {
        m_CmbLocation.AddString(Provinces[i].Name);
        //m_CmbLocation.SetItemData(i, Citys[i].Value);
        i++;
    }

    m_CmbLocation.SetCurSel(0);
}


void CBatchMessageDlg::OnDestroy()
{
    // 保存消息内容
    UpdateData(TRUE);

    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_REGKEY_NAME, m_DhtmlEditMsg.get_DocumentHTML());

    CString szLoginUser = "";
    for (int i = 0; i < m_UserList.GetCount(); i++)
    {
        CString szTemp;
        m_UserList.GetText(i, szTemp);
        if ( i != m_UserList.GetCount()-1) szTemp += "\n";
        szLoginUser += szTemp;
    }
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_LOGINUSER, szLoginUser);    

    int speed = m_CmbSendSpeed.GetCurSel();
    AfxGetApp()->WriteProfileInt(REG_REGSECTION_NAME, "speed", speed);

    // 导出最近的用户
    char lpBuf[MAX_PATH];
    ::GetTempPath(MAX_PATH, lpBuf);
    CString szTempPath(lpBuf);
    szTempPath += "{0110CD3E-069D-4b92-B5FD-B20435A1BC27}.txt";
    ExportUser(szTempPath);

    CDialog::OnDestroy();
}

void CBatchMessageDlg::OnBnClickedBtnMembername()
{ 
    UpdateData(FALSE);
}


BOOL CBatchMessageDlg::OpenSendWindow(CString szSenderID, CString szPassword, CString szReceiverID, int nMsgType)
{    
    CString strWndTitle("-阿里旺旺 2008");
        strWndTitle = szSenderID + strWndTitle;
        HWND hwnd = FindTopWindowExactly(strWndTitle.GetBuffer(0), "");
        // 没有找到窗口，表示已掉线
        if (hwnd == NULL)
        {       
            if (m_bChkAutoLogin == TRUE)
            {           
                CSendMsgThread *pSendMsgThread = new CSendMsgThread();
                pSendMsgThread->Init(this->GetSafeHwnd(), FALSE);
                pSendMsgThread->SendMsg(szSenderID, "", szPassword, "", 4);
            }

            return FALSE;
        }
    
    if (nMsgType == 3)
    {
        //CString strWndTitle("阿里旺旺 - ");
        //strWndTitle += szSenderID;
        //HWND hwnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
        //::SendMessage(hwnd, WM_COMMAND, 0x0000C384, 0);
        //CString szURL;
        //szURL.Format("Alitalk:SendIM?%s", szReceiverID.GetBuffer(0));
        //m_SendExplorer.Navigate(szURL, NULL, NULL, NULL, NULL);
    }
    else
    {
        CString szURL;
        szURL.Format(szTaobaoSendUrl, szReceiverID.GetBuffer(0));        
        m_SendExplorer.Navigate(szURL, NULL, NULL, NULL, NULL);
    }

    return TRUE;
}

void CBatchMessageDlg::OnCbnSelchangeCmbTarget()
{
    m_nMsgType = m_CmbTarget.GetItemData(m_CmbTarget.GetCurSel());

    if (m_nMsgType != 3)
    {
        InitCategory();
        InitLocation();

		if (m_nMsgType == 1)
		{
			m_ChkOnlyBuyer.ShowWindow(SW_HIDE);
		}
		else
		{
			m_ChkOnlyBuyer.ShowWindow(SW_SHOW);
		}

		if (m_nMsgType == 4)
		{
			WINDOWPLACEMENT wndpl;
			this->GetDlgItem(IDC_STATIC101)->GetWindowPlacement(&wndpl);
			wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + 85;
			this->GetDlgItem(IDC_STATIC101)->SetWindowPlacement(&wndpl);
			this->GetDlgItem(IDC_STATIC101)->SetWindowText("店铺评价页面：");
			
			this->GetDlgItem(IDC_EDIT_KEYWORD)->GetWindowPlacement(&wndpl);
			wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - 275;
			this->GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowPlacement(&wndpl);
			this->GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowText("");

			this->GetDlgItem(IDC_STATIC102)->ShowWindow(SW_HIDE);
			this->m_CmbCategory.ShowWindow(SW_HIDE);

		} 
		else if (m_nMsgType == 5)
		{
			WINDOWPLACEMENT wndpl;

			this->GetDlgItem(IDC_STATIC101)->GetWindowPlacement(&wndpl);
			wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + 85;
			this->GetDlgItem(IDC_STATIC101)->SetWindowPlacement(&wndpl);
			this->GetDlgItem(IDC_STATIC101)->SetWindowText("商品评价页面：");

			this->GetDlgItem(IDC_EDIT_KEYWORD)->GetWindowPlacement(&wndpl);
			wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - 275;
			this->GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowPlacement(&wndpl);
			this->GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowText("");

			this->GetDlgItem(IDC_STATIC102)->ShowWindow(SW_HIDE);
			this->m_CmbCategory.ShowWindow(SW_HIDE);
		}
		else
		{				
			WINDOWPLACEMENT wndpl;

			this->GetDlgItem(IDC_STATIC101)->GetWindowPlacement(&wndpl);
			wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + 38;
			this->GetDlgItem(IDC_STATIC101)->SetWindowPlacement(&wndpl);
			this->GetDlgItem(IDC_STATIC101)->SetWindowText("类别：");

			this->GetDlgItem(IDC_EDIT_KEYWORD)->GetWindowPlacement(&wndpl);
			wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - 95;
			this->GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowPlacement(&wndpl);		
			this->GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowText("");

			this->GetDlgItem(IDC_STATIC102)->ShowWindow(SW_SHOW);
			this->m_CmbCategory.ShowWindow(SW_SHOW);
		}
    }
    else
    {
        InitBizType();
        InitProvince();
        m_btnOnlyOnline.EnableWindow(TRUE);
    }
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

void CBatchMessageDlg::OnBnClickedImportSender()
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
                int sp = strLine.Find("/");
                CString s = strLine.Left(sp);
                s += SZ_SPLIT_FLAG;
                s += strLine.Mid(sp + 1);
                m_UserList.AddString(s);
            }
            file.Close();
        }
    }            
}

void CBatchMessageDlg::OnBnClickedClearSender()
{
    m_UserList.ResetContent();
}

void CBatchMessageDlg::OnBnClickedExportSender()
{
    CFileDialog fileOpen(FALSE, ".txt", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt|*.txt|*.*|*.*||");
    if (fileOpen.DoModal() == IDOK)
    {
        CString fileName = fileOpen.GetPathName();

        CStdioFile file; CFileException ex;

        if (file.Open(fileName, CFile::modeCreate|CFile::modeReadWrite, &ex))
        {
            for (int i = 0; i < m_UserList.GetCount(); i++)
            {
                CString s;
                m_UserList.GetText(i, s);
                int k = s.Find(SZ_SPLIT_FLAG);
                CString szUserID = s.Left(k);
                CString szPassword = s.Mid(k+strlen(SZ_SPLIT_FLAG));
                
                file.WriteString(szUserID+"/" +szPassword + "\n");
            }
            file.Close();
        }        
    }    
}


static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CString *buf = (CString*)dwCookie;

   buf->Append((char*)pbBuff);
   *pcb = cb;

   return 0;
}

int CBatchMessageDlg::GetSendedCount()
{
	int n=0;
	for (int i = 0; i < m_MemberList.GetItemCount(); i++)
	{
		if (m_MemberList.GetItemText(i, 2) == STATUS_SENDED)
		{
			n++;
		}
	}
	return n;
}

void CBatchMessageDlg::SetMemberStatInfo()
{
	CString szMemberCount;
    szMemberCount.Format("好友(共%d，已发送%d，未发送：%d)", m_MemberList.GetItemCount(), nSendedTotal, m_MemberList.GetItemCount()-nSendedTotal);
    this->SetDlgItemText(IDC_STATIC_MEMBER, szMemberCount);
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
void CBatchMessageDlg::OnBnClickedButton2()
{
    CEditMsgDlg dlg;
    dlg.DoModal();
}
