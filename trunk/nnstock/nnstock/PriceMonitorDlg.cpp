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
END_MESSAGE_MAP()


// CPriceMonitorDlg message handlers

void CPriceMonitorDlg::OnBnClickedOk()
{
    // OnOK();

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

}
