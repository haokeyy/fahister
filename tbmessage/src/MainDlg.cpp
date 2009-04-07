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
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_FuncTab.AddPage("�������/����", &m_SearchPage, IDD_SEARCH_VIEW);
    m_FuncTab.AddPage("���/������Ϣ", &m_MemberPage, IDD_MEMBER_VIEW);
    m_FuncTab.AddPage("������Ϣ", &m_SendPage, IDD_SEND_VIEW);
    m_FuncTab.Show();
    //TCITEM tcItem1;
    //tcItem1.mask = TCIF_TEXT;
    //tcItem1.pszText = _T("��ӭ");
    //m_FuncTab.InsertItem(0, &tcItem1);

    //TCITEM tcItem2;
    //tcItem2.mask = TCIF_TEXT;
    //tcItem2.pszText = _T("�������/����");
    //m_FuncTab.InsertItem(1, &tcItem2);

    //TCITEM tcItem3;
    //tcItem3.mask = TCIF_TEXT;
    //tcItem3.pszText = _T("���/������Ϣ");
    //m_FuncTab.InsertItem(2, &tcItem3);

    //TCITEM tcItem4;
    //tcItem4.mask = TCIF_TEXT;
    //tcItem4.pszText = _T("������Ϣ");
    //m_FuncTab.InsertItem(3, &tcItem4);

    //TCITEM tcItem5;
    //tcItem5.mask = TCIF_TEXT;
    //tcItem5.pszText = _T("�û�ע��");
    //m_FuncTab.InsertItem(4, &tcItem5);

    //this->SetWindowPos(NULL, 0, 0, 300, 210, WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);

    

    return TRUE;
}
