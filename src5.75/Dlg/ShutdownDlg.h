#pragma once


// CShutdownDlg �Ի���

class CShutdownDlg : public CDialog
{
	DECLARE_DYNAMIC(CShutdownDlg)

public:
	CShutdownDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShutdownDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SHUTDOWN };

protected:
    int nTimerElapse;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnBnClickedBtnShutdown();
};

int MySystemShutdown();