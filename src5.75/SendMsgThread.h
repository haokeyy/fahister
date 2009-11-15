#pragma once


class CSendMsgThread
{
public:
    CSendMsgThread();
    ~CSendMsgThread();

    void Init(HWND hwnd, BOOL bAddFriend);

    void SendMsg(CString szSenderID, CString szUserID, CString szMessage, CString szMessage2, int nMsgType);
    void DeleteFriend(LPTSTR szSenderID);
private:
    CString m_szSenderID;
    CString m_szUserID;
    CString m_szMessage;
    CString m_szMessage2;
    int m_nMsgType;
    HWND m_hPostWnd;
    BOOL m_bAddFriend;

    UINT ExecuteSendMsg();
    UINT ExecuteLogin(CString uid, CString pwd);
    int AddToFriend(LPTSTR strWndTitle, LPTSTR strUser);
    void SendOneMessage(LPTSTR strWndTitle, LPTSTR strMessage, LPTSTR strMessage2, LPTSTR strUser, int msgType);
    static UINT SendMsgWorkerThread(LPVOID pvThread);
    static UINT DeleteFriendThread(LPVOID pvThread);
};

UINT DeleteFriendThread(LPVOID pvThread);
UINT SendMsgWorkerThread(LPVOID pvThread);
void DeleteAllFriend(CString szSender);
int GetFriendCount(CString strWndTitle);
