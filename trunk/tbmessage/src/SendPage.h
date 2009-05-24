#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "d:\projects\fahister\trunk\tbmessage\src\libraries\explorerocx.h"


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

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    CString szTaobaoSendUrl;
    afx_msg void OnBnClickedBtnSendmsg();
    afx_msg void OnBnClickedBtnAddAccount();
    afx_msg void OnBnClickedBtnDelAccount();
    CListCtrl m_AccountList;
    CComboBox m_CmbSpeed;
    void InitSpeed();
    void StartSendMsg();
    void StopSendMsg();
    int GetNextMessage(CString& szNextMessage);
    int GetNextMember(CString& szNextReceiver);
    BOOL OpenSendWindow(CString szSenderID, CString szReceiverID);
    LRESULT OnSendMsgCompleted(WPARAM wParam, LPARAM lParam);
    CExplorerOcx m_ExprMsgHelp;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void SaveProfile();
    void LoadProfile();
    afx_msg void OnClose();
};
