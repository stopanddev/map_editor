cmake_minimum_required(VERSION 3.16)
project(MySDL3App C)

find_package(SDL3 REQUIRED)

add_executable(myapp 
    main.c
    configs/init.c
    system_utils/system_utils.c
    grid_utils/grid_utils.c
)

target_link_libraries(myapp PRIVATE SDL3::SDL3 m)
