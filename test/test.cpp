#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>

// ģ������¼��ĺ���
void moveMouse(int dx, int dy) {
    // ģ������ƶ�������dx, dy ������ڵ�ǰ���λ�õ�ƫ����
    mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);
}

void clickMouse() {
    // ģ���������������²��ͷţ�
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

// ����������֮��ľ��룬����ѡ�������Ŀ��
int calculateDistance(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); // ʹ��ƽ������⸡�����
}

int main() {
    // ʾ�����������Ǵ�ͼ����õ���һ��Ŀ��λ��
    // ���Ŀ����ͨ��ͼ��ʶ���õģ�������ͼ�����ҵ���ĳ�������λ��
    int target_x = 500; // ����Ŀ��� x ����
    int target_y = 300; // ����Ŀ��� y ����

    // ������Ļ�����������ĳ�ʼλ��
    int center_x = 640;  // ��Ļ������ x ���꣨1280px ������
    int center_y = 360;  // ��Ļ������ y ���꣨720px ������

    // ����Ŀ��λ������Ļ���ĵ�ƫ��
    int dx = target_x - center_x;
    int dy = target_y - center_y;

    // ���ǵ���ʾ���� (���� DPI ����)�������ʵ�����
    const double DPI_ZOOM = 1.5; // ������ 1.5 ���� DPI ����
    dx = static_cast<int>(dx * DPI_ZOOM);
    dy = static_cast<int>(dy * DPI_ZOOM);

    // �ƶ���굽Ŀ��λ��
    moveMouse(dx, dy);

    // ���Ŀ�����ܽ�����������ť��������е������
    int min_dis = 900;  // ����һ��������ֵ������С�� 900 ���ؾ͵��
    if (calculateDistance(target_x, target_y, center_x, center_y) < min_dis) {
        clickMouse();
    }

    // ��ӡ������Ϣ
    std::cout << "Mouse moved to (" << target_x << ", " << target_y << ")" << std::endl;
    std::cout << "DPI-adjusted movement: (" << dx << ", " << dy << ")" << std::endl;

    return 0;
}