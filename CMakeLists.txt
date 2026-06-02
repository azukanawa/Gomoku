cmake_minimum_required(VERSION 3.10)
project(gomoku C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
if(WIN32)
    add_executable(gomoku WIN32 ${SRC_FILES})
    # MSVC 默认查找 WinMain，需要手动指定入口为 mainCRTStartup
    if(MSVC)
        set_target_properties(gomoku PROPERTIES
            LINK_FLAGS "/ENTRY:mainCRTStartup"
        )
    endif()
else()
    add_executable(gomoku ${SRC_FILES})
endif()
target_include_directories(gomoku PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/raylib/include
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(gomoku PRIVATE -O2 -march=native -flto)
    target_link_options(gomoku PRIVATE -flto)
    add_custom_command(TARGET gomoku POST_BUILD
        COMMAND strip "$<TARGET_FILE:gomoku>"
        COMMENT "Stripping gomoku release binary"
    )
endif()

if(WIN32)
    target_link_libraries(gomoku PRIVATE
        ${PROJECT_SOURCE_DIR}/raylib/lib/windows/libraylib.a
        opengl32 gdi32 winmm pthread
    )
elseif(APPLE)
    target_link_libraries(gomoku PRIVATE
        ${PROJECT_SOURCE_DIR}/raylib/lib/mac/libraylib.a
        "-framework CoreFoundation"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework OpenGL"
    )
else()
    foreach(lib m dl pthread)
        find_library(LIB_${lib} ${lib} REQUIRED)
    endforeach()
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    set(X11_EXT_LIBS Xrandr Xi Xcursor Xinerama)
    foreach(ext ${X11_EXT_LIBS})
        find_library(LIB_${ext} ${ext} REQUIRED)
    endforeach()
    find_library(ASOUND_LIB asound REQUIRED)
    find_path(ASOUND_INCLUDE_DIR alsa/asoundlib.h REQUIRED)

    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(WAYLAND wayland-client wayland-egl xkbcommon)
        if(NOT WAYLAND_FOUND)
            message(WARNING "Wayland dev libs not found.")
        endif()
    endif()

    target_link_libraries(gomoku PRIVATE
        ${PROJECT_SOURCE_DIR}/raylib/lib/linux/libraylib.a
        OpenGL::GL
        X11::X11
        ${LIB_Xrandr} ${LIB_Xi} ${LIB_Xcursor} ${LIB_Xinerama}
        ${ASOUND_LIB}
        ${LIB_m} ${LIB_dl} ${LIB_pthread}
    )

    set(RUN_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/run_gomoku.sh")
    file(WRITE ${RUN_SCRIPT}
        "#!/bin/bash\n"
        "SCRIPT_DIR=\"$(cd \"$(dirname \"$0\")\" && pwd)\"\n"
        "cd \"$SCRIPT_DIR\"\n"
        "if [ \"$XDG_SESSION_TYPE\" = wayland ] && command -v gamescope >/dev/null 2>&1; then\n"
        "    exec gamescope -w 940 -h 740 -S integer -- \"$SCRIPT_DIR/gomoku\" \"$@\"\n"
        "else\n"
        "    exec \"$SCRIPT_DIR/gomoku\" \"$@\"\n"
        "fi\n"
    )
    execute_process(COMMAND chmod +x ${RUN_SCRIPT})
endif()

message(STATUS "Source files: ${SRC_FILES}")
