#pragma once
#include <dwmapi.h>
#include <windows.h> 
#include <vector> 
#include <string>  
#include <iostream>

//需要查找的目标窗口关键词
#define TARGET_WindowTitle "aimlab_tb"

//目标窗口的句柄
HWND targetWindow = NULL;
//用于临时存放窗口标题
char windowText[256];

//处理窗口数据的回调函数
BOOL CALLBACK WindowEnumerationCallback(HWND hwnd, LPARAM lParam) {
    //通过IsWindow函数检查窗口是否有效  
    if (IsWindow(hwnd)) {
        //通过IsWindowEnabled函数检查窗口是否启用  
        if (IsWindowEnabled(hwnd)) {
            //通过IsWindowVisible函数检查窗口是否可见  
            if (IsWindowVisible(hwnd)) {
                //获取窗口标题
                GetWindowTextA(hwnd, windowText, sizeof(windowText));

                //判断窗口标题是否包含目标字符串
                std::string windowTitle(windowText);
                if (windowTitle.find(TARGET_WindowTitle) != std::string::npos) {
                    std::cout << "找到目标窗口: " << windowTitle << std::endl;
                    targetWindow = hwnd;
                    //返回 FALSE 停止枚举
                    return FALSE;
                }
            }
        }
    }
    //继续枚举其他窗口  
    return TRUE;
}

//返回包含目标窗口关键词的桌面窗口句柄
HWND getWindowHWND()
{
    //初始化结果变量
    targetWindow = NULL;
    //使用EnumWindows函数枚举所有窗口，并传递给回调函数处理  
    EnumWindows(WindowEnumerationCallback, NULL);
    //返回目标窗口句柄
    return targetWindow;
}