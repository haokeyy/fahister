// HLink.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchMessage.h"
#include "HLink.h"

#ifdef _DEBUG

#define new DEBUG_NEW

#undef THIS_FILE

static char THIS_FILE[] = __FILE__;

#endif

/////////////////////////////////////////////////////////////////////////////

// CHLink

CHLink::CHLink()
{
    m_cNormalColor=RGB(0,0,255);

    m_cPointingColor=RGB(0,125,155);

    m_cVisitedColor=RGB(85,26,139);

    m_bNormal=TRUE;

    m_bVisited=FALSE;

    m_bPointing=FALSE;

    m_Title=FALSE;

    m_tip=FALSE;

    m_bUnderLine=TRUE;

    m_bAdjustFit=TRUE;

}

CHLink::~CHLink()
{
    m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHLink, CStatic)

    //{{AFX_MSG_MAP(CHLink)

    ON_WM_MOUSEMOVE()

    ON_WM_CTLCOLOR_REFLECT()

    ON_WM_LBUTTONDOWN()

    ON_WM_SETCURSOR()

    //}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// CHLink message handlers

void CHLink::SetURL(CString strURL)
{
    m_strURL=strURL;

    if(!m_Title)
    {
        SetWindowText(m_strURL);
    }

    AutoSizeWindow();

    if(!m_tip)
    {
        m_ToolTip.UpdateTipText(strURL, this, 1);
    }

    Invalidate();
}

void CHLink::PreSubclassWindow()
{
    DWORD dwStyle = GetStyle();

    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    m_cursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    LOGFONT lf;

    GetFont()->GetLogFont(&lf);

    lf.lfUnderline = m_bUnderLine;

    m_Font.CreateFontIndirect(&lf);

    SetFont(&m_Font);
    CRect rect;

    GetClientRect(rect);

    m_ToolTip.Create(this);

    m_ToolTip.AddTool(this, m_strURL, rect, 1);
    SetLinkCur();
    CStatic::PreSubclassWindow();
}

HBRUSH CHLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // TODO: Change any attributes of the DC here

    if(m_bPointing)
    {
        pDC->SetTextColor(m_cPointingColor);
    }
    else
    {
        if(m_bVisited)
        {
            pDC->SetTextColor(m_cVisitedColor);
        }
        else
        {
            if(m_bNormal)
            {
                pDC->SetTextColor(m_cNormalColor);
            }
        }
    }

    // TODO: Return a different brush if the default is not desired

    pDC->SetBkMode(TRANSPARENT);

    return (HBRUSH)GetStockObject(NULL_BRUSH);

}

void CHLink::SetPointingColor(COLORREF PointingColor)
{
    m_cPointingColor=PointingColor;
}

void CHLink::SetVisitedColor(COLORREF VisitedColor)
{
    m_cVisitedColor=VisitedColor;
}

void CHLink::SetNormalColor(COLORREF NormalColor)
{
    m_cNormalColor=NormalColor;
}

void CHLink::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CStatic::OnMouseMove(nFlags, point);

    CRect rect;

    GetClientRect(rect);

    if (!rect.PtInRect(point))
    {
        m_bPointing = FALSE;

        ReleaseCapture();
    }

    else // Cursor has just moved over control
    {
        m_bPointing = TRUE;

        SetLinkCur();

        SetCapture();
    }

    Invalidate();
}

void CHLink::AutoSizeWindow()
{
    if(m_bAdjustFit)
    {
        CRect rect;

        GetWindowRect(rect);

        CWnd *pParent=GetParent();

        if(pParent)
        {
            pParent->ScreenToClient(rect);
        }

        CString strText;

        GetWindowText(strText);

        CDC* pDC=GetDC();

        CFont* oldFont=pDC->SelectObject(&m_Font);

        CSize strExtent=pDC->GetTextExtent(strText);

        pDC->SelectObject(oldFont);

        ReleaseDC(pDC);

        DWORD dwStyle = GetStyle();

        if (dwStyle & SS_CENTERIMAGE)
        {
            rect.DeflateRect(0, (rect.Height() - strExtent.cy)/2);
        }
        else
        {
            rect.bottom = rect.top + strExtent.cy;
        }
        if (dwStyle & SS_CENTER)
        {
            rect.DeflateRect((rect.Width() - strExtent.cx)/2, 0);
        }
        else
        {
            if (dwStyle & SS_RIGHT)
            {
                rect.left = rect.right - strExtent.cx;
            }
            else
            {
                rect.right = rect.left + strExtent.cx;
            }
        }
        SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
    }
}

void CHLink::OnLButtonDown(UINT nFlags, CPoint point)
{

    // TODO: Add your message handler code here and/or call default

    int result = (int)GotoURL(m_strURL, SW_SHOW);

    m_bVisited = (result > HINSTANCE_ERROR);

    Invalidate();

    CStatic::OnLButtonDown(nFlags, point);
}

BOOL CHLink::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    m_ToolTip.RelayEvent(pMsg);

    return CStatic::PreTranslateMessage(pMsg);
}

void CHLink::SetLinkCur()
{
    //::LoadCursor(
    m_cursor= AfxGetApp()->LoadCursor(IDC_CURSOR1); // >LoadCursor();
}

BOOL CHLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default

    ::SetCursor(m_cursor);

    return TRUE;

    //return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

HINSTANCE CHLink::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    if ((UINT)result <= HINSTANCE_ERROR) 
    {
        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
        {
            lstrcat(key, _T("\shell\open\command"));
            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS)
            {

                TCHAR *pos;

                pos = _tcsstr(key, _T("%1"));

                if (pos == NULL)
                {
                    pos = strstr(key, _T("%1"));

                    if (pos == NULL)
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '0';
                }
                else
                    *pos = '0'; // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);
                result = (HINSTANCE) WinExec(key,showcmd);
            }
        }
    }
    return result;
}

LONG CHLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{

    HKEY hkey;

    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS)
    {
        long datasize = MAX_PATH;

        TCHAR data[MAX_PATH];

        RegQueryValue(hkey, NULL, data, &datasize);

        lstrcpy(retdata,data);

        RegCloseKey(hkey);
    }
    return retval;
}

void CHLink::SetWindowTitle(CString tStr)
{
    m_Title=TRUE;

    SetWindowText(tStr);

    Invalidate();
}

void CHLink::SetTipTitle(CString tipStr)
{
    m_tip=TRUE;
    m_ToolTip.UpdateTipText(tipStr, this, 1);
    Invalidate();
}

void CHLink::SetUnderline(BOOL Underline)
{
    m_bUnderLine = Underline;

    if (::IsWindow(GetSafeHwnd()))
    {
        LOGFONT lf;
        GetFont()->GetLogFont(&lf);
        lf.lfUnderline = m_bUnderLine;
        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&lf);
        SetFont(&m_Font);
        Invalidate();
    }
}
