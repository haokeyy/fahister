#include "StdAfx.h"
#include "StoredMember.h"
#include "StoredMessage.h"
#include ".\Libraries\Ado\Ado.h"
#include ".\Libraries\Ado\AdoCommand.h"
#include ".\Libraries\Ado\AdoRecordSet.h"

void CStoredMessage::AddMessage(CString strMessage)
{
    if (!strMessage.IsEmpty())
    {
        CAdoConnection conn;
        conn.ConnectAccess(CStoredMember::GetFilePath());

        CString szCmdText;
        szCmdText.Format("insert into messages(message) values('%s')", strMessage);
        conn.Execute(szCmdText);
        conn.Close();
    }
}

void CStoredMessage::ClearMessage()
{
    CAdoConnection conn;
    conn.ConnectAccess(CStoredMember::GetFilePath());
    
    CString szCmdText;
    szCmdText.Format("drop table messages");
	conn.Execute(szCmdText);
    szCmdText.Format("create table messages(id autoincrement, message Memo)");
    conn.Execute(szCmdText);

    conn.Close();
}

int CStoredMessage::GetNextMessage(int startId, CString& szMessage)
{
    long id = -1;

    CString szCmdText;
    szCmdText.Format("select top 1 id, message from messages where id > %d", startId);
    
    CAdoConnection conn;
    if (conn.ConnectAccess(CStoredMember::GetFilePath()))
    {
        CAdoRecordSet rs(&conn);
        if (rs.Open(szCmdText) && rs.MoveFirst())
        {
            rs.GetCollect(0L, id);
            rs.GetCollect(1L, szMessage);
        }
        conn.Close();
    }

    return id;
}
