#include "StdAfx.h"
#include "StoredProfile.h"
#include "StoredMember.h"
#include ".\Libraries\Ado\Ado.h"
#include ".\Libraries\Ado\AdoCommand.h"
#include ".\Libraries\Ado\AdoRecordSet.h"


void CStoredProfile::AddItem(CString strkey, CString strvalue)
{    
    if (!strkey.IsEmpty())
    {
        CAdoConnection conn;
        conn.ConnectAccess(CStoredMember::GetFilePath());

        CString szCmdText;
        szCmdText.Format("insert into profiles(key, value) values('%s', '%s')", strkey, strvalue);
        conn.Execute(szCmdText);
        conn.Close();
    }
}

BOOL CStoredProfile::GetItem(CString strkey, CString& strvalue)
{
    CString szCmdText;
    szCmdText.Format("select top 1 key, value from profiles where key = '%s'", strkey);
    
    CAdoConnection conn;
    if (conn.ConnectAccess(CStoredMember::GetFilePath()))
    {
        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            rs.GetCollect(0L, strkey);
            rs.GetCollect(1L, strvalue);
        }
        else
        {
            return FALSE;
        }
        conn.Close();
    }

    return TRUE;
}

void CStoredProfile::ClearProfile()
{
    CAdoConnection conn;
    conn.ConnectAccess(CStoredMember::GetFilePath());
    
    CString szCmdText;
    szCmdText.Format("drop table profiles");
	conn.Execute(szCmdText);
    szCmdText.Format("create table profiles(key varchar(50), value varchar(50))");
    conn.Execute(szCmdText);

    conn.Close();
}
