
// tbmsgDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CtbmsgDlg 对话框
class CtbmsgDlg : public CDialog
{
// 构造
public:
	CtbmsgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TBMESSAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CExplorerOcx m_ExprMsgHelp;
    afx_msg void OnBnClickedBtnAddMsg();
    CListCtrl m_MessageList;
    afx_msg void OnBnClickedBtnEditMsg();
    afx_msg void OnBnClickedBtnDelMsg();
    afx_msg void OnBnClickedBtnAddAccount();
    CListCtrl m_AccountList;
    afx_msg void OnBnClickedBtnDelAccount();
    afx_msg void OnBnClickedBtnSendmsg();
    CListCtrl m_MemberList;
    afx_msg void OnBnClickedBtnImport();
    afx_msg void OnBnClickedBtnClear();
    CComboBox m_CmbTarget;
    CComboBox m_CmbCategory;
    CComboBox m_CmbLocation;
    void InitCategory();
    void InitLocation();
    void InitSpeed();
    CComboBox m_CmbSpeed;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
