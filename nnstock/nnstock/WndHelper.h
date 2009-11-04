#pragma once

class CWndHelper
{
public:
    
    // find top window: title like '%strTitle%' and className like '%strClassName%'
    static HWND FindTopWindowBlur(char* strTitle, char* strClassName);

    // find top window: title = 'strTitle' and className = 'strClassName'
    static HWND FindTopWindowExactly(char* strTitle, char* strClassName);

    static HWND FindChildWindowExactly(HWND hWnd, char* strText, char* strClass, BOOL bVisable = TRUE);

    static HWND FindChildWindowBlur(HWND hWnd, char* strText, char* strClass, BOOL bVisable = TRUE);

    static HWND FindChildWindowByPoint(HWND hWnd, char* strText, char* strClass, long x, long y, BOOL bVisable = TRUE);
};
