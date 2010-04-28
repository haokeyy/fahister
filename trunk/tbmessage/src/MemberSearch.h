#pragma once

class CSearchCondition
{
public:
    //int nType;
    int nCategoryId;
    CString szCategoryName;
    CString szLocation;
    CString szKeyword;
    BOOL bIsOnline;
    //CString szRateSum;
    int nStartPage;
    int nLimit;
};

class CMemberSearch
{
public:
    CMemberSearch(HWND hMainWnd, CString taobaoAddr);
    ~CMemberSearch();
public:    
    HWND m_hMainWnd;
    int nFoundCount;
    CSearchCondition condition;
    void SearchMember(CSearchCondition _condition);
    UINT ExecuteSearch();
    void Stop();
    void AddFoundCount();
    CString szTaobaoAddress;
private:    
    UINT GetTaobaoSellerMember();
    BOOL ParseShopListForSeller(CString szHtml);
    CString ExtractShopMember(CString szShopHtml);

    UINT GetTaobaoBuyerMember();
    BOOL ParseShopListForBuyer(CString szHtml);
    CString ExtractShopRatePage(CString szShopHtml);
    BOOL GetBuyerMemberByShopRatePage(CString szShopRatePage, BOOL bSingleUser = FALSE);
    BOOL ParseShopForBuyer(CString szHtml);
    CString ExtractShopBuyer(CString szShopHtml);
};

UINT SearchMemberThread(LPVOID pvThread);