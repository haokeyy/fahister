// PriceMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nnstock.h"
#include "PriceMonitorDlg.h"
#include "WndHelper.h"
#include "MsgHelper.h"
#include "BatchMsg.h"


// CPriceMonitorDlg dialog

IMPLEMENT_DYNAMIC(CPriceMonitorDlg, CDialog)

CPriceMonitorDlg::CPriceMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPriceMonitorDlg::IDD, pParent)
{

}

CPriceMonitorDlg::~CPriceMonitorDlg()
{
}

void CPriceMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPriceMonitorDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CPriceMonitorDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BTN_DISCARD, &CPriceMonitorDlg::OnBnClickedBtnDiscard)
END_MESSAGE_MAP()


// CPriceMonitorDlg message handlers

void CPriceMonitorDlg::OnBnClickedOk()
{
    // OnOK();

}

void CPriceMonitorDlg::OnBnClickedBtnDiscard()
{

}
