#include <iostream>
#include <vector>
#include <string>
#include "getWindow.h"
#include "windowShot.h"

using namespace std;
using namespace cv;

void moveMouse(int dx, int dy) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE; // 设置为移动事件
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = 0;

    //调用 SendInput 函数发送事件
    SendInput(1, &input, sizeof(INPUT));
}

void clickMouse() {
    //模拟鼠标点击：按下和抬起
    INPUT input[2] = { 0 };

    //按下鼠标左键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input[0].mi.mouseData = 0;
    input[0].mi.dwExtraInfo = 0;

    //抬起鼠标左键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    input[1].mi.mouseData = 0;
    input[1].mi.dwExtraInfo = 0;

    //发送按下和抬起鼠标左键的事件
    SendInput(2, input, sizeof(INPUT));
}

//获取矩形的中心点
Point getCenterPoint(const Rect& rect) {
    //获取矩形左上角坐标以及宽高
    int x = rect.x, y = rect.y, width = rect.width, height = rect.height;
    int centerx = x + width / 2, centery = y + height / 2;
    return Point(centerx, centery);
}

int main() {
    //获取窗口句柄
    HWND hwnd = getWindowHWND();
    if (hwnd != NULL) {
        cout << "找到窗口句柄: " << hwnd << endl;
    }
    else {
        cout << "未找到符合条件的窗口。" << endl;
        return 0;
    }

    //根据窗口句柄，创建WindowShot实例
    WindowShot windowShot(hwnd);
    //获取窗口中心坐标
    int centerWindowX = windowShot.x + windowShot.width / 2;
    int centerWindowY = windowShot.y + windowShot.height / 2;
    cout << centerWindowX << "," << centerWindowY << endl;
    Scalar lower_cyan(82, 205, 144);  // 青色下界
    Scalar upper_cyan(255, 255, 255);  // 青色上界

    namedWindow("result", WINDOW_FREERATIO);
    moveWindow("result", 0, 0);
    resizeWindow("result", Size(1280, 720));

    while (1) {
        //获取窗口截图
        Mat src = windowShot.getWindowMat();
        //检查图像是否获取成功
        if (src.empty()) {
            break;
        }

        //BGR转HSV
        Mat hsv;
        cvtColor(src, hsv, COLOR_BGR2HSV);

        //二值化
        Mat binary;
        inRange(hsv, lower_cyan, upper_cyan, binary);

        //找到图像中的轮廓
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(binary, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

        //绘制轮廓并计算中心点
        Mat result = src.clone();
        vector<Rect> rects;//用于存储检测到的轮廓矩形
        for (const auto& contour : contours) {
            //用最小外接矩形来框住轮廓
            Rect boundingBox = boundingRect(contour);
            rects.push_back(boundingBox);
            rectangle(result, boundingBox, Scalar(0, 0, 255), 2); //绘制矩形框
        }

        //计算目标物体与中心的偏移量,并筛选最小值
        int target_dx = INT_MAX, target_dy = INT_MAX, min_dis = INT_MAX;
        for (const auto& rect : rects) {
            //获取矩形中心点
            Point rectCenter = getCenterPoint(rect);
            //在中心点绘制圆圈
            circle(result, rectCenter, 8, Scalar(255, 0, 0), -1);
            //计算x，y方向的偏移量
            int dx = rectCenter.x - centerWindowX + windowShot.x;
            int dy = rectCenter.y - centerWindowY + windowShot.y;
            dy += 15;//标题误差修正
            //计算距离最近的点
            if (dx * dx + dy * dy < min_dis) {
                min_dis = dx * dx + dy * dy;
                target_dx = dx;
                target_dy = dy;
            }
        }

        //操控鼠标移动并点击
        if (min_dis < INT_MAX) {
            //移动到目标点
            moveMouse(target_dx, target_dy);
            //点击
            if (min_dis < 900) {
                clickMouse();
            }
        }
        imshow("result", result);
        waitKey(1);
        //释放内存
        src.release();
        hsv.release();
        binary.release();
        result.release();
    }

    return 0;
}
