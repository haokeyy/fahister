#pragma once


class TaobaoMember
{
public:
	long Id;
	CString MemberName;
	int Status;
};
class CStoredMember
{
public:
    static long GetCount();
    static long GetUnSendCount();
    static void AddMember(CString strMemberName);
    static long GetNextUnSendedMember(CString& szMemberName);
    static long GetNextMember(long start, int count, CList<TaobaoMember>* lpMemberList);
    static void SetMemberStatus(long id, int status);
    static CString GetFilePath();
    static void DeleteAllMembers();
    static void DeleteSendedMembers();
};
