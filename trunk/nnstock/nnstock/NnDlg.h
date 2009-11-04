
// NnDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CNnDlg 对话框
class CNnDlg : public CDialog
{
// 构造
public:
	CNnDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NNSTOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnAboutbox();
    afx_msg void OnHelpReg();
    afx_msg void OnBnClickedBtnAddPriceMonitor();
    CListCtrl m_ListMonitor;
	afx_msg void OnBnClickedBtnTest();
};
