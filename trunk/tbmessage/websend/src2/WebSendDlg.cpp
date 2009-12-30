// WebSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "WebSendDlg.h"
#include ".\websenddlg.h"
#include "DHtmlHelper.h"

#define WEBSEND_TIMER_ID 1025
#define WEBSEND_TIMEOUT_TIMER_ID 1026
//

void GetUnicodeUserID(CString szUserID, CString& szUnicodeUserID)
{
    WCHAR wszUserID[256];

    szUnicodeUserID.Empty();

    MultiByteToWideChar( CP_ACP, 0, szUserID.GetBuffer(0),
        szUserID.GetLength()+1, wszUserID, sizeof(wszUserID)/sizeof(wszUserID[0]));

    int len = wcslen(wszUserID);
    for (int i = 0; i < len; i++)
    {
        int cn = wszUserID[i];
        if (cn < 0x7F)
        {
            szUnicodeUserID.AppendFormat("%c", wszUserID[i]);
        }
        else if (cn < 0xFF)
        {
            szUnicodeUserID.AppendFormat("$%X", wszUserID[i]);
        }
        else
        {
            szUnicodeUserID.AppendFormat("$u%X", wszUserID[i]);
        }
    }
}

HRESULT FindElementByID(IHTMLElementCollection *pColl, CString szID, IHTMLElement** pElem)
{
    BSTR bstrID = szID.AllocSysString();

    VARIANT varName, varIndex;
    varName.vt = VT_BSTR;
    varName.bstrVal = bstrID;
    varIndex.vt = VT_I4;
    varIndex.intVal = 0;

    IDispatch* pDisp; 
    HRESULT hr = pColl->item(varName, varIndex, &pDisp);
    if (pDisp)
    {
        hr = pDisp->QueryInterface( IID_IHTMLElement, (void**)pElem );
    }
    else
    {
        *pElem = NULL;
    }
    
    ::SysFreeString(bstrID);
    return hr;
}

HRESULT FindElementByOuterHTML(IHTMLElementCollection *pColl, CString szOuterHTML, IHTMLElement** pElem)
{
    *pElem = NULL;
    HRESULT hr = 1;
    long len;
    pColl->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        VARIANT varName, varIndex;
        varName.vt = VT_UINT;
        varName.lVal = i;
        varIndex.vt = VT_I4;
        varIndex.intVal = 0;

        IDispatch* pDisp; 
        hr = pColl->item(varName, varIndex, &pDisp);
        if (hr == S_OK)
        {
            hr = pDisp->QueryInterface( IID_IHTMLElement, (void**)pElem);
            if (hr == S_OK)
            {
                BSTR tempOuterHTML;
                (*pElem)->get_outerHTML(&tempOuterHTML);

                if (szOuterHTML == tempOuterHTML)
                {
                    ::SysFreeString(tempOuterHTML);
                    break;
                }
                ::SysFreeString(tempOuterHTML);
            }
        }
    }

    return hr;
}


HRESULT FindElementByName(IHTMLElementCollection *pColl, CString szOuterHTML, IHTMLElement** pElem)
{
    *pElem = NULL;
    HRESULT hr = 1;
    long len;
    pColl->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        VARIANT varName, varIndex;
        varName.vt = VT_UINT;
        varName.lVal = i;
        varIndex.vt = VT_I4;
        varIndex.intVal = 0;

        IDispatch* pDisp; 
        hr = pColl->item(varName, varIndex, &pDisp);
        if (hr == S_OK)
        {
            hr = pDisp->QueryInterface( IID_IHTMLElement, (void**)pElem);
            if (hr == S_OK)
            {
                BSTR tempOuterHTML;
                (*pElem)->get_outerHTML(&tempOuterHTML);

                if (szOuterHTML == tempOuterHTML)
                {
                    ::SysFreeString(tempOuterHTML);
                    break;
                }
                ::SysFreeString(tempOuterHTML);
            }
        }
    }

    return hr;
}

// CWebSendDlg 对话框
IMPLEMENT_DYNAMIC(CWebSendDlg, CDialog)
CWebSendDlg::CWebSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebSendDlg::IDD, pParent)
    , m_SendStatus(_T(""))
{
    nCurrentStep = 0;
    bIsRunning = TRUE;
    nTryLoginTimers = 0;
}

CWebSendDlg::~CWebSendDlg()
{
}

