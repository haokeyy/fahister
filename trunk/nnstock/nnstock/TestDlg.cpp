// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nnstock.h"
#include "TestDlg.h"
#include "WndHelper.h"
#include "MsgHelper.h"
#include "BatchMsg.h"
#include "Utility.h"


// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestDlg message handlers

void CTestDlg::OnBnClickedButton1()
{ 
	CString szMainWnd;
	this->GetDlgItemText(IDC_EDIT2, szMainWnd);
	HWND hMainWnd = CWndHelper::FindTopWindowBlur(szMainWnd.GetBuffer(), "");

	CString szCommands;
	this->GetDlgItemText(IDC_EDIT3, szCommands);
	char* lpstrCmds = szCommands.GetBuffer();

	NameValuePair parameters[] = 
	{
		NameValuePair("{STC}", "159901"),
		NameValuePair("{PRC}", "4.010"),
		NameValuePair("{QTY}", "1200"),
		NameValuePair("END", "")
	};

	CBatchMsg::ProcessBatchCommand(hMainWnd, lpstrCmds, NULL);
}
