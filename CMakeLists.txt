cmake_minimum_required(VERSION 3.29)

project(Battleship LANGUAGES C)

set(CMAKE_C_STANDARD 23)

# Add the library
add_library(asst src/asst/asst.c)

# Specify the include directories for the library
target_include_directories(asst PUBLIC src/asst)

# Add the executable
add_executable(Battleship src/main.c)

# Set C standard for the executable
set_property(TARGET Battleship PROPERTY C_STANDARD 23)

# Link the library to the executable
target_link_libraries(Battleship m asst)