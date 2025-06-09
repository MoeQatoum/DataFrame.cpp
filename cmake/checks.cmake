
include_guard()

if(WIN32)
    set(CEX_CXX_STANDARD 23)
elseif(APPLE)
    set(CEX_CXX_STANDARD 26)
elseif(UNIX)
    set(CEX_CXX_STANDARD 26)
endif()