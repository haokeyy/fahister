#include "StdAfx.h"
#include "MsgHelper.h"


// ���Ͱ�����Ϣ(��ϼ�)
void CMsgHelper::SM_PressKey(HWND hWnd, UINT vSysKey, UINT vKey)
{
    INPUT input[4];	
    memset(input, 0, 4*sizeof(INPUT));

    //����ģ���������
    input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[3].ki.wVk = vSysKey;
    input[1].ki.wVk  = input[2].ki.wVk = vKey;

    // �ͷŰ�������ǳ���Ҫ
    input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hWnd);

    SendInput(4, input, sizeof(INPUT));
}


// ���Ͱ�����Ϣ(���ܼ�)
void CMsgHelper::SM_PressKey(HWND hWnd, UINT vFuncKey)
{
}

// ���͵��������Ϣ�����ݰ�ť���������
void CMsgHelper::SM_Click(HWND hWnd, char* strBtnText, char* strBtnClass)
{
}

// ���͵��������Ϣ�����ݰ�ť����λ��
void CMsgHelper::SM_Click(HWND hWnd, int x, int y)
{
}

// ���������ı�������Ϣ���ı����뵱ǰ��ý���Ŀؼ�
void CMsgHelper::SM_Text(HWND hWnd, char* text)
{
}
    
// ���������ı�������Ϣ���ı�����ָ��λ�õĿؼ�
void CMsgHelper::SM_Text(HWND hWnd, int x, int y, char* text)
{
}
    
// ���������ı�������Ϣ���ı������Ӧ�����Ĵ���
void CMsgHelper::SM_Text(HWND hWnd, char* strClass, char* text)
{
}