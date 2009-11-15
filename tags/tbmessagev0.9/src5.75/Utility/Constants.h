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

	NameValuePair(1041, "�ƶ���ͨ��ֵ����/IP��;"),
	NameValuePair(35, "������Ϸ������Ʒ������"),

	NameValuePair(13, "�ֻ�"),
	NameValuePair(17, "�������/������Ӱ "),
	NameValuePair(12, "��������/����/����"),
	NameValuePair(11, "����Ӳ��/̨ʽ��"),
	NameValuePair(1043, "�ʼǱ�����"),
	NameValuePair(1048, "�������/����ԭ���г�"),
	NameValuePair(21, "�칫�豸/�ľ�/�Ĳ�"),
	NameValuePair(1046, "HI-FI/��ͥӰԺ/����/����"),
	NameValuePair(36, "���õ��ӵ���/������е"),
	NameValuePair(1052, "�������/�������"),

	NameValuePair(1055, "Ůʿ����"),
	NameValuePair(1056, "ŮЬ"),
	NameValuePair(14, "Ůװ/Ůʿ��Ʒ"),
	NameValuePair(31, "Ůʿ���/���"),
	NameValuePair(23, "�鱦/��ʯ/���/�ƽ�"),
	NameValuePair(15, "��ױ/��ˮ/����/����"),
	NameValuePair(1044, "Ʒ���ֱ�/�����ֱ�"),
	NameValuePair(1054, "��Ʒ/��������/ʱ����Ʒ"),

	NameValuePair(37, "��װ/�������"),
	NameValuePair(22, "����/���/��װ/Ħ��/���г�"),
	NameValuePair(1040, "ZIPPO/��ʿ����/��Ʒ/�۾�"),
	NameValuePair(18, "�˶�/����/�˶�����"),
	NameValuePair(1045, "����/��Ʒ/����/��Ʊ"),

	NameValuePair(24, "�Ӽ�/����/װ��/����Ʒ"),
	NameValuePair(1050, "�Ҿ�/�Ҿ߶���/�˼Ҵ���"),
	NameValuePair(29, "ʳƷ/��Ҷ/��ʳ/�ز�"),
	NameValuePair(1051, "����Ʒ/�̲�Ʒ"),
	NameValuePair(1049, "������Ʒ/����/����/����"),
	NameValuePair(26, "װ��/�ƾ�/���/����/��ԡ"),
	NameValuePair(1020, "ͯװ/Ӥ��/�и���Ʒ"),

	NameValuePair(16, "����/���/��Ϸ/����"),
	NameValuePair(30, "���/����/ģ��/��ͨ"),

	NameValuePair(20, "�Ŷ�/�ʱ�/�ֻ�/�ղ�"),
	NameValuePair(32, "����/����ʳƷ����Ʒ"),

	NameValuePair(34, "�鼮/��־/��ֽ"),
	NameValuePair(33, "����/Ӱ��/����/����"),

	NameValuePair(1053, "�����Ż�ȯ/��Ʊ/���ݽ���"),
	NameValuePair(1047, "�ʻ��ٵ�/��������/԰�ջ���"),
	NameValuePair(1042, "�������/����/����/����"),
	NameValuePair(27, "������Ʒ/������Ʒ/��Ȥ����"),

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
    NameValuePair("̨��"),

    NameValuePair("����", -1)
};

NameValuePair BizTypes[] = {

    NameValuePair("���о�Ӫģʽ", 0),
    NameValuePair("������", 1),
    NameValuePair("ó����", 2),
    NameValuePair("������", 4),
    NameValuePair("��������������", 8),

    NameValuePair("����", -1)
};

NameValuePair Provinces[] = {
    NameValuePair("����ʡ��"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�㶫"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("����"),
    NameValuePair("�ӱ�"),
    NameValuePair("����"),
    NameValuePair("������"),
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
    NameValuePair("����"),
    NameValuePair("�Ϻ�"),
    NameValuePair("�Ĵ�"),
    NameValuePair("���"),
    NameValuePair("����"),
    NameValuePair("�½�"),
    NameValuePair("����"),
    NameValuePair("�㽭"),
    NameValuePair("����"),
    NameValuePair("HK"),
    NameValuePair("MO"),
    NameValuePair("TW"),
    
    NameValuePair("����", -1)
};