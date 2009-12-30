
HWND FindWndInChildren(HWND hWnd, char* strKey, int nKeyType);

BOOL CALLBACK WindowsEnumProc(HWND hwnd, LPARAM lParam);

HWND FindTopWindow(char* strTitle, char* strClassName);

CString URLEncode(CString sIn);

BOOL GetMachineCode(CString& szMachineCode);

ULONGLONG GetMachineCodeCount();

CString GetPageDirect(CString rAddress);

void GetProcessNameByProcessID( DWORD processID, CString& szProcessName);

CString GetFileMD5Checksum();