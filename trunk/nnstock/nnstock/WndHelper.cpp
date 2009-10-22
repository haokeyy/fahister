#include "StdAfx.h"
#include "WndHelper.h"

CWndHelper::CWndHelper(void)
{
}

CWndHelper::~CWndHelper(void)
{
}


struct CWindowAttribute
{
    char* title;
    char* className;
    HWND hwnd;
};

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


BOOL CALLBACK WindowsEnumProcExactly(HWND hwnd, LPARAM lParam)
{
    CWindowAttribute *p = (CWindowAttribute*)lParam;
    char strTitle[1024];
    char strClass[1024];
    ::GetWindowText(hwnd, strTitle, 1023);
    ::GetClassName(hwnd, strClass, 1023);
    if (strcmp(strTitle, p->title) == 0 || strcmp(strClass, p->className) == 0) //ПаµИ
    {
        p->hwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}


HWND CWndHelper::FindTopWindowBlur(char* strTitle, char* strClassName)
{
    CWindowAttribute w;
    w.className = strClassName;
    w.title = strTitle;
    w.hwnd = NULL;
  
    ::EnumWindows(WindowsEnumProcBlur, (LPARAM)&w);

    return w.hwnd;
}

HWND CWndHelper::FindTopWindowExactly(char* strTitle, char* strClassName)
{
    CWindowAttribute w;
    w.className = strClassName;
    w.title = strTitle;
    w.hwnd = NULL;
  
    ::EnumWindows(WindowsEnumProcExactly, (LPARAM)&w);

    return w.hwnd;
}


HWND CWndHelper::FindChildWindowExactly(HWND hWnd, char* strText, char* strClass)
{
    while (hWnd)
    {
        char strTemp[1024];
        ::GetClassName(hWnd, strTemp, 1023);
        if (strcmp(strTemp, strClass) == 0)
        {
            ::GetWindowText(hWnd, strTemp, 1023);
            if (strcmp(strTemp, strText) == 0)
            {
                return hWnd;
            }
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindChildWindowExactly(hChild, strText, strClass);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowBlur(HWND hWnd, char* strText, char* strClass)
{
    while (hWnd)
    {
        char strTemp1[1024];
        char strTemp2[1024];

        ::GetWindowText(hWnd, strTemp1, 1023);        
        ::GetClassName(hWnd, strTemp2, 1023);

        if (strstr(strTemp1, strText) && strstr(strTemp2, strClass))
        {
            return hWnd;
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindChildWindowBlur(hChild, strText, strClass);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowByPoint(HWND hWnd, long x, long y)
{
    if (hWnd)
    {
        POINT pt;
        pt.x = x;
        pt.y = y;
        HWND hChild = ChildWindowFromPointEx(hWnd, pt, CWP_ALL);

        if (hChild == hWnd) // find not
        {
            return NULL;
        }
        else
        {
            return hChild;
        }
    }

    return NULL;
}
