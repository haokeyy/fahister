#pragma once

//
// Message to post when thread is done.
//


class CSendMsgThread
{
public:
    CSendMsgThread();
    ~CSendMsgThread();

    void Init(HWND hwnd);

    void SendMsg(CString szSenderID, CString szUserID,CString szMessage,int nMsgType);
private:
    CString m_szSenderID;
    CString m_szUserID;
    CString m_szMessage;
    int m_nMsgType;
    HWND m_hPostWnd;

    UINT ExecuteSendMsg();
    static UINT SendMsgWorkerThread(LPVOID pvThread);
};

UINT SendMsgWorkerThread(LPVOID pvThread);
void SendOneMessage(LPTSTR strWndTitle, LPTSTR strMessage, LPTSTR strUser, int msgType);