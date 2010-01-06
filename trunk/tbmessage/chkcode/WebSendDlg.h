#pragma once


// CWebSendDlg dialog

class CWebSendDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebSendDlg)

public:
	CWebSendDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWebSendDlg();

// Dialog Data
	enum { IDD = IDD_WEBSEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
