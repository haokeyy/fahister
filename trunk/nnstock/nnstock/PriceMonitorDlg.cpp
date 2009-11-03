// PriceMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nnstock.h"
#include "PriceMonitorDlg.h"
#include "WndHelper.h"
#include "MsgHelper.h"


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

    /*
    HWND hMainWnd = CWndHelper::FindTopWindowBlur("网上股票交易系统5.0", "Afx:400000:b:");

    CMsgHelper::SM_PressKey(hMainWnd, VK_F1);
    CMsgHelper::SM_Click(hMainWnd, "重填", "Button");
    CMsgHelper::SM_Text(hMainWnd, "159902");
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    CMsgHelper::SM_Text(hMainWnd, "2.413");
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    CMsgHelper::SM_Text(hMainWnd, "100");
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    Sleep(500);
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    */

    HWND hMainWnd = CWndHelper::FindTopWindowBlur("按价位监视Test", "#32770");

    CMsgHelper::SM_Text(hMainWnd, "", "Edit", 100, 70, "159901");
    CMsgHelper::SM_Text(hMainWnd, "", "Edit", 100, 170, "200");


}

void CPriceMonitorDlg::OnBnClickedBtnDiscard()
{
    HWND hMainWnd = CWndHelper::FindTopWindowBlur("按价位监视Test", "#32770");

    HWND hChildWnd1 = CWndHelper::FindChildWindowByPoint(hMainWnd, "", "Edit", 100, 70);
    HWND hChildWnd2 = CWndHelper::FindChildWindowByPoint(hMainWnd, "", "Edit", 100, 170);

}
