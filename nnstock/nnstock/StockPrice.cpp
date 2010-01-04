#include "StdAfx.h"
#include "StockPrice.h"
#include "Utility.h"

CStockPrice::CStockPrice(void)
{
}

CStockPrice::~CStockPrice(void)
{
}

double CStockPrice::GetCurrentPrice(CString szStkCode)
{
    CString rAddress = "http://hq.sinajs.cn/list=";
    if (szStkCode.GetAt(0) == '6')
    {
        rAddress.Append("sh");
    }
    else if (szStkCode.GetAt(0) == '0')
    {
        rAddress.Append("sz");
    }
    rAddress.Append(szStkCode);

    CString szHqData = CUtility::GetPageDirect(rAddress);

    int sep1, sep2, sep3, sep4;
    CString szPrice;
    sep1 = szHqData.Find(",", 0);
    if (sep1 > 0)
    {
        sep2 = szHqData.Find(",", sep1 + 1);
    }
    if (sep2 > 0)
    {
        sep3 = szHqData.Find(",", sep2 + 1);
    }
    if (sep3 > 0)
    {
        sep4 = szHqData.Find(",", sep3 + 1);
    }
    if (sep4 > 0)
    {
        szPrice = szHqData.Mid(sep3 + 1, sep4 - sep3 - 1);
    }

    double dPrice = atof(szPrice);

    return dPrice;
}
