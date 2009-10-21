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

HWND CWndHelper::FindWndInChildren(HWND hWnd, char* strKey, int nKeyType)
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


HWND CWndHelper::FindChildWnd(HWND hWnd, char* strText, char* strClass)
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

HWND CWndHelper::FindTopWindow(char* strTitle, char* strClassName)
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
    if (strcmp(strTitle, p->title) == 0 || strcmp(strClass, p->className) == 0) //ПаµИ
    {
        p->hwnd = hwnd;
        return FALSE;
    }
    return TRUE;
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
