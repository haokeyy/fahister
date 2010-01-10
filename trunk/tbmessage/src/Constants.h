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

    NameValuePair(16, "女装/女士精品"),
    NameValuePair(30, "男装"),
    NameValuePair(50006843, "女鞋"),
    NameValuePair(50011740, "流行男鞋"),
    NameValuePair(50010388, "运动鞋"),
    NameValuePair(1625, "女士内衣/男士内衣/家居服"),
    NameValuePair(50006842, "箱包皮具/热销女包/男包"),
    NameValuePair(50011699, "运动服/运动包/颈环配件"),
    NameValuePair(50010404, "服饰配件/皮带/帽子/围巾"),
    NameValuePair(1512, "手机"),
    NameValuePair(50012081, "国货精品手机"),
    NameValuePair(1101, "笔记本电脑"),
    NameValuePair(11, "电脑硬件/台式整机/网络设备"),
    NameValuePair(1201, "MP3/MP4/iPod/录音笔"),
    NameValuePair(14, "数码相机/摄像机/图形冲印"),
    NameValuePair(50008090, "3C数码配件市场"),
    NameValuePair(50008168, "网络服务/电脑软件"),
    NameValuePair(50012164, "闪存卡/U盘/移动存储"),
    NameValuePair(20, "电玩/配件/游戏/攻略"),
    NameValuePair(50007218, "办公设备/文具/耗材"),
    NameValuePair(50011972, "影音电器"),
    NameValuePair(1801, "美容护肤/美体/精油"),
    NameValuePair(50010788, "彩妆/香水/美发/工具"),
    NameValuePair(50002768, "个人护理/保健/按摩器材"),
    NameValuePair(21, "居家日用/厨房餐饮/卫浴洗浴"),
    NameValuePair(2128, "时尚家饰/工艺品/十字绣"),
    NameValuePair(50008164, "家具/家具定制/宜家代购"),
    NameValuePair(50008163, "家纺/床品/地毯/布艺"),
    NameValuePair(27, "装潢/灯具/五金/安防/卫浴"),
    NameValuePair(50012472, "保健食品"),
    NameValuePair(50002766, "食品/茶叶/零食/特产"),
    NameValuePair(35, "奶粉/尿片/母婴用品"),
    NameValuePair(50005998, "益智玩具/童车/童床/书包"),
    NameValuePair(50008165, "童装/童鞋/孕妇装"),
    NameValuePair(29, "宠物/宠物食品及用品"),
    NameValuePair(50012082, "厨房电器"),
    NameValuePair(50012100, "生活电器"),
    NameValuePair(33, "书籍/杂志/报纸"),
    NameValuePair(34, "音乐/影视/明星/乐器"),
    NameValuePair(50010728, "运动/瑜伽/健身/球迷用品"),
    NameValuePair(2203, "户外/登山/野营/涉水"),
    NameValuePair(26, "汽车/配件/改装/摩托/自行车"),
    NameValuePair(1705, "饰品/流行首饰/时尚饰品"),
    NameValuePair(50011397, "珠宝/钻石/翡翠/黄金"),
    NameValuePair(50005700, "品牌手表/流行手表"),
    NameValuePair(23, "古董/邮币/字画/收藏"),
    NameValuePair(25, "玩具/模型/娃娃/人偶"),
    NameValuePair(28, "ZIPPO/瑞士军刀/眼镜"),
    NameValuePair(40, "腾讯QQ专区"),
    NameValuePair(50011665, "网游装备/游戏币/帐号/代练"),
    NameValuePair(99, "网络游戏点卡"),
    NameValuePair(50004958, "移动/联通/小灵通充值中心"),
    NameValuePair(50008907, "IP卡/网络电话/手机号码"),
    NameValuePair(2813, "成人用品/避孕用品/情趣内衣"),
    NameValuePair(50003754, "网店装修/物流快递/图片存储"),
    NameValuePair(50007216, "鲜花速递/蛋糕配送/园艺花艺"),
    NameValuePair(50008075, "演出/旅游/吃喝玩乐折扣券"),

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
    NameValuePair("安徽"),
    NameValuePair("福建"),
    NameValuePair("甘肃"),
    NameValuePair("广东"),
    NameValuePair("广西"),
    NameValuePair("贵州"),
    NameValuePair("海南"),
    NameValuePair("河北"),
    NameValuePair("黑龙江"),
    NameValuePair("河南"),
    NameValuePair("湖北"),
    NameValuePair("湖南"),
    NameValuePair("江苏"),
    NameValuePair("江西"),
    NameValuePair("吉林"),
    NameValuePair("辽宁"),
    NameValuePair("内蒙古"),
    NameValuePair("宁夏"),
    NameValuePair("青海"),
    NameValuePair("山东"),
    NameValuePair("山西"),
    NameValuePair("陕西"),
    NameValuePair("四川"),
    NameValuePair("新疆"),
    NameValuePair("西藏"),
    NameValuePair("云南"),
    NameValuePair("浙江"),
    NameValuePair("所有地区"),
    NameValuePair("其它"),

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
