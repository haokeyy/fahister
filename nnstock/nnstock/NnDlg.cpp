
// NnDlg.cpp : ʵ���ļ�
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


// CNnDlg ��Ϣ�������

BOOL CNnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ʼ���б�ؼ�
    m_ListMonitor.InsertColumn(0, "���", 0, 40);
    m_ListMonitor.InsertColumn(1, "֤ȯ����", 0, 80);
    m_ListMonitor.InsertColumn(2, "֤ȯ����", 0, 80);
    m_ListMonitor.InsertColumn(3, "�趨�۸�", 0, 80);
    m_ListMonitor.InsertColumn(4, "����", 0, 180);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNnDlg::OnPaint()
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
