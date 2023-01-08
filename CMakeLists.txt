cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DATA_FRAME DataFrame CACHE INTERNAL "")

project(
    ${DATA_FRAME}
    VERSION 0.1.0
    DESCRIPTION "DataFrame"
    LANGUAGES CXX
)

file(GLOB_RECURSE DATA_FRAME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

add_executable(${DATA_FRAME} 
    ${DATA_FRAME_SOURCES} 
)

# target_link_libraries(${DATA_FRAME} PRIVATE
# )

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${DATA_FRAME} PUBLIC
        -O3
    )
elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    target_compile_options(${DATA_FRAME} PUBLIC
        -g -Wall -Weverything -Wconversion -Wextra -Wpedantic -O0
    )
endif()

install(
    TARGETS ${DATA_FRAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)