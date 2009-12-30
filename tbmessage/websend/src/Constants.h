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

	NameValuePair(1041, "移动联通充值中心/IP长途"),
	NameValuePair(35, "网络游戏虚拟商品交易区"),

	NameValuePair(13, "手机"),
	NameValuePair(17, "数码相机/摄像摄影 "),
	NameValuePair(12, "随身视听/音响/耳机"),
	NameValuePair(11, "电脑硬件/台式机"),
	NameValuePair(1043, "笔记本电脑"),
	NameValuePair(1048, "数码配件/电子原件市场"),
	NameValuePair(21, "办公设备/文具/耗材"),
	NameValuePair(1046, "HI-FI/家庭影院/电视/音响"),
	NameValuePair(36, "家用电子电器/保健器械"),
	NameValuePair(1052, "电脑软件/网络服务"),

	NameValuePair(1055, "女士内衣"),
	NameValuePair(1056, "女鞋"),
	NameValuePair(14, "女装/女士精品"),
	NameValuePair(31, "女士箱包/配件"),
	NameValuePair(23, "珠宝/钻石/翡翠/黄金"),
	NameValuePair(15, "彩妆/香水/护肤/美体"),
	NameValuePair(1044, "品牌手表/流行手表"),
	NameValuePair(1054, "饰品/流行首饰/时尚饰品"),

	NameValuePair(37, "男装/服饰配件"),
	NameValuePair(22, "汽车/配件/改装/摩托/自行车"),
	NameValuePair(1040, "ZIPPO/瑞士军刀/饰品/眼镜"),
	NameValuePair(18, "运动/健身/运动明星"),
	NameValuePair(1045, "户外/军品/旅游/机票"),

	NameValuePair(24, "居家/厨房/装饰/工艺品"),
	NameValuePair(1050, "家具/家具定制/宜家代购"),
	NameValuePair(29, "食品/茶叶/零食/特产"),
	NameValuePair(1051, "保健品/滋补品"),
	NameValuePair(1049, "床上用品/靠垫/窗帘/布艺"),
	NameValuePair(26, "装潢/灯具/五金/安防/卫浴"),
	NameValuePair(1020, "童装/婴幼/孕妇用品"),

	NameValuePair(16, "电玩/配件/游戏/攻略"),
	NameValuePair(30, "玩具/动漫/模型/卡通"),

	NameValuePair(20, "古董/邮币/字画/收藏"),
	NameValuePair(32, "宠物/宠物食品及用品"),

	NameValuePair(34, "书籍/杂志/报纸"),
	NameValuePair(33, "音乐/影视/明星/乐器"),

	NameValuePair(1053, "购物优惠券/门票/美容健身卡"),
	NameValuePair(1047, "鲜花速递/蛋糕配送/园艺花艺"),
	NameValuePair(1042, "网店服务/物流/家政/婚庆"),
	NameValuePair(27, "成人用品/避孕用品/情趣内衣"),

    NameValuePair("结束", -1)
};

NameValuePair Citys[] = {
    NameValuePair("所有地区"),
    NameValuePair("北京"),
    NameValuePair("上海"),
    NameValuePair("杭州"),
    NameValuePair("广州"),
    NameValuePair("深圳"),
    NameValuePair("南京"),
    NameValuePair("武汉"),
    NameValuePair("天津"),
    NameValuePair("香港"),
    NameValuePair("成都"),
    NameValuePair("哈尔滨"),
    NameValuePair("重庆"),
    NameValuePair("宁波"),
    NameValuePair("无锡"),
    NameValuePair("济南"),
    NameValuePair("苏州"),
    NameValuePair("温州"),
    NameValuePair("青岛"),
    NameValuePair("沈阳"),
    NameValuePair("福州"),
    NameValuePair("西安"),
    NameValuePair("长沙"),
    NameValuePair("合肥"),
    NameValuePair("南宁"),
    NameValuePair("南昌"),
    NameValuePair("郑州"),
    NameValuePair("厦门"),
    NameValuePair("大连"),
    NameValuePair("常州"),
    NameValuePair("石家庄"),
    NameValuePair("东莞"),
    NameValuePair("台湾"),

    NameValuePair("结束", -1)
};

NameValuePair BizTypes[] = {

    NameValuePair("所有经营模式", 0),
    NameValuePair("生产型", 1),
    NameValuePair("贸易型", 2),
    NameValuePair("服务型", 4),
    NameValuePair("政府或其他机构", 8),

    NameValuePair("结束", -1)
};

NameValuePair Provinces[] = {
    NameValuePair("所有省份"),
    NameValuePair("安徽"),
    NameValuePair("北京"),
    NameValuePair("甘肃"),
    NameValuePair("广东"),
    NameValuePair("广西"),
    NameValuePair("贵州"),
    NameValuePair("海南"),
    NameValuePair("河北"),
    NameValuePair("河南"),
    NameValuePair("黑龙江"),
    NameValuePair("湖北"),
    NameValuePair("湖南"),
    NameValuePair("吉林"),
    NameValuePair("江苏"),
    NameValuePair("江西"),
    NameValuePair("辽宁"),
    NameValuePair("内蒙古"),
    NameValuePair("宁夏"),
    NameValuePair("青海"),
    NameValuePair("山东"),
    NameValuePair("山西"),
    NameValuePair("福建"),
    NameValuePair("陕西"),
    NameValuePair("上海"),
    NameValuePair("四川"),
    NameValuePair("天津"),
    NameValuePair("西藏"),
    NameValuePair("新疆"),
    NameValuePair("云南"),
    NameValuePair("浙江"),
    NameValuePair("重庆"),
    NameValuePair("HK"),
    NameValuePair("MO"),
    NameValuePair("TW"),
    
    NameValuePair("结束", -1)
};