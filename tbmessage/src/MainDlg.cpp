// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MainDlg.h"
#include ".\Libraries\WindowHelp.h"


// CMainDlg dialog
BOOL bHasMemberChanged;

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FUNC_TAB, m_FuncTab);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TCN_SELCHANGE, IDC_FUNC_TAB, &CMainDlg::OnTcnSelchangeFuncTab)
END_MESSAGE_MAP()

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 验证用户是否注册
void CMainDlg::ValidateReg()
{

#ifdef DEBUG
    CString szRet   = "1000000";
    CString szData1 = "http://shopsearch.taobao.com/browse/shop_search.htm?title=title&nick=nick";
    CString szData2 = "aliim:sendmsg?uid=cntaobao%s&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1";
    CString szData3 = "呵呵";
#else    
    CString szMachineCode, szFileChecksum;
    GetMachineCode(szMachineCode);
    szFileChecksum = GetFileMD5Checksum();

    CString szAuthURL;
    szAuthURL.Format("%s?machinecode=%s&checksum=%s", URL_VALIDATE, szMachineCode, szFileChecksum);

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
    
    // 阿里旺旺发送用的URL地址
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

    int nRetValue = 0;
    try
    {
        nRetValue = atoi(szRet.Trim().GetBuffer(0));
    }
    catch(CSimpleException* )
    {

    }
    m_MemberPage.m_bHasReged = (nRetValue == 1000000);
    CTime t = CTime::GetTickCount();
    CTime t_limit = CTime(2010, 3, 1, 0, 0, 0);
    //if (t < t_limit)
    //{
        m_SearchPage.m_szSearchUrl = szData1.Trim();
        m_SendPage.m_nSendLimit = nRetValue;
        m_SendPage.m_szSendUrl = szData2.Trim();
        m_SendPage.m_szAdText = szData3.Trim();
    //}
}

BOOL CMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    ValidateReg();

    m_FuncTab.AddPage("软件动态", &m_MemberPage, IDD_MEMBER_VIEW);
    m_FuncTab.AddPage("查找用户", &m_SearchPage, IDD_SEARCH_VIEW);
    m_FuncTab.AddPage("群发消息", &m_SendPage, IDD_SEND_VIEW);
    m_FuncTab.Show();

    return TRUE;
}

void CMainDlg::OnTcnSelchangeFuncTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    int curSel = m_FuncTab.GetCurSel();
    if (curSel == 2 && bHasMemberChanged)
    {
        m_SendPage.OnBnClickedBtnFirstPage();
    }
    *pResult = 0;
}
