#pragma once
#include "resource.h"	
#include "afxcmn.h"

// CWebSendDlg 对话框

class CWebSendDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebSendDlg)

public:
	CWebSendDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebSendDlg();

// 对话框数据
	enum { IDD = IDD_WEBSEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCopy();
	CRichEditCtrl m_Message;
	afx_msg void OnBnClickedBtnSetfont();
	afx_msg void OnBnClickedButton3();
	CRichEditCtrl m_Message2;
};
