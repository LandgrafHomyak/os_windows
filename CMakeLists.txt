cmake_minimum_required(VERSION 3.20)
project(os_windows C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(include)

add_library(
        os_windows
        SHARED

        src/info.c
        src/init.c
        src/finalize.c
)

target_link_libraries(os_windows PUBLIC COMiC::os::_include)
target_include_directories(os_windows PUBLIC _include)
set_target_properties(os_windows PROPERTIES LINKER_LANGUAGE C)

if (${WIN32})
    set_target_properties(os_windows PROPERTIES OUTPUT_NAME os)
    set_target_properties(os_windows PROPERTIES LINKER_LANGUAGE C)
    add_library(COMiC::os ALIAS os_windows)
endif ()