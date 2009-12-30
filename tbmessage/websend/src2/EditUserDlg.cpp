// EditUserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "EditUserDlg.h"
#include ".\edituserdlg.h"


// CEditUserDlg �Ի���

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


// CEditUserDlg ��Ϣ�������

void CEditUserDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
    if (m_UserId.IsEmpty())
    {
        MessageBox("�û�������Ϊ�ա�", "����");
        return;
    }

    OnOK();
}
