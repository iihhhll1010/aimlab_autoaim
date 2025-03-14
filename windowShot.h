#pragma once
#include <opencv2/opencv.hpp>
#include "getWindow.h"

class WindowShot {
public:
    [[maybe_unused]] HWND hwnd;
    int width, height, x, y;
    RECT rect{};
    LPVOID shotData;
    HDC screenDC{}, compatibleDC{};
    HBITMAP hBitmap{};

    explicit WindowShot(HWND hwnd) : hwnd(hwnd), shotData(nullptr) {
        //如果窗口小化 就将其展示
        if (IsIconic(hwnd)) {
            ShowWindow(hwnd, SW_RESTORE);
        }
        SetForegroundWindow(hwnd); //将窗口置顶

        //获取窗口的位置和尺寸
        rect = getWindowLoc(hwnd);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        x = rect.left;
        y = rect.top;
    }

    //获取窗口的位置和尺寸
    static RECT getWindowLoc(HWND hwnd) {
        RECT frame;
        DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));
        std::cout << "窗口位置：(" << frame.left << ", " << frame.top << ")" << std::endl;
        std::cout << "窗口大小：宽" << frame.right - frame.left << ", 高" << frame.bottom - frame.top << ")" << std::endl;
        return frame;
    }

    //获取窗口截图
    cv::Mat getWindowMat() {
        //释放之前分配的内存
        delete[] shotData;
        shotData = new char[width * height * 4];  // 每个像素4字节 (BGRA)

        //获取屏幕的设备上下文
        screenDC = GetDC(nullptr);
        compatibleDC = CreateCompatibleDC(screenDC);

        //创建位图对象
        hBitmap = CreateCompatibleBitmap(screenDC, width, height);
        SelectObject(compatibleDC, hBitmap);

        //从屏幕上截取窗口区域的图像
        BitBlt(compatibleDC, 0, 0, width, height-30, screenDC, x, y+30, SRCCOPY);

        //获取位图数据
        GetBitmapBits(hBitmap, width * height * 4, shotData);

        //将位图数据转换为 OpenCV 的 cv::Mat 格式
        cv::Mat windowMat(height, width, CV_8UC4, shotData);

        //释放资源
        ReleaseDC(nullptr, screenDC);
        DeleteDC(compatibleDC);
        DeleteObject(hBitmap);

        return windowMat;
    }

    ~WindowShot() {
        delete[] shotData;  //释放内存
    }
};
