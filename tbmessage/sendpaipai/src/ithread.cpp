#include "stdafx.h"
#include "IThread.h"
#include "Utility\WindowHelp.h"

void CGetMemberThread::GetMember(CString szIndustry, int nMemberType, CString szLocation, CString szKeyWord, BOOL bIsOnline,BOOL bIsVip, int nSkipCount, int nMaxCount)
{
    m_szLocation = szLocation;
    m_szKeyWord = szKeyWord;
    m_nCategoryid = 0;
    m_szIndustry = szIndustry;
    m_nMemberType = nMemberType;
    m_nSkipCount = nSkipCount;
    m_nMaxCount = nMaxCount;
    m_bChkOnlyOnline = bIsOnline;
	m_IsVip = bIsVip;
    AfxBeginThread(GetMemberWorkerThread, this) ;
}

//
// This is the thread function. 
//
UINT CGetMemberThread::GetMemberWorkerThread(LPVOID pvThread)
{
    CGetMemberThread* pThread = (CGetMemberThread*) pvThread ;

    if (pThread->m_nMemberType == 0) // 淘宝买家
    {
        return pThread->GetTaobaoBuyerMember();
    }
    //else if (pThread->m_nMemberType == 3) // 阿里巴巴商家
    //{
    //    return pThread->GetAliCompanyMember();
    //}
    else
    {
        return pThread->GetTaobaoSellerMember(); // 淘宝卖家
    }
}

/* 淘宝卖家开始 */
UINT CGetMemberThread::GetTaobaoSellerMember()
{
    CString szShopListURL;

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;
	
	BOOL hasNextPage = TRUE;

#ifdef TEST_VERSION
	szShopListURL.Format("%d.htm", 1);
	szShopListURL = szTaobaoAddress + m_szKeyWord + szShopListURL;

    CString szShopListHTML = GetPageDirect(szShopListURL);

    bFound = ParseShopListForSeller(szShopListHTML);
#else
    for (int pageIndex = 1;bFound && nFoundCount < m_nMaxCount && hasNextPage;pageIndex++)
    {
		szShopListURL.Format("%d.htm", pageIndex);
		szShopListURL = szTaobaoAddress + m_szKeyWord + szShopListURL;

        CString szShopListHTML = GetPageDirect(szShopListURL);

        bFound = ParseShopListForSeller(szShopListHTML);

        //hasNextPage =  GetNextPageUrl(szShopListHTML);
    }    
#endif

    ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}


BOOL CGetMemberThread::ParseShopListForSeller(CString szHtml)
{
    int tempCount = nFoundCount;
    int nStart = 0, nEnd = 0;

    while(nFoundCount < m_nMaxCount)
    {
        //<span align="left"><a href="#" target="_blank" onclick="return CheckInstallQQ3('24741611',''||'EB86790100000000006B33C5001BEB34-5DCB'||''||'','d03c53e6e32cebe33c1bb7ff010a7629b96b87c469d58491e40a32957e40dd996e8e2343f189ff6d8be0850fdf61ffd68b09a245f6c75732','f683df3137aa7428e16cc253c58edac7e5012f9b93204ac9bb38ec80b6fd2f8f3d5288cc5aeb3165e1c2b76140d8c934c81d461f902070ba')"><img alt="点击可与对方沟通，咨询商品信息，交流购物心得。" src="http://wpa.paipai.com/pa?p=1:24741611:16" border="0" align="absmiddle" width="77"></a></span>
        CString startFlag = "<span align=\"left\"><a href=\"#\" target=\"_blank\" onclick=\"return CheckInstallQQ3('";
        nStart = szHtml.Find(startFlag, nEnd);
        if (nStart < 0)
        {
            break;
        }
        nStart += startFlag.GetLength();
        nEnd = szHtml.Find("','", nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString seller = szHtml.Mid(nStart, nEnd-nStart);
		CString sellerScript = GetMemberScript(seller);
		
        if (sellerScript != "")
        {
			LPTSTR szSeller = sellerScript.GetBuffer(0);
			::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szSeller);
        }
    }

    return (tempCount < nFoundCount);
}

