#pragma once


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

	DECLARE_MESSAGE_MAP()
};
