#pragma once

class CStoredAccount
{
public:
    static void AddAccount(CString strUserName, CString strPassword);
    static BOOL GetAccounts(CMapStringToString accounts);

};
