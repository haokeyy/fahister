#include "StdAfx.h"
#include "WndHelper.h"




BOOL CALLBACK CWndHelper::WindowsEnumProcBlur(HWND hwnd, LPARAM lParam)
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

HWND CWndHelper::FindTopWindowBlur(char* strTitle, char* strClassName)
{
    CWindowAttribute w;
    w.className = strClassName;
    w.title = strTitle;
    w.hwnd = NULL;
  
    ::EnumWindows(WindowsEnumProcBlur, (LPARAM)&w);

    return w.hwnd;
}

BOOL CALLBACK CWndHelper::WindowsEnumProcExactly(HWND hwnd, LPARAM lParam)
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

HWND CWndHelper::FindTopWindowExactly(char* strTitle, char* strClassName)
{
    CWindowAttribute w;
    w.className = strClassName;
    w.title = strTitle;
    w.hwnd = NULL;
  
    ::EnumWindows(WindowsEnumProcExactly, (LPARAM)&w);

    return w.hwnd;
}



BOOL CALLBACK CWndHelper::ParentWindowsEnumProcBlur(HWND hwnd, LPARAM lParam)
{
    CParentWindowAttribute *p = (CParentWindowAttribute*)lParam;
    char strTitle[1024];
    ::GetWindowText(hwnd, strTitle, 1023);
    if (strstr(strTitle, p->toptitle))
    {
        char strClass[1024];
        ::GetClassName(hwnd, strClass, 1023);
        if (strstr(strClass, p->topclassName))
        {
            HWND hChild = FindChildWindowByPoint(hwnd, p->childtitle, p->childclassName, p->childx, p->childy, p->isVisible);
            if (hChild != NULL)
            {
                p->hwnd = hwnd;
                return FALSE;
            }
        }
    }
    return TRUE;
}

HWND CWndHelper::FindTopWindowRefChildWnd(char* strTopTitle, char* strTopClassName, char* strChildTitle, char* strChildClassName, long child_x, long child_y, BOOL bVisable)
{    
    CParentWindowAttribute w;
    w.toptitle = strTopTitle;
    w.topclassName = strTopClassName;
    w.childtitle = strChildTitle;
    w.childclassName = strChildClassName;
    w.childx = child_x;
    w.childy = child_y;
    w.isVisible = bVisable;
    w.hwnd = NULL;
  
    ::EnumWindows(ParentWindowsEnumProcBlur, (LPARAM)&w);

    return w.hwnd;
}

