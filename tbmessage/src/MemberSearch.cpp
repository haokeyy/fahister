#include "StdAfx.h"
#include "MemberSearch.h"
#include ".\Libraries\WindowHelp.h"

CMemberSearch::CMemberSearch(HWND hMainWnd, CString taobaoAddr)
{
    m_hMainWnd = hMainWnd;
    szTaobaoAddress = taobaoAddr;
}

CMemberSearch::~CMemberSearch()
{
}

void CMemberSearch::SearchMember(CSearchCondition _condition)
{
    condition = _condition;
    nFoundCount = 0;

    AfxBeginThread(SearchMemberThread, this);
}

UINT SearchMemberThread(LPVOID pvThread)
{
    CMemberSearch* pSearch = (CMemberSearch*)pvThread;

    return pSearch->ExecuteSearch();
}

UINT CMemberSearch::ExecuteSearch()
{
    if (condition.nType == 0)
    {
        GetTaobaoSellerMember();
    }
    else
    {
        GetTaobaoBuyerMember();
    }

    return 0;
}

void CMemberSearch::Stop()
{
    nFoundCount = condition.nLimit;
}

void CMemberSearch::AddFoundCount()
{
    nFoundCount++;
}

/* 淘宝卖家开始 */
UINT CMemberSearch::GetTaobaoSellerMember()
{
	int startIndex = szTaobaoAddress.Find("browse_shop.htm");
	CString szShopListURL = szTaobaoAddress.Left(startIndex); //"http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    // 类别
    CString szCategory;
    szCategory.Format("shop-%d.htm?emptyparam=1", condition.nCategoryId);
    szShopListURL += szCategory;
    // 地区
    if (!condition.szLocation.IsEmpty() && condition.szLocation != "所有地区")
    {        
        CString szLocation = "&loc=" + URLEncode(condition.szLocation);
        szShopListURL += szLocation;
    }
    // 关键字
    if (!condition.szKeyword.IsEmpty())
    {        
        CString szKeyword = "&shopname=" + URLEncode(condition.szKeyword);
        szShopListURL += szKeyword;
    }

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex = 1;bFound && nFoundCount < condition.nLimit; pageIndex++)
    {
        // 页码
        CString szPageIndex;
        szPageIndex.Format("&page=%d", pageIndex);
        CString szTempURL = szShopListURL + szPageIndex;

        CString szShopListHTML = GetPageDirect(szTempURL);
        CString szCurPageFlag;
        szCurPageFlag.Format("<strong>%d</strong>", pageIndex);
        if (szShopListHTML.Find(szCurPageFlag) < 0)
        {
            bFound = FALSE;
        }
        else
        {
            bFound = ParseShopListForSeller(szShopListHTML);
        }
    }    

    ::SendMessage(this->m_hMainWnd, WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}

