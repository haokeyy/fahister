#include "StdAfx.h"
#include "WndHelper.h"


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
    if (strcmp(strTitle, p->title) == 0 || strcmp(strClass, p->className) == 0) //相等
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


HWND CWndHelper::FindChildWindowExactly(HWND hWnd, char* strText, char* strClass, BOOL bVisable)
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
				if (!bVisable || ::IsWindowVisible(hWnd))
				{
					return hWnd;
				}
            }
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindChildWindowExactly(hChild, strText, strClass, bVisable);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowBlur(HWND hWnd, char* strText, char* strClass, BOOL bVisable)
{
    while (hWnd)
    {
        char strTemp1[1024];
        char strTemp2[1024];

        ::GetWindowText(hWnd, strTemp1, 1023);        
        ::GetClassName(hWnd, strTemp2, 1023);

        if (strstr(strTemp1, strText) && strstr(strTemp2, strClass))
        {
			if (!bVisable || ::IsWindowVisible(hWnd))
			{
				return hWnd;
			}
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            HWND hChild2 = FindChildWindowBlur(hChild, strText, strClass, bVisable);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowByPoint(HWND hWnd, char* strText, char* strClass, long x, long y, BOOL bVisable)
{
    // 将相对坐标转换成绝对坐标
    CRect rc;
    ::GetWindowRect(hWnd, &rc);
    POINT pt;
    pt.x = rc.left + x;
    pt.y = rc.top + y;


    while (hWnd)
    {
        char strTemp1[1024];
        char strTemp2[1024];

        ::GetWindowText(hWnd, strTemp1, 1023);        
        ::GetClassName(hWnd, strTemp2, 1023);

        if (strstr(strTemp1, strText) && strstr(strTemp2, strClass))
        {
            CRect rc1;
            ::GetWindowRect(hWnd, &rc1);
            if (rc1.PtInRect(pt))
            {
				if (!bVisable || ::IsWindowVisible(hWnd))
				{
					return hWnd;
				}
            }
        }

        HWND hChild = ::GetWindow(hWnd, GW_CHILD);

        if(hChild)
        {
            // 将绝对坐标转换成为相对hChild的坐标
            RECT rc2;
            ::GetWindowRect(hChild, &rc2);
            int xx = pt.x - rc2.left;
            int yy = pt.y - rc2.top;

            HWND hChild2 = FindChildWindowByPoint(hChild, strText, strClass, xx, yy, bVisable);
            if (hChild2 != NULL)
            {
                return hChild2;
            }
        }

        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }

	return NULL;
}
