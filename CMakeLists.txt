cmake_minimum_required(VERSION 3.10)
project(gomoku C)

# C语言标准
set(CMAKE_C_STANDARD 99)

# 自动收集源码
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
)

# 生成可执行文件
add_executable(gomoku ${SRC_FILES})

# 头文件目录（所有平台通用）
target_include_directories(gomoku PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/raylib/include
)

# ──────────── 平台判断与依赖处理 ────────────
if(WIN32)
    # Windows
    target_link_directories(gomoku PRIVATE
        ${PROJECT_SOURCE_DIR}/raylib/lib/windows
    )
    target_link_libraries(gomoku PRIVATE
        raylib
        opengl32
        gdi32
        winmm
        pthread
    )

elseif(APPLE)
    # macOS
    target_link_directories(gomoku PRIVATE
        ${PROJECT_SOURCE_DIR}/raylib/lib/mac
    )
    target_link_libraries(gomoku PRIVATE
        raylib
        "-framework CoreFoundation"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework OpenGL"
    )

else()
    # ── Linux 及类似系统 ──
    # 1. 基础库检查 (libm, libdl, libpthread) —— 几乎总是存在，但仍做确认
    foreach(lib m dl pthread)
        find_library(LIB_${lib} ${lib} REQUIRED)
    endforeach()

    # 2. OpenGL 检查（带发行版指引）
    find_package(OpenGL QUIET)
    if(NOT OPENGL_FOUND)
        message(FATAL_ERROR
            "OpenGL development files not found!\n"
            "Install them using your package manager:\n"
            "  Ubuntu/Debian   : sudo apt install libgl1-mesa-dev libglu1-mesa-dev\n"
            "  Fedora          : sudo dnf install mesa-libGL-devel\n"
            "  Arch            : sudo pacman -S mesa\n"
            "  openSUSE        : sudo zypper install Mesa-libGL-devel\n"
            "  Void            : sudo xbps-install mesa MesaLib-devel"
        )
    endif()

    # 3. X11 核心库检查
    find_package(X11 QUIET)
    if(NOT X11_FOUND)
        message(FATAL_ERROR
            "X11 development files not found!\n"
            "Install them using your package manager:\n"
            "  Ubuntu/Debian   : sudo apt install libx11-dev\n"
            "  Fedora          : sudo dnf install libX11-devel\n"
            "  Arch            : sudo pacman -S libx11\n"
            "  openSUSE        : sudo zypper install libX11-devel\n"
            "  Void            : sudo xbps-install libX11-devel"
        )
    endif()

    # 4. X11 扩展库检查（Xrandr, Xi, Xcursor, Xinerama）
    set(X11_EXT_LIBS Xrandr Xi Xcursor Xinerama)
    set(X11_EXT_NOTFOUND "")
    foreach(ext ${X11_EXT_LIBS})
        find_library(LIB_${ext} ${ext})
        if(NOT LIB_${ext})
            list(APPEND X11_EXT_NOTFOUND ${ext})
        endif()
    endforeach()
    if(X11_EXT_NOTFOUND)
        string(REPLACE ";" ", " missing_list "${X11_EXT_NOTFOUND}")
        message(FATAL_ERROR
            "Missing X11 extension libraries: ${missing_list}\n"
            "Install them with your package manager:\n"
            "  Ubuntu/Debian   : sudo apt install libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev\n"
            "  Fedora          : sudo dnf install libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel\n"
            "  Arch            : sudo pacman -S libxrandr libxi libxcursor libxinerama\n"
            "  openSUSE        : sudo zypper install libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel\n"
            "  Void            : sudo xbps-install libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel"
        )
    endif()

    # 5. ALSA 音频库检查（使用 find_library 和 find_path 确保开发包完整）
    find_library(ASOUND_LIB asound)
    find_path(ASOUND_INCLUDE_DIR alsa/asoundlib.h)
    if(NOT ASOUND_LIB OR NOT ASOUND_INCLUDE_DIR)
        message(FATAL_ERROR
            "ALSA development files not found!\n"
            "Install them with your package manager:\n"
            "  Ubuntu/Debian   : sudo apt install libasound2-dev\n"
            "  Fedora          : sudo dnf install alsa-lib-devel\n"
            "  Arch            : sudo pacman -S alsa-lib\n"
            "  openSUSE        : sudo zypper install alsa-devel\n"
            "  Void            : sudo xbps-install alsa-lib-devel"
        )
    endif()

    # 6. 可选：Wayland 支持检查（若缺失只警告，不阻止构建）
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(WAYLAND_CLIENT wayland-client)
        pkg_check_modules(WAYLAND_EGL wayland-egl)
        pkg_check_modules(XKBCOMMON xkbcommon)
        if(NOT WAYLAND_CLIENT_FOUND OR NOT WAYLAND_EGL_FOUND OR NOT XKBCOMMON_FOUND)
            message(WARNING
                "Wayland development libraries not found.\n"
                "If you plan to build raylib with native Wayland support, install them:\n"
                "  Ubuntu/Debian   : sudo apt install libwayland-dev libxkbcommon-dev\n"
                "  Fedora          : sudo dnf install wayland-devel libxkbcommon-devel\n"
                "  Arch            : sudo pacman -S wayland libxkbcommon\n"
                "  openSUSE        : sudo zypper install wayland-devel libxkbcommon-devel\n"
                "  Void            : sudo xbps-install wayland-devel libxkbcommon-devel"
            )
        endif()
    else()
        message(WARNING
            "pkg-config not found, unable to check Wayland dependencies.\n"
            "If you need Wayland, install pkg-config and the required Wayland packages."
        )
    endif()

    # ── 链接配置 ──
    target_link_directories(gomoku PRIVATE
        ${PROJECT_SOURCE_DIR}/raylib/lib/linux
    )
    target_link_libraries(gomoku PRIVATE
        raylib
        OpenGL::GL
        X11::X11
        ${LIB_Xrandr}
        ${LIB_Xi}
        ${LIB_Xcursor}
        ${LIB_Xinerama}
        ${ASOUND_LIB}
        ${LIB_m}
        ${LIB_dl}
        ${LIB_pthread}
    )
endif()

message(STATUS "CMake 找到的源文件: ${SRC_FILES}")