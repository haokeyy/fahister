#pragma once

class CStoredMember
{
public:
    static long GetCount();
    static long GetUnSendCount();
    static void AddMember(CString strMemberName);
    static long GetNextUnSendedMember(CString& szMemberName);
    static long GetNextMember(long start, int count, CStringList* lpMemberList);
    static void SetMemberStatus(long id, int status);
    static CString GetFilePath();
    static void DeleteAllMembers();
    static void DeleteSendedMembers();
};
