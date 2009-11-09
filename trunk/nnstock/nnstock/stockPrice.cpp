#include "StdAfx.h"
#include "stockPrice.h"
#include "Utility.h"

double CStockPrice::GetCurrentPrice(char* stkCode)
{
    CString szStockMaketUrl = "http://nhqquery.jrj.com.cn/hqquery/?q=cn|s&i=%s&n=hqs1&c=l&q=cn";
    CString szUrl;
    szUrl.Format(szStockMaketUrl, stkCode);

    CString pageContent = CUtility::GetPageDirect(szUrl);

    int p = pageContent.Find("HqData:[");
    if (p > 0)
    {
        CString szhqData = pageContent.Mid(p);
        char *strHq = szhqData.GetBuffer();

        char *substr[100];
        CUtility::SplitString(strHq, ",", substr);

        double price = atof(substr[5]);

        return price;
    }

    return 0;
}

double CStockPrice::GetOpeningPrice(char* stkCode)
{
    CString szStockMaketUrl = "http://nhqquery.jrj.com.cn/hqquery/?q=cn|s&i=%s&n=hqs1&c=l&q=cn";
    CString szUrl;
    szUrl.Format(szStockMaketUrl, stkCode);

    CString pageContent = CUtility::GetPageDirect(szUrl);

    int p = pageContent.Find("HqData:[");
    if (p > 0)
    {
        CString szhqData = pageContent.Mid(p);
        char *strHq = szhqData.GetBuffer();

        char *substr[100];
        CUtility::SplitString(strHq, ",", substr);

        double price = atof(substr[6]);

        return price;
    }

    return 0;
}