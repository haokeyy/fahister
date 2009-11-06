#include "StdAfx.h"
#include "MsgHelper.h"
#include "WndHelper.h"


// ���Ͱ�����Ϣ(��ϼ�)
void CMsgHelper::SM_PressKey(HWND hMainWnd, UINT vSysKey, UINT vKey)
{
    INPUT input[4];	
    memset(input, 0, 4*sizeof(INPUT));

    //����ģ���������
    input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[3].ki.wVk = vSysKey;
    input[1].ki.wVk  = input[2].ki.wVk = vKey;

    // �ͷŰ�������ǳ���Ҫ
    input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hMainWnd);
	::SetFocus(hMainWnd);

    SendInput(4, input, sizeof(INPUT));
}


// ���Ͱ�����Ϣ(���ܼ�)
void CMsgHelper::SM_PressKey(HWND hMainWnd, UINT vKey)
{
    INPUT input[2];	
    memset(input, 0, 2*sizeof(INPUT));

    //����ģ���������
    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[1].ki.wVk = vKey;

    // �ͷŰ�������ǳ���Ҫ
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hMainWnd);
	::SetFocus(hMainWnd);

    SendInput(2, input, sizeof(INPUT));
}

// ���͵��������Ϣ�����ݰ�ť���������
void CMsgHelper::SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass)
{
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hMainWnd, strBtnText, strBtnClass);
    ::SendMessage(hChildWnd, WM_LBUTTONDOWN, 0, 0);
    ::SendMessage(hChildWnd, WM_LBUTTONUP, 0, 0);
}

// �����ť(�ܻ�ȡ������С����)�ϵ�ĳ��λ�ã����ݱ����������ȡ��ť��x,yΪ��Դ�ť������
void CMsgHelper::SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass, int x, int y)
{
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hMainWnd, strBtnText, strBtnClass);
        
    ::SendMessage(hChildWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
    ::SendMessage(hChildWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
}

void SendClick(int x, int y)
{    
    int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

    INPUT input[3];	
    memset(input, 0, 3*sizeof(INPUT));

    input[0].type = input[1].type = input[2].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    input[0].mi.dx = input[1].mi.dx = input[2].mi.dx = (long)(x * 65535/cx); 
    input[0].mi.dy = input[1].mi.dy = input[2].mi.dy = (long)(y * 65535/cy);
    input[0].mi.dwExtraInfo = input[0].mi.dwExtraInfo = input[0].mi.dwExtraInfo = 0;//GetMessageExtraInfo();

    SendInput(3, input, sizeof(INPUT));
}

// ���͵��������Ϣ�����ݰ�ť����λ��
void CMsgHelper::SM_Click(HWND hMainWnd, int x, int y)
{
    RECT rc;
    ::GetWindowRect(hMainWnd, &rc);
    int xx = rc.left + x;
    int yy = rc.top + y;

    SendClick(xx, yy);
}
    
// ���������ı�������Ϣ���ı�����ָ�������Ŀؼ�
void CMsgHelper::SM_Text(HWND hMainWnd, char* strEditClass, char* text)
{
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hMainWnd, "", strEditClass);
    ::SendMessage(hChildWnd, WM_SETTEXT, 0, (LPARAM)text);
}
    
// ���������ı�������Ϣ���ı������Ӧ������λ�õĴ���
void CMsgHelper::SM_Text(HWND hMainWnd, char* strEditClass, int x, int y, char* text)
{    
    HWND hChildWnd = CWndHelper::FindChildWindowByPoint(hMainWnd, "", strEditClass, x, y);
    ::SendMessage(hChildWnd, WM_SETTEXT, 0, (LPARAM)text);
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
void CMsgHelper::SM_Text(HWND hMainWnd, char* text)
{
    ::SetForegroundWindow(hMainWnd);
    SendString(text);
}
