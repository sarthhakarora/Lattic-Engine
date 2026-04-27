cmake_minimum_required(VERSION 3.10)

project(latticEngine LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_executable(latticEngine
    src/main.c
    src/core.c
    src/timer.c
    src/camera.c
    src/planet.c
    src/world.c
    src/app.c
    src/assetmanager.c
    src/texture_streamer.c
    src/log.c
    src/ui/ui.c
    src/ui/ui_widgets.c
    src/platform/platform_win32.c
    src/platform/platform_win32_threading.c
    src/luaapi/luaapi.c
)

target_include_directories(latticEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/lua
)

target_link_libraries(latticEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
    ${CMAKE_SOURCE_DIR}/lib/lua/liblua54.a
)

if(WIN32)
    target_link_libraries(latticEngine PRIVATE
        winmm gdi32 opengl32 user32 kernel32
    )
endif()

add_custom_command(TARGET latticEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:latticEngine>/assets
)

add_custom_target(run
    COMMAND $<TARGET_FILE:latticEngine>
    DEPENDS latticEngine
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
) 
