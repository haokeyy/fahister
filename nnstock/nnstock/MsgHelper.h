#pragma once

class CMsgHelper
{
public:

    // ���Ͱ�����Ϣ(��ϼ�)�� �輤��Ŀ�괰��
    static void SM_PressKey(HWND hMainWnd, UINT vSysKey, UINT vKey);

    // ���Ͱ�����Ϣ(���ܼ�)�� �輤��Ŀ�괰��
    static void SM_PressKey(HWND hMainWnd, UINT vFuncKey);

    // �����ť�����ݰ�ť������������Ұ�ť
    static void SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass);

    // �����ť(�ܻ�ȡ������С���ڣ��繤����)�ϵ�ĳ��λ��(С��ť)�����ݱ����������ȡ��ť��x,yΪ��Դ�ť������
    static void SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass, int x, int y);

    // �����꣬x, y Ϊ��Դ���(hWnd)�����꣬ʹ�ô˷������ƶ����ָ��
    static void SM_Click(HWND hMainWnd, int x, int y);

    // �����ı����ݣ��ı������Ӧ���ݺ������Ĵ���
    static void SM_Text(HWND hMainWnd, char* strEditClass, char* text);

    // �����ı����ݣ��ı������Ӧ���ݺ�����������λ����(x,y)�Ĵ���
    static void SM_Text(HWND hMainWnd, char* strEditClass, int x, int y, char* text);

    // �����ı����ݣ��ı����뵱ǰ��ý���Ŀؼ�
    static void SM_Text(HWND hMainWnd, char* text); 
    
};
