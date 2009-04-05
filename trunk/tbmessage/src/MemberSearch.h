#pragma once

class CSearchCondition
{
public:
    int nType;
    int nCategoryId;
    CString szLocation;
    CString szKeyword;
    int nLimit;
};

class CMemberSearch
{
public:
    CMemberSearch(HWND hMainWnd);
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
    CString ExtractShopUserID(CString szShopHtml);
    BOOL GetBuyerMemberByShopUserID(CString sellerUserID, BOOL bSingleUser = FALSE);
    BOOL ParseShopForBuyer(CString szHtml);
    CString ExtractShopBuyer(CString szShopHtml);
};

UINT SearchMemberThread(LPVOID pvThread);