#pragma once


// HLink.h : header file

//

/////////////////////////////////////////////////////////////////////////////

// CHLink window

class CHLink : public CStatic
{

    // Construction

public:

    void SetUnderline(BOOL Underline);

    void SetTipTitle(CString tipStr);

    void SetWindowTitle(CString tStr);

    LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);

    HINSTANCE GotoURL(LPCTSTR url, int showcmd);

    void SetLinkCur();

    void SetPointingColor(COLORREF PointingColor);

    void SetVisitedColor(COLORREF VisitedColor);

    void SetNormalColor(COLORREF NormalColor);

    void SetURL(CString strURL);

    CHLink();

private:

    void AutoSizeWindow();

    CString m_strURL;

    COLORREF m_cPointingColor;

    COLORREF m_cVisitedColor;

    COLORREF m_cNormalColor;

    BOOL m_bVisited;

    BOOL m_bPointing;

    BOOL m_bNormal;

    BOOL m_bUnderLine;

    BOOL m_bAdjustFit;

    BOOL m_Title;

    BOOL m_tip;

    CFont m_Font;

    CToolTipCtrl m_ToolTip;

    HCURSOR m_cursor;

    // Attributes

public:

    // Operations

public:

    // Overrides

    // ClassWizard generated virtual function overrides

    //{{AFX_VIRTUAL(CHLink)

public:

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:

    virtual void PreSubclassWindow();

    //}}AFX_VIRTUAL

    // Implementation

public:

    virtual ~CHLink();

    // Generated message map functions

protected:

    //{{AFX_MSG(CHLink)

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

};
