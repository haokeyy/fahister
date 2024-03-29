// ShutdownDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\BatchMessage.h"
#include "ShutdownDlg.h"
#include ".\shutdowndlg.h"


// CShutdownDlg 对话框

IMPLEMENT_DYNAMIC(CShutdownDlg, CDialog)
CShutdownDlg::CShutdownDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShutdownDlg::IDD, pParent)
{
    nTimerElapse = 15;
}

CShutdownDlg::~CShutdownDlg()
{
}

void CShutdownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CShutdownDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    this->SetTimer(101, 1000, NULL);

    return TRUE;
}

BEGIN_MESSAGE_MAP(CShutdownDlg, CDialog)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_SHUTDOWN, OnBnClickedBtnShutdown)
END_MESSAGE_MAP()


// CShutdownDlg 消息处理程序

void CShutdownDlg::OnTimer(UINT nIDEvent)
{ 

    CString szPrompt;
    szPrompt.Format("您的计算机将在%d秒内关闭。", nTimerElapse--);
    this->SetDlgItemText(IDC_PROMPT, szPrompt);

    if (nTimerElapse < 0)
    {
        MySystemShutdown();
        this->KillTimer(100);
    }

    CDialog::OnTimer(nIDEvent);
}

void CShutdownDlg::OnBnClickedBtnShutdown()
{
    MySystemShutdown();
}

BOOL MySystemShutdown()
{
   HANDLE hToken; 
   TOKEN_PRIVILEGES tkp; 
 
   // Get a token for this process. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return( FALSE ); 
 
   // Get the LUID for the shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
        &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get the shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 
 
   // Shut down the system and force all applications to close. 
 
   if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) 
      return FALSE; 

   return TRUE;
}

