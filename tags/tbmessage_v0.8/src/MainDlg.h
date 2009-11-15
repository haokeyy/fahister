#pragma once

#include ".\Libraries\TabSheet.h"
#include "SearchPage.h"
#include "MemberPage.h"
#include "SendPage.h"

// CMainDlg dialog

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();

// Dialog Data
	enum { IDD = IDD_TBMSG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CTabSheet m_FuncTab;    
	CSearchPage m_SearchPage;
	CMemberPage m_MemberPage;
	CSendPage m_SendPage;
    afx_msg void OnTcnSelchangeFuncTab(NMHDR *pNMHDR, LRESULT *pResult);
};
