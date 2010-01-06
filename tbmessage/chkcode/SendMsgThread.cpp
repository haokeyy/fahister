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
    CString strWndTitle("阿里旺旺 - ");
    
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
    
    HWND hBtnWnd = FindWndInChildren(hWnd, "指定发送", 1);
    
    ::PostMessage(hBtnWnd,WM_LBUTTONDOWN,0,0);
    ::SendMessage(hBtnWnd,WM_LBUTTONDOWN,0,0);
    ::PostMessage(hBtnWnd,WM_LBUTTONUP,0,0);
    ::SendMessage(hBtnWnd,WM_LBUTTONUP,0,0);

    Sleep(1000);

    HWND hPopupWnd = ::FindWindow("#32770", "发送消息给指定用户");
    HWND hEdit = ::FindWindowEx(hPopupWnd, NULL, "Edit", "");
    if (hPopupWnd==NULL || hEdit==NULL)return;

    ::SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)strUser);

    if (msgType == 3)
    {
        HWND hCmb = ::FindWindowEx(hPopupWnd, NULL, "ComboBox", NULL);
        ::SendMessage(hCmb, CB_SETCURSEL, 1, 0);
    }

    HWND hBtn = ::FindWindowEx(hPopupWnd, NULL, "Button", "发送(&S)");
    ::SendMessage(hBtn,WM_LBUTTONDOWN,0,0);
    ::SendMessage(hBtn,WM_LBUTTONUP,0,0);  

    Sleep(1200);

    // 点击发送后可能弹出三种窗口,1.正常窗口,2.用户不存在,3.请求验证

    // 发送消息窗口，正常情况应该出现该窗口
    HWND hWndMsg = FindTopWindow(strUser, "#32770");

    // 没有找到正常窗口
    if (hWndMsg == NULL)
    {
        // 验证窗口
        HWND hAuthWnd = FindTopWindow("添加到联系人列表", "#32770");
        if (hAuthWnd)
        {
            HWND hAuthEdit = ::FindWindowEx(hAuthWnd, NULL, "Edit", "");
            // 消息内容到认证信息框
            ::SendMessage(hAuthEdit, WM_SETTEXT, NULL, (LPARAM)strMessage);

            Sleep(500);

            // 点击发送按钮
            HWND hBtnSendAuth = ::FindWindowEx(hAuthWnd, NULL, "Button", "发送(&S)");

            ::SendMessage(hBtnSendAuth,WM_LBUTTONDOWN,0,0);
            ::SendMessage(hBtnSendAuth,WM_LBUTTONUP,0,0);  
        }
        else // 用户不存在
        {   
            ::SendMessage(hPopupWnd, WM_CLOSE, 0, 0);
        }
    }
    else // 找到正常窗口
    {        
        // 消息内容窗口
        HWND hMsgEdit = ::FindWindowEx(hWndMsg, NULL, "RichEdit20A", "");

        ::SendMessage(hMsgEdit, WM_SETTEXT, NULL, (LPARAM)strMessage);
        //Sleep(300);
            
        // 发送
        ::PostMessage(hWndMsg, WM_KEYDOWN , VK_RETURN, 0);
        ::PostMessage(hWndMsg, WM_KEYUP , VK_RETURN, 0); 

        // 关闭窗口
        Sleep(1000);
        ::SendMessage(hWndMsg, WM_CLOSE, 0, 0);
    }

}
