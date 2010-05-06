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
    else if (condition.nType == 1)
    {
        GetTaobaoBuyerMember();
    }
    else if (condition.nType == 2)
    {
        GetBuyerMemberByShopRatePage(condition.szKeyword, TRUE);
    }
    else
    {
        ::SendMessage(this->m_hMainWnd, WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);
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
CString GetNextPageUrl(CString szShopListHtml)
{
    int iStart = 0, iEnd = 0;
        
    iEnd = szShopListHtml.Find("\" class=\"page-next\"><span>下一页</span></a>", 0);
    if (iEnd > 0)
    {
        iStart = szShopListHtml.Find("http://shopsearch.taobao.com/browse/shop_search/", iEnd - 260);
        if (iEnd > iStart && iStart > 0)
        {
            return szShopListHtml.Mid(iStart, iEnd - iStart);
        }
    }
    return "";
}

UINT CMemberSearch::GetTaobaoSellerMember()
{
	CString szShopListURL = szTaobaoAddress;
    // 类别
    szShopListURL.AppendFormat("&cat=%d", condition.nCategoryId);
    // 地区
    if (!condition.szLocation.IsEmpty() && condition.szLocation != "所有地区")
    {        
        szShopListURL.AppendFormat("&loc=%s", URLEncode(condition.szLocation));
    }
    // 关键字
    if (!condition.szKeyword.IsEmpty())
    {        
        szShopListURL.AppendFormat("&q=%s", URLEncode(condition.szKeyword));
    }
    // 关键字
    if (!condition.szRateSum.IsEmpty())
    {        
        szShopListURL.AppendFormat("&ratesum=%s", URLEncode(condition.szRateSum));
    }

    // 将找到个数初始化为0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex = 1;bFound && nFoundCount < condition.nLimit; pageIndex++)
    {
        CString szShopListHTML = GetPageDirect(szShopListURL);
        CString szCurPageFlag = "<span class=\"page-cur\">";
        if (szShopListHTML.Find(szCurPageFlag) < 0)
        {
            bFound = FALSE;
        }
        else
        {
            bFound = ParseShopListForSeller(szShopListHTML);
            szShopListURL = GetNextPageUrl(szShopListHTML);
            if (szShopListURL.IsEmpty())
            {
                break;
            }
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
        nStart = szHtml.Find("<p class=\"nick\"><a target=\"_blank\" href=\"http://my.taobao.com/mytaobao/rate/rate.jhtml?user_id=", nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find("<p><span class=\"J_WangWang\" data-nick=", nStart);
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
    CString szStartFlag = "&faction=show\">", szEndFlag = " </a></p>";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}

/* 淘宝卖家结束 */


/* 淘宝买家开始 */
UINT CMemberSearch::GetTaobaoBuyerMember()
{
	CString szShopListURL = szTaobaoAddress;
    // 类别
    szShopListURL.AppendFormat("&cat=%d", condition.nCategoryId);
    // 地区
    if (!condition.szLocation.IsEmpty() && condition.szLocation != "所有地区")
    {        
        szShopListURL.AppendFormat("&loc=%s", URLEncode(condition.szLocation));
    }
    // 关键字
    if (!condition.szKeyword.IsEmpty())
    {        
        szShopListURL.AppendFormat("&q=%s", URLEncode(condition.szKeyword));
    }
    // 关键字
    if (!condition.szRateSum.IsEmpty())
    {        
        szShopListURL.AppendFormat("&ratesum=%s", URLEncode(condition.szRateSum));
    }

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex=1; bFound && nFoundCount < condition.nLimit; pageIndex++)
    {
        CString szShopListHTML = GetPageDirect(szShopListURL);
        CString szCurPageFlag = "<span class=\"page-cur\">";
        if (szShopListHTML.Find(szCurPageFlag) < 0)
        {
            bFound = FALSE;
        }
        else
        {
            bFound = ParseShopListForBuyer(szShopListHTML);
            szShopListURL = GetNextPageUrl(szShopListHTML);
            if (szShopListURL.IsEmpty())
            {
                break;
            }
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
        nStart = szHtml.Find("<p class=\"nick\"><a target=\"_blank\" href=\"http://my.taobao.com/mytaobao/rate/rate.jhtml?user_id=", nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find("<p><span class=\"J_WangWang\" data-nick=", nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart);
        CString szShopRatePage = ExtractShopRatePage(szMemberHtml);
   
        GetBuyerMemberByShopRatePage(szShopRatePage);

        bFount = TRUE;
    }

    return bFount;
}

CString CMemberSearch::ExtractShopRatePage(CString szShopHtml)
{
    CString szStartFlag = "http://my.taobao.com/mytaobao/rate/rate.jhtml?user_id=", szEndFlag = "&faction=show";
    int s = szShopHtml.Find(szStartFlag);
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}


BOOL CMemberSearch::GetBuyerMemberByShopRatePage(CString szShopRatePage, BOOL bSingleUser)
{
    CString szShopURL = szShopRatePage;

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
            szShopURL.Replace("--auctionId|--lastModified|--snapId|0--pc|200.htm", ".htm");
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