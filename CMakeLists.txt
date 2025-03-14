# cmake最低版本要求
cmake_minimum_required(VERSION 3.30)
# 项目名称
project(opencv4110)
# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
# 包含目录
include_directories("D:/Document/UGit/code/opencv/build/include")
include_directories("D:/Document/UGit/code/opencv/build/include/opencv2")
# 库目录
link_directories("D:/Document/UGit/code/opencv/build/x64/vc16/lib")
#生成可执行文件
add_executable(opencv4110 main.cpp
        getWindow.h
        windowShot.h
)

# 链接静态库
target_link_libraries(opencv4110 opencv_world4110d.lib
        dwmapi.lib)