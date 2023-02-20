cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DF_TESTS tests CACHE INTERNAL "")

file(GLOB_RECURSE DF_TESTS_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

add_executable(${DF_TESTS} 
    ${DF_TESTS_SOURCES}
)
target_link_libraries(${DF_TESTS} PRIVATE 
    GTest::gtest_main
    ${DATA_FRAME}
)

include(GoogleTest)
gtest_discover_tests(${DF_TESTS})