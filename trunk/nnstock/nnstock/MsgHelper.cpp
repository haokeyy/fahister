#include "StdAfx.h"
#include "MsgHelper.h"


// 发送按键消息(组合键)
void CMsgHelper::SM_PressKey(HWND hWnd, UINT vSysKey, UINT vKey)
{
    INPUT input[4];	
    memset(input, 0, 4*sizeof(INPUT));

    //设置模拟键盘输入
    input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
    input[0].ki.wVk  = input[3].ki.wVk = vSysKey;
    input[1].ki.wVk  = input[2].ki.wVk = vKey;

    // 释放按键，这非常重要
    input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;

    ::SetForegroundWindow(hWnd);

    SendInput(4, input, sizeof(INPUT));
}


// 发送按键消息(功能键)
void CMsgHelper::SM_PressKey(HWND hWnd, UINT vFuncKey)
{
}

// 发送单击鼠标消息，根据按钮标题和类名
void CMsgHelper::SM_Click(HWND hWnd, char* strBtnText, char* strBtnClass)
{
}

// 发送单击鼠标消息，根据按钮坐标位置
void CMsgHelper::SM_Click(HWND hWnd, int x, int y)
{
}

// 发送设置文本内容消息，文本填入当前获得焦点的控件
void CMsgHelper::SM_Text(HWND hWnd, char* text)
{
}
    
// 发送设置文本内容消息，文本填入指定位置的控件
void CMsgHelper::SM_Text(HWND hWnd, int x, int y, char* text)
{
}
    
// 发送设置文本内容消息，文本填入对应类名的窗口
void CMsgHelper::SM_Text(HWND hWnd, char* strClass, char* text)
{
}