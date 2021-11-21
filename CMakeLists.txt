cmake_minimum_required(VERSION 3.20)
project(COMiC_os_windows C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(include)

add_library(
        __COMiC_os_windows
        SHARED

        include/os.def

        src/info.c
        src/os.c
        src/get_time.c
        src/threads.c
        src/errors.c
        src/mem.c
        src/lock.c)

target_link_libraries(__COMiC_os_windows PUBLIC COMiC::os::_include)
target_include_directories(__COMiC_os_windows PUBLIC _include)
set_target_properties(__COMiC_os_windows PROPERTIES LINKER_LANGUAGE C)

if (${WIN32})
    set_target_properties(__COMiC_os_windows PROPERTIES OUTPUT_NAME os)
    install(TARGETS __COMiC_os_windows DESTINATION .)
    add_library(COMiC::os ALIAS __COMiC_os_windows)
endif ()