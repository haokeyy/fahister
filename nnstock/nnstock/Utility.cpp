#include "StdAfx.h"
#include "Utility.h"
#include <afxinet.h>

int CUtility::GetSafeInt(char* s, int defaultValue)
{
	BOOL isnumber = TRUE;
	char *p = s;
	while (*p)
	{
		if (!isalnum(*p))
		{
			isnumber = FALSE;
		}
		p++;
	}
	if (!isnumber)
	{
		return defaultValue;
	}
	else
	{
		return atoi(s);
	}
}

int CUtility::SplitString(char* s, char* separator, char *substr[])
{
	int cnt = 0;
	int seplen = strlen(separator);

	char *p, *q;
	p = q = s;
	while (*p)
	{
		if (strncmp(p, separator, seplen) == 0)
		{
			*p = 0;
			if (strncmp(q, separator, seplen) != 0)
			{
				substr[cnt++] = q;
			}

			p += seplen;
			q = p;
		}
		else
		{
			p++;
		}
	}
	if (strlen(q) > 0)
	{
		substr[cnt++] = q;
	} 
	return cnt;
}


CString CUtility::GetPageDirect(CString rAddress)
{
    CString szResult;
    DWORD dwRet = 0; // HTTP·µ»ØÂë

    CString strServerName, strObject;
    DWORD dwSvrType;
    INTERNET_PORT nPort;
    const TCHAR szHeaders[] = _T("Accept: text/*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;\r\n");

    AfxParseURL(rAddress, dwSvrType, strServerName, strObject, nPort);

    CInternetSession session("CInternetThread");
    CHttpConnection* pServer = NULL;
    CHttpFile* pFile = NULL;
    try
    {        
        pServer = session.GetHttpConnection(strServerName, nPort);
        pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
        pFile->AddRequestHeaders(szHeaders);
        pFile->SendRequest();
        pFile->QueryInfoStatusCode(dwRet);

        if (dwRet < 400)
        {
            char szBuff[1024];
            UINT nRead = pFile->Read(szBuff, 1023);
            while (nRead > 0)
            {
                szBuff[nRead] = '\0';
                szResult.Append(szBuff);
                nRead = pFile->Read(szBuff, 1023);
            }
        }
        delete pFile;
        delete pServer;
    }
    catch (CInternetException* pEx)
    {
        //uiResult = 0;
    }
    session.Close();

    return szResult;
}