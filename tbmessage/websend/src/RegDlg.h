#pragma once
#include "afxwin.h"
#include "HLink.h"

// CRegDlg �Ի���

class CRegDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegDlg)

public:
	CRegDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegDlg();

// �Ի�������
	enum { IDD = IDD_DLG_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CHLink m_RegURL;
    virtual BOOL OnInitDialog();
    CEdit m_MachineCode;
    afx_msg void OnBnClickedBtnCopy();
    afx_msg void OnBnClickedBtnSaveuserid();
    CString m_UserID;
};
