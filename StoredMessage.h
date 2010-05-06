#pragma once

class CStoredMessage
{
public:
    static void AddMessage(CString strMessage);
    static int CStoredMessage::GetNextMessage(int startId, CString& szMessage);
    static void CStoredMessage::ClearMessage();
};
