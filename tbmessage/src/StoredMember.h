#pragma once

class CStoredMember
{
public:
    static long GetCount();
    static long GetUnSendCount();
    static void AddMember(CString strMemberName);
    static long GetNextUnSenderMember(CString& szMemberName);
    static void SetMemberStatus(long id, int status);
    static CString GetFilePath();
    static void DeleteAllMembers();
    static void DeleteSendedMembers();
};
