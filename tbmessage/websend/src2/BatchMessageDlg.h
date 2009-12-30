// BatchMessageDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "IThread.h"
#include "SendMsgThread.h"
#include "HLink.h"
#include <afxtempl.h>
#include "explorer_ocx.h"
#include "WebSendDlg.h"

// CBatchMessageDlg 对话框
class CBatchMessageDlg : public CDialog
{
// 构造
public:
	CBatchMessageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BATCHMESSAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
    CGetMemberThread m_GetMemberThread;
    CWebSendDlg* m_pWebSendDlg;
    void ImportUser(CString fileName);
    void ExportUser(CString fileName);
    CString szVersionAD;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_MemberList;
    CComboBox m_CmbTarget;
    CString m_Message;
    CButton m_BtnSendMsg;
    BOOL m_bHideWnd;
    UINT foundCount;
    UINT unitCount;
    CButton m_BtnSearch;
    UINT m_MemberLimit;
    CComboBox m_CmbSendSpeed;
    CComboBox m_CmbCategory;
    CComboBox m_CmbLocation;
    CListBox m_UserList;
    CString m_Keyword;
    BOOL m_bShutdown;
    CButton m_BtnAddUser;
    CButton m_BtnDelUser;
    CEdit m_EditMsg;
    CHLink m_CompanyLink;
    UINT ValidUser;
    int nSendCount;
    int m_nMsgType;

    int GetUnSendedMember(CString& szMemberName);
    void OnStartSend();
    void OnFinishSend();
    void AddMember(CString szItem);
    void AddMemberNoRepeat(CString szMember);
    void SetMemberStatus(CString szMember, int nStatus);
    void InitCategory();
    void InitLocation();
    void InitBizType();
    void InitProvince();
    int GetSenderUser(CString& szUserID, CString& szPassword);
    long ValidateReg(void);

    afx_msg LRESULT OnFoundMember(WPARAM wParam, LPARAM lParam); 
    afx_msg LRESULT OnSendMsgCompleted(WPARAM wParam, LPARAM lParam); 
    LRESULT OnNotifyChangeUser(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedBtnImport();
    afx_msg void OnBnClickedBtnExport();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnClear();
    afx_msg void OnBnClickedBtnAdduser();
    afx_msg void OnBnClickedBtnSearch();
    afx_msg void OnBnClickedBtnSendmsg();
    afx_msg void OnBnClickedBtnDeluser();
    afx_msg void OnBnClickedBtnHelp();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnBnClickedBtnExport2();
    afx_msg void OnBnClickedBtnClear2();
    afx_msg void OnBnClickedBtnReg();
    CExplorerOcx m_Browser;
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnMembername();
    afx_msg void OnCbnSelchangeCmbTarget();
};
