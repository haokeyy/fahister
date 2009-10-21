#pragma once

class CWndHelper
{
public:
    CWndHelper(void);
    ~CWndHelper(void);

public:
    
    HWND FindTopWindow(char* strTitle, char* strClassName);

    HWND FindTopWindowExactly(char* strTitle, char* strClassName);

    HWND FindChildWnd(HWND hWnd, char* strText, char* strClass);

    HWND FindWndInChildren(HWND hWnd, char* strKey, int nKeyType);
};
