// About.cpp : implementation file
//

#include "stdafx.h"
#include "nnstock.h"
#include "AboutDlg.h"
#include "WndHelper.h"


// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAboutDlg message handlers
void CAboutDlg::OnBnClickedOk()
{
    HWND hMainWnd = CWndHelper::FindTopWindowBlur("网上股票交易系统5.0", "Afx:400000:b:10011:6:120445");

    if (hMainWnd != NULL)
    {
        HWND hwnd1 = CWndHelper::FindChildWindowExactly(hMainWnd, "", "");

    }
}
