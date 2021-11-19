cmake_minimum_required(VERSION 3.20)
project(os_windows C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(include)

add_library(
        os_windows
        SHARED

        src/info.c
)

target_link_libraries(os_windows PUBLIC _os_include)
target_include_directories(os_windows PUBLIC _include)