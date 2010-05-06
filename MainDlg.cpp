// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MainDlg.h"
#include ".\Libraries\WindowHelp.h"


// CMainDlg dialog
BOOL bHasMemberChanged;

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FUNC_TAB, m_FuncTab);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TCN_SELCHANGE, IDC_FUNC_TAB, &CMainDlg::OnTcnSelchangeFuncTab)
END_MESSAGE_MAP()

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��֤�û��Ƿ�ע��
void CMainDlg::ValidateReg()
{

#ifdef DEBUG
    CString szRet   = "1000000";
    CString szData1 = "http://shopsearch.taobao.com/browse/shop_search.htm?title=title&nick=nick";
    CString szData2 = "aliim:sendmsg?uid=cntaobao%s&touid=cntaobao%s:3&siteid=cntaobao&status=&fenliu=1";
    CString szData3 = "�Ǻ�";
#else    
    CString szMachineCode, szFileChecksum;
    GetMachineCode(szMachineCode);
    szFileChecksum = GetFileMD5Checksum();

    CString szAuthURL;
    szAuthURL.Format("%s?machinecode=%s&checksum=%s", URL_VALIDATE, szMachineCode, szFileChecksum);

    CString szResult = GetPageDirect(szAuthURL);

    int p1, p2;
    CString szFlag;

    szFlag = "<Span id=\"result\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szRet = szResult.Mid(p1,p2-p1);
        
    // �Ա���ַ
    szFlag = "<Span id=\"data1\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData1 = szResult.Mid(p1,p2-p1);
    
    // �������������õ�URL��ַ
    szFlag = "<Span id=\"data2\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData2 = szResult.Mid(p1,p2-p1);

    // ûע���û�����Ϣβ��
    szFlag = "<Span id=\"data3\">";
    p1 = szResult.Find(szFlag) + szFlag.GetLength();
    p2 = szResult.Find("</Span>", p1);
    CString szData3 = szResult.Mid(p1,p2-p1);

#endif

    int nRetValue = 0;
    try
    {
        nRetValue = atoi(szRet.Trim().GetBuffer(0));
    }
    catch(CSimpleException* )
    {

    }
    m_MemberPage.m_bHasReged = (nRetValue == 1000000);
    CTime t = CTime::GetTickCount();
    CTime t_limit = CTime(2010, 3, 1, 0, 0, 0);
    //if (t < t_limit)
    //{
        m_SearchPage.m_szSearchUrl = szData1.Trim();
        m_SendPage.m_nSendLimit = nRetValue;
        m_SendPage.m_szSendUrl = szData2.Trim();
        m_SendPage.m_szAdText = szData3.Trim();
    //}
}

BOOL CMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    ValidateReg();

    m_FuncTab.AddPage("�����̬", &m_MemberPage, IDD_MEMBER_VIEW);
    m_FuncTab.AddPage("�����û�", &m_SearchPage, IDD_SEARCH_VIEW);
    m_FuncTab.AddPage("Ⱥ����Ϣ", &m_SendPage, IDD_SEND_VIEW);
    m_FuncTab.Show();

    return TRUE;
}

void CMainDlg::OnTcnSelchangeFuncTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    int curSel = m_FuncTab.GetCurSel();
    if (curSel == 2 && bHasMemberChanged)
    {
        m_SendPage.OnBnClickedBtnFirstPage();
    }
    *pResult = 0;
}
