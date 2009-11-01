
// NnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "nnstock.h"
#include "NnDlg.h"
#include "AboutDlg.h"
#include "RegDlg.h"
#include "PriceMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CNnDlg::CNnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNnDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNnDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MONITOR, m_ListMonitor);
}

BEGIN_MESSAGE_MAP(CNnDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_COMMAND(IDM_ABOUTBOX, &CNnDlg::OnAboutbox)
    ON_COMMAND(ID_HELP_REG, &CNnDlg::OnHelpReg)
    ON_BN_CLICKED(IDC_BTN_ADD_PRICE_MONITOR, &CNnDlg::OnBnClickedBtnAddPriceMonitor)
END_MESSAGE_MAP()


// CNnDlg 消息处理程序

BOOL CNnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化列表控件
    m_ListMonitor.InsertColumn(0, "序号", 0, 40);
    m_ListMonitor.InsertColumn(1, "证券代码", 0, 80);
    m_ListMonitor.InsertColumn(2, "证券名称", 0, 80);
    m_ListMonitor.InsertColumn(3, "设定价格", 0, 80);
    m_ListMonitor.InsertColumn(4, "操作", 0, 180);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNnDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNnDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNnDlg::OnAboutbox()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CNnDlg::OnHelpReg()
{
    CRegDlg dlg;
    dlg.DoModal();
}

void CNnDlg::OnBnClickedBtnAddPriceMonitor()
{
    CPriceMonitorDlg dlg;
    if (dlg.DoModal() == IDOK)
    {

    }
}
