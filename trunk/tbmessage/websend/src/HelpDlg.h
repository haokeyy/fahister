#pragma once
#include "explorer_ocx.h"


// CHelpDlg 对话框

class CHelpDlg : public CDialog
{
	DECLARE_DYNCREATE(CHelpDlg)

public:
	CHelpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHelpDlg();


// 对话框数据
    enum { IDD = IDD_DLG_HELP}; //, IDH = IDR_HTML_HELPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
    BOOL m_bIsFirst;

	DECLARE_MESSAGE_MAP()
	//DECLARE_DHTML_EVENT_MAP()
public:
    CExplorerOcx m_Browser;
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
