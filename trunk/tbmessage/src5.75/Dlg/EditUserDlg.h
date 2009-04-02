#pragma once


// CEditUserDlg 对话框

class CEditUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditUserDlg();

// 对话框数据
	enum { IDD = IDD_DLG_EDIT_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_UserId;
    afx_msg void OnBnClickedOk();
    CString m_Password;
};
