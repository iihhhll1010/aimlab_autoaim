#include <iostream>
#include <vector>
#include <string>
#include "getWindow.h"
#include "windowShot.h"

using namespace std;
using namespace cv;

void moveMouse(int dx, int dy) {
    // ��������ƶ��¼�
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE; // ����Ϊ�ƶ��¼�
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = 0;

    // ���� SendInput ���������¼�
    SendInput(1, &input, sizeof(INPUT));  // ����һ�������¼�
}

void clickMouse() {
    // ģ������������º�̧��
    INPUT input[2] = { 0 };

    // ����������
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input[0].mi.mouseData = 0;
    input[0].mi.dwExtraInfo = 0;

    // ̧��������
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    input[1].mi.mouseData = 0;
    input[1].mi.dwExtraInfo = 0;

    // ���Ͱ��º�̧�����������¼�
    SendInput(2, input, sizeof(INPUT));  // �������������¼������º�̧��
}

//��ȡ���ε����ĵ�
Point getCenterPoint(const Rect& rect) {
    //��ȡ�������Ͻ������Լ����
    int x = rect.x, y = rect.y, width = rect.width, height = rect.height;
    int centerx = x + width / 2, centery = y + height / 2;
    return Point(centerx, centery);
}

int main() {
    //��ȡ���ھ��
    HWND hwnd = getWindowHWND();
    if (hwnd != NULL) {
        cout << "�ҵ����ھ��: " << hwnd << endl;
    }
    else {
        cout << "δ�ҵ����������Ĵ��ڡ�" << endl;
        return 0;
    }

    //���ݴ��ھ��������WindowShotʵ��
    WindowShot windowShot(hwnd);
    //��ȡ������������
    int centerWindowX = windowShot.x + windowShot.width / 2;
    int centerWindowY = windowShot.y + windowShot.height / 2;
    cout << centerWindowX << "," << centerWindowY << endl;
    Scalar lower_cyan(82, 205, 144);  // ��ɫ�½�
    Scalar upper_cyan(255, 255, 255);  // ��ɫ�Ͻ�

    namedWindow("result", WINDOW_FREERATIO);
    moveWindow("result", 0, 0);
    resizeWindow("result", Size(1280, 720));

    while (1) {
        //��ȡ���ڽ�ͼ
        Mat src = windowShot.getWindowMat();
        //���ͼ���Ƿ��ȡ�ɹ�
        if (src.empty()) {
            break;
        }

        //BGRתHSV
        Mat hsv;
        cvtColor(src, hsv, COLOR_BGR2HSV);

        //��ֵ��
        Mat binary;
        inRange(hsv, lower_cyan, upper_cyan, binary);

        //�ҵ�ͼ���е�����
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(binary, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

        //�����������������ĵ�
        Mat result = src.clone();
        vector<Rect> rects;//���ڴ洢��⵽����������
        for (const auto& contour : contours) {
            //����С��Ӿ�������ס����
            Rect boundingBox = boundingRect(contour);
            rects.push_back(boundingBox);
            rectangle(result, boundingBox, Scalar(0, 0, 255), 2); //���ƾ��ο�
        }

        //����Ŀ�����������ĵ�ƫ����,��ɸѡ��Сֵ
        int target_dx = INT_MAX, target_dy = INT_MAX, min_dis = INT_MAX;
        for (const auto& rect : rects) {
            //��ȡ�������ĵ�
            Point rectCenter = getCenterPoint(rect);
            //�����ĵ����ԲȦ
            circle(result, rectCenter, 8, Scalar(255, 0, 0), -1);
            //����x��y�����ƫ����
            int dx = rectCenter.x - centerWindowX + windowShot.x;
            int dy = rectCenter.y - centerWindowY + windowShot.y;
            dy += 15;//�����������
            //�����������ĵ�
            if (dx * dx + dy * dy < min_dis) {
                min_dis = dx * dx + dy * dy;
                target_dx = dx;
                target_dy = dy;
            }
        }

        //�ٿ�����ƶ������
        if (min_dis < INT_MAX) {
            //�ƶ���Ŀ���
            moveMouse(target_dx, target_dy);
            //���
            if (min_dis < 900) {
                clickMouse();
            }
        }
        imshow("result", result);
        waitKey(1);
        //�ͷ��ڴ�
        src.release();
        hsv.release();
        binary.release();
        result.release();
    }

    return 0;
}