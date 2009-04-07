#pragma once


// CSendPage dialog

class CSendPage : public CDialog
{
	DECLARE_DYNAMIC(CSendPage)

public:
	CSendPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSendPage();

// Dialog Data
	enum { IDD = IDD_SEND_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
