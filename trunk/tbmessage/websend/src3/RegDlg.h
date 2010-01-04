#pragma once
#include "afxwin.h"
#include "HLink.h"

// CRegDlg 对话框

class CRegDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegDlg)

public:
	CRegDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegDlg();

// 对话框数据
	enum { IDD = IDD_DLG_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CHLink m_RegURL;
    virtual BOOL OnInitDialog();
    CEdit m_MachineCode;
    afx_msg void OnBnClickedBtnCopy();
    afx_msg void OnBnClickedBtnSaveuserid();
    CString m_UserID;
};
