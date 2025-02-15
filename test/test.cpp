#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>

// 模拟鼠标事件的函数
void moveMouse(int dx, int dy) {
    // 模拟鼠标移动，参数dx, dy 是相对于当前鼠标位置的偏移量
    mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);
}

void clickMouse() {
    // 模拟鼠标点击（左键按下并释放）
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

// 计算两个点之间的距离，用于选择最近的目标
int calculateDistance(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); // 使用平方差避免浮动误差
}

int main() {
    // 示例：假设我们从图像处理得到了一个目标位置
    // 这个目标是通过图像识别获得的，比如在图像中找到了某个物体的位置
    int target_x = 500; // 假设目标的 x 坐标
    int target_y = 300; // 假设目标的 y 坐标

    // 假设屏幕的中心是鼠标的初始位置
    int center_x = 640;  // 屏幕的中心 x 坐标（1280px 宽屏）
    int center_y = 360;  // 屏幕的中心 y 坐标（720px 高屏）

    // 计算目标位置与屏幕中心的偏移
    int dx = target_x - center_x;
    int dy = target_y - center_y;

    // 考虑到显示缩放 (例如 DPI 缩放)，进行适当调整
    const double DPI_ZOOM = 1.5; // 假设是 1.5 倍的 DPI 缩放
    dx = static_cast<int>(dx * DPI_ZOOM);
    dy = static_cast<int>(dy * DPI_ZOOM);

    // 移动鼠标到目标位置
    moveMouse(dx, dy);

    // 如果目标距离很近（例如点击按钮），则进行点击操作
    int min_dis = 900;  // 设置一个距离阈值，假设小于 900 像素就点击
    if (calculateDistance(target_x, target_y, center_x, center_y) < min_dis) {
        clickMouse();
    }

    // 打印调试信息
    std::cout << "Mouse moved to (" << target_x << ", " << target_y << ")" << std::endl;
    std::cout << "DPI-adjusted movement: (" << dx << ", " << dy << ")" << std::endl;

    return 0;
}