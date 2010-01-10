#pragma once
#include "afxwin.h"
#include "MemberSearch.h"


// CSearchPage dialog

class CSearchPage : public CDialog
{
	DECLARE_DYNAMIC(CSearchPage)

public:
	CSearchPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchPage();

// Dialog Data
	enum { IDD = IDD_SEARCH_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_CmbTarget;
    CComboBox m_CmbCategory;
    CComboBox m_CmbLocation;
    
    CMemberSearch *m_pMemberSearch;
    CString szTaobaoSearchUrl;
    
    void SetConditionCtrlStatus(BOOL bEnabled);
    void InitCategory();
    void InitLocation();
    int m_nFoundCount;
    
    CString m_szSearchUrl;
    
    afx_msg LRESULT OnFoundMember(WPARAM wParam, LPARAM lParam); 

    afx_msg void OnBnClickedBtnSearch();
    afx_msg void OnCbnSelchangeCmbTarget();
};
