#pragma once
#include "afxcmn.h"


// CMemberPage dialog

class CMemberPage : public CDialog
{
	DECLARE_DYNAMIC(CMemberPage)

public:
	CMemberPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMemberPage();

// Dialog Data
	enum { IDD = IDD_MEMBER_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

    

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnAddMsg();
    afx_msg void OnBnClickedBtnEditMsg();
    afx_msg void OnBnClickedBtnDelMsg();
    CListCtrl m_MemberList;
    CListCtrl m_MessageList;
    afx_msg void OnBnClickedBtnImport2();
    afx_msg void OnBnClickedBtnClear();
    afx_msg void OnBnClickedBtnExport2();
    afx_msg void OnBnClickedBtnClear2();

    void LoadMembers();
};
