
// tbmsgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CtbmsgDlg �Ի���
class CtbmsgDlg : public CDialog
{
// ����
public:
	CtbmsgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TBMESSAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CExplorerOcx m_ExprMsgHelp;
    afx_msg void OnBnClickedBtnAddMsg();
    CListCtrl m_MessageList;
    afx_msg void OnBnClickedBtnEditMsg();
    afx_msg void OnBnClickedBtnDelMsg();
    afx_msg void OnBnClickedBtnAddAccount();
    CListCtrl m_AccountList;
    afx_msg void OnBnClickedBtnDelAccount();
    afx_msg void OnBnClickedBtnSendmsg();
    CListCtrl m_MemberList;
    afx_msg void OnBnClickedBtnImport();
    afx_msg void OnBnClickedBtnClear();
    CComboBox m_CmbTarget;
    CComboBox m_CmbCategory;
    CComboBox m_CmbLocation;
    void InitCategory();
    void InitLocation();
    void InitSpeed();
    void StartSendMsg();
    void StopSendMsg();
    BOOL OpenSendWindow(CString szSenderID, CString szReceiverID);
    CComboBox m_CmbSpeed;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg LRESULT OnFoundMember(WPARAM wParam, LPARAM lParam); 
    afx_msg LRESULT OnSendMsgCompleted(WPARAM wParam, LPARAM lParam); 
};
