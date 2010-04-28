// Constants.h
// 常量定义

struct NameValuePair
{
public:
    CString Name;
    DWORD   Value;
public:
    NameValuePair(CString strName, DWORD nValue)
    {
        Value   = nValue;
        Name    = strName;
    }
    NameValuePair(CString strName)
    {
        Value   = 0;
        Name    = strName;
    }
	NameValuePair(DWORD nValue, CString strName)
    {
        Value   = nValue;
        Name    = strName;
    }
};

NameValuePair ShopCategorys[] = {

    NameValuePair("所有类别", 0),

    NameValuePair(1, "生产加工"),
    NameValuePair(2, "经销批发"),
    NameValuePair(4, "招商代理"),
    NameValuePair(8, "商业服务"),

    NameValuePair("结束", -1)
};

NameValuePair Citys[] = {
    NameValuePair("所有省份"),

    NameValuePair("海外"),
    NameValuePair("江苏,浙江,上海"), 
    NameValuePair("山东,江苏,安徽,浙江,福建,上海"), 
    NameValuePair("广东,广西,海南"), 
    NameValuePair("湖北,湖南,河南,江西"), 
    NameValuePair("北京,天津,河北,山西,内蒙古"), 
    NameValuePair("北京"), 
    NameValuePair("上海"), 
    NameValuePair("天津"), 
    NameValuePair("重庆"),
    NameValuePair("浙江"), 
    NameValuePair("广东"), 
    NameValuePair("福建"), 
    NameValuePair("江苏"), 
    NameValuePair("安徽"), 
    NameValuePair("广西"), 
    NameValuePair("贵州"), 
    NameValuePair("海南"), 
    NameValuePair("河北"), 
    NameValuePair("河南"), 
    NameValuePair("黑龙江"), 
    NameValuePair("湖北"), 
    NameValuePair("湖南"), 
    NameValuePair("吉林"), 
    NameValuePair("江西"), 
    NameValuePair("辽宁"), 
    NameValuePair("内蒙古"), 
    NameValuePair("宁夏"), 
    NameValuePair("青海"), 
    NameValuePair("山东"), 
    NameValuePair("山西"), 
    NameValuePair("陕西"), 
    NameValuePair("云南"), 
    NameValuePair("四川"), 
    NameValuePair("甘肃"), 
    NameValuePair("新疆"), 
    NameValuePair("西藏"), 
    NameValuePair("香港"), 
    NameValuePair("澳门"), 
    NameValuePair("台湾"),

    NameValuePair("结束", -1)
};

NameValuePair Speeds[] = {

    NameValuePair("1秒每条", 1000),
    NameValuePair("2秒每条", 2000),
    NameValuePair("3秒每条", 3000),
    NameValuePair("5秒每条", 5000),
    NameValuePair("8秒每条", 8000),
    NameValuePair("10秒每条", 10000),
    NameValuePair("15秒每条", 15000),

    NameValuePair("结束", -1)
};
