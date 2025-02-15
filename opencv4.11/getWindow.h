#pragma once
#include <dwmapi.h>
#include <windows.h> 
#include <vector> 
#include <string>  
#include <iostream>

//��Ҫ���ҵ�Ŀ�괰�ڹؼ���
#define TARGET_WindowTitle "aimlab_tb"

//Ŀ�괰�ڵľ��
HWND targetWindow = NULL;
//������ʱ��Ŵ��ڱ���
char windowText[256];

//���������ݵĻص�����
BOOL CALLBACK WindowEnumerationCallback(HWND hwnd, LPARAM lParam) {
    //ͨ��IsWindow������鴰���Ƿ���Ч  
    if (IsWindow(hwnd)) {
        //ͨ��IsWindowEnabled������鴰���Ƿ�����  
        if (IsWindowEnabled(hwnd)) {
            //ͨ��IsWindowVisible������鴰���Ƿ�ɼ�  
            if (IsWindowVisible(hwnd)) {
                //��ȡ���ڱ���
                GetWindowTextA(hwnd, windowText, sizeof(windowText));

                //�жϴ��ڱ����Ƿ����Ŀ���ַ���
                std::string windowTitle(windowText);
                if (windowTitle.find(TARGET_WindowTitle) != std::string::npos) {
                    std::cout << "�ҵ�Ŀ�괰��: " << windowTitle << std::endl;
                    targetWindow = hwnd;
                    //���� FALSE ֹͣö��
                    return FALSE;
                }
            }
        }
    }
    //����ö����������  
    return TRUE;
}

//���ذ���Ŀ�괰�ڹؼ��ʵ����洰�ھ��
HWND getWindowHWND()
{
    //��ʼ���������
    targetWindow = NULL;
    //ʹ��EnumWindows����ö�����д��ڣ������ݸ��ص���������  
    EnumWindows(WindowEnumerationCallback, NULL);
    //����Ŀ�괰�ھ��
    return targetWindow;
}