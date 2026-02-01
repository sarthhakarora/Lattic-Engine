cmake_minimum_required(VERSION 3.10)

# IMPORTANT: enable BOTH C and C++
project(orbitalPhysics LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# --- executable ---
add_executable(orbitalPhysics
    # C files
    src/main.c
    src/core.c
    src/timer.c
    src/camera.c
    src/planet.c
    src/world.c
    src/ui/ui.c
    src/ui/ui_widgets.c
    src/platform/platform_win32.c

)

# --- include paths ---
target_include_directories(orbitalPhysics PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/nuklear
)

# --- libraries ---
target_link_directories(orbitalPhysics PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(orbitalPhysics PRIVATE raylib)

if (WIN32)
    target_link_libraries(orbitalPhysics PRIVATE winmm gdi32)
endif()

# --- run target ---
add_custom_target(run
    COMMAND orbitalPhysics
    DEPENDS orbitalPhysics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# --- copy assets next to the executable (runtime layout) ---
add_custom_command(TARGET orbitalPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:orbitalPhysics>/assets
)