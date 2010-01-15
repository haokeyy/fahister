// HelpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\BatchMessage.h"
#include "HelpDlg.h"
#include ".\helpdlg.h"


// CHelpDlg 对话框

IMPLEMENT_DYNCREATE(CHelpDlg, CDHtmlDialog)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD,  pParent) //CHelpDlg::IDH,
{
    m_bIsFirst = TRUE;
}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXPLORER_OCX, m_Browser);
}

BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    CString szHelpUrl = MAIN_WEB_SITE;
    szHelpUrl += "/link.aspx?type=2&id=";
    szHelpUrl += FILE_ID;


    //szHelpUrl = "http://www.search.hc360.com/cgi-bin/ls?m=1&c=%C6%F3%D2%B5%BF%E2&z=%D6%D0%B9%FA%3A%B9%F3%D6%DD%CA%A1";

    m_Browser.Navigate(szHelpUrl, NULL, NULL, NULL, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

//BEGIN_DHTML_EVENT_MAP(CHelpDlg)
//END_DHTML_EVENT_MAP()

void CHelpDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    CRect rc;
    this->GetClientRect(&rc);

    if (m_bIsFirst)
    {
        m_bIsFirst = FALSE;
    }
    else
    {
        m_Browser.MoveWindow(&rc);
    }
}

void CHelpDlg::OnBnClickedButton1()
{
    m_Browser.Navigate("wangwang:SendIM?s=1&v=3&mul=1&uid=ruiqi0011&uid_t=ruiqi0011&suid=a0s4o5&desc=", NULL, NULL, NULL, NULL);
            
 /*       HRESULT hr = 1;    
        IDispatch *pDisp = m_Browser.get_Document();
        if (pDisp)
        {
            IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
            IHTMLWindow2 *pWindow, *pTopWindow;
            VARIANT pvarRet;
            pDoc->get_parentWindow(&pWindow);
            pWindow->get_top(&pTopWindow);
            CString szScript1 = "function confirm(str){return true;}";
            CString szScript2 = "function alert(str){return;}";
            CString szScript3 = "function beforeUnload(){}";
            CString szLanguage = "javascript";
            BSTR bstrScript1 = szScript1.AllocSysString();
            BSTR bstrScript2 = szScript2.AllocSysString();
            BSTR bstrScript3 = szScript3.AllocSysString();
            BSTR bstrLanguage = szLanguage.AllocSysString();
            hr = pTopWindow->execScript(bstrScript1, bstrLanguage, &pvarRet);
            hr = pTopWindow->execScript(bstrScript2, bstrLanguage, &pvarRet);
            hr = pTopWindow->execScript(bstrScript3, bstrLanguage, &pvarRet);

            ::SysFreeString(bstrScript1);
            ::SysFreeString(bstrScript2);
            ::SysFreeString(bstrScript3);
            pTopWindow->Release();
            pWindow->Release();
            pDisp->Release();
        }*/
}
