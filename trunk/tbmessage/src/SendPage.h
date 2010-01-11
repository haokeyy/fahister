#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "libraries\explorerocx.h"


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
	long lastMemberId;
    BOOL m_IsStop;
    int m_nSendedCount;

	DECLARE_MESSAGE_MAP()
public:
    CString szTaobaoSendUrl;
    CString m_szLastSenderId;
    afx_msg void OnBnClickedBtnSendmsg();
    afx_msg void OnBnClickedBtnAddAccount();
    afx_msg void OnBnClickedBtnDelAccount();
    CListCtrl m_AccountList;
    CComboBox m_CmbSpeed;
    void InitSpeed();
    void LoadMembers(long startId, long stepCount);
	void SetPagging(long start, long pageSize);
    void SaveProfile();
    void LoadProfile();
    void StartSendMsg();
    void StopSendMsg();
    int GetNextMessage(CString& szNextMessage);
    BOOL IsOnline(CString szUserId);
    int GetNextMember(CString& szNextReceiver);
    BOOL OpenSendWindow(CString szSenderID, CString szReceiverID);
    LRESULT OnSendMsgCompleted(WPARAM wParam, LPARAM lParam);
    CExplorerOcx m_ExprMsgHelp;
    void SendImMsg();
	CListCtrl m_MessageList;
	CListCtrl m_MemberList;
	afx_msg void OnBnClickedBtnAddMsg();
	afx_msg void OnBnClickedBtnEditMsg();
	afx_msg void OnBnClickedBtnDelMsg();
	afx_msg void OnBnClickedBtnImport2();
	afx_msg void OnBnClickedBtnExport2();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnClear2();
	afx_msg void OnBnClickedBtnFirstPage();
	afx_msg void OnBnClickedBtnPrevPage();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnBnClickedBtnLastPage();
    UINT m_nSendLimit;
    BOOL m_bHasReged;
    CString m_szSendUrl;
    CString m_szAdText;
    afx_msg void OnBnClickedBtnReset();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnImport();
};
