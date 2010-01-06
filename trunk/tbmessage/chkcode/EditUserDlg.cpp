// EditUserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "EditUserDlg.h"
#include ".\edituserdlg.h"
#include "WindowHelp.h"

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
    
	CString szFileName;
	DWORD nProcID = ::GetCurrentProcessId();
	GetProcessNameByProcessID(nProcID, szFileName);

	int index = szFileName.ReverseFind('\\');
	m_chkFilePath = szFileName.Left(index) + "\\chkcode.txt";

    this->SetTimer(ID_TIMER, 1000, 0);

    return TRUE;
}
// CEditUserDlg 消息处理程序

void CEditUserDlg::OnBnClickedOk()
{
    CString szLine;
    this->GetDlgItemText(IDC_EDIT_PWD, szLine);
    szLine = m_szUrl + "-" + szLine;

    CStdioFile file;
    if(file.Open(m_chkFilePath, CFile::modeReadWrite))
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
    CStdioFile file;
    if(file.Open(m_chkFilePath, CFile::modeRead))
    {
        file.ReadString(szLine);

        if (szLine.Find("-") < 0)
        {
            m_szUrl = szLine;
            m_expChkCode.Navigate(m_szUrl, 0,0,0,0);
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
