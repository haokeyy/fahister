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
    HWND hMainWnd = CWndHelper::FindTopWindowBlur("", "WordPadClass");
    HWND hChildWnd = CWndHelper::FindChildWindowExactly(hMainWnd, "±ê×¼ÐÍ", "ToolbarWindow32");
    
    //SendMessage(WM_CLICK, 0, 0);

    //::SendMessage(hChildWnd, WM_LBUTTONDOWN, 0, 0x0009006A);
    //::SendMessage(hChildWnd, WM_LBUTTONUP, 0, 0x0009006A);

    
    ::SendMessage(hChildWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(110,16));
    ::SendMessage(hChildWnd, WM_LBUTTONUP, 0, MAKELPARAM(110,16));

}
