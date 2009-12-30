#include <afxinet.h>


//
// Message to post when thread is done.
//
#define WM_FOUND_MEMBER (WM_USER + 100)

//
//  CInternetThread Class...manages worker thread which reads from Internet.
//
class CGetMemberThread
{
public:
    // Construction
    CGetMemberThread()
    {
    }

    // Initialization
    BOOL Init(HWND hPostMsgWnd)
    {    
        m_hPostMsgWnd = hPostMsgWnd ;

        return TRUE ;
    }

    // 使用多线程获得成员
    void GetMember(int nMemberType, int nCategoryid, CString szLocation, CString szKeyWord, CString szRateSum, int nMaxCount);

    int AddFoundCount()
    {
        return ++nFoundCount;
    }

    int Stop()
    {
        m_nMaxCount = nFoundCount;
        return nFoundCount;
    }
    
    int m_nMemberType;
    
    CString szTaobaoAddress;//  = "http://search1.taobao.com/browse/browse_shop.htm?title_type=name";
    CString szAlibabaAddress;// = "http://search.china.alibaba.com/search/company_search.htm?categoryId=0";

private:
    // Worker thread calls _GetPageWorker.
    static UINT GetMemberWorkerThread(LPVOID pvThread);

    // This is where the actually work is done.
    // 淘宝掌柜
    UINT GetTaobaoSellerMember();
    BOOL ParseShopListForSeller(CString szHtml);
    CString ExtractShopMember(CString szShopHtml);

    // 淘宝买家
    UINT GetTaobaoBuyerMember();
    BOOL ParseShopListForBuyer(CString szHtml);
    CString ExtractShopUserID(CString szShopHtml);
    BOOL GetBuyerMemberByShopUserID(CString sellerUserID);
    BOOL ParseShopForBuyer(CString szHtml);
    CString ExtractShopBuyer(CString szShopHtml);

    // 阿里巴巴商家
    UINT GetAliCompanyMember();
    BOOL ParseAliCompany(CString szHtml);
    CString ExtractCompanyUserID(CString szShopHtml);

    int nFoundCount;
    int m_nMaxCount;
    int m_nCategoryid;
    CString m_szLocation;
    CString m_szKeyWord;
	CString m_szRateSum;

    HWND m_hPostMsgWnd ;
};

//
// Working Thread which does all the actually internet work.
//
UINT GetMemberWorkerThread(LPVOID pvThreadData);
