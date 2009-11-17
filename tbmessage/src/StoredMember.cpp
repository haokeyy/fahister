#include "StdAfx.h"
#include "StoredMember.h"
#include ".\Libraries\WindowHelp.h"
#include ".\Libraries\Ado\Ado.h"
#include ".\Libraries\Ado\AdoCommand.h"
#include ".\Libraries\Ado\AdoRecordSet.h"

long CStoredMember::GetCount()
{
    long cnt = 0;

    CAdoConnection conn;
    if (conn.ConnectAccess(GetFilePath()))
    {
        CString szCmdText;
        szCmdText.Format("select count(*) from members");

        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            rs.GetCollect(0L, cnt);
        }

        conn.Close();
    }
    return cnt;
}

long CStoredMember::GetUnSendCount()
{
    long cnt = 0;

    CAdoConnection conn;
    if (conn.ConnectAccess(GetFilePath()))
    {
        CString szCmdText;
        szCmdText.Format("select count(*) from members where status = 0");

        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            rs.GetCollect(0L, cnt);
        }

        conn.Close();
    }
    return cnt;
}

void CStoredMember::AddMember(CString strMemberName)
{
    CAdoConnection conn;
    conn.ConnectAccess(GetFilePath());

    CString szCmdText;
    szCmdText.Format("insert into members(name, status) values('%s', 0)", strMemberName);
    conn.Execute(szCmdText);
    conn.Close();
}

long CStoredMember::GetNextUnSendedMember(CString& szMemberName)
{
    long id = -1;

    CAdoConnection conn;
    if (conn.ConnectAccess(GetFilePath()))
    {
        CString szCmdText;
        szCmdText.Format("select top 1 id, name from members where status = 0");

        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            rs.GetCollect(0L, id);
            rs.GetCollect(1L, szMemberName);
        }

        conn.Close();
    }
    return id;
}

// 返回最后一个的id
long CStoredMember::GetNextMember(long start, int count, CStringList* lpMemberlist)
{
    long lastId = 0;

    CAdoConnection conn;
    if (conn.ConnectAccess(GetFilePath()))
    {
        CString szCmdText;
        szCmdText.Format("select top %d id, name from members where id > %d", count, start);

        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            do
            {
                CString szMemberName;
                rs.GetCollect(0L, lastId);
                rs.GetCollect(1L, szMemberName);

                lpMemberlist->AddTail(szMemberName);
            }
            while (rs.MoveNext());
        }

        conn.Close();
    }
    return lastId;
}

void CStoredMember::SetMemberStatus(long id, int status)
{
    CAdoConnection conn;
    conn.ConnectAccess(GetFilePath());

    CString szCmdText;
    szCmdText.Format("update members set status = %d where id = %d", status, id);
    conn.Execute(szCmdText);
    conn.Close();
}

CString CStoredMember::GetFilePath()
{    
    CString szFileName;
    DWORD nProcID = ::GetCurrentProcessId();
    GetProcessNameByProcessID(nProcID, szFileName);

    int index = szFileName.ReverseFind('\\');
    CString mdbFilePath = szFileName.Left(index) + "\\tb.mdb";

    return mdbFilePath;
}

void CStoredMember::DeleteAllMembers()
{
    CAdoConnection conn;
    conn.ConnectAccess(GetFilePath());

    CString szCmdText;
    szCmdText.Format("delete from members");
    conn.Execute(szCmdText);
    conn.Close();
}

void CStoredMember::DeleteSendedMembers()
{
    CAdoConnection conn;
    conn.ConnectAccess(GetFilePath());

    CString szCmdText;
    szCmdText.Format("delete from members where status = 1");
    conn.Execute(szCmdText);
    conn.Close();
}
