#pragma once

class CMsgHelper
{
public:

    // ���Ͱ�����Ϣ
    static void SM_PressKey(HWND hWnd, UINT vKey);

    // ���͵��������Ϣ�����ݰ�ť���������
    static void SM_Click(HWND hWnd, char* strBtnText, char* strBtnClass);

    // ���͵��������Ϣ�����ݰ�ť����λ��
    static void SM_Click(HWND hWnd, int x, int y);

    // ���������ı�������Ϣ���ı����뵱ǰ��ý���Ŀؼ�
    static void SM_Text(HWND hWnd, char* text);
    
    // ���������ı�������Ϣ���ı�����ָ��λ�õĿؼ�
    static void SM_Text(HWND hWnd, int x, int y, char* text);
    
    // ���������ı�������Ϣ���ı������Ӧ�����Ĵ���
    static void SM_Text(HWND hWnd, char* strClass, char* text);
};