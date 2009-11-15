#include "stdafx.h"
#include "IThread.h"
#include "Utility\WindowHelp.h"

void CGetMemberThread::GetMember(int nMemberType, int nCategoryid, CString szLocation, CString szKeyWord, BOOL bIsOnline, CString strRateSum, int nSkipCount, int nMaxCount)
{
    m_szLocation = szLocation;
    m_szKeyWord = szKeyWord;
    m_nCategoryid = nCategoryid;
    m_nMemberType = nMemberType;
    m_nSkipCount = nSkipCount;
    m_nMaxCount = nMaxCount;
    m_bChkOnlyOnline = bIsOnline;
	m_szRateSum = strRateSum;
	nFoundCount = 0;
    AfxBeginThread(GetMemberWorkerThread, this) ;
}

CString SubString(CString src, CString strStartFlag, CString strEndFlag, BOOL includeFlag)
{
	int s = src.Find(strStartFlag, 0);
	if (s < 0) return "";
	int e = src.Find(strEndFlag, s);
	if (e < 0) return "";

	if (includeFlag)
	{
		return src.Mid(s, e - s + strEndFlag.GetLength());
	}
	else
	{
		s = s + strStartFlag.GetLength();
		return src.Mid(s, e - s);
	}
}

//
// This is the thread function. 
//
UINT CGetMemberThread::GetMemberWorkerThread(LPVOID pvThread)
{
    CGetMemberThread* pThread = (CGetMemberThread*) pvThread ;

    if (pThread->m_nMemberType == 2) // 淘宝买家
    {
        return pThread->GetTaobaoBuyerMember();
    }
    else if (pThread->m_nMemberType == 3) // 阿里巴巴商家
    {
        return pThread->GetAliCompanyMember();
    }
    else if (pThread->m_nMemberType == 4) 
	{
		// 提取用户名
		CString strUserId = SubString(pThread->m_szKeyWord, "user-rate-", ".htm", FALSE);
		if (strUserId.IsEmpty())
		{
			strUserId = SubString(pThread->m_szKeyWord, "user_id=", "&", FALSE);
		}

		return pThread->GetBuyerMemberByShopUserID(strUserId, TRUE);
	}
	else if (pThread->m_nMemberType == 5) 
	{
		return pThread->GetBuyerMemberByItemPage(pThread->m_szKeyWord);
	}
	else
    {
        return pThread->GetTaobaoSellerMember(); // 淘宝卖家
    }
}

/* 淘宝卖家开始 */
UINT CGetMemberThread::GetTaobaoSellerMember()
{
	int startIndex = szTaobaoAddress.Find("browse_shop.htm");
	CString szShopListURL = szTaobaoAddress.Left(startIndex); //"http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    // 类别
    CString szCategory;
    szCategory.Format("shop-%d.htm?emptyparam=1", m_nCategoryid);
    szShopListURL += szCategory;
    // 地区
    if (!m_szLocation.IsEmpty())
    {        
        CString szLocation = "&loc=" + URLEncode(m_szLocation);
        szShopListURL += szLocation;
    }
    // 关键字
    if (!m_szKeyWord.IsEmpty())
    {        
        CString szKeyword = "&shopname=" + URLEncode(m_szKeyWord);
        szShopListURL += szKeyword;
    }
	// 信誉级别
	if (!m_szRateSum.IsEmpty())
	{
		CString szRateSum = "&ratesum=" + URLEncode(m_szRateSum);
		szShopListURL += szRateSum;
	}

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex=m_nSkipCount/40 + 1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
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

    ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}

BOOL CGetMemberThread::ParseShopListForSeller(CString szHtml)
{
    int tempCount = nFoundCount;
    int nStart = 0, nEnd = 0;

    while(nFoundCount < m_nMaxCount)
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

        ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szSeller);
    }

    return (tempCount < nFoundCount);
}

