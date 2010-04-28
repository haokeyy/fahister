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

    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    //CDHtmlEditOcx m_MsgEdit;
    CString GetMsgHtml();
    CString GetMsgText();
    void SetMsgHtml(CString szMsgHtml);
    afx_msg void OnBnClickedOk();

private:
    CString bodyInnerHtml;
    CString bodyInnerText;
    //CString InternalGetMsgHtml();
    //CString InternalGetMsgText();
    //void InternalSetMsgHtml(CString szMsgHtml);

//public:
//    DECLARE_EVENTSINK_MAP()
//    void DocumentCompleteDhtmlEditMsg();
};