BOOL CWebSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    
    this->SetDlgItemText(IDC_SENDFROM, "zhi3385");
    this->SetDlgItemText(IDC_PASSWORD, "zhi3385");
    this->SetDlgItemText(IDC_SENDTO, "citypop2009");
    this->SetDlgItemText(IDC_MESSAGE, "hi");
    this->SetDlgItemText(IDC_MAINWNDTITLE, "zhi3385");

    OnSendWebMessage(0, (LPARAM)1);

    // 超时后用该定时器结束程序, 超时时间30s
    //this->SetTimer(WEBSEND_TIMEOUT_TIMER_ID, 30000, NULL);

    return TRUE;
}

void CWebSendDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IE_SEND_MSG, m_IESendMsg);
    DDX_Text(pDX, IDC_SEND_STATUS, m_SendStatus);
}


BEGIN_MESSAGE_MAP(CWebSendDlg, CDialog)
    ON_MESSAGE(WM_SEND_WEB_MESSAGE, OnSendWebMessage) 
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// 第一步，输入好友名
BOOL CWebSendDlg::SendStep1()
{
    HRESULT hr = 1;

    IHTMLElementCollection *pColl;    
    IDispatch *pDisp = m_IESendMsg.get_Document();   
    IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
    hr = pDoc->get_all(&pColl);

    // 用户名
    IHTMLElement *ptbUser = NULL;
    hr = FindElementByID(pColl, "tbUser", &ptbUser);
    if (hr == S_OK && ptbUser)
    {
        BSTR bstrSendTo = m_szSendTo.AllocSysString();
        ptbUser->put_innerText(bstrSendTo);
        ::SysFreeString(bstrSendTo);
        ptbUser->Release();
    }
    pColl->Release();
    pDisp->Release();

    this->SetTimer(WEBSEND_TIMER_ID, 1000, NULL);

    return TRUE;
}

// 第二步，开始聊天
BOOL CWebSendDlg::SendStep2()
{
    HRESULT hr = 1;

    IHTMLElementCollection *pColl;    
    IDispatch *pDisp = m_IESendMsg.get_Document();   
    IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
    hr = pDoc->get_all(&pColl);

    // 开始聊天
    IHTMLElement *psimpleSayToBtn=NULL;
    hr = FindElementByID(pColl, "simpleSayToBtn", &psimpleSayToBtn);

    if (hr == S_OK && psimpleSayToBtn)
    {
        psimpleSayToBtn->click();
        psimpleSayToBtn->Release();
    }
    
    pColl->Release();
    pDisp->Release();

    this->SetTimer(WEBSEND_TIMER_ID, 1000, NULL);

    return TRUE;
}

void SaveCheckCodeUrl(CString szCheckCodeUrl)
{
    CStdioFile file("c:\\checkcodeurl.txt", CFile::modeCreate|CFile::modeReadWrite);
    file.WriteString(szCheckCodeUrl);
}

CString GetCheckCode(CString szCheckCodeUrl)
{
    CString szCheckCode;
    CStdioFile file("c:\\checkcode.txt", CFile::modeReadWrite);
    file.ReadString(szCheckCode);

    return szCheckCode;
}

// 第三步，输入验证码
BOOL CWebSendDlg::SendStep3()
{
    HRESULT hr = 1;

    IHTMLElement *pSubmit;
    IHTMLElementCollection *pColl;    
    IDispatch *pDisp = m_IESendMsg.get_Document();   
    IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
    hr = pDoc->get_all(&pColl);

    
    CString szUnicodeID;
    GetUnicodeUserID(m_szSendTo, szUnicodeID);
    CString szChkCodeFrame = "validateFramecntaobao" + szUnicodeID;

    CString szCheckCodeUrl = GetControlStringAttribute(pDoc, szChkCodeFrame, "src");
    CString szCheckCode;
    if (!szCheckCodeUrl.IsEmpty())
    {
        SaveCheckCodeUrl(szCheckCodeUrl);
        szCheckCode = GetCheckCode(szCheckCodeUrl);
        if (szCheckCode.IsEmpty())
        {
            nCurrentStep--;
        }
        else // 输入验证码
        {
            CString szChkCodeInput = "checkcodecntaobao" + szUnicodeID;
            SetControlValue(pDoc, szChkCodeInput, szCheckCode);
            GetElementByName(pDoc, "codeSubmitBtn", &pSubmit);
            pSubmit->click();
        }
    }

    this->SetTimer(WEBSEND_TIMER_ID, 1000, NULL);

    return TRUE;
}

