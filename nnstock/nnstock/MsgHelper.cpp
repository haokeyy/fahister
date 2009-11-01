#include "StdAfx.h"
#include "MsgHelper.h"
#include "WndHelper.h"


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
void CMsgHelper::SM_PressKey(HWND hWnd, UINT vKey)
{
    INPUT input[2];	
    memset(input, 0, 2*sizeof(INPUT));

    //����ģ���������
    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[1].ki.wVk = vKey;

    // �ͷŰ�������ǳ���Ҫ
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hWnd);

    SendInput(2, input, sizeof(INPUT));
}

// ���͵��������Ϣ�����ݰ�ť���������
void CMsgHelper::SM_Click(HWND hWnd, char* strBtnText, char* strBtnClass)
{
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hWnd, strBtnText, strBtnClass);
    ::SendMessage(hChildWnd, WM_LBUTTONDOWN, 0, 0);
    ::SendMessage(hChildWnd, WM_LBUTTONUP, 0, 0);
}

// ���͵��������Ϣ�����ݰ�ť����λ��
void CMsgHelper::SM_Click(HWND hWnd, int x, int y)
{
}

void SendString(LPCTSTR str)
{
	INPUT inp[2];
	memset(inp,0,sizeof(INPUT));
	inp[0].type = INPUT_KEYBOARD;
	inp[0].ki.dwFlags = KEYEVENTF_UNICODE; // to avoid shift, etc.
	inp[1] = inp[0];
	inp[1].ki.dwFlags = KEYEVENTF_KEYUP;

	for (LPCTSTR p=str; *p; p++) {
        inp[0].ki.wScan = inp[1].ki.wScan = *p;
		SendInput(2, inp, sizeof(INPUT));
	}
}

// ���������ı�������Ϣ���ı����뵱ǰ��ý���Ŀؼ�
void CMsgHelper::SM_Text(HWND hWnd, char* text)
{
    ::SetForegroundWindow(hWnd);
    SendString(text);
}
    
// ���������ı�������Ϣ���ı�����ָ��λ�õĿؼ�
void CMsgHelper::SM_Text(HWND hWnd, int x, int y, char* text)
{
    HWND hChildWnd = CWndHelper::FindChildWindowByPoint(hWnd, x, y);
    ::SendMessage(hChildWnd, WM_SETTEXT, 0, (LPARAM)text);
}
    
// ���������ı�������Ϣ���ı������Ӧ�����Ĵ���
void CMsgHelper::SM_Text(HWND hWnd, char* strClass, char* text)
{    
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hWnd, "", strClass);
    ::SendMessage(hChildWnd, WM_SETTEXT, 0, (LPARAM)text);
}