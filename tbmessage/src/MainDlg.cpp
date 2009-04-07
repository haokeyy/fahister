// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MainDlg.h"


// CMainDlg dialog

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

BOOL CMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_FuncTab.AddPage("查找买家/卖家", &m_SearchPage, IDD_SEARCH_VIEW);
    m_FuncTab.AddPage("买家/卖家信息", &m_MemberPage, IDD_MEMBER_VIEW);
    m_FuncTab.AddPage("发送消息", &m_SendPage, IDD_SEND_VIEW);
    m_FuncTab.Show();
    //TCITEM tcItem1;
    //tcItem1.mask = TCIF_TEXT;
    //tcItem1.pszText = _T("欢迎");
    //m_FuncTab.InsertItem(0, &tcItem1);

    //TCITEM tcItem2;
    //tcItem2.mask = TCIF_TEXT;
    //tcItem2.pszText = _T("查找买家/卖家");
    //m_FuncTab.InsertItem(1, &tcItem2);

    //TCITEM tcItem3;
    //tcItem3.mask = TCIF_TEXT;
    //tcItem3.pszText = _T("买家/卖家信息");
    //m_FuncTab.InsertItem(2, &tcItem3);

    //TCITEM tcItem4;
    //tcItem4.mask = TCIF_TEXT;
    //tcItem4.pszText = _T("发送消息");
    //m_FuncTab.InsertItem(3, &tcItem4);

    //TCITEM tcItem5;
    //tcItem5.mask = TCIF_TEXT;
    //tcItem5.pszText = _T("用户注册");
    //m_FuncTab.InsertItem(4, &tcItem5);

    //this->SetWindowPos(NULL, 0, 0, 300, 210, WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);

    

    return TRUE;
}
