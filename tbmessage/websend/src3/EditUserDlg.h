#pragma once
#include "explorer_ocx.h"


// CEditUserDlg �Ի���

class CEditUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditUserDlg();

	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DLG_EDIT_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    CString szUuid;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CExplorerOcx m_expChkCode;
    afx_msg void OnBnClickedButton1();
};
