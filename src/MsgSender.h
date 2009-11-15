#pragma once

class CInstantMessage
{
public:
    int nItemIndex;
    CString SendUserId;
    CString SendUserPassword;
    CString ReceiverId;
    CString MessageHtml;
    BOOL AddToFriend;
    BOOL AutoLogin;
};

class CMessageSender
{
public:
    CMessageSender(HWND hMainWnd);
    ~CMessageSender();

    HWND m_hMainWnd;
    CInstantMessage message;
    void SendMsg(CInstantMessage msg);
    UINT ExecuteSendMsg();
    UINT ExecuteLogin();
    UINT SendOneMsg();
    UINT AddToFriend();
    UINT DeleteAllFriend();
    static BOOL UserIsLogined(CString senderId);
};

UINT SendMsgWorkerThread(LPVOID pvThread);