// HelpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "HelpDlg.h"
#include ".\helpdlg.h"


// CHelpDlg �Ի���

IMPLEMENT_DYNCREATE(CHelpDlg, CDHtmlDialog)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD,  pParent) //CHelpDlg::IDH,
{
    m_bIsFirst = TRUE;
}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXPLORER_OCX, m_Browser);
}

BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_Browser.Navigate("http://www.21cn2.com/aliww/help_web.htm", NULL, NULL, NULL, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()

//BEGIN_DHTML_EVENT_MAP(CHelpDlg)
//END_DHTML_EVENT_MAP()

void CHelpDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    CRect rc;
    this->GetClientRect(&rc);

    if (m_bIsFirst)
    {
        m_bIsFirst = FALSE;
    }
    else
    {
        m_Browser.MoveWindow(&rc);
    }
}
