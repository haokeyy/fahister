#pragma once


// CEditUserDlg �Ի���

class CEditUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditUserDlg();

// �Ի�������
	enum { IDD = IDD_DLG_EDIT_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_UserId;
    afx_msg void OnBnClickedOk();
    CString m_Password;
};
