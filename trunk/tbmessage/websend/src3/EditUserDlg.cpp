// EditUserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "EditUserDlg.h"
#include ".\edituserdlg.h"

#define CHKCODE_FILE "c:\\chkcode\\chkcode.txt"
#define ID_TIMER 100
// CEditUserDlg 对话框

IMPLEMENT_DYNAMIC(CEditUserDlg, CDialog)
CEditUserDlg::CEditUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditUserDlg::IDD, pParent)
{
}

CEditUserDlg::~CEditUserDlg()
{
}

void CEditUserDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXPLORER_OCX, m_expChkCode);
}


BEGIN_MESSAGE_MAP(CEditUserDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON1, &CEditUserDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


BOOL CEditUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    this->SetTimer(ID_TIMER, 1000, 0);

    return TRUE;
}
// CEditUserDlg 消息处理程序

void CEditUserDlg::OnBnClickedOk()
{
    CString szLine;
    this->GetDlgItemText(IDC_EDIT_PWD, szLine);

    szLine = szUuid + "-" + szLine;

    CStdioFile file;
    if(file.Open(CHKCODE_FILE, CFile::modeReadWrite))
    {
        file.WriteString(szLine);

        CString szUrl = "about:blank";
        m_expChkCode.Navigate(szUrl, 0,0,0,0);

        this->SetTimer(ID_TIMER, 1000, 0);
        this->GetDlgItem(IDC_EDIT_PWD)->SetFocus();
    }
}

void CEditUserDlg::OnTimer(UINT_PTR nIDEvent)
{
    CString szLine;
    CString szUrl = "http://checkcode.alisoft.com/alisoft/checkcode?sessionID=";
    CStdioFile file;
    if(file.Open(CHKCODE_FILE, CFile::modeRead))
    {
        file.ReadString(szLine);

        if (szLine.Find("-") < 0)
        {
            szUuid = szLine;
            szUrl += szUuid;
            m_expChkCode.Navigate(szUrl, 0,0,0,0);
            this->KillTimer(ID_TIMER);

            this->GetDlgItem(IDC_EDIT_PWD)->SetFocus();
        }

    }

    CDialog::OnTimer(nIDEvent);
}

void CEditUserDlg::OnBnClickedButton1()
{    
    this->SetTimer(ID_TIMER, 10, 0);
}
