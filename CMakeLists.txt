cmake_minimum_required(VERSION 3.16)
project(MySDL3App C)

find_package(SDL3 REQUIRED)

add_executable(myapp main.c)

target_link_libraries(myapp PRIVATE SDL3::SDL3 m)
