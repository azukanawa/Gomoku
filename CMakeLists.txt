cmake_minimum_required(VERSION 3.10)
project(gomoko)

# 自动收集 C 和 C++ 源码
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 创建可执行程序
add_executable(gomoko ${SRC_FILES})

# 只为当前项目添加头文件路径（现代 CMake 标准）
target_include_directories(gomoko PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)