/* 淘宝卖家结束 */

/* 淘宝买家开始 */

// 店铺列表页面
UINT CGetMemberThread::GetTaobaoBuyerMember()
{
    CString szShopListURL;

    // 将找到个数初始化位0
    nFoundCount=0;
    BOOL bFound = TRUE;
	
	BOOL hasNextPage = TRUE;

#ifdef TEST_VERSION
	szShopListURL.Format("%d.htm", 1);
	szShopListURL = szTaobaoAddress + m_szKeyWord + szShopListURL;

    CString szShopListHTML = GetPageDirect(szShopListURL);

    bFound = ParseShopListForBuyer(szShopListHTML);
#else
    for (int pageIndex = 1;bFound && nFoundCount < m_nMaxCount && hasNextPage;pageIndex++)
    {
		szShopListURL.Format("%d.htm", pageIndex);
		szShopListURL = szTaobaoAddress + m_szKeyWord + szShopListURL;

        CString szShopListHTML = GetPageDirect(szShopListURL);

        bFound = ParseShopListForBuyer(szShopListHTML);
    }
#endif

    ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)FALSE, NULL);

    return 1;
}

// 处理一个店铺
BOOL CGetMemberThread::ParseShopListForBuyer(CString szHtml)
{	
    int tempCount = nFoundCount;
    int nStart = 0, nEnd = 0;

    while(nFoundCount < m_nMaxCount)
    {
        //<span align="left"><a href="#" target="_blank" onclick="return CheckInstallQQ3('24741611',''||'EB86790100000000006B33C5001BEB34-5DCB'||''||'','d03c53e6e32cebe33c1bb7ff010a7629b96b87c469d58491e40a32957e40dd996e8e2343f189ff6d8be0850fdf61ffd68b09a245f6c75732','f683df3137aa7428e16cc253c58edac7e5012f9b93204ac9bb38ec80b6fd2f8f3d5288cc5aeb3165e1c2b76140d8c934c81d461f902070ba')"><img alt="点击可与对方沟通，咨询商品信息，交流购物心得。" src="http://wpa.paipai.com/pa?p=1:24741611:16" border="0" align="absmiddle" width="77"></a></span>
        CString startFlag = "<span align=\"left\"><a href=\"#\" target=\"_blank\" onclick=\"return CheckInstallQQ3('";
        nStart = szHtml.Find(startFlag, nEnd);
        if (nStart < 0)
        {
            break;
        }
        nStart += startFlag.GetLength();
        nEnd = szHtml.Find("','", nStart);
        if (nEnd < 0)
        {
            break;
        }

		// 掌柜QQ号
        CString seller = szHtml.Mid(nStart, nEnd-nStart);
		//CString sellerScript = GetMemberScript(seller);
		//
  //      if (sellerScript != "")
  //      {
		//	LPTSTR szSeller = sellerScript.GetBuffer(0);
		//	::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szSeller);
  //      }
		
        GetBuyerMemberByShopUserID(seller);
		// 处理店铺中的评论者
#ifndef TEST_VERSION
		GetBuyerMemberByShopUserID2(seller);
#endif
    }

    return (tempCount < nFoundCount);
}

// 近6个月的评价
BOOL CGetMemberThread::GetBuyerMemberByShopUserID(CString sellerUserID)
{
    CString szShopURL = "http://shop1.paipai.com/cgi-bin/credit_detailinfo?uin=" + sellerUserID + "&type=3&evallevel=0&date=4&page=";

    BOOL bFound = TRUE; // 在该店是否找到买家

#ifdef TEST_VERSION
    CString szPageIndex;
    szPageIndex.Format("%d", 1);
    CString szTempURL = szShopURL + szPageIndex;

    CString szShopHTML = GetPageDirect(szTempURL);
    bFound = ParseShopForBuyer(szShopHTML);
#else
    for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
    {
        // 页码
        CString szPageIndex;
        szPageIndex.Format("%d", pageIndex);
        CString szTempURL = szShopURL + szPageIndex;

        CString szShopHTML = GetPageDirect(szTempURL);
        bFound = ParseShopForBuyer(szShopHTML);
    }  
#endif
    return TRUE;
}

