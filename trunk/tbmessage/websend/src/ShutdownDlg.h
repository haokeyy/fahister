#pragma once


// CShutdownDlg 对话框

class CShutdownDlg : public CDialog
{
	DECLARE_DYNAMIC(CShutdownDlg)

public:
	CShutdownDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShutdownDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SHUTDOWN };

protected:
    int nTimerElapse;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnBnClickedBtnShutdown();
};

int MySystemShutdown();