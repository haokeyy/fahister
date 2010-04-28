// SearchPage.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "tbmessage.h"
#include "SearchPage.h"
#include ".\Libraries\ListViewHelp.h"
#include "Constants.h"
#include "StoredMember.h"

// CSearchPage dialog

extern BOOL bHasMemberChanged;

IMPLEMENT_DYNAMIC(CSearchPage, CDialog)

CSearchPage::CSearchPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchPage::IDD, pParent)
{
    m_nFoundCount = 0;
}

CSearchPage::~CSearchPage()
{
}

void CSearchPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMB_CATEGORY, m_CmbCategory);
    DDX_Control(pDX, IDC_CMB_LOCATION, m_CmbLocation);
}


BEGIN_MESSAGE_MAP(CSearchPage, CDialog)
    ON_BN_CLICKED(IDC_BTN_SEARCH, &CSearchPage::OnBnClickedBtnSearch)
	//}}AFX_MSG_MAP    
    ON_MESSAGE(WM_FOUND_MEMBER, OnFoundMember) 
    ON_BN_CLICKED(IDC_BTN_DISTINCT, &CSearchPage::OnBnClickedBtnDistinct)
END_MESSAGE_MAP()


BOOL CSearchPage::OnInitDialog()
{
	CDialog::OnInitDialog();

    szTaobaoSearchUrl = m_szSearchUrl; //"http://shopsearch.taobao.com/browse/shop_search.htm?title=title&nick=nick";

    m_pMemberSearch = new CMemberSearch(this->GetSafeHwnd(), szTaobaoSearchUrl);

    InitCategory();
    InitLocation();

    this->SetDlgItemInt(IDC_EDIT_LIMIT, 100);
    this->SetDlgItemInt(IDC_EDIT_START_PAGE, 1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// CSearchPage message handlers


void CSearchPage::InitCategory()
{
    m_CmbCategory.ResetContent();
    int i=0;
    while (ShopCategorys[i].Value != -1)
    {
        m_CmbCategory.AddString(ShopCategorys[i].Name);
        m_CmbCategory.SetItemData(i, ShopCategorys[i].Value);
        i++;
    }

    m_CmbCategory.SetCurSel(0);
}

void CSearchPage::InitLocation()
{
    m_CmbLocation.ResetContent();
   
    int i=0;
    while (Citys[i].Value != -1)
    {
        m_CmbLocation.AddString(Citys[i].Name);
        i++;
    }

    m_CmbLocation.SetCurSel(0);
}

void CSearchPage::OnBnClickedBtnSearch()
{
    CString szSearchCaption;
    this->GetDlgItem(IDC_BTN_SEARCH)->GetWindowText(szSearchCaption);
    if (szSearchCaption == "开始查找")
    {
        m_nFoundCount = 0;

        CSearchCondition condition;
        int i = m_CmbCategory.GetCurSel();
        condition.nCategoryId = m_CmbCategory.GetItemData(i);
        m_CmbLocation.GetWindowText(condition.szLocation);
        this->GetDlgItemText(IDC_EDIT_KEYWORD, condition.szKeyword);
        CButton *btnIsOnline = (CButton *)this->GetDlgItem(IDC_CHK_ISONLINE);
        condition.bIsOnline = btnIsOnline->GetCheck();       

        if ((condition.szLocation.IsEmpty() || condition.szLocation == "所有省份") && condition.szKeyword.IsEmpty())
        {
            MessageBox("请选择一个地区或输入一个关键字", "错误");
            return;
        }
        
        condition.nLimit = this->GetDlgItemInt(IDC_EDIT_LIMIT);

        this->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText("停止");
        m_pMemberSearch->SearchMember(condition);
    }
    else
    {
        m_pMemberSearch->Stop();
    }
    bHasMemberChanged = TRUE;
}


LRESULT CSearchPage::OnFoundMember(WPARAM wParam, LPARAM lParam)
{
    BOOL bFinish = (BOOL)wParam;
    if (wParam)
    {
        LPTSTR szUserId = (LPTSTR)lParam;        
        //CListViewHelp::AddListItem(m_MemberList, szUserId, STATUS_UNSEND);
        CStoredMember::AddMember(szUserId);
        BOOL bAdded = TRUE;
        if (bAdded)
        {
            CString szGroupCaption;
            szGroupCaption.Format("共找到%d个用户", ++m_nFoundCount);
            this->SetDlgItemText(IDC_STATIC_STATUS, szGroupCaption);
            m_pMemberSearch->AddFoundCount();
        }
    }
    else
    {
        this->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText("开始查找");
    }

    return 0;
}

void CSearchPage::OnBnClickedBtnDistinct()
{
    long oldCnt = CStoredMember::GetCount();
    CStoredMember::ApplyDistinct();
    long newCnt = CStoredMember::GetCount();
    
    CString szGroupCaption;
    szGroupCaption.Format("共删除%d个重复的用户, 删除之后共有%d用户", oldCnt - newCnt, newCnt);
    this->SetDlgItemText(IDC_STATIC_STATUS, szGroupCaption);

    if (oldCnt > newCnt)
    {
        bHasMemberChanged = TRUE;
    }
}
