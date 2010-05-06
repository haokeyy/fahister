#pragma once


// CShutdownDlg dialog

class CShutdownDlg : public CDialog
{
	DECLARE_DYNAMIC(CShutdownDlg)

public:
	CShutdownDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShutdownDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SHUTDOWN };

protected:
    int nTimerElapse;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnShutdown();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
