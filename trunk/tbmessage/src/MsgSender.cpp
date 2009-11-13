#include "StdAfx.h"
#include "MsgSender.h"
#include ".\Libraries\WindowHelp.h"
#include ".\Libraries\DHtmlHelper.h"

CMessageSender::CMessageSender(HWND hMainWnd)
{
    m_hMainWnd = hMainWnd;
}

CMessageSender::~CMessageSender()
{
}

void CMessageSender::SendMsg(CInstantMessage msg)
{
    this->message = msg;

    AfxBeginThread(SendMsgWorkerThread, this);
}

UINT SendMsgWorkerThread(LPVOID pvThread)
{
    CMessageSender* pSendMsgThread = (CMessageSender*)pvThread;

    return pSendMsgThread->ExecuteSendMsg();
}

UINT CMessageSender::ExecuteSendMsg()
{
    Sleep(1800);

    if (message.AddToFriend)
    {
        AddToFriend();
    }

    // �û��Ƿ��Ѿ���¼
    CString strWndTitle("-�������� 2008");
    strWndTitle = message.SendUserId + strWndTitle;
    HWND hMainHwnd = FindTopWindowExactly(strWndTitle.GetBuffer(), "");
    if (hMainHwnd == NULL && message.AutoLogin) // û�е�¼
    {
        ExecuteLogin();
    }

    SendOneMsg();

    CString szIndex;
    szIndex.Format("%d", message.nItemIndex);
    ::PostMessage(this->m_hMainWnd, WM_SENDMSG_COMPLETED, (WPARAM)0, (LPARAM)szIndex.GetBuffer());

    // �ر������ϵ�˳ɹ���ʾ����
    HWND hAddSuccess = ::FindWindow("#32770", "�����Ϊ��ϵ����ʾ");
    while (hAddSuccess)
    {
        ::PostMessage(hAddSuccess, WM_CLOSE, 0, 0);

        hAddSuccess = ::FindWindow("#32770", "�����Ϊ��ϵ����ʾ");
    }

    // �ر������ϵ��ʧ����ʾ����
    HWND hAddFail = ::FindWindow("#32770", "������󱻾ܾ�");
    ::PostMessage(hAddFail, WM_CLOSE, 0, 0);

    return 0;
}

CString Recognition(CString szUrl)
{
    CString strLine;
    
    CString szFileName;
    DWORD nProcID = ::GetCurrentProcessId();
    GetProcessNameByProcessID(nProcID, szFileName);

    int index = szFileName.ReverseFind('\\');
    CString chkFilePath = szFileName.Left(index) + "\\checkcode.txt";

    CStdioFile file(chkFilePath, CFile::modeRead);
    file.ReadString(strLine);
    return strLine;
}

UINT CMessageSender::SendOneMsg()
{    
    // ������Ϣ���ڣ��������Ӧ�ó��ָô���
    HWND hWndMsg = FindTopWindow(message.SendUserId.GetBuffer(), "StandardFrame");

    // �ҵ���������
    if (hWndMsg != NULL)
    {
        // ��Ϣ���ݴ���
        HWND hSplitterBar = ::FindWindowEx(hWndMsg, NULL, "SplitterBar", "");
        HWND hHtmlEditor = FindChildWnd(hSplitterBar, "", "HtmlEditor");
        HWND hMsgEdit = FindChildWnd(hHtmlEditor, "", "Internet Explorer_Server");

        // ���ݴ��ھ���õ�IHTMLDocument2�ӿ�
        IHTMLDocument2 *pDoc; 
        DWORD lRes; 

        UINT MSG = RegisterWindowMessage("WM_HTML_GETOBJECT"); 
        SendMessageTimeout(hMsgEdit, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes); 
        ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&pDoc); 

        if (pDoc != NULL)
        {
            // ������Ϣ����
            IHTMLElement *pBody;
            pDoc->get_body(&pBody);
            CString szMessage(message.MessageHtml);
            BSTR bstrMessage = szMessage.AllocSysString();
            pBody->put_innerHTML(bstrMessage);
            ::SysFreeString(bstrMessage);
        }

        // ��������ַ������򡰷��͡���ť�ǻ�ɫ���ɵ��
        ::PostMessage(hMsgEdit, WM_KEYDOWN , '.', 0);
        ::PostMessage(hMsgEdit, WM_KEYUP , '.', 0); 
        
        Sleep(100);

        // ������Ͱ�ť
        HWND hbtnSend = ::FindWindowEx(hWndMsg, NULL, "StandardButton", "����");
        ::PostMessage(hbtnSend, WM_LBUTTONDOWN , 0, 0);
        ::PostMessage(hbtnSend, WM_LBUTTONUP , 0, 0); 

        // ���У����
        Sleep(500);
        // У���봰��
        HWND hValidCodeWnd = FindTopWindow("�������� - ��ȫ��֤", "#32770");
        if (hValidCodeWnd)
        {
            HWND hValidCodeExp = FindChildWnd(hValidCodeWnd, "", "Internet Explorer_Server");
                        
            IHTMLDocument2 *pChkDoc; 
            DWORD lChkRes; 

            UINT MSGCHK = RegisterWindowMessage("WM_HTML_GETOBJECT"); 
            SendMessageTimeout(hValidCodeExp, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lChkRes); 
            ObjectFromLresult(lChkRes, IID_IHTMLDocument2, 0, (void**)&pChkDoc); 

            if (pChkDoc)
            {
                IHTMLElement* pChkBody;
                HRESULT re = GetDocumentBody(pChkDoc, &pChkBody);

                BSTR bstrHtml;
                pChkBody->get_innerHTML(&bstrHtml);
                CString szHtml(bstrHtml);
                CString szUrl = szHtml.Mid(29, 89);

                // ʶ��
                CString checkCode = Recognition(szUrl);
            
                HWND hValidCodeEdit = FindChildWnd(hValidCodeWnd, "", "EditComponent");
                ::SendMessage(hValidCodeEdit, WM_SETTEXT, 0, (LPARAM)checkCode.GetBuffer());

                HWND hValidCodeOK = FindChildWnd(hValidCodeWnd, "ȷ��", "StandardButton");
                ::PostMessage(hValidCodeOK, WM_LBUTTONDOWN , 0, 0);
                ::PostMessage(hValidCodeOK, WM_LBUTTONUP , 0, 0); 
            }
        }




        // �رմ���
        Sleep(400);
        ::PostMessage(hWndMsg, WM_CLOSE, 0, 0);        
    }

    return 0;
}

UINT CMessageSender::ExecuteLogin()
{
    return 0;
}

UINT CMessageSender::AddToFriend()
{
    return 0;
}

UINT CMessageSender::DeleteAllFriend()
{
    return 0;
}

BOOL CMessageSender::UserIsLogined(CString senderId)
{
    // �û��Ƿ��Ѿ���¼
    CString strWndTitle("-�������� 2008");
    strWndTitle = senderId + strWndTitle;
    HWND hMainHwnd = FindTopWindowExactly(strWndTitle.GetBuffer(), "");

    return hMainHwnd != NULL;
}