#pragma once
#include "resource.h"	
#include "afxcmn.h"

// CWebSendDlg �Ի���

class CWebSendDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebSendDlg)

public:
	CWebSendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWebSendDlg();

// �Ի�������
	enum { IDD = IDD_WEBSEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCopy();
	CRichEditCtrl m_Message;
	afx_msg void OnBnClickedBtnSetfont();
	afx_msg void OnBnClickedButton3();
	CRichEditCtrl m_Message2;
};
