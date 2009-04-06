#pragma once


// CMainDlg dialog

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();
    virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TBMSG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CTabCtrl m_FuncTab;
    afx_msg void OnBnClickedBtnSearch();
};
