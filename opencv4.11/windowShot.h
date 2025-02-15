#pragma once
#include <opencv2/opencv.hpp>
#include "getWindow.h"

class WindowShot {
public:
    HWND hwnd;
    int width, height, x, y;
    RECT rect;
    LPVOID shotData;
    HDC screenDC, compatibleDC;
    HBITMAP hBitmap;

    WindowShot(HWND hwnd) : hwnd(hwnd), shotData(nullptr) {
        //�������С�� �ͽ���չʾ 
        if (IsIconic(hwnd)) {
            ShowWindow(hwnd, SW_RESTORE);
        }
        SetForegroundWindow(hwnd); //�������ö�

        //��ȡ���ڵ�λ�úͳߴ�
        rect = getWindowLoc(hwnd);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        x = rect.left;
        y = rect.top;
    }

    //��ȡ���ڵ�λ�úͳߴ�
    RECT getWindowLoc(HWND hwnd) {
        RECT frame;
        DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));
        std::cout << "����λ�ã�(" << frame.left << ", " << frame.top << ")" << std::endl;
        std::cout << "���ڴ�С����" << frame.right - frame.left << ", ��" << frame.bottom - frame.top << ")" << std::endl;
        return frame;
    }

    //��ȡ���ڽ�ͼ
    cv::Mat getWindowMat() {
        //�ͷ�֮ǰ������ڴ�
        delete[] shotData;
        shotData = new char[width * height * 4];  // ÿ������4�ֽ� (BGRA)

        //��ȡ��Ļ���豸������
        screenDC = GetDC(NULL);
        compatibleDC = CreateCompatibleDC(screenDC);

        //����λͼ����
        hBitmap = CreateCompatibleBitmap(screenDC, width, height);
        SelectObject(compatibleDC, hBitmap);

        //����Ļ�Ͻ�ȡ���������ͼ��
        BitBlt(compatibleDC, 0, 0, width, height-30, screenDC, x, y+30, SRCCOPY);

        //��ȡλͼ����
        GetBitmapBits(hBitmap, width * height * 4, shotData);

        //��λͼ����ת��Ϊ OpenCV �� cv::Mat ��ʽ
        cv::Mat windowMat(height, width, CV_8UC4, shotData);

        //�ͷ���Դ
        ReleaseDC(NULL, screenDC);
        DeleteDC(compatibleDC);
        DeleteObject(hBitmap);

        return windowMat;
    }

    ~WindowShot() {
        delete[] shotData;  //�ͷ��ڴ�
    }
};