// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MainDlg.h"


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

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    m_FuncTab.AddPage("��ӭ", &m_MemberPage, IDD_MEMBER_VIEW);
    m_FuncTab.AddPage("�������/����", &m_SearchPage, IDD_SEARCH_VIEW);
    m_FuncTab.AddPage("Ⱥ����Ϣ", &m_SendPage, IDD_SEND_VIEW);
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