CString CGetMemberThread::ExtractShopMember(CString szShopHtml)
{
    CString szStartFlag = "_blank>", szEndFlag = "</a>";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}

/* 淘宝卖家结束 */

/* 淘宝买家开始 */
UINT CGetMemberThread::GetTaobaoBuyerMember()
{
    CString szShopListURL = szTaobaoAddress; //"http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    // 类别
    CString szCategory;
    szCategory.Format("&id=%d", m_nCategoryid);
    szShopListURL += szCategory;
    // 地区
    /*if (!m_szLocation.IsEmpty())
    {        
        CString szLocation = "&loc=" + URLEncode(m_szLocation);
        szShopListURL += szLocation;
    }*/
    // 关键字
    if (!m_szKeyWord.IsEmpty())
    {        
        CString szKeyword = "&keybiz=" + URLEncode(m_szKeyWord);
        szShopListURL += szKeyword;
    }	
	// 信誉级别
	if (!m_szRateSum.IsEmpty())
	{
		CString szRateSum = "&ratesum=" + URLEncode(m_szRateSum);
		szShopListURL += szRateSum;
	}

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
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

    ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}

BOOL CGetMemberThread::ParseShopListForBuyer(CString szHtml)
{
    BOOL bFount = FALSE; // 是否找到掌柜
    int nStart = 0, nEnd = 0;

    while(nFoundCount < m_nMaxCount)
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

CString CGetMemberThread::ExtractShopUserID(CString szShopHtml)
{
    CString szStartFlag = "?user_id=", szEndFlag = "&faction=show";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}


BOOL CGetMemberThread::GetBuyerMemberByShopUserID(CString sellerUserID, BOOL bSingleUser)
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
    while (bFound && nFoundCount<m_nMaxCount);

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
		::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);
	}

    return TRUE;
}

BOOL CGetMemberThread::ParseShopForBuyer(CString szHtml)
{
    int tempCount = nFoundCount; // 开始前找到的个数
    int nStart = 0, nEnd = 0;
    CString szStartFlag = "<p class=\"rate\"";
    CString szEndFlag = "<p class=\"exp\">";
    while(nFoundCount < m_nMaxCount)
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
			::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szMember);
		}
    }

	CString szLastPageFlag = "#RateType\">下一页";
	int p = szHtml.Find(szLastPageFlag);

    return (nFoundCount > tempCount || p > 0); // 目前数目大于查找前的数目，返回TRUE
}

CString CGetMemberThread::ExtractShopBuyer(CString szShopHtml)
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

/* 阿里巴巴商家开始 */
UINT CGetMemberThread::GetAliCompanyMember()
{    
    CString szShopListURL = szAlibabaAddress; //"http://search.china.alibaba.com/search/company_search.htm?categoryId=0"; //"&province=%B1%B1%BE%A9&biztype=0&keywords=%CA%D6%BB%FA&beginPage=2";
    szShopListURL += "&page_size=40";
    // 公司类型
    CString szCategory;
    szCategory.Format("&biztype=%d", m_nCategoryid);
    szShopListURL += szCategory;
    // 地区
    if (!m_szLocation.IsEmpty())
    {        
        CString szLocation = "&province=" + URLEncode(m_szLocation);
        szShopListURL += szLocation;
    }
    // 关键字
    if (!m_szKeyWord.IsEmpty())
    {        
        CString szKeyword = "&keywords=" + URLEncode(m_szKeyWord);
        szShopListURL += szKeyword;
    }
    if (m_bChkOnlyOnline)
    {        
        CString szIsOnline = "&onlineStatus=yes";
        szShopListURL += szIsOnline;
    }  

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;

    for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
    {
        // 页码
        CString szPageIndex;
        szPageIndex.Format("&beginPage=%d", pageIndex);
        CString szTempURL = szShopListURL + szPageIndex;

        CString szShopListHTML = GetPageDirect(szTempURL);
        bFound = ParseAliCompany(szShopListHTML);
    }    

    ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}

