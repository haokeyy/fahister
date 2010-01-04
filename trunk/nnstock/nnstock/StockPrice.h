#pragma once

class CStockPrice
{
public:
    CStockPrice(void);
    ~CStockPrice(void);

public:

    static double GetCurrentPrice(CString szStkCode);
};
