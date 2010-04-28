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

    NameValuePair(1, "�����ӹ�"),
    NameValuePair(2, "��������"),
    NameValuePair(4, "���̴���"),
    NameValuePair(8, "��ҵ����"),

    NameValuePair("����", -1)
};

NameValuePair Citys[] = {
    NameValuePair("����ʡ��"),

    NameValuePair("����"),
    NameValuePair("����,�㽭,�Ϻ�"), 
    NameValuePair("ɽ��,����,����,�㽭,����,�Ϻ�"), 
    NameValuePair("�㶫,����,����"), 
    NameValuePair("����,����,����,����"), 
    NameValuePair("����,���,�ӱ�,ɽ��,���ɹ�"), 
    NameValuePair("����"), 
    NameValuePair("�Ϻ�"), 
    NameValuePair("���"), 
    NameValuePair("����"),
    NameValuePair("�㽭"), 
    NameValuePair("�㶫"), 
    NameValuePair("����"), 
    NameValuePair("����"), 
    NameValuePair("����"), 
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
    NameValuePair("���ɹ�"), 
    NameValuePair("����"), 
    NameValuePair("�ຣ"), 
    NameValuePair("ɽ��"), 
    NameValuePair("ɽ��"), 
    NameValuePair("����"), 
    NameValuePair("����"), 
    NameValuePair("�Ĵ�"), 
    NameValuePair("����"), 
    NameValuePair("�½�"), 
    NameValuePair("����"), 
    NameValuePair("���"), 
    NameValuePair("����"), 
    NameValuePair("̨��"),

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
