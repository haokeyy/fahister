#pragma once

class CMsgHelper
{
public:

    // ���Ͱ�����Ϣ(��ϼ�)�� �輤��Ŀ�괰��
    static void SM_PressKey(HWND hWnd, UINT vSysKey, UINT vKey);

    // ���Ͱ�����Ϣ(���ܼ�)�� �輤��Ŀ�괰��
    static void SM_PressKey(HWND hWnd, UINT vFuncKey);

    // �����ť�����ݰ�ť������������Ұ�ť
    static void SM_Click(HWND hWnd, char* strBtnText, char* strBtnClass);

    // �����ť(�ܻ�ȡ������С����)�ϵ�ĳ��λ�ã����ݱ����������ȡ��ť��x,yΪ��Դ�ť������
    static void SM_Click(HWND hWnd, char* strBtnText, char* strBtnClass, int x, int y);

    // �����꣬x, y Ϊ��Դ���(hWnd)�����꣬ʹ�ô˷������ƶ����ָ��
    static void SM_Click(HWND hWnd, int x, int y);

    // �����ı����ı����뵱ǰ��ý���Ŀؼ�
    static void SM_Text(HWND hWnd, char* text);
    
    // ���������ı�������Ϣ���ı�����ָ��λ�õĿؼ�
    //static void SM_Text(HWND hWnd, int x, int y, char* text);
    
    // ���������ı�������Ϣ���ı������Ӧ�����Ĵ���
    static void SM_Text(HWND hWnd, char* strClass, char* text);
};
