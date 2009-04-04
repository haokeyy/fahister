#pragma once
#include "Libraries\DHtmlEditOcx.h"


// CEditMsgDlg dialog

class CEditMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditMsgDlg)

public:
	CEditMsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMsgDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_MSG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CDHtmlEditOcx m_MsgEdit;
};
