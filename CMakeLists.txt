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
    ${CMAKE_CURRENT_LIST_DIR}/include/**.h
)

add_library(${DATA_FRAME} INTERFACE
    ${DATA_FRAME_SOURCES} 
)

# target_link_libraries(${DATA_FRAME} PRIVATE
# )

if (DF_QT_IMPLEMENTATION)
    find_package(Qt6 HINTS $ENV{QTDIR} REQUIRED COMPONENTS Core)
    target_link_libraries(${DATA_FRAME} INTERFACE
        Qt6::Core
    )
    target_compile_definitions(${DATA_FRAME} INTERFACE
        QT_IMPLEMENTATION
    )
    message(STATUS "Building dataframelib with QT_IMPLEMENTATION.")
else()
    message(STATUS "Building dataframelib with std implementation.")
endif(DF_QT_IMPLEMENTATION)

target_include_directories(${DATA_FRAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${DATA_FRAME} INTERFACE
        -O3 -Wall -Wextra -Wreorder-ctor
        # -O3 #-Wconversion -Wno-c++98-compat #-static-libstdc++
        #profile CPU with as small performance impact as possible: use -pg
    )
elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    # target_compile_definitions(${DATA_FRAME} INTERFACE _GLIBCXX_DEBUG)

    target_compile_options(${DATA_FRAME} INTERFACE
    -O0 -g -Wconversion # -Wall -Weverything -Wextra -Wpedantic
    )
endif()

if(DF_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(DF_BUILD_BENCH_MARKS)
    add_subdirectory(benchmark)
endif()

if(DF_BUILD_TESTS)
    if(DF_UPDATE_SUBMODULES)
        include(cmake/UpdateSubmodules.cmake)
    endif()
    include (CTest)
    add_subdirectory(third_party/GoogleTest)
    add_subdirectory(tests)
endif()

