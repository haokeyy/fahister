// EditAccDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "EditAccDlg.h"


// CEditAccountDlg dialog

IMPLEMENT_DYNAMIC(CEditAccountDlg, CDialog)

CEditAccountDlg::CEditAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditAccountDlg::IDD, pParent)
    , m_szUserId(_T(""))
    , m_szPassword(_T(""))
{

}

CEditAccountDlg::~CEditAccountDlg()
{
}

void CEditAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditAccountDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CEditAccountDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditAccountDlg message handlers

CString CEditAccountDlg::GetUserId()
{
    return m_szUserId;
}

CString CEditAccountDlg::GetPassword()
{
    return m_szPassword;
}

void CEditAccountDlg::OnBnClickedOk()
{
    this->GetDlgItemText(IDC_ACC_USER_ID, m_szUserId);    
    this->GetDlgItemText(IDC_ACC_PASSWORD, m_szPassword);

    if (m_szUserId.IsEmpty() || m_szPassword.IsEmpty())
    {
        MessageBox("用户名和密码不能为空。", "错误", MB_ICONERROR);
        return;
    }

    OnOK();
}
