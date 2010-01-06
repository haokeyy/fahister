#include "StdAfx.h"
#include "sendmsgthread.h"
#include "WindowHelp.h"

CSendMsgThread::CSendMsgThread()
{
}

CSendMsgThread::~CSendMsgThread()
{
}

void CSendMsgThread::Init(HWND hwnd)
{
    m_hPostWnd = hwnd;
}

void CSendMsgThread::SendMsg(CString szSenderID, CString szUserID,CString szMessage,int nMsgType)
{
    m_szSenderID = szSenderID;
    m_szUserID = szUserID;
    m_szMessage = szMessage;
    m_nMsgType = nMsgType;

    AfxBeginThread(SendMsgWorkerThread, this);
}


UINT CSendMsgThread::SendMsgWorkerThread(LPVOID pvThread)
{
    CSendMsgThread* pSendMsgThread = (CSendMsgThread*)pvThread;

    return pSendMsgThread->ExecuteSendMsg();
}

UINT CSendMsgThread::ExecuteSendMsg()
{
    CString strWndTitle("�������� - ");
    
    strWndTitle += m_szSenderID;

    SendOneMessage(strWndTitle.GetBuffer(0), m_szMessage.GetBuffer(0), m_szUserID.GetBuffer(0), m_nMsgType);
  
    BOOL bResult = TRUE;
    LPTSTR szUserID = m_szUserID.GetBuffer(m_szUserID.GetLength());

    ::PostMessage(m_hPostWnd,WM_SENDMSGCOMPLETED, (WPARAM)bResult, (LPARAM)szUserID);

    return 0;
}


void SendOneMessage(LPTSTR strWndTitle, LPTSTR strMessage, LPTSTR strUser, int msgType)
{
    HWND hWnd = FindTopWindow(strWndTitle, "");
    
    HWND hBtnWnd = FindWndInChildren(hWnd, "ָ������", 1);
    
    ::PostMessage(hBtnWnd,WM_LBUTTONDOWN,0,0);
    ::SendMessage(hBtnWnd,WM_LBUTTONDOWN,0,0);
    ::PostMessage(hBtnWnd,WM_LBUTTONUP,0,0);
    ::SendMessage(hBtnWnd,WM_LBUTTONUP,0,0);

    Sleep(1000);

    HWND hPopupWnd = ::FindWindow("#32770", "������Ϣ��ָ���û�");
    HWND hEdit = ::FindWindowEx(hPopupWnd, NULL, "Edit", "");
    if (hPopupWnd==NULL || hEdit==NULL)return;

    ::SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)strUser);

    if (msgType == 3)
    {
        HWND hCmb = ::FindWindowEx(hPopupWnd, NULL, "ComboBox", NULL);
        ::SendMessage(hCmb, CB_SETCURSEL, 1, 0);
    }

    HWND hBtn = ::FindWindowEx(hPopupWnd, NULL, "Button", "����(&S)");
    ::SendMessage(hBtn,WM_LBUTTONDOWN,0,0);
    ::SendMessage(hBtn,WM_LBUTTONUP,0,0);  

    Sleep(1200);

    // ������ͺ���ܵ������ִ���,1.��������,2.�û�������,3.������֤

    // ������Ϣ���ڣ��������Ӧ�ó��ָô���
    HWND hWndMsg = FindTopWindow(strUser, "#32770");

    // û���ҵ���������
    if (hWndMsg == NULL)
    {
        // ��֤����
        HWND hAuthWnd = FindTopWindow("��ӵ���ϵ���б�", "#32770");
        if (hAuthWnd)
        {
            HWND hAuthEdit = ::FindWindowEx(hAuthWnd, NULL, "Edit", "");
            // ��Ϣ���ݵ���֤��Ϣ��
            ::SendMessage(hAuthEdit, WM_SETTEXT, NULL, (LPARAM)strMessage);

            Sleep(500);

            // ������Ͱ�ť
            HWND hBtnSendAuth = ::FindWindowEx(hAuthWnd, NULL, "Button", "����(&S)");

            ::SendMessage(hBtnSendAuth,WM_LBUTTONDOWN,0,0);
            ::SendMessage(hBtnSendAuth,WM_LBUTTONUP,0,0);  
        }
        else // �û�������
        {   
            ::SendMessage(hPopupWnd, WM_CLOSE, 0, 0);
        }
    }
    else // �ҵ���������
    {        
        // ��Ϣ���ݴ���
        HWND hMsgEdit = ::FindWindowEx(hWndMsg, NULL, "RichEdit20A", "");

        ::SendMessage(hMsgEdit, WM_SETTEXT, NULL, (LPARAM)strMessage);
        //Sleep(300);
            
        // ����
        ::PostMessage(hWndMsg, WM_KEYDOWN , VK_RETURN, 0);
        ::PostMessage(hWndMsg, WM_KEYUP , VK_RETURN, 0); 

        // �رմ���
        Sleep(1000);
        ::SendMessage(hWndMsg, WM_CLOSE, 0, 0);
    }

}
