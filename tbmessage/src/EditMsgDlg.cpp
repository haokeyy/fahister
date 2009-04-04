// EditMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "EditMsgDlg.h"


// CEditMsgDlg dialog

IMPLEMENT_DYNAMIC(CEditMsgDlg, CDialog)

CEditMsgDlg::CEditMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMsgDlg::IDD, pParent)
{

}

CEditMsgDlg::~CEditMsgDlg()
{
}

void CEditMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditMsgDlg, CDialog)
END_MESSAGE_MAP()


// CEditMsgDlg message handlers
