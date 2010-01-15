#include "stdafx.h"
#include "WindowHelp.h"
#include <windows.h>
#include <afxinet.h>
#include "Iphlpapi.h"
#include "MD5Checksum.h"
#include "MyDiskInfo.h"

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")



struct CWindowAttribute
{
    char* title;
    char* className;
    HWND hwnd;
};

HWND FindWndInChildren(HWND hWnd, char* strKey, int nKeyType)
{
    while (hWnd)
    {
        char strTemp[1024];
        if (nKeyType == 1)
        {
            ::GetWindowText(hWnd, strTemp, 1023);
        }
        else
        {
            ::GetClassName(hWnd, strTemp, 1023);
        }

        if (strcmp(strTemp, strKey) == 0)
        {
            return hWnd;
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindWndInChildren(hChild, strKey, nKeyType);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}


HWND FindChildWnd(HWND hWnd, char* strText, char* strClass)
{
    while (hWnd)
    {
        char strTemp[1024];
        ::GetWindowText(hWnd, strTemp, 1023);
        if (strstr(strTemp, strText))
        {
            ::GetClassName(hWnd, strTemp, 1023);
            if (strstr(strTemp, strClass))
            {
                return hWnd;
            }
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindChildWnd(hChild, strText, strClass);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}



HWND FindChildWndExactly(HWND hWnd, char* strText, char* strClass)
{
    while (hWnd)
    {
        char strTemp[1024];
        ::GetWindowText(hWnd, strTemp, 1023);
        if (strcmp(strTemp, strText) == 0)
        {
            ::GetClassName(hWnd, strTemp, 1023);
            if (strcmp(strTemp, strClass) == 0)
            {
                return hWnd;
            }
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindChildWnd(hChild, strText, strClass);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}


BOOL CALLBACK WindowsEnumProcBlur(HWND hwnd, LPARAM lParam)
{
    CWindowAttribute *p = (CWindowAttribute*)lParam;
    char strTitle[1024];
    ::GetWindowText(hwnd, strTitle, 1023);
    if (strstr(strTitle, p->title))
    {
        char strClass[1024];
        ::GetClassName(hwnd, strClass, 1023);
        if (strstr(strClass, p->className))
        {
            p->hwnd = hwnd;
            return FALSE;
        }
    }
    return TRUE;
}

HWND FindTopWindow(char* strTitle, char* strClassName)
{
    CWindowAttribute w;
    w.className = strClassName;
    w.title = strTitle;
    w.hwnd = NULL;
  
    ::EnumWindows(WindowsEnumProcBlur, (LPARAM)&w);

    return w.hwnd;
}


BOOL CALLBACK WindowsEnumProcExactly(HWND hwnd, LPARAM lParam)
{
    CWindowAttribute *p = (CWindowAttribute*)lParam;
    char strTitle[1024];
    ::GetWindowText(hwnd, strTitle, 1023);
    if (strcmp(strTitle, p->title) != 0) //不相等
    {
        char strClass[1024];
        ::GetClassName(hwnd, strClass, 1023);
        if (strcmp(strClass, p->className) != 0)
        {
            p->hwnd = hwnd;
            return FALSE;
        }
    }
    return TRUE;
}

HWND FindTopWindowExactly(char* strTitle, char* strClassName)
{
    CWindowAttribute w;
    w.className = strClassName;
    w.title = strTitle;
    w.hwnd = NULL;
  
    ::EnumWindows(WindowsEnumProcExactly, (LPARAM)&w);

    return w.hwnd;
}

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

CString URLEncode(CString sIn)
{
    CString sOut;
	
    const int nLen = sIn.GetLength() + 1;

    register LPBYTE pOutTmp = NULL;
    LPBYTE pOutBuf = NULL;
    register LPBYTE pInTmp = NULL;
    LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
    BYTE b = 0;
	
    //alloc out buffer
    pOutBuf = (LPBYTE)sOut.GetBuffer(nLen  * 3 - 2);//new BYTE [nLen  * 3];

    if(pOutBuf)
    {
        pInTmp	= pInBuf;
	pOutTmp = pOutBuf;
		
	// do encoding
	while (*pInTmp)
	{
	    if(isalnum(*pInTmp))
	        *pOutTmp++ = *pInTmp;
	    else
	        if(isspace(*pInTmp))
		    *pOutTmp++ = '+';
		else
		{
		    *pOutTmp++ = '%';
		    *pOutTmp++ = toHex(*pInTmp>>4);
		    *pOutTmp++ = toHex(*pInTmp%16);
		}
	    pInTmp++;
	}
	*pOutTmp = '\0';
	//sOut=pOutBuf;
	//delete [] pOutBuf;
	sOut.ReleaseBuffer();
    }
    sIn.ReleaseBuffer();
    return sOut;
}

LPWORD StringToWords(CString sIn, int& size)
{
    int len = sIn.GetLength() + 1;
    LPWORD lpOut = new WORD[len];
    memset(lpOut, 0, len*sizeof(WORD));

    size = 0;
    LPCTSTR ip = sIn;
    LPWORD op = lpOut;
    while (*ip)
    {
        if (*ip < 255 && *ip > 0)
        {
            *op = *ip;
        }
        else
        {
            char a = *ip;
            ip++;
            char b = *ip;
            *op = MAKEWORD(b, a);
        }
        op++;
        ip++;
        size++;
    }
    return lpOut;
}

// 根据网卡MAC计算得到一个唯一机器码
BOOL GetMachineCode(CString& szMachineCode)
{    
    char strSerial[512];
    memset(strSerial, 0, 512);
    for (int i = 0; i < 4; i++)
    {
        CMyDiskInfo myDisk;
        if (!myDisk.GetDiskInfo(i))
        {
            strcat(strSerial, myDisk.szModelNumber);
            strcat(strSerial, myDisk.szSerialNumber);
        }
    }

    BYTE lpMd5Buf[17];
    char lpBuf[25];
    memset(lpMd5Buf, 0, 17);
    memset(lpBuf, 0, 25);

    CMD5Checksum::GetMD5(lpMd5Buf, (BYTE*)strSerial, strlen(strSerial));
    int nLen;
    Base64Encode(lpMd5Buf, 16, lpBuf, &nLen);

    szMachineCode.Format("%s%X", lpBuf, PRODUCT_ID);

	return TRUE;
}

CString GetPageDirect(CString rAddress)
{
    CString szResult;
    DWORD dwRet = 0; // HTTP返回码

    CString strServerName, strObject;
    DWORD dwSvrType;
    INTERNET_PORT nPort;
    const TCHAR szHeaders[] = _T("Accept: text/*\r\nUser-Agent: CInternetThread\r\n");

    AfxParseURL(rAddress, dwSvrType, strServerName, strObject, nPort);

    CInternetSession session("MySessionDirect");
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


CString GetPageDirect(CString szURL, CString szHeaders, CString szData, BOOL bIsPOST)
{
    CString szResult;

    DWORD dwRet = 0; // HTTP返回码

    DWORD dwSvrType;
    CString strServerName, strObject;
    INTERNET_PORT nPort;
    
    AfxParseURL(szURL, dwSvrType, strServerName, strObject, nPort);

    CInternetSession session(CString("MySessionDirect"));
    CHttpConnection* pServer = NULL;
    CHttpFile* pFile = NULL;
    try
    {
        pServer = session.GetHttpConnection(strServerName, nPort);
        pFile = pServer->OpenRequest(!bIsPOST, strObject);
        pFile->SendRequest(szHeaders, (LPVOID)(LPCTSTR)szData, szData.GetLength());

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

void GetProcessNameByProcessID( DWORD processID, CString& szProcessName)
{
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );

    // Get the process name.
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;
        char str[MAX_PATH] = "unknown";

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
            GetModuleFileNameEx( hProcess, hMod, str, sizeof(str) );
            szProcessName.Format("%s", str);
        }
    }
    else 
        return;

    CloseHandle( hProcess );

    //return szProcessName;
}

CString GetFileMD5Checksum()
{
    CString szFileName;
    DWORD nProcID = ::GetCurrentProcessId();

    GetProcessNameByProcessID(nProcID, szFileName);

    CString szChecksum = CMD5Checksum::GetMD5(szFileName);

    return szChecksum;
}

BOOL Base64Encode(const BYTE *pbSrcData,int nSrcLen,LPSTR szDest,int *pnDestLen)
{
	static const char s_chBase64EncodingTable[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',	'h',
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
        'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_' };

	if (!pbSrcData || !szDest || !pnDestLen)
	{
		return FALSE;
	}

	int nWritten( 0 );
	int nLen1( (nSrcLen/3)*4 );
	int nLen2( nLen1/76 );
	int nLen3( 19 );

	for (int i=0; i<=nLen2; i++)
	{
		if (i==nLen2)
			nLen3 = (nLen1%76)/4;

		for (int j=0; j<nLen3; j++)
		{
			DWORD dwCurr(0);
			for (int n=0; n<3; n++)
			{
				dwCurr |= *pbSrcData++;
				dwCurr <<= 8;
			}
			for (int k=0; k<4; k++)
			{
				BYTE b = (BYTE)(dwCurr>>26);
				*szDest++ = s_chBase64EncodingTable[b];
				dwCurr <<= 6;
			}
		}
		nWritten+= nLen3*4;

		*szDest++ = '\r';
		*szDest++ = '\n';
		nWritten+= 2;
	}

	if (nWritten)
	{
		szDest-= 2;
		nWritten -= 2;
	}

	nLen2 = nSrcLen%3 ? nSrcLen%3 + 1 : 0;
	if (nLen2)
	{
		DWORD dwCurr(0);
		for (int n=0; n<3; n++)
		{
			if (n<(nSrcLen%3))
				dwCurr |= *pbSrcData++;
			dwCurr <<= 8;
		}
		for (int k=0; k<nLen2; k++)
		{
			BYTE b = (BYTE)(dwCurr>>26);
			*szDest++ = s_chBase64EncodingTable[b];
			dwCurr <<= 6;
		}
		nWritten+= nLen2;
		
        //nLen3 = nLen2 ? 4-nLen2 : 0;
		//for (int j=0; j<nLen3; j++)
		//{
		//	*szDest++ = '=';
		//}
		//nWritten+= nLen3;
	}

	*pnDestLen = nWritten;
	return TRUE;
}

void ConvertUtf8ToGBK(CString& strUtf8) 
{ 
    int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
    wchar_t *wszGBK = new wchar_t[len+1]; 
    memset(wszGBK, 0, (len + 1)*sizeof(wchar_t)); 
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len); 
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 
    char *szGBK=new char[len + 1]; 
    memset(szGBK, 0, len + 1); 
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL); 
    strUtf8 = szGBK; 
    delete[] szGBK; 
    delete[] wszGBK; 
}