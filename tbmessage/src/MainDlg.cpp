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

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FUNC_TAB, m_FuncTab);
}

BOOL CMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    TCITEM tcItem1;
    tcItem1.mask = TCIF_TEXT;
    tcItem1.pszText = _T("��ӭ");
    m_FuncTab.InsertItem(0, &tcItem1);

    TCITEM tcItem2;
    tcItem2.mask = TCIF_TEXT;
    tcItem2.pszText = _T("�������/����");
    m_FuncTab.InsertItem(1, &tcItem2);

    TCITEM tcItem3;
    tcItem3.mask = TCIF_TEXT;
    tcItem3.pszText = _T("���/������Ϣ");
    m_FuncTab.InsertItem(2, &tcItem3);

    TCITEM tcItem4;
    tcItem4.mask = TCIF_TEXT;
    tcItem4.pszText = _T("������Ϣ");
    m_FuncTab.InsertItem(3, &tcItem4);

    TCITEM tcItem5;
    tcItem5.mask = TCIF_TEXT;
    tcItem5.pszText = _T("�û�ע��");
    m_FuncTab.InsertItem(4, &tcItem5);

    this->SetWindowPos(NULL, 0, 0, 300, 210, WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);

    return TRUE;
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_SEARCH, &CMainDlg::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// CMainDlg message handlers

void CMainDlg::OnBnClickedBtnSearch()
{
    // TODO: Add your control notification handler code here
}
