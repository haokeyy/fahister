
// NnDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CNnDlg �Ի���
class CNnDlg : public CDialog
{
// ����
public:
	CNnDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NNSTOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnAboutbox();
    afx_msg void OnHelpReg();
    afx_msg void OnBnClickedBtnAddPriceMonitor();
    CListCtrl m_ListMonitor;
	afx_msg void OnBnClickedBtnTest();
};
