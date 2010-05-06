#pragma once

class CStoredAccount
{
public:
    static void AddAccount(CString strUserName, CString strSendedCnt);
    static int GetNextAccount(int id, CString& strUserName, CString& strSendedCnt);
    static void ClearAccount();

};
