#include "StdAfx.h"
#include "MsgHelper.h"


// 发送按键消息
void CMsgHelper::SM_PressKey(HWND hWnd, UINT vKey)
{
    INPUT in;
    
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