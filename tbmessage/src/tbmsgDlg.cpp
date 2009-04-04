
// tbmsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tbmessage.h"
#include "tbmsgDlg.h"
#include "EditMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtbmsgDlg 对话框
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
}

BEGIN_MESSAGE_MAP(CtbmsgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_ADD_MSG, &CtbmsgDlg::OnBnClickedBtnAddMsg)
    ON_BN_CLICKED(IDC_BTN_EDIT_MSG, &CtbmsgDlg::OnBnClickedBtnEditMsg)
END_MESSAGE_MAP()


// CtbmsgDlg 消息处理程序

BOOL CtbmsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_ExprMsgHelp.Navigate("about:blank", 0, 0, 0, 0);

    // 设置列表框样式
    m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// 添加列
    m_MessageList.InsertColumn(0, "序号", LVCFMT_LEFT, 36);
    m_MessageList.InsertColumn(1, "消息内容", LVCFMT_LEFT, 420);
    m_MessageList.InsertColumn(2, "消息", LVCFMT_LEFT, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtbmsgDlg::OnPaint()
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

        int nIndex = m_MessageList.GetItemCount();
        CString szIndex;
        szIndex.Format("%d", nIndex + 1);
        m_MessageList.InsertItem(nIndex, szIndex);
        m_MessageList.SetItemText(nIndex, 1, szBodyText);
        m_MessageList.SetItemText(nIndex, 2, szBodyHtml);
    }
}

void CtbmsgDlg::OnBnClickedBtnEditMsg()
{    
    CEditMsgDlg dlg;
    dlg.SetMsgHtml("<p>aa</p>");
    if (dlg.DoModal() == IDOK)
    {
        CString szBodyHtml = dlg.GetMsgHtml();
        CString szBodyText = dlg.GetMsgText();

        int nIndex = m_MessageList.GetItemCount();
        CString szIndex;
        szIndex.Format("%d", nIndex + 1);
        m_MessageList.InsertItem(nIndex, szIndex);
        m_MessageList.SetItemText(nIndex, 1, szBodyText);
        m_MessageList.SetItemText(nIndex, 2, szBodyHtml);
    }
}
