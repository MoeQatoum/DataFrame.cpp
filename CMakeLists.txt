cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DATA_FRAME dataframe CACHE INTERNAL "")

project(
    ${DATA_FRAME}
    VERSION 0.1.0
    DESCRIPTION "simple dataframe object with python pandas like api, in c++"
    LANGUAGES CXX
)

file(GLOB_RECURSE DATA_FRAME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/dataframe.h
    ${CMAKE_CURRENT_LIST_DIR}/cell.h
    ${CMAKE_CURRENT_LIST_DIR}/row_series.h
    ${CMAKE_CURRENT_LIST_DIR}/column_series.h
)

add_library(${DATA_FRAME} INTERFACE
    ${DATA_FRAME_SOURCES} 
)

# target_link_libraries(${DATA_FRAME} PRIVATE
# )

target_include_directories(${DATA_FRAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${DATA_FRAME} INTERFACE
        -O3
    )
elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    target_compile_options(${DATA_FRAME} INTERFACE
        -g -Wall -Weverything -Wconversion -Wextra -Wpedantic -O0
    )
endif()

if(BUILD_CPP_DATA_FRAME_EXAMPLES)
    add_subdirectory(example)
endif()

