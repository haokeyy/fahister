#pragma once

struct CWindowAttribute
{
    char* title;
    char* className;
    HWND hwnd;
};

struct CParentWindowAttribute
{
    char* toptitle;
    char* topclassName;
    char* childtitle;
    char* childclassName;
    long childx;
    long childy;
    BOOL isVisible;
    HWND hwnd;
};

class CWndHelper
{

private:

    static BOOL CALLBACK WindowsEnumProcBlur(HWND hwnd, LPARAM lParam);
    static BOOL CALLBACK WindowsEnumProcExactly(HWND hwnd, LPARAM lParam);
    static BOOL CALLBACK ParentWindowsEnumProcBlur(HWND hwnd, LPARAM lParam);
    static HWND FindChildWindowBlurInternal(HWND hParentWnd, char* strText, char* strClass, BOOL bVisable = TRUE);
    static HWND FindChildWindowExactlyInternal(HWND hWnd, char* strText, char* strClass, BOOL bVisable = TRUE);

public:
    
    // find top window: title like '%strTitle%' and className like '%strClassName%'
    static HWND FindTopWindowBlur(char* strTitle, char* strClassName);

    // find top window: title = 'strTitle' and className = 'strClassName'
    static HWND FindTopWindowExactly(char* strTitle, char* strClassName);
    
    // 参照子窗口查找主窗口: title like '%strTitle%' and className like '%strClassName%' and exists FindChildWindowByPoint(strChildTitle, strChildClassName, x, y)
    static HWND FindTopWindowRefChildWnd(char* strTopTitle, char* strTopClassName, char* strChildTitle, char* strChildClassName, long child_x = 0, long child_y = 0, BOOL bVisable = TRUE);

    static HWND FindChildWindowExactly(HWND hWnd, char* strText, char* strClass, BOOL bVisable = TRUE);

    static HWND FindChildWindowBlur(HWND hWnd, char* strText, char* strClass, BOOL bVisable = TRUE);

    static HWND FindChildWindowByPoint(HWND hWnd, char* strText, char* strClass, long x, long y, BOOL bVisable = TRUE);
};
