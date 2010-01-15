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
	NameValuePair(DWORD nValue, CString strName)
    {
        Value   = nValue;
        Name    = strName;
    }
    NameValuePair(CString strName)
    {
        Value   = 0;
        Name    = strName;
    }
};

NameValuePair ShopCategorys[] = {

    NameValuePair("请选择类别", 0),

    NameValuePair("手机/通讯/卡", 13),
    NameValuePair("数码相机/摄像摄影", 17),
    NameValuePair("MP3/随身视听/音响/家电", 12),
    NameValuePair("电脑、网络及相关设备", 11),
    NameValuePair("办公设备及耗材", 21),
    NameValuePair("充值中心/IP长途/网络电话", 1041),

    NameValuePair("女装/女士精品", 14),
    NameValuePair("女士箱包/鞋帽/女士配件", 31),
    NameValuePair("珠宝首饰/手表/眼镜", 23),
    NameValuePair("彩妆/香水/护肤/美体", 15),

    NameValuePair("居家日用/装饰/文具/服务", 24),
    NameValuePair("家用电器/保健器械", 36),
    NameValuePair("玩具/模型/卡通产品", 30),
    NameValuePair("装潢/五金工具/房产", 26),
    NameValuePair("童装/婴幼/孕妇用品", 1020),
    NameValuePair("食品/保健品", 29),
    NameValuePair("旅游/票务/成人/五花八门", 27),

    NameValuePair("收藏品/邮币/古董/字画", 20),
    NameValuePair("宠物/工艺品/礼品/鲜花", 32),

    NameValuePair("书籍/杂志/报纸", 34),
    NameValuePair("音乐/影视/明星/娱乐", 33),

    NameValuePair("动漫/游戏/周边", 16),
    NameValuePair("虚拟物品/游戏装备", 35),

    NameValuePair("男装/服饰配件", 37),
    NameValuePair("汽车/摩托/自行车", 22),
    NameValuePair("ZIPPO/瑞士军刀/男士精品", 1040),

    NameValuePair("运动/休闲/户外", 18),

    NameValuePair("结束", -1)
};

NameValuePair Citys[] = {
    NameValuePair(""),
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

    NameValuePair("按店铺名", 1),
    NameValuePair("按QQ号码", 2),
    NameValuePair("按店主昵称", 3),

    NameValuePair("结束", -1)
};

NameValuePair Provinces[] = {
    NameValuePair("所有省份"),

    NameValuePair("安徽省"),
    NameValuePair("北京"),
    NameValuePair("福建省"),
    NameValuePair("甘肃省"),
    NameValuePair("广东省"),
    NameValuePair("广西省"),
    NameValuePair("贵州省"),
    NameValuePair("海南省"),
    NameValuePair("河北省"),
    NameValuePair("河南省"),
    NameValuePair("黑龙江省"),
    NameValuePair("湖北省"),
    NameValuePair("湖南省"),
    NameValuePair("吉林省"),
    NameValuePair("江苏省"),
    NameValuePair("江西省"),
    NameValuePair("辽宁省"),
    NameValuePair("内蒙古自治区"),
    NameValuePair("宁夏回族自治区"),
    NameValuePair("青海省"),
    NameValuePair("山东省"),
    NameValuePair("山西省"),
    NameValuePair("陕西省"),
    NameValuePair("上海"),
    NameValuePair("四川省"),
    NameValuePair("天津"),
    NameValuePair("西藏自治区"),
    NameValuePair("新疆维吾尔自治区"),
    NameValuePair("云南省"),
    NameValuePair("浙江省"),
    NameValuePair("重庆"),
    NameValuePair("香港"),
    NameValuePair("澳门"),
    NameValuePair("台湾"),

    NameValuePair("结束", -1)
};


NameValuePair Industries[] = {
    NameValuePair(0, "请选择类别"),

	NameValuePair(3119, "充值卡 - 手机卡 - 电话卡"),
	NameValuePair(12001, "网络游戏虚拟商品"),
	NameValuePair(24590, "腾讯QQ专区"),
	NameValuePair(20501, "女装 - 女士精品"),
	NameValuePair(27158, "女士内衣 - 袜子 - 泳装"),
	NameValuePair(20001, "彩妆 - 香水 - 护肤 - 美体"),
	NameValuePair(2001, "珠宝首饰 - 时尚饰品"),
	NameValuePair(21001, "女士箱包 - 配件"),
	NameValuePair(21036, "女鞋"),
	NameValuePair(28001, "品牌手表 - 流行手表"),
	NameValuePair(21501, "瑞士军刀 - Zippo - 男士饰品"),
	NameValuePair(22001, "男装 - 男包 - 服饰配件"),
	NameValuePair(6001, "运动 - 休闲 - 健身"),
	NameValuePair(6070, "户外 - 军品 - 旅游 - 机票"),
	NameValuePair(22501, "婴幼 - 孕妇用品 - 童装"),
	NameValuePair(3002, "手机"),
	NameValuePair(28038, "笔记本电脑"),
	NameValuePair(1, "电脑硬件 - 台式电脑"),
	NameValuePair(5001, "随身视听 - 音响 - 耳机"),
	NameValuePair(4001, "数码相机 - 摄像机 - 冲印"),
	NameValuePair(28039, "网络服务 - 电脑软件"),
	NameValuePair(28009, "数码配件 - 电子元件"),
	NameValuePair(28046, "办公设备 - 文具 - 耗材"),
	NameValuePair(11001, "动漫周边 - 游戏周边"),
	NameValuePair(23001, "卡通产品 - 玩具 - 模型"),
	NameValuePair(28055, "居家 - 日用 - 床品 - 厨房"),
	NameValuePair(28053, "家电 - 厨房电器 - 保健器械"),
	NameValuePair(28052, "食品 - 保健品 - 茶叶 - 特产"),
	NameValuePair(28054, "家具 - 宜家代购"),
	NameValuePair(28056, "装潢 - 灯具 - 五金 - 卫浴"),
	NameValuePair(8001, "邮币 - 古董 - 字画 - 收藏"),
	NameValuePair(24001, "宠物 - 工艺品 - 礼品 - 鲜花"),
	NameValuePair(23501, "书籍 - 报纸 - 杂志"),
	NameValuePair(24501, "电影 - 电视 - 音乐 - 曲艺"),
	NameValuePair(9001, "汽车 - 摩托 - 自行车"),
	NameValuePair(21591, "成人 - 避孕 - 情趣内衣"),
	NameValuePair(10001, "住宅 - 商铺 - 办公楼租售"),

	NameValuePair(-1, "结束")
};


NameValuePair SalesLevels[] = {
    NameValuePair(0, "不限制"),

	NameValuePair(10, "10分以上"),
	NameValuePair(50, "50分以上"),
	NameValuePair(100, "100分以上"),
	NameValuePair(200, "200分以上"),
	NameValuePair(500, "500分以上"),
	NameValuePair(1000, "1000分以上"),
	NameValuePair(2000, "2000分以上"),
	NameValuePair(5000, "5000分以上"),

	NameValuePair(-1, "结束")
};