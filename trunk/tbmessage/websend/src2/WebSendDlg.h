#pragma once
#include "explorer_ocx.h"
#include "afxwin.h"
#include "IMMessage.h"

// CWebSendDlg 对话框

class CWebSendDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebSendDlg)

public:
	CWebSendDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebSendDlg();
    int nMsgType;
    HWND hWndFrom;
    int nUserFlag; //用户在发送方的标识，比如索引

    CString m_szSendFrom;
    CString m_szPassword;
    CString m_szSendTo;
    CString m_szMessage;
    CString m_szMainWndTitle;

// 对话框数据
	enum { IDD = IDD_WEBSEND_DLG };

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg LRESULT OnSendWebMessage(WPARAM wParam, LPARAM lParam);
    void NotifyMainWindow(int nResult);
    int nCurrentStep;
    BOOL bIsRunning;
    int nTryLoginTimers;

	DECLARE_MESSAGE_MAP()
public:
    CExplorerOcx m_IESendMsg;
    DECLARE_EVENTSINK_MAP()
    void DocumentCompleteIeSendMsg(LPDISPATCH pDisp, VARIANT* URL);
    BOOL SendStep1();
    BOOL SendStep2();
    BOOL SendStep3();
    BOOL SendStep4();
    afx_msg void OnTimer(UINT nIDEvent);
    CString m_SendStatus;
    afx_msg void OnDestroy();
};