BOOL CMemberSearch::ParseShopListForSeller(CString szHtml)
{
    int tempCount = nFoundCount;
    int nStart = 0, nEnd = 0;

    while(nFoundCount < condition.nLimit)
    {
        nStart = szHtml.Find("<div class=\"Seller\"><i>掌柜:</i>", nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find("<div class=\"Amount\">", nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart);
        CString seller = ExtractShopMember(szMemberHtml);
        LPTSTR szSeller = seller.GetBuffer(0);

        ::SendMessage(this->m_hMainWnd, WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szSeller);
    }

    return (tempCount < nFoundCount);
}

CString CMemberSearch::ExtractShopMember(CString szShopHtml)
{
    CString szStartFlag = "_blank>", szEndFlag = "</a>";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}

/* 淘宝卖家结束 */


/* 淘宝买家开始 */
UINT CMemberSearch::GetTaobaoBuyerMember()
{
	int startIndex = szTaobaoAddress.Find("browse_shop.htm");
	CString szShopListURL = szTaobaoAddress.Left(startIndex); //"http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    // 类别
    CString szCategory;
    szCategory.Format("shop-%d.htm?emptyparam=1", condition.nCategoryId);
    szShopListURL += szCategory;
    // 地区
    if (!condition.szLocation.IsEmpty() && condition.szLocation != "所有地区")
    {        
        CString szLocation = "&loc=" + URLEncode(condition.szLocation);
        szShopListURL += szLocation;
    }
    // 关键字
    if (!condition.szKeyword.IsEmpty())
    {        
        CString szKeyword = "&shopname=" + URLEncode(condition.szKeyword);
        szShopListURL += szKeyword;
    }

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex=1; bFound && nFoundCount < condition.nLimit; pageIndex++)
    {
        // 页码
        CString szPageIndex;
        szPageIndex.Format("&page=%d", pageIndex);
        CString szTempURL = szShopListURL + szPageIndex;

        CString szShopListHTML = GetPageDirect(szTempURL);
        CString szCurPageFlag;
        szCurPageFlag.Format("<strong>%d</strong>", pageIndex);
        if (szShopListHTML.Find(szCurPageFlag) < 0)
        {
            bFound = FALSE;
        }
        else
        {
            bFound = ParseShopListForBuyer(szShopListHTML);
        }
    }    

    ::SendMessage(this->m_hMainWnd, WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}

BOOL CMemberSearch::ParseShopListForBuyer(CString szHtml)
{
    BOOL bFount = FALSE; // 是否找到掌柜
    int nStart = 0, nEnd = 0;

    while(nFoundCount < condition.nLimit)
    {
        nStart = szHtml.Find("<div class=\"Seller\"><i>掌柜:</i>", nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find("<div class=\"Amount\">", nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart);
        CString sellerUserID = ExtractShopUserID(szMemberHtml);
   
        GetBuyerMemberByShopUserID(sellerUserID);

        bFount = TRUE;
    }

    return bFount;
}

CString CMemberSearch::ExtractShopUserID(CString szShopHtml)
{
    CString szStartFlag = "?user_id=", szEndFlag = "&faction=show";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}


BOOL CMemberSearch::GetBuyerMemberByShopUserID(CString sellerUserID, BOOL bSingleUser)
{
    CString szShopURL = "http://my.taobao.com/mytaobao/rate/rate.htm?received_or_posted=0&buyer_or_seller=0&user_id=" + sellerUserID; //current_page=3";

    BOOL bFound = TRUE; // 在该店是否找到买家

    do
    {
        CString szShopHTML = GetPageDirect(szShopURL);
        bFound = ParseShopForBuyer(szShopHTML);
        if (bFound)
        {
	        CString szLastPageFlag = "#RateType\">下一页";
	        int p = szShopHTML.Find(szLastPageFlag);
            int q = szShopHTML.Find("http://rate.taobao.com/user-rate-", p - 250);
            szShopURL = szShopHTML.Mid(q, p - q);
            szShopURL.Replace("--ismore|0", "--ismore|1");
        }
    }
    while (bFound && nFoundCount < condition.nLimit);

    //for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
    //{
    //    // 页码
    //    CString szPageIndex;
    //    szPageIndex.Format("&current_page=%d", pageIndex);
    //    CString szTempURL = szShopURL + szPageIndex;

    //    CString szShopHTML = GetPageDirect(szTempURL);
    //    bFound = ParseShopForBuyer(szShopHTML);
    //}
	if (bSingleUser == TRUE)
	{
        ::SendMessage(this->m_hMainWnd, WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);
	}

    return TRUE;
}

BOOL CMemberSearch::ParseShopForBuyer(CString szHtml)
{
    int tempCount = nFoundCount; // 开始前找到的个数
    int nStart = 0, nEnd = 0;
    CString szStartFlag = "<p class=\"rate\"";
    CString szEndFlag = "<p class=\"exp\">";
    while(nFoundCount < condition.nLimit)
    {
        nStart = szHtml.Find(szStartFlag, nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find(szEndFlag, nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart+szEndFlag.GetLength());
        CString buyer = ExtractShopBuyer(szMemberHtml);

		if (!buyer.IsEmpty())
		{
			LPTSTR szMember = buyer.GetBuffer(0);     
            ::SendMessage(this->m_hMainWnd, WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szMember);
		}
    }

	CString szLastPageFlag = "#RateType\">下一页";
	int p = szHtml.Find(szLastPageFlag);

    return (nFoundCount > tempCount && p > 0); // 目前数目大于查找前的数目，返回TRUE
}

CString CMemberSearch::ExtractShopBuyer(CString szShopHtml)
{
    CString szStartFlag1 = "target=\"_blank\">", szStartFlag2 = "target=_blank>", szEndFlag = "</a>";
    int s = szShopHtml.Find(szStartFlag1);
    if (s >= 0)
    {
        s += szStartFlag1.GetLength();
    }
    else
    {
        s = szShopHtml.Find(szStartFlag2) + szStartFlag2.GetLength();
    }
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}
/* 淘宝买家结束 */