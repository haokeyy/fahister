#include "StdAfx.h"
#include "MsgHelper.h"
#include "WndHelper.h"


// 发送按键消息(组合键)
void CMsgHelper::SM_PressKey(HWND hMainWnd, UINT vSysKey, UINT vKey)
{
    INPUT input[4];	
    memset(input, 0, 4*sizeof(INPUT));

    //设置模拟键盘输入
    input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[3].ki.wVk = vSysKey;
    input[1].ki.wVk  = input[2].ki.wVk = vKey;

    // 释放按键，这非常重要
    input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hMainWnd);
	::SetFocus(hMainWnd);

    SendInput(4, input, sizeof(INPUT));
}


// 发送按键消息(功能键)
void CMsgHelper::SM_PressKey(HWND hMainWnd, UINT vKey)
{
    INPUT input[2];	
    memset(input, 0, 2*sizeof(INPUT));

    //设置模拟键盘输入
    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[1].ki.wVk = vKey;

    // 释放按键，这非常重要
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hMainWnd);
	::SetFocus(hMainWnd);

    SendInput(2, input, sizeof(INPUT));
}

// 发送单击鼠标消息，根据按钮标题和类名
void CMsgHelper::SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass)
{
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hMainWnd, strBtnText, strBtnClass);
    ::SendMessage(hChildWnd, WM_LBUTTONDOWN, 0, 0);
    ::SendMessage(hChildWnd, WM_LBUTTONUP, 0, 0);
}

// 点击大按钮(能获取到的最小窗口)上的某个位置，根据标题和类名获取大按钮，x,y为相对大按钮的坐标
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

// 发送单击鼠标消息，根据按钮坐标位置
void CMsgHelper::SM_Click(HWND hMainWnd, int x, int y)
{
    RECT rc;
    ::GetWindowRect(hMainWnd, &rc);
    int xx = rc.left + x;
    int yy = rc.top + y;

    SendClick(xx, yy);
}
    
// 发送设置文本内容消息，文本填入指定类名的控件
void CMsgHelper::SM_Text(HWND hMainWnd, char* strEditClass, char* text)
{
    HWND hChildWnd = CWndHelper::FindChildWindowBlur(hMainWnd, "", strEditClass);
    ::SendMessage(hChildWnd, WM_SETTEXT, 0, (LPARAM)text);
}
    
// 发送设置文本内容消息，文本填入对应类名和位置的窗口
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

// 发送设置文本内容消息，文本填入当前获得焦点的控件
void CMsgHelper::SM_Text(HWND hMainWnd, char* text)
{
    ::SetForegroundWindow(hMainWnd);
    SendString(text);
}
