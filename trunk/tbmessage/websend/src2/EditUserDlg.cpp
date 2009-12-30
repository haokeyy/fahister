// EditUserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "EditUserDlg.h"
#include ".\edituserdlg.h"


// CEditUserDlg 对话框

IMPLEMENT_DYNAMIC(CEditUserDlg, CDialog)
CEditUserDlg::CEditUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditUserDlg::IDD, pParent)
    , m_UserId(_T(""))
    , m_Password(_T(""))
{
}

CEditUserDlg::~CEditUserDlg()
{
}

void CEditUserDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_USERID, m_UserId);
    DDX_Text(pDX, IDC_EDIT_PWD, m_Password);
}


BEGIN_MESSAGE_MAP(CEditUserDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEditUserDlg 消息处理程序

void CEditUserDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
    if (m_UserId.IsEmpty())
    {
        MessageBox("用户名不能为空。", "错误");
        return;
    }

    OnOK();
}
