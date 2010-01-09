#include "stdafx.h"
#include "WindowHelp.h"
#include <windows.h>
#include <afxinet.h>
#include "Iphlpapi.h"
#include "MD5Checksum.h"
#include "MyDiskInfo.h"
#include <TLHELP32.H>

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
    char strClass[1024];
    ::GetWindowText(hwnd, strTitle, 1023);
    ::GetClassName(hwnd, strClass, 1023);
    if (strcmp(strTitle, p->title) == 0 && strcmp(strClass, p->className) == 0) //相等
    {
        p->hwnd = hwnd;
        return FALSE;
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

// 根据网卡MAC计算得到一个唯一机器码
BOOL GetMachineCode(CString& szMachineCode)
{    
 //   /////////////////////////////////////////
 //   u_char	g_ucLocalMac[6];	// 本地MAC地址
 //   //DWORD	g_dwGatewayIP;		// 网关IP地址
 //   //DWORD	g_dwLocalIP;		// 本地IP地址
 //   //DWORD	g_dwMask;			// 子网掩码
	//PIP_ADAPTER_INFO pAdapterInfo = NULL;
	//ULONG ulLen = 0;

	//// 为适配器结构申请内存
	//::GetAdaptersInfo(pAdapterInfo,&ulLen);
	//pAdapterInfo = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR, ulLen);

	//// 取得本地适配器结构信息
	//if(::GetAdaptersInfo(pAdapterInfo,&ulLen) ==  ERROR_SUCCESS)
	//{
	//	if(pAdapterInfo != NULL)
	//	{
	//		memcpy(g_ucLocalMac, pAdapterInfo->Address, 6);
	//		//g_dwGatewayIP = ::inet_addr(pAdapterInfo->GatewayList.IpAddress.String);
	//		//g_dwLocalIP = ::inet_addr(pAdapterInfo->IpAddressList.IpAddress.String);
	//		//g_dwMask = ::inet_addr(pAdapterInfo->IpAddressList.IpMask.String);
	//	}
	//}

	////printf(" \n -------------------- 本地主机信息 -----------------------\n\n");
	////in_addr in;
	////in.S_un.S_addr = g_dwLocalIP;
	////printf("      IP Address : %s \n", ::inet_ntoa(in));

	////in.S_un.S_addr = g_dwMask;
	////printf("     Subnet Mask : %s \n", ::inet_ntoa(in));

	////in.S_un.S_addr = g_dwGatewayIP;
	////printf(" Default Gateway : %s \n", ::inet_ntoa(in));

	//u_char *p = g_ucLocalMac;
 //   int nMasterVer = (int)PRODUCT_ID%100;
 //   int nMonirVer = (int)PRODUCT_ID/100;
 //   szMachineCode.Format("%02X%02X-%02X%d-%02X%02X-%02X%d", p[1]^0xFF, p[4]^0xFF, p[5]^0xFF, nMonirVer, p[0]^0xFF, p[2]^0xFF, p[3]^0xFF, nMasterVer);
 //   szMachineCode.MakeReverse();
	
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

DWORD GetProcessIDByProcessName(CString szProcessName)
{
    DWORD dwPid = 0;
    szProcessName.MakeLower();

    PROCESSENTRY32 proc;
    proc.dwSize = sizeof(proc);
    HANDLE hSysSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSysSnapshot && Process32First (hSysSnapshot, &proc))
    {
        do
        {
            CString strProcName(proc.szExeFile);
            strProcName.MakeLower();
            
            if (strProcName.Find(szProcessName) >= 0)
            {
                dwPid = proc.th32ProcessID;
                break;
            }
        }
        while ( Process32Next (hSysSnapshot, &proc));
    }
        
    CloseHandle(hSysSnapshot);

    return dwPid;
}

void KillProcess(DWORD dwProcessID)
{
    if(dwProcessID)
    { 
        //HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
        //if(hProc)
        //{ 
        //    TerminateProcess(hProc, 99);
        //    CloseHandle(hProc);
        //}
        CString szCmdLine;
        szCmdLine.Format("ntsd -c q -p %d", dwProcessID);
        WinExec(szCmdLine, SW_HIDE);
    }
}

void RefreshTrayWnd()
{
    HWND hTaskBar = FindTopWindow("", "Shell_TrayWnd");
    HWND hToolbarWindow32 = FindChildWnd(hTaskBar, "", "ToolbarWindow32");
    CRect rc;
    ::GetClientRect(hToolbarWindow32, &rc);
    int y = (rc.top + rc.bottom) / 2;
    for (int x = rc.left; x < rc.right; x+=5)
    {
        ::SendMessage(hToolbarWindow32, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));         
    }
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

void ConvertGBKToUtf8(CString& strGBK) 
{ 
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0); 
    wchar_t * wszUtf8 = new wchar_t[len+1]; 
    memset(wszUtf8, 0, (len + 1)*sizeof(wchar_t)); 
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, len); 
    len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL); 
    char *szUtf8=new char[len + 1]; 
    memset(szUtf8, 0, len + 1); 
    WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL); 
    strGBK = szUtf8; 
    delete[] szUtf8; 
    delete[] wszUtf8; 
}