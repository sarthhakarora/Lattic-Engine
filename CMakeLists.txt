cmake_minimum_required(VERSION 3.10)

# IMPORTANT: enable BOTH C and C++
project(orbitalPhysics LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# --- executable ---
add_executable(orbitalPhysics
    # C files
    src/main.c
    src/game.c
    src/timer.c
    src/camera.c
    src/planet.c
    src/world.c
    src/ui.c

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