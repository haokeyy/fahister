
// tbmsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tbmessage.h"
#include "tbmsgDlg.h"
#include "EditMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtbmsgDlg �Ի���
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


// CtbmsgDlg ��Ϣ�������

BOOL CtbmsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_ExprMsgHelp.Navigate("about:blank", 0, 0, 0, 0);

    // �����б����ʽ
    m_MessageList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// �����
    m_MessageList.InsertColumn(0, "���", LVCFMT_LEFT, 36);
    m_MessageList.InsertColumn(1, "��Ϣ����", LVCFMT_LEFT, 420);
    m_MessageList.InsertColumn(2, "��Ϣ", LVCFMT_LEFT, 0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtbmsgDlg::OnPaint()
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
