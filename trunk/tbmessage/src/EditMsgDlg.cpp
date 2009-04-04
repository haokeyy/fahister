// EditMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "EditMsgDlg.h"


// CEditMsgDlg dialog

IMPLEMENT_DYNAMIC(CEditMsgDlg, CDialog)

CEditMsgDlg::CEditMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMsgDlg::IDD, pParent)
{

}

CEditMsgDlg::~CEditMsgDlg()
{
}

void CEditMsgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DHTML_EDIT_MSG, m_MsgEdit);
}


BEGIN_MESSAGE_MAP(CEditMsgDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CEditMsgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


CString CEditMsgDlg::GetMsgHtml()
{
    return bodyInnerHtml;
}

CString CEditMsgDlg::GetMsgText()
{
    return bodyInnerText;
}

void CEditMsgDlg::SetMsgHtml(CString szMsgHtml)
{
    bodyInnerHtml = szMsgHtml;
}

CString CEditMsgDlg::InternalGetMsgHtml()
{
    IDispatch *pDisp = m_MsgEdit.get_DOM();
    if (pDisp)
    {
        IHTMLElement *pBody;
        BSTR bstrBody;
        IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
        pDoc->get_body(&pBody);
        pBody->get_innerHTML(&bstrBody);
        CString szBody(bstrBody);
        ::SysFreeString(bstrBody);

        return szBody;
    }
    return CString();
}

CString CEditMsgDlg::InternalGetMsgText()
{
    IDispatch *pDisp = m_MsgEdit.get_DOM();
    if (pDisp)
    {
        IHTMLElement *pBody;
        BSTR bstrBody;
        IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
        pDoc->get_body(&pBody);
        pBody->get_innerText(&bstrBody);
        CString szBody(bstrBody);
        ::SysFreeString(bstrBody);

        return szBody;
    }
    return CString();
}

void CEditMsgDlg::InternalSetMsgHtml(CString szMsgHtml)
{
    BSTR bstrBody = szMsgHtml.AllocSysString();
    IDispatch *pDisp = m_MsgEdit.get_DOM();
    if (pDisp)
    {
        IHTMLDocument2 *pDoc = (IHTMLDocument2 *)pDisp;
        IHTMLElement *pBody;
        pDoc->get_body(&pBody);
        pBody->put_innerHTML(bstrBody);

        ::SysFreeString(bstrBody);
    }
}

void CEditMsgDlg::OnBnClickedOk()
{
    bodyInnerHtml = InternalGetMsgHtml();
    bodyInnerText = InternalGetMsgText();

    if (bodyInnerText.IsEmpty())
    {
        MessageBox("没有输入任何消息内容。");
    }

    OnOK();
}

BEGIN_EVENTSINK_MAP(CEditMsgDlg, CDialog)
ON_EVENT(CEditMsgDlg, IDC_DHTML_EDIT_MSG, 1, CEditMsgDlg::DocumentCompleteDhtmlEditMsg, VTS_NONE)
END_EVENTSINK_MAP()

void CEditMsgDlg::DocumentCompleteDhtmlEditMsg()
{
    if (!bodyInnerHtml.IsEmpty())
    {
        InternalSetMsgHtml(bodyInnerHtml);
    }
}
