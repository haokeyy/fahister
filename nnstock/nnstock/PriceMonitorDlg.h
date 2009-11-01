#pragma once


// CPriceMonitorDlg dialog

class CPriceMonitorDlg : public CDialog
{
	DECLARE_DYNAMIC(CPriceMonitorDlg)

public:
	CPriceMonitorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPriceMonitorDlg();

// Dialog Data
	enum { IDD = IDD_PRICE_MONITOR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
