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
    HWND hMainWnd = CWndHelper::FindTopWindowBlur("网上股票交易系统5.0", "Afx:400000:b:10011");
    CMsgHelper::SM_PressKey(hMainWnd, VK_F1);
    //CMsgHelper::SM_Text(hMainWnd, "159901");    
    //CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    //CMsgHelper::SM_Text(hMainWnd, "3.801");   
    //CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);
    //CMsgHelper::SM_Text(hMainWnd, "1200");  
    //CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN); 

    CMsgHelper::SM_Text(hMainWnd, 260, 120, "159901"); 
    CMsgHelper::SM_PressKey(hMainWnd, VK_RETURN);

}