// 第四步，输入消息, 发送
BOOL CWebSendDlg::SendStep4()
{
    
    HRESULT hr1 = 1, hr2 = 1;

    IHTMLElementCollection *pColl;    
    IDispatch *pDisp = m_IESendMsg.get_Document();   
    IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
    hr1 = pDoc->get_all(&pColl);

    IHTMLElement *pmsg=NULL, *psend=NULL;
    CString szUnicodeID;
    GetUnicodeUserID(m_szSendTo, szUnicodeID);

    // 发送消息
    CString szInput = "chatInput_cntaobao"; 
    szInput += szUnicodeID; 
    CString szBtn = "btn_cntaobao"; 
    szBtn += szUnicodeID;
    hr1 = FindElementByID(pColl, szInput, &pmsg);
    hr2 = FindElementByID(pColl, szBtn, &psend);
    if (hr1 == S_OK && hr2 == S_OK && pmsg && psend)
    {
        BSTR bstrMessage = CString(m_szMessage).AllocSysString();
        pmsg->put_innerText(bstrMessage);
        psend->click();

        ::SysFreeString(bstrMessage);
        pmsg->Release();
        psend->Release();
    }

    pColl->Release();
    pDisp->Release();

    //HRESULT hr1 = 1, hr2 = 1;

    //IHTMLElementCollection *pColl;    
    //IDispatch *pDisp = m_IESendMsg.get_Document();   
    //IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
    //hr1 = pDoc->get_all(&pColl);

    //CString szUnicodeID;
    //GetUnicodeUserID(m_szSendTo, szUnicodeID);

    //// 关闭当前用户
    //IHTMLElement *pclose=NULL;
    //CString szHTML;
    //szHTML.Format("<A class=Close href=\"javascript:_closeAW('%s')\">关闭</A>", szUnicodeID.GetBuffer(0));
    //hr1 = FindElementByOuterHTML(pColl, szHTML, &pclose);
    //if (hr1 == S_OK && pclose)
    //{
    //    pclose->click();
    //    pclose->Release();
    //}

    //pColl->Release();
    //pDisp->Release();

    //::PostMessage(hWndFrom,WM_SENDMSGCOMPLETED, (WPARAM)1, (LPARAM)nUserFlag);
    NotifyMainWindow(1);

    //this->OnCancel();

    return TRUE;
}

// 发送消息
LRESULT CWebSendDlg::OnSendWebMessage(WPARAM wParam1, LPARAM lParam1)
{
    nUserFlag = (int)lParam1;
    
    this->GetDlgItemText(IDC_SENDFROM, m_szSendFrom);
    this->GetDlgItemText(IDC_PASSWORD, m_szPassword);
    this->GetDlgItemText(IDC_SENDTO, m_szSendTo);
    this->GetDlgItemText(IDC_MESSAGE, m_szMessage);
    this->GetDlgItemText(IDC_MAINWNDTITLE, m_szMainWndTitle);
    hWndFrom = ::FindWindow("#32770", m_szMainWndTitle.GetBuffer(0));

    
    CString szTitle("发送淘宝消息-");
    this->SetWindowText(szTitle + m_szSendFrom);

    CString url = m_IESendMsg.get_LocationURL();

    if (url.Find("http://webwwtb.im.alisoft.com/wangwang/webww.htm?") < 0)
    {
        CString szURL = "http://webwwtb.im.alisoft.com/wangwang/webww.htm";
    
        m_IESendMsg.Navigate(szURL, 0,0,0,0);
    }
    else
    {
        nCurrentStep = 1;
        this->SetTimer(WEBSEND_TIMER_ID, 1000, NULL);

        m_SendStatus = "正在发送：";
        m_SendStatus += m_szSendTo;
        UpdateData(FALSE);
    }

    return 0;
}

