cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DF_EXAMPLE dataframe-example CACHE INTERNAL "")

project(
    ${DF_EXAMPLE}
    VERSION 0.1.0
    DESCRIPTION "dataframe example"
    LANGUAGES CXX
)

file(GLOB_RECURSE DATA_FRAME_EXAMPLE_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main-example.cpp
)

add_executable(${DF_EXAMPLE} 
    ${DATA_FRAME_EXAMPLE_SOURCES} 
)

target_link_libraries(${DF_EXAMPLE} PRIVATE
    ${DATA_FRAME}
)

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${DF_EXAMPLE} PUBLIC
        -O3
    )

elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    target_compile_options(${DF_EXAMPLE} PUBLIC
        -g -Wall -Weverything -Wconversion -Wextra -Wpedantic -O0
    )
    
endif()