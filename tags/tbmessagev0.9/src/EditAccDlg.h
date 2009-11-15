#pragma once


// CEditAccountDlg dialog

class CEditAccountDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditAccountDlg)

public:
	CEditAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditAccountDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_ACC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CString m_szUserId;
    CString m_szPassword;
public:
    CString GetUserId();
    CString GetPassword();
    afx_msg void OnBnClickedOk();
};
