#pragma once

#include "Resource.h"
#include "dhtmledit1.h"


// CEditMsgDlg dialog

class CEditMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditMsgDlg)

public:
	CEditMsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMsgDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EDITMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CDhtmledit1 m_DhtmlMsg;
};
