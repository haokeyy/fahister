
// tbmsgDlg.h : ͷ�ļ�
//

#pragma once



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
};
