#include "StdAfx.h"
#include "StoredAccount.h"
#include "StoredMember.h"
#include ".\Libraries\Ado\Ado.h"
#include ".\Libraries\Ado\AdoCommand.h"
#include ".\Libraries\Ado\AdoRecordSet.h"

void CStoredAccount::AddAccount(CString strUserName, CString pwd)
{    
    if (!strUserName.IsEmpty())
    {
        CAdoConnection conn;
        conn.ConnectAccess(CStoredMember::GetFilePath());

        CString szCmdText;
        szCmdText.Format("insert into accounts(uid, pwd) values('%s', '%s')", strUserName, pwd);
        conn.Execute(szCmdText);
        conn.Close();
    }
}

int CStoredAccount::GetNextAccount(int startId, CString& strUserName, CString& pwd)
{
    long id = -1;

    CString szCmdText;
    szCmdText.Format("select top 1 id, uid, pwd from accounts where id > %d", startId);
    
    CAdoConnection conn;
    if (conn.ConnectAccess(CStoredMember::GetFilePath()))
    {
        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            rs.GetCollect(0L, id);
            rs.GetCollect(1L, strUserName);
            rs.GetCollect(2L, pwd);
        }
        conn.Close();
    }

    return id;
}

void CStoredAccount::ClearAccount()
{
    CAdoConnection conn;
    conn.ConnectAccess(CStoredMember::GetFilePath());
    
    CString szCmdText;
    szCmdText.Format("drop table accounts");
	conn.Execute(szCmdText);
    szCmdText.Format("create table accounts(id autoincrement, uid varchar(50), pwd varchar(50))");
    conn.Execute(szCmdText);

    conn.Close();
}
