// PriceMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nnstock.h"
#include "PriceMonitorDlg.h"


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
END_MESSAGE_MAP()


// CPriceMonitorDlg message handlers
