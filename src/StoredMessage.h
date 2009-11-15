#pragma once

class CStoredMessage
{
public:
    static void AddMessage(CString strMessage);
    static CString GetNextMessage();
};
