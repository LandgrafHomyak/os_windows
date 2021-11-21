cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 99)

add_library(
        __COMiC_os_windows
        SHARED

        ../os.def

        src/info.c
        src/static.c
        src/get_time.c
        src/threads.c
        src/errors.c
        src/mem.c
        src/lock.c
        src/instance.c
)

target_link_libraries(__COMiC_os_windows PUBLIC COMiC::os::_include)
target_include_directories(__COMiC_os_windows PUBLIC include)
set_target_properties(__COMiC_os_windows PROPERTIES LINKER_LANGUAGE C)

if (${WIN32})
    set_target_properties(__COMiC_os_windows PROPERTIES OUTPUT_NAME os)
    add_library(COMiC::os ALIAS __COMiC_os_windows)
endif ()