#include "stdafx.h"
#include "WindowHelp.h"
#include <windows.h>
#include <afxinet.h>
#include "Iphlpapi.h"
#include "MD5Checksum.h"

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "psapi.lib")



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

BOOL CALLBACK WindowsEnumProc(HWND hwnd, LPARAM lParam)
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
  
    ::EnumWindows(WindowsEnumProc, (LPARAM)&w);

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

// 根据网卡MAC计算得到一个唯一机器码
BOOL GetMachineCode(CString& szMachineCode)
{    
    /////////////////////////////////////////
    u_char	g_ucLocalMac[6];	// 本地MAC地址
    //DWORD	g_dwGatewayIP;		// 网关IP地址
    //DWORD	g_dwLocalIP;		// 本地IP地址
    //DWORD	g_dwMask;			// 子网掩码
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	ULONG ulLen = 0;

	// 为适配器结构申请内存
	::GetAdaptersInfo(pAdapterInfo,&ulLen);
	pAdapterInfo = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR, ulLen);

	// 取得本地适配器结构信息
	if(::GetAdaptersInfo(pAdapterInfo,&ulLen) ==  ERROR_SUCCESS)
	{
		if(pAdapterInfo != NULL)
		{
			memcpy(g_ucLocalMac, pAdapterInfo->Address, 6);
			//g_dwGatewayIP = ::inet_addr(pAdapterInfo->GatewayList.IpAddress.String);
			//g_dwLocalIP = ::inet_addr(pAdapterInfo->IpAddressList.IpAddress.String);
			//g_dwMask = ::inet_addr(pAdapterInfo->IpAddressList.IpMask.String);
		}
	}

	//printf(" \n -------------------- 本地主机信息 -----------------------\n\n");
	//in_addr in;
	//in.S_un.S_addr = g_dwLocalIP;
	//printf("      IP Address : %s \n", ::inet_ntoa(in));

	//in.S_un.S_addr = g_dwMask;
	//printf("     Subnet Mask : %s \n", ::inet_ntoa(in));

	//in.S_un.S_addr = g_dwGatewayIP;
	//printf(" Default Gateway : %s \n", ::inet_ntoa(in));

	u_char *p = g_ucLocalMac;
    int nMasterVer = (int)PRODUCT_ID%100;
    int nMonirVer = (int)PRODUCT_ID/100;
    szMachineCode.Format("%02X%02X-%02X%d-%02X%02X-%02X%d", p[1]^0xFF, p[4]^0xFF, p[5]^0xFF, nMonirVer, p[0]^0xFF, p[2]^0xFF, p[3]^0xFF, nMasterVer);
    szMachineCode.MakeReverse();
	
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