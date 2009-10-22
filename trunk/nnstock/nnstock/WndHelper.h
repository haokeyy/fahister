#pragma once

class CWndHelper
{
public:
    CWndHelper(void);
    ~CWndHelper(void);

public:
    
    // find top window: title like '%strTitle%' and className like '%strClassName%'
    static HWND FindTopWindowBlur(char* strTitle, char* strClassName);

    // find top window: title = 'strTitle' and className = 'strClassName'
    static HWND FindTopWindowExactly(char* strTitle, char* strClassName);

    static HWND FindChildWindowExactly(HWND hWnd, char* strText, char* strClass);

    static HWND FindChildWindowBlur(HWND hWnd, char* strText, char* strClass);

    static HWND FindChildWindowByPoint(HWND hWnd, long x, long y);
};
