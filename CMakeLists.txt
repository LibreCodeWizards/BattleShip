cmake_minimum_required(VERSION 3.29)
project(Project C)

set(CMAKE_C_STANDARD 23)

add_executable(Project src/main.c
        src/asst/asst.c
        src/asst/asst.h)
target_link_libraries(Project m)