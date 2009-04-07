#pragma once


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

	DECLARE_MESSAGE_MAP()
};
