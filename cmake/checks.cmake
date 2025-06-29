
include_guard()

if(WIN32)
    add_compile_definitions(OS_WINDOWS)
    set(DF_CXX_STANDARD 23)
elseif(APPLE)
    add_compile_definitions(OS_OSX)
    set(DF_CXX_STANDARD 23)
elseif(UNIX)
    add_compile_definitions(OS_LINUX)
    set(DF_CXX_STANDARD 26)
endif()