BEGIN_EVENTSINK_MAP(CWebSendDlg, CDialog)
    ON_EVENT(CWebSendDlg, IDC_IE_SEND_MSG, 259, DocumentCompleteIeSendMsg, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CWebSendDlg::DocumentCompleteIeSendMsg(LPDISPATCH pDisp, VARIANT* URL)
{
    CString url = m_IESendMsg.get_LocationURL();

    // 登录
    if (url.Find("http://member1.taobao.com/member/login.jhtml?redirect_url=") >= 0)
    {
        IDispatch *pDisp = m_IESendMsg.get_Document();        
        IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;

        IHTMLElement *pChangeMode;

        GetElementByInnerHtml(pDoc, "标准登录", &pChangeMode);
        if (pChangeMode)
            pChangeMode->click();

    }
	else if (url.Find("http://login.taobao.com/member/login.jhtml?login_type=3") >= 0)
    {
        BSTR bstrSendFrom = m_szSendFrom.AllocSysString();
        BSTR bstrPassword = m_szPassword.AllocSysString();

        m_SendStatus = "正在登录...";
        UpdateData(FALSE);
            
        HRESULT hr = 1;        
        IHTMLElementCollection *pColl;    
        IDispatch *pDisp = m_IESendMsg.get_Document();        
        IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
        hr = pDoc->get_all(&pColl);

        IHTMLElement *pUserName, *pPwd, *pLogin;

        GetElementByName(pDoc, "TPL_username", &pUserName);
        FindElementByID(pColl, "TPL_password", &pPwd);
        GetElementByOuterHtml(pDoc, "<BUTTON tabIndex=3 type=submit value=\"\">登&nbsp;&nbsp;&nbsp;&nbsp;录</BUTTON>", &pLogin);

        if (pUserName && pPwd && pLogin)
        {
            pUserName->put_innerText(bstrSendFrom);
            pPwd->put_innerText(bstrPassword);
            pLogin->click();


            pUserName->Release();
            pPwd->Release();
            pLogin->Release();
        }

        pColl->Release();
        pDisp->Release();
        ::SysFreeString(bstrSendFrom);
        ::SysFreeString(bstrPassword);

        nTryLoginTimers++;

        //if (nTryLoginTimers > 3)
        //{
        //    NotifyMainWindow(-1);
        //}
    }
    else if (url.Find("http://webwwtb.im.alisoft.com/wangwang/webww.htm?") >= 0)
    {   
        HRESULT hr = 1;    
        IDispatch *pDisp = m_IESendMsg.get_Document();
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
           
        }
         
        
        nCurrentStep = 1;
        this->SetTimer(WEBSEND_TIMER_ID, 2000, NULL);

        m_SendStatus = "正在发送：";
        m_SendStatus += m_szSendTo;
        UpdateData(FALSE);
    }
        /*
    else if(url.Find("http://member1.taobao.com/member/login.jhtml") >= 0)
    {
        NotifyMainWindow(-1);
    }*/
}

void CWebSendDlg::OnTimer(UINT nIDEvent)
{
    if ( nIDEvent == WEBSEND_TIMER_ID)
    {
        this->KillTimer(WEBSEND_TIMER_ID);

        switch (nCurrentStep)
        {
            case 1:
                SendStep1();
                break;
            case 2:
                SendStep2();
                break;
            case 3:            
                SendStep3();
                break;
            case 4:            
                SendStep4();
                break;
            default:
                break;
        }
        nCurrentStep++;
    }
    else if (nIDEvent == WEBSEND_TIMEOUT_TIMER_ID)
    {
        this->KillTimer(WEBSEND_TIMEOUT_TIMER_ID);
        NotifyMainWindow(0);
    }

    CDialog::OnTimer(nIDEvent);
}

void CWebSendDlg::OnDestroy()
{
    m_IESendMsg.DestroyWindow();

    CDialog::OnDestroy();
}

// 给主窗口发送通知， nResult:0:登录失败,1:发送成功
void CWebSendDlg::NotifyMainWindow(int nResult)
{
    HWND hPrompt = ::FindWindowEx(hWndFrom, NULL, "Static", "发送者和接收者");
    HWND hSendFrom = ::GetWindow(hPrompt, GW_HWNDNEXT);
    HWND hSendTo = ::GetWindow(hSendFrom, GW_HWNDNEXT);

    ::SendMessage(hSendFrom, WM_SETTEXT, 0, (LPARAM)m_szSendFrom.GetBuffer(0));
    ::SendMessage(hSendTo, WM_SETTEXT, 0, (LPARAM)m_szSendTo.GetBuffer(0));

    ::SendMessage(hWndFrom, WM_SENDMSGCOMPLETED, (WPARAM)0, (LPARAM)nResult);

    //this->OnCancel();
}