BOOL CGetMemberThread::ParseShopForBuyer(CString szHtml)
{
    int tempCount = 0; // 开始前找到的个数
    int nStart = 0, nEnd = 0;
    CString szStartFlag = "<uin1>";
    CString szEndFlag = "</uin1>";
    while(nFoundCount < m_nMaxCount)
    {
        nStart = szHtml.Find(szStartFlag, nEnd);
        if (nStart < 0)
        {
            break;
        }
        nStart += szStartFlag.GetLength();
        nEnd = szHtml.Find(szEndFlag, nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart);
        if (szMemberHtml != "0")
        {			
			CString buyerScript = GetMemberScript(szMemberHtml);
			if (buyerScript != "")
			{
				LPTSTR szBuyer = buyerScript.GetBuffer(0);
				::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szBuyer);
			}
        }
		tempCount++;
    }

    return tempCount;
}

// 6个月以前的评价
BOOL CGetMemberThread::GetBuyerMemberByShopUserID2(CString sellerUserID)
{
    CString szShopURL = "http://archive.paipai.com/cgi-bin/rate/credit_history_info?uin=" + sellerUserID + "&type=0&level=0";

    BOOL bFound = TRUE; // 在该店是否找到买家
    for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
    {
        // 页码
        CString szData;
        szData.Format("uin=%s&type=0&level=0&page=%d", sellerUserID, pageIndex);

        CString szShopHTML = GetPageDirect(szShopURL, "Content-Type: application/x-www-form-urlencoded", szData, TRUE);
        bFound = ParseShopForBuyer2(szShopHTML);
    }  

    return TRUE;
}

BOOL CGetMemberThread::ParseShopForBuyer2(CString szHtml)
{
    int tempCount = 0; // 开始前找到的个数
    int nStart = 0, nEnd = 0;
    CString szStartFlag = "<script>ShowUserPrivacy('";
    CString szEndFlag = "','";
    while(nFoundCount < m_nMaxCount)
    {
        nStart = szHtml.Find(szStartFlag, nEnd);
        if (nStart < 0)
        {
            break;
        }
        nStart += szStartFlag.GetLength();
        nEnd = szHtml.Find(szEndFlag, nStart);
        if (nEnd < 0)
        {
            break;
        }

        CString szMemberHtml = szHtml.Mid(nStart, nEnd-nStart);
        if (szMemberHtml.Find("*") < 0)
        {			
			CString buyerScript = GetMemberScript(szMemberHtml);
			if (buyerScript != "")
			{
				LPTSTR szBuyer = buyerScript.GetBuffer(0);
				::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szBuyer);
			}
        }
		tempCount++;
    }

    return tempCount;
}
/* 淘宝买家结束 */

/* 阿里巴巴商家开始 */
UINT CGetMemberThread::GetAliCompanyMember()
{    
    CString szShopListURL = szAlibabaAddress; //"http://search.china.alibaba.com/search/company_search.htm?categoryId=0"; //"&province=%B1%B1%BE%A9&biztype=0&keywords=%CA%D6%BB%FA&beginPage=2";
    
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
        nStart = szHtml.Find("<div class=\"companylistmyt\">", nEnd);
        if (nStart < 0)
        {
            break;
        }
        nEnd = szHtml.Find("<div class=\"companylistfund\">", nStart);
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

CString CGetMemberThread::GetMemberScript(CString strMemeberId)
{
	CString strStartFlag = "CheckInstall_TM_Hummer('";
	CString strEndFlag = "\">";

	CString szUrl = "http://shop1.paipai.com/cgi-bin/user_info?uin=" + strMemeberId;
	CString szUserInfoHTML = GetPageDirect(szUrl);

    int s = szUserInfoHTML.Find(strStartFlag);
	if (s < 0)
    {
		return "";
    }
    int e = szUserInfoHTML.Find(strEndFlag, s);
	if (e < s)
    {
		return "";
    }

    CString szUserScript = szUserInfoHTML.Mid(s, e - s);

	return szUserScript;
}