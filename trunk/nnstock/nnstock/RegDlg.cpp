// RegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nnstock.h"
#include "RegDlg.h"
#include "WndHelper.h"
#include "MsgHelper.h"


// CRegDlg dialog

IMPLEMENT_DYNAMIC(CRegDlg, CDialog)

CRegDlg::CRegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegDlg::IDD, pParent)
{

}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_REG, &CRegDlg::OnBnClickedBtnReg)
END_MESSAGE_MAP()


// CRegDlg message handlers

void CRegDlg::OnBnClickedBtnReg()
{

}
