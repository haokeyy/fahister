#pragma once

class CMsgHelper
{
public:

    // 发送按键消息(组合键)， 需激活目标窗口
    static void SM_PressKey(HWND hMainWnd, UINT vSysKey, UINT vKey);

    // 发送按键消息(功能键)， 需激活目标窗口
    static void SM_PressKey(HWND hMainWnd, UINT vFuncKey);

    // 点击按钮，根据按钮标题和类名查找按钮
    static void SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass);

    // 点击大按钮(能获取到的最小窗口，如工具栏)上的某个位置(小按钮)，根据标题和类名获取大按钮，x,y为相对大按钮的坐标
    static void SM_Click(HWND hMainWnd, char* strBtnText, char* strBtnClass, int x, int y);

    // 点击鼠标，x, y 为相对窗口(hWnd)的坐标，使用此方法会移动鼠标指针
    static void SM_Click(HWND hMainWnd, int x, int y);

    // 设置文本，文本填入当前获得焦点的控件
    static void SM_Text(HWND hMainWnd, char* text);
    
    // 发送设置文本内容消息，文本填入指定位置的控件
    //static void SM_Text(HWND hWnd, int x, int y, char* text);
    
    // 发送设置文本内容消息，文本填入对应类名的窗口
    static void SM_Text(HWND hMainWnd, char* strClass, char* text);
};