HWND CWndHelper::FindChildWindowExactlyInternal(HWND hParentWnd, char* strText, char* strClass, BOOL bVisable)
{
    HWND hChild = ::GetWindow(hParentWnd, GW_CHILD);
    while(hChild)
    {
        char strTemp[1024];
        ::GetClassName(hChild, strTemp, 1023);
        if (strcmp(strTemp, strClass) == 0)
        {
            ::GetWindowText(hChild, strTemp, 1023);
            if (strcmp(strTemp, strText) == 0)
            {
			    if (!bVisable || ::IsWindowVisible(hChild))
			    {
				    return hChild;
			    }
            }
        }
        
        HWND hChild2 = FindChildWindowExactlyInternal(hChild, strText, strClass, bVisable);
        if(hChild2)
        {
            return hChild2;
        }

        hChild = ::GetWindow(hChild, GW_HWNDNEXT);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowExactly(HWND hParentWnd, char* strText, char* strClass, BOOL bVisable)
{
    HWND hChild = FindChildWindowExactlyInternal(hParentWnd, strText, strClass, bVisable);
    if (hChild)
    {
        return hChild;
    }

    HWND hNext = ::GetWindow(hParentWnd, GW_HWNDNEXT);
    while (hNext)
    {
        HWND hNextParent = ::GetParent(hNext);
        if (hNextParent == hParentWnd)
        {
            HWND hNextChild = FindChildWindowExactlyInternal(hNext, strText, strClass, bVisable);
            if (hNextChild)
            {
                return hNextChild;
            }
        }
        hNext = ::GetWindow(hNext, GW_HWNDNEXT);
    }

    HWND hPrev = ::GetWindow(hParentWnd, GW_HWNDPREV);
    while (hPrev)
    {
        HWND hPrevParent = ::GetParent(hPrev);
        if (hPrevParent == hParentWnd)
        {
            HWND hPrevChild = FindChildWindowExactlyInternal(hPrev, strText, strClass, bVisable);
            if (hPrevChild)
            {
                return hPrevChild;
            }
        }
        hPrev = ::GetWindow(hPrev, GW_HWNDPREV);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowBlurInternal(HWND hParentWnd, char* strText, char* strClass, BOOL bVisable)
{
    HWND hChild = ::GetWindow(hParentWnd, GW_CHILD);
    while (hChild)
    {
        char strTemp1[1024];
        char strTemp2[1024];

        ::GetWindowText(hChild, strTemp1, 1023);        
        ::GetClassName(hChild, strTemp2, 1023);

        if (strstr(strTemp1, strText) && strstr(strTemp2, strClass))
        {
			if (!bVisable || ::IsWindowVisible(hChild))
			{
				return hChild;
			}
        }

        HWND hChild2 = FindChildWindowBlurInternal(hChild, strText, strClass, bVisable);
        if(hChild2)
        {
            return hChild2;
        }

        hChild = ::GetWindow(hChild, GW_HWNDNEXT);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowBlur(HWND hParentWnd, char* strText, char* strClass, BOOL bVisable)
{
    HWND hChild = FindChildWindowBlurInternal(hParentWnd, strText, strClass, bVisable);
    if (hChild)
    {
        return hChild;
    }

    HWND hNext = ::GetWindow(hParentWnd, GW_HWNDNEXT);
    while (hNext)
    {
        HWND hNextParent = ::GetParent(hNext);
        if (hNextParent == hParentWnd)
        {
            HWND hNextChild = FindChildWindowBlurInternal(hNext, strText, strClass, bVisable);
            if (hNextChild)
            {
                return hNextChild;
            }
        }
        hNext = ::GetWindow(hNext, GW_HWNDNEXT);
    }

    HWND hPrev = ::GetWindow(hParentWnd, GW_HWNDPREV);
    while (hPrev)
    {
        HWND hPrevParent = ::GetParent(hPrev);
        if (hPrevParent == hParentWnd)
        {
            HWND hPrevChild = FindChildWindowBlurInternal(hPrev, strText, strClass, bVisable);
            if (hPrevChild)
            {
                return hPrevChild;
            }
        }
        hPrev = ::GetWindow(hPrev, GW_HWNDPREV);
    }

    return NULL;
}

HWND CWndHelper::FindChildWindowByPoint(HWND hParentWnd, char* strText, char* strClass, long x, long y, BOOL bVisable)
{
    if (x == 0 || y == 0)
    {
        return FindChildWindowBlur(hParentWnd, strText, strClass, bVisable);
    }

    // 将相对坐标转换成绝对坐标
    CRect rc;
    ::GetWindowRect(hParentWnd, &rc);
    POINT pt;
    pt.x = rc.left + x;
    pt.y = rc.top + y;

    HWND hChild = ::GetWindow(hParentWnd, GW_CHILD);

    while (hChild)
    {
        char strTemp1[1024];
        char strTemp2[1024];

        ::GetWindowText(hChild, strTemp1, 1023);        
        ::GetClassName(hChild, strTemp2, 1023);

        if (strstr(strTemp1, strText) && strstr(strTemp2, strClass))
        {
            CRect rc1;
            ::GetWindowRect(hChild, &rc1);
            if (rc1.PtInRect(pt))
            {
				if (!bVisable || ::IsWindowVisible(hChild))
				{
					return hChild;
				}
            }
        }

        // 将绝对坐标转换成为相对hChild的坐标
        RECT rc2;
        ::GetWindowRect(hChild, &rc2);
        int xx = pt.x - rc2.left;
        int yy = pt.y - rc2.top;
        
        HWND hChild2 = FindChildWindowByPoint(hChild, strText, strClass, xx, yy, bVisable);

        if(hChild2)
        {
            return hChild2;
        }

        hChild = ::GetWindow(hChild, GW_HWNDNEXT);
    }

	return NULL;
}