// 解码的函数的代码
CString szDecodeFunction;

BOOL CGetMemberThread::ParseAliCompany(CString szHtml)
{    
    int tempCount = nFoundCount;
    int nStart = 0, nEnd = 0;

    // 查找解码的函数
    nStart = szHtml.Find("if(needDecode)", 0);
    if (nStart < 0)
    {
        return FALSE;
    }
    nEnd = szHtml.Find("}", nStart)+1;
 
    szDecodeFunction = szHtml.Mid(nStart, nEnd-nStart);

    while(nFoundCount < m_nMaxCount)
    {
        nStart = szHtml.Find("<div class=\"listmyt lh15\">", nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find("onclick=\"return aliclick(this,", nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart);
        CString seller = ExtractCompanyUserID(szMemberHtml);
        LPTSTR szSeller = seller.GetBuffer(0);

        ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szSeller);
    }

    return (tempCount < nFoundCount);
}

CString DecodeAliUserID(CString szEncodeUserID)
{
    CString szFlag1 = "id.replace(\"";
    CString szFlag2 = "\",\"";
    CString szFlag3 = "\");";

    CString rs, rd; // 源字符串和目标字符串

    int iStart = 0, iEnd = 0;
    while(TRUE)
    {
        iStart = szDecodeFunction.Find(szFlag1, iEnd) + szFlag1.GetLength();
        if (iStart < szFlag1.GetLength()) // 没有找到更多
        {
            break;
        }
        iEnd = szDecodeFunction.Find(szFlag2, iStart);
        rs = szDecodeFunction.Mid(iStart, iEnd-iStart); // 源字符串

        iStart = iEnd + szFlag2.GetLength();
        iEnd = szDecodeFunction.Find(szFlag3, iStart);
        rd = szDecodeFunction.Mid(iStart, iEnd-iStart); // 目标字符串

        szEncodeUserID.Replace(rs, rd);
    }

    return szEncodeUserID;
}

CString CGetMemberThread::ExtractCompanyUserID(CString szShopHtml)
{    
    CString szStartFlag = "('','", szEndFlag = "','";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    CString szEncodeID = szShopHtml.Mid(s, e-s); // 解密前的id
    return DecodeAliUserID(szEncodeID);
}

/* 阿里巴巴商家结束 */

BOOL CGetMemberThread::GetBuyerMemberByItemPage(CString szShopURL)
{
    //CString szShopURL = "http://my.taobao.com/mytaobao/rate/rate.htm?received_or_posted=0&user_id=" + sellerUserID; //current_page=3";

	BOOL bFound = TRUE; // 在该店是否找到买家
    for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
    {
        // 页码
        CString szPageIndex;
        szPageIndex.Format("?page_size=100&bid_page=%d", pageIndex);
        CString szTempURL = szShopURL + szPageIndex;

        CString szShopHTML = GetPageDirect(szTempURL);
        bFound = ParseItemPageForBuyer(szShopHTML);
    }

	::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return TRUE;
}

BOOL CGetMemberThread::ParseItemPageForBuyer(CString szHtml)
{
    int tempCount = nFoundCount; // 开始前找到的个数
    int nStart = 0, nEnd = 0;
    CString szStartFlag = "portal/personal_portal.htm\" target=\"_blank\">";
    CString szEndFlag = "</a>";
    while(nFoundCount < m_nMaxCount)
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
        CString buyer = ExtractItemPageBuyer(szMemberHtml);

		if (!buyer.IsEmpty())
		{
			LPTSTR szMember = buyer.GetBuffer(0);     
			::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szMember);
		}
    }

    return (nFoundCount > tempCount); // 目前数目大于查找前的数目，返回TRUE
}

CString CGetMemberThread::ExtractItemPageBuyer(CString szShopHtml)
{
    CString szStartFlag = ".htm\" target=\"_blank\">", szEndFlag = "</a>";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
}
