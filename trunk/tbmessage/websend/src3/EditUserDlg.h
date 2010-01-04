#pragma once
#include "explorer_ocx.h"


// CEditUserDlg 对话框

class CEditUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditUserDlg();

	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DLG_EDIT_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    CString szUuid;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CExplorerOcx m_expChkCode;
    afx_msg void OnBnClickedButton1();
};
