#pragma once

class CInstantMessage
{
public:
    CString SendUserId;
    CString SendUserPassword;
    CString ReceiverId;
    CString MessageHtml;
};

class CMessageSender
{
public:
    CMessageSender();
    ~CMessageSender();

    static void SendMsg(CInstantMessage msg);

};
