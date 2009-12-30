#include "stdafx.h"
#include "IThread.h"
#include "WindowHelp.h"

void CGetMemberThread::GetMember(int nMemberType, int nCategoryid, CString szLocation, CString szKeyWord, int nMaxCount)
{
    m_szLocation = szLocation;
    m_szKeyWord = szKeyWord;
    m_nCategoryid = nCategoryid;
    m_nMemberType = nMemberType;
    m_nMaxCount = nMaxCount;
    AfxBeginThread(GetMemberWorkerThread, this) ;
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
    else
    {
        return pThread->GetTaobaoSellerMember(); // 淘宝卖家
    }
}

/* 淘宝卖家开始 */
UINT CGetMemberThread::GetTaobaoSellerMember()
{
    CString szShopListURL = szTaobaoAddress; //"http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    // 类别
    CString szCategory;
    szCategory.Format("&id=%d", m_nCategoryid);
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
        CString szKeyword = "&keybiz=" + URLEncode(m_szKeyWord);
        szShopListURL += szKeyword;
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
    if (!m_szLocation.IsEmpty())
    {        
        CString szLocation = "&loc=" + URLEncode(m_szLocation);
        szShopListURL += szLocation;
    }
    // 关键字
    if (!m_szKeyWord.IsEmpty())
    {        
        CString szKeyword = "&keybiz=" + URLEncode(m_szKeyWord);
        szShopListURL += szKeyword;
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


BOOL CGetMemberThread::GetBuyerMemberByShopUserID(CString sellerUserID)
{
    CString szShopURL = "http://my.taobao.com/mytaobao/rate/rate.htm?buyer_or_seller=0&received_or_posted=0&user_id=" + sellerUserID; //current_page=3";

    BOOL bFound = TRUE; // 在该店是否找到买家
    for (int pageIndex=1;bFound && nFoundCount<m_nMaxCount;pageIndex++)
    {
        // 页码
        CString szPageIndex;
        szPageIndex.Format("&current_page=%d", pageIndex);
        CString szTempURL = szShopURL + szPageIndex;

        CString szShopHTML = GetPageDirect(szTempURL);
        bFound = ParseShopForBuyer(szShopHTML);
    }  

    return TRUE;
}

BOOL CGetMemberThread::ParseShopForBuyer(CString szHtml)
{
    int tempCount = nFoundCount; // 开始前找到的个数
    int nStart = 0, nEnd = 0;
    CString szStartFlag = "<div class=\"RatterName\">";
    CString szEndFlag = "</div>";
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

        LPTSTR szMember = buyer.GetBuffer(0);     
        ::SendMessage(m_hPostMsgWnd,WM_FOUND_MEMBER, (WPARAM)TRUE, (LPARAM)szMember);
    }

    return (nFoundCount > tempCount); // 目前数目大于查找前的数目，返回TRUE
}

CString CGetMemberThread::ExtractShopBuyer(CString szShopHtml)
{
    CString szStartFlag = "target=_blank>", szEndFlag = "</a>";
    int s = szShopHtml.Find(szStartFlag) + szStartFlag.GetLength();
    int e = szShopHtml.Find(szEndFlag, s);
    return szShopHtml.Mid(s, e-s);
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