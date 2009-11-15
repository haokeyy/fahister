// WebSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WebSendDlg.h"


// CWebSendDlg 对话框

IMPLEMENT_DYNAMIC(CWebSendDlg, CDialog)

CWebSendDlg::CWebSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebSendDlg::IDD, pParent)
{

}

CWebSendDlg::~CWebSendDlg()
{
}

void CWebSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_Message);
	DDX_Control(pDX, IDC_RICHEDIT22, m_Message2);
}


BEGIN_MESSAGE_MAP(CWebSendDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_COPY, &CWebSendDlg::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_SETFONT, &CWebSendDlg::OnBnClickedBtnSetfont)
	ON_BN_CLICKED(IDC_BUTTON3, &CWebSendDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CWebSendDlg 消息处理程序

void CWebSendDlg::OnBnClickedBtnCopy()
{
	m_Message.SetSel(0, -1);
	m_Message.Copy();
}

void CWebSendDlg::OnBnClickedBtnSetfont()
{
	CHARFORMAT cf;
	DWORD d = m_Message.GetSelectionCharFormat(cf);

	CFontDialog dlg(cf);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCharFormat(cf);

		m_Message.SetSelectionCharFormat(cf);
	}
}

static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CString *buf = (CString*)dwCookie;

   buf->Append((char*)pbBuff);
   *pcb = cb;

   return 0;
}

void CWebSendDlg::OnBnClickedButton3()
{
	CString buf;

    EDITSTREAM es;
    es.dwCookie = (DWORD) &buf;
    es.pfnCallback = MyStreamOutCallback; 
    m_Message.StreamOut(SF_RTF, es);

	m_Message2.SetWindowText(buf);
	//char buf[256];
	//buf[0] = 255;
	//
	//m_Message.SetTextMode(TM_RICHTEXT);
	//m_Message.GetLine(0,buf);
	//MessageBox(buf);
}
