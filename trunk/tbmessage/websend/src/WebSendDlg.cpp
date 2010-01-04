// WebSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "WebSendDlg.h"


// CWebSendDlg dialog

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
}


BEGIN_MESSAGE_MAP(CWebSendDlg, CDialog)
END_MESSAGE_MAP()


// CWebSendDlg message handlers
