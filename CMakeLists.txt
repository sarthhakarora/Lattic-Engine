cmake_minimum_required(VERSION 3.10)

project(orbitalPhysics LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_executable(orbitalPhysics
    src/main.c
    src/core.c
    src/timer.c
    src/camera.c
    src/planet.c
    src/world.c
    src/app.c
    src/assetmanager.c
    src/texture_streamer.c
    src/ui/ui.c
    src/ui/ui_widgets.c
    src/platform/platform_win32.c
    src/luaapi/luaapi.c
)

target_include_directories(orbitalPhysics PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/nuklear
)

target_link_libraries(orbitalPhysics PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
)
target_link_libraries(orbitalPhysics PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/lua/liblua54.a
)

if (WIN32)
    target_link_libraries(orbitalPhysics PRIVATE winmm gdi32)
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:orbitalPhysics>
    DEPENDS orbitalPhysics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(TARGET orbitalPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:orbitalPhysics>/assets
)

add_custom_command(TARGET orbitalPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/lib/raylib.dll
            $<TARGET_FILE_DIR:orbitalPhysics>
)

add_custom_command(TARGET orbitalPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/lib/lua/lua54.dll
            $<TARGET_FILE_DIR:orbitalPhysics>
)