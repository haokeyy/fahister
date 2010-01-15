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

    NameValuePair("��ѡ�����", 0),

    NameValuePair("�ֻ�/ͨѶ/��", 13),
    NameValuePair("�������/������Ӱ", 17),
    NameValuePair("MP3/��������/����/�ҵ�", 12),
    NameValuePair("���ԡ����缰����豸", 11),
    NameValuePair("�칫�豸���Ĳ�", 21),
    NameValuePair("��ֵ����/IP��;/����绰", 1041),

    NameValuePair("Ůװ/Ůʿ��Ʒ", 14),
    NameValuePair("Ůʿ���/Ьñ/Ůʿ���", 31),
    NameValuePair("�鱦����/�ֱ�/�۾�", 23),
    NameValuePair("��ױ/��ˮ/����/����", 15),

    NameValuePair("�Ӽ�����/װ��/�ľ�/����", 24),
    NameValuePair("���õ���/������е", 36),
    NameValuePair("���/ģ��/��ͨ��Ʒ", 30),
    NameValuePair("װ��/��𹤾�/����", 26),
    NameValuePair("ͯװ/Ӥ��/�и���Ʒ", 1020),
    NameValuePair("ʳƷ/����Ʒ", 29),
    NameValuePair("����/Ʊ��/����/�廨����", 27),

    NameValuePair("�ղ�Ʒ/�ʱ�/�Ŷ�/�ֻ�", 20),
    NameValuePair("����/����Ʒ/��Ʒ/�ʻ�", 32),

    NameValuePair("�鼮/��־/��ֽ", 34),
    NameValuePair("����/Ӱ��/����/����", 33),

    NameValuePair("����/��Ϸ/�ܱ�", 16),
    NameValuePair("������Ʒ/��Ϸװ��", 35),

    NameValuePair("��װ/�������", 37),
    NameValuePair("����/Ħ��/���г�", 22),
    NameValuePair("ZIPPO/��ʿ����/��ʿ��Ʒ", 1040),

    NameValuePair("�˶�/����/����", 18),

    NameValuePair("����", -1)
};

NameValuePair Citys[] = {
    NameValuePair(""),
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

    NameValuePair("��������", 1),
    NameValuePair("��QQ����", 2),
    NameValuePair("�������ǳ�", 3),

    NameValuePair("����", -1)
};

NameValuePair Provinces[] = {
    NameValuePair("����ʡ��"),

    NameValuePair("����ʡ"),
    NameValuePair("����"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("�㶫ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("�ӱ�ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("������ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("���ɹ�������"),
    NameValuePair("���Ļ���������"),
    NameValuePair("�ຣʡ"),
    NameValuePair("ɽ��ʡ"),
    NameValuePair("ɽ��ʡ"),
    NameValuePair("����ʡ"),
    NameValuePair("�Ϻ�"),
    NameValuePair("�Ĵ�ʡ"),
    NameValuePair("���"),
    NameValuePair("����������"),
    NameValuePair("�½�ά���������"),
    NameValuePair("����ʡ"),
    NameValuePair("�㽭ʡ"),
    NameValuePair("����"),
    NameValuePair("���"),
    NameValuePair("����"),
    NameValuePair("̨��"),

    NameValuePair("����", -1)
};


NameValuePair Industries[] = {
    NameValuePair(0, "��ѡ�����"),

	NameValuePair(3119, "��ֵ�� - �ֻ��� - �绰��"),
	NameValuePair(12001, "������Ϸ������Ʒ"),
	NameValuePair(24590, "��ѶQQר��"),
	NameValuePair(20501, "Ůװ - Ůʿ��Ʒ"),
	NameValuePair(27158, "Ůʿ���� - ���� - Ӿװ"),
	NameValuePair(20001, "��ױ - ��ˮ - ���� - ����"),
	NameValuePair(2001, "�鱦���� - ʱ����Ʒ"),
	NameValuePair(21001, "Ůʿ��� - ���"),
	NameValuePair(21036, "ŮЬ"),
	NameValuePair(28001, "Ʒ���ֱ� - �����ֱ�"),
	NameValuePair(21501, "��ʿ���� - Zippo - ��ʿ��Ʒ"),
	NameValuePair(22001, "��װ - �а� - �������"),
	NameValuePair(6001, "�˶� - ���� - ����"),
	NameValuePair(6070, "���� - ��Ʒ - ���� - ��Ʊ"),
	NameValuePair(22501, "Ӥ�� - �и���Ʒ - ͯװ"),
	NameValuePair(3002, "�ֻ�"),
	NameValuePair(28038, "�ʼǱ�����"),
	NameValuePair(1, "����Ӳ�� - ̨ʽ����"),
	NameValuePair(5001, "�������� - ���� - ����"),
	NameValuePair(4001, "������� - ����� - ��ӡ"),
	NameValuePair(28039, "������� - �������"),
	NameValuePair(28009, "������� - ����Ԫ��"),
	NameValuePair(28046, "�칫�豸 - �ľ� - �Ĳ�"),
	NameValuePair(11001, "�����ܱ� - ��Ϸ�ܱ�"),
	NameValuePair(23001, "��ͨ��Ʒ - ��� - ģ��"),
	NameValuePair(28055, "�Ӽ� - ���� - ��Ʒ - ����"),
	NameValuePair(28053, "�ҵ� - �������� - ������е"),
	NameValuePair(28052, "ʳƷ - ����Ʒ - ��Ҷ - �ز�"),
	NameValuePair(28054, "�Ҿ� - �˼Ҵ���"),
	NameValuePair(28056, "װ�� - �ƾ� - ��� - ��ԡ"),
	NameValuePair(8001, "�ʱ� - �Ŷ� - �ֻ� - �ղ�"),
	NameValuePair(24001, "���� - ����Ʒ - ��Ʒ - �ʻ�"),
	NameValuePair(23501, "�鼮 - ��ֽ - ��־"),
	NameValuePair(24501, "��Ӱ - ���� - ���� - ����"),
	NameValuePair(9001, "���� - Ħ�� - ���г�"),
	NameValuePair(21591, "���� - ���� - ��Ȥ����"),
	NameValuePair(10001, "סլ - ���� - �칫¥����"),

	NameValuePair(-1, "����")
};


NameValuePair SalesLevels[] = {
    NameValuePair(0, "������"),

	NameValuePair(10, "10������"),
	NameValuePair(50, "50������"),
	NameValuePair(100, "100������"),
	NameValuePair(200, "200������"),
	NameValuePair(500, "500������"),
	NameValuePair(1000, "1000������"),
	NameValuePair(2000, "2000������"),
	NameValuePair(5000, "5000������"),

	NameValuePair(-1, "����")
};