cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DF_BENCHMARK dataframe-benchmark CACHE INTERNAL "")

project(
    ${DF_BENCHMARK}
    VERSION 0.1.0
    DESCRIPTION "dataframe benchmark"
    LANGUAGES CXX
)

file(GLOB_RECURSE DF_BENCHMARK_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

add_executable(${DF_BENCHMARK} 
    ${DF_BENCHMARK_SOURCES} 
)

target_link_libraries(${DF_BENCHMARK} PRIVATE
    ${DATA_FRAME}
)

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${DF_BENCHMARK} PUBLIC
        -O3
    )

elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    target_compile_options(${DF_BENCHMARK} PUBLIC
        -g -Wall -Weverything -Wconversion -Wextra -Wpedantic -O0
    )
    
endif()