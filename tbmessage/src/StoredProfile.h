#pragma once

class CStoredProfile
{
public:
    static void AddItem(CString strkey, CString strvalue);
    static BOOL GetItem(CString strkey, CString& strvalue);
    static void ClearProfile();

};
