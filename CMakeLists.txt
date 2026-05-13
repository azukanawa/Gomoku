cmake_minimum_required(VERSION 3.10)
project(gomoku)

# C语言标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# 自动收集源码
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 生成可执行文件
add_executable(gomoku ${SRC_FILES}
    src/Button.c
    include/Button.h
    src/Rendering.c
    include/Rendering.h)

target_include_directories(gomoku PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/raylib/include
)

# 链接库目录：raylib 的 lib 文件夹
target_link_directories(gomoku PRIVATE
    ${PROJECT_SOURCE_DIR}/raylib/lib
)

# 链接 raylib + MinGW 必需依赖
target_link_libraries(gomoku PRIVATE
    raylib
    opengl32
    gdi32
    winmm
    pthread
)

# Windows 图形窗口（去掉控制台）
if(WIN32)
    set_target_properties(gomoku PROPERTIES
        WIN32_EXECUTABLE off
    )
endif()

message(STATUS "CMake找到的源文件: ${SRC_FILES}")