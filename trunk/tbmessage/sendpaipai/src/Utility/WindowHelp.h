
HWND FindWndInChildren(HWND hWnd, char* strKey, int nKeyType);

HWND FindChildWnd(HWND hWnd, char* strText, char* strClass);

HWND FindChildWndExactly(HWND hWnd, char* strText, char* strClass);

BOOL CALLBACK WindowsEnumProc(HWND hwnd, LPARAM lParam);

HWND FindTopWindow(char* strTitle, char* strClassName);

BOOL CALLBACK WindowsEnumProcExactly(HWND hwnd, LPARAM lParam);

HWND FindTopWindowExactly(char* strTitle, char* strClassName);

CString URLEncode(CString sIn);

LPWORD StringToWords(CString sIn, int& size);

BOOL GetMachineCode(CString& szMachineCode);

ULONGLONG GetMachineCodeCount();

CString GetPageDirect(CString rAddress);

CString GetPageDirect(CString szURL, CString szHeaders, CString szData, BOOL bIsPOST);

void GetProcessNameByProcessID( DWORD processID, CString& szProcessName);

CString GetFileMD5Checksum();

BOOL Base64Encode(const BYTE *pbSrcData, int nSrcLen, LPSTR szDest,	int *pnDestLen);


void ConvertUtf8ToGBK(CString& strUtf8);