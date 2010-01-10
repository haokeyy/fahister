// Constants.h
// ��������

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

    NameValuePair("�������", 0),

    NameValuePair(16, "Ůװ/Ůʿ��Ʒ"),
    NameValuePair(30, "��װ"),
    NameValuePair(50006843, "ŮЬ"),
    NameValuePair(50011740, "������Ь"),
    NameValuePair(50010388, "�˶�Ь"),
    NameValuePair(1625, "Ůʿ����/��ʿ����/�Ҿӷ�"),
    NameValuePair(50006842, "���Ƥ��/����Ů��/�а�"),
    NameValuePair(50011699, "�˶���/�˶���/�������"),
    NameValuePair(50010404, "�������/Ƥ��/ñ��/Χ��"),
    NameValuePair(1512, "�ֻ�"),
    NameValuePair(50012081, "������Ʒ�ֻ�"),
    NameValuePair(1101, "�ʼǱ�����"),
    NameValuePair(11, "����Ӳ��/̨ʽ����/�����豸"),
    NameValuePair(1201, "MP3/MP4/iPod/¼����"),
    NameValuePair(14, "�������/�����/ͼ�γ�ӡ"),
    NameValuePair(50008090, "3C��������г�"),
    NameValuePair(50008168, "�������/�������"),
    NameValuePair(50012164, "���濨/U��/�ƶ��洢"),
    NameValuePair(20, "����/���/��Ϸ/����"),
    NameValuePair(50007218, "�칫�豸/�ľ�/�Ĳ�"),
    NameValuePair(50011972, "Ӱ������"),
    NameValuePair(1801, "���ݻ���/����/����"),
    NameValuePair(50010788, "��ױ/��ˮ/����/����"),
    NameValuePair(50002768, "���˻���/����/��Ħ����"),
    NameValuePair(21, "�Ӽ�����/��������/��ԡϴԡ"),
    NameValuePair(2128, "ʱ�м���/����Ʒ/ʮ����"),
    NameValuePair(50008164, "�Ҿ�/�Ҿ߶���/�˼Ҵ���"),
    NameValuePair(50008163, "�ҷ�/��Ʒ/��̺/����"),
    NameValuePair(27, "װ��/�ƾ�/���/����/��ԡ"),
    NameValuePair(50012472, "����ʳƷ"),
    NameValuePair(50002766, "ʳƷ/��Ҷ/��ʳ/�ز�"),
    NameValuePair(35, "�̷�/��Ƭ/ĸӤ��Ʒ"),
    NameValuePair(50005998, "�������/ͯ��/ͯ��/���"),
    NameValuePair(50008165, "ͯװ/ͯЬ/�и�װ"),
    NameValuePair(29, "����/����ʳƷ����Ʒ"),
    NameValuePair(50012082, "��������"),
    NameValuePair(50012100, "�������"),
    NameValuePair(33, "�鼮/��־/��ֽ"),
    NameValuePair(34, "����/Ӱ��/����/����"),
    NameValuePair(50010728, "�˶�/�٤/����/������Ʒ"),
    NameValuePair(2203, "����/��ɽ/ҰӪ/��ˮ"),
    NameValuePair(26, "����/���/��װ/Ħ��/���г�"),
    NameValuePair(1705, "��Ʒ/��������/ʱ����Ʒ"),
    NameValuePair(50011397, "�鱦/��ʯ/���/�ƽ�"),
    NameValuePair(50005700, "Ʒ���ֱ�/�����ֱ�"),
    NameValuePair(23, "�Ŷ�/�ʱ�/�ֻ�/�ղ�"),
    NameValuePair(25, "���/ģ��/����/��ż"),
    NameValuePair(28, "ZIPPO/��ʿ����/�۾�"),
    NameValuePair(40, "��ѶQQר��"),
    NameValuePair(50011665, "����װ��/��Ϸ��/�ʺ�/����"),
    NameValuePair(99, "������Ϸ�㿨"),
    NameValuePair(50004958, "�ƶ�/��ͨ/С��ͨ��ֵ����"),
    NameValuePair(50008907, "IP��/����绰/�ֻ�����"),
    NameValuePair(2813, "������Ʒ/������Ʒ/��Ȥ����"),
    NameValuePair(50003754, "����װ��/�������/ͼƬ�洢"),
    NameValuePair(50007216, "�ʻ��ٵ�/��������/԰�ջ���"),
    NameValuePair(50008075, "�ݳ�/����/�Ժ������ۿ�ȯ"),

    NameValuePair("����", -1)
};

NameValuePair Citys[] = {
    NameValuePair("���е���"),

    NameValuePair("����"),
    NameValuePair("�Ϻ�"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�Ͼ�"),
    NameValuePair("�人"),
    NameValuePair("���"),
    NameValuePair("�ɶ�"),
    NameValuePair("������"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�ൺ"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("��ɳ"),
    NameValuePair("�Ϸ�"),
    NameValuePair("����"),
    NameValuePair("�ϲ�"),
    NameValuePair("֣��"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("ʯ��ׯ"),
    NameValuePair("��ݸ"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�㶫"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�ӱ�"),
    NameValuePair("������"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("���ɹ�"),
    NameValuePair("����"),
    NameValuePair("�ຣ"),
    NameValuePair("ɽ��"),
    NameValuePair("ɽ��"),
    NameValuePair("����"),
    NameValuePair("�Ĵ�"),
    NameValuePair("�½�"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�㽭"),
    NameValuePair("���е���"),
    NameValuePair("����"),

    NameValuePair("����", -1)
};

NameValuePair Speeds[] = {

    NameValuePair("1��ÿ��", 1000),
    NameValuePair("2��ÿ��", 2000),
    NameValuePair("3��ÿ��", 3000),
    NameValuePair("5��ÿ��", 5000),
    NameValuePair("8��ÿ��", 8000),
    NameValuePair("10��ÿ��", 10000),
    NameValuePair("15��ÿ��", 15000),

    NameValuePair("����", -1)
};
