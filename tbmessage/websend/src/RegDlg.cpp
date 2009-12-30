// RegDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "RegDlg.h"
#include ".\regdlg.h"
#include "WindowHelp.h"


// CRegDlg 对话框

IMPLEMENT_DYNAMIC(CRegDlg, CDialog)
CRegDlg::CRegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegDlg::IDD, pParent)
    , m_UserID(_T(""))
{
}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC1, m_RegURL);
    DDX_Control(pDX, IDC_EDIT_MACHINECODE, m_MachineCode);
    DDX_Text(pDX, IDC_EDIT_USERID, m_UserID);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_COPY, OnBnClickedBtnCopy)
    ON_BN_CLICKED(IDC_BTN_SAVEUSERID, OnBnClickedBtnSaveuserid)
END_MESSAGE_MAP()


// CRegDlg 消息处理程序

BOOL CRegDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_RegURL.SetURL("http://www.21cn2.com");

    CString szMachineCode;
    GetMachineCode(szMachineCode);
    m_MachineCode.SetWindowText(szMachineCode);

    m_UserID = AfxGetApp()->GetProfileString(REG_REGSECTION_NAME, REG_REGUSERID);
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CRegDlg::OnBnClickedBtnCopy()
{
    m_MachineCode.Copy();
}

void CRegDlg::OnBnClickedBtnSaveuserid()
{
    UpdateData(TRUE);
    AfxGetApp()->WriteProfileString(REG_REGSECTION_NAME, REG_REGUSERID, m_UserID);

    CString szPath;
    GetProcessNameByProcessID(::GetCurrentProcessId(), szPath);

    ::PostQuitMessage(0);
    
    ::WinExec(szPath, SW_SHOW);
}
