cmake_minimum_required(VERSION 3.16)
project(MySDL3App C)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug)
find_package(SDL3 REQUIRED)

add_executable(myapp 
    main.c
    configs/init.c
    system_utils/system_utils.c
    grid_utils/grid_utils.c
    left_menu/left_menu.c
    controls/controls.c
    motion_utils/motion_utils.c
    tile_map_utils/tile_map_utils.c
)

target_link_libraries(myapp PRIVATE SDL3::SDL3 m)
