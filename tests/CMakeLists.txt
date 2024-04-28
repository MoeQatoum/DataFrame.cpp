
set(DF_TESTS dataframe-tests CACHE INTERNAL "")

project(
    ${DF_TESTS}
    VERSION 0.1.0
    DESCRIPTION "dataframe tests"
    LANGUAGES CXX
)

set(INSTALL_GTEST OFF)
include(CTest)
add_subdirectory(third_party/GoogleTest)

target_compile_options(gtest PRIVATE -stdlib=libc++ -std=c++23)

file(GLOB_RECURSE DF_TESTS_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(${DF_TESTS} ${DF_TESTS_SOURCES})

target_link_libraries(${DF_TESTS} PRIVATE
    GTest::gtest_main
    dataframe
)

include(../cmake/CompilerCheck.cmake)
include(../cmake/compilerFlags.cmake)

target_compile_options(${DF_TESTS} PRIVATE
    $<$<CONFIG:RELEASE>: ${DF_REL_BUILD_FLAGS} ${DF_STD_LIB_CXX_FLAGS} ${DF_REL_WARN_FLAGS} ${DF_TCMALLOC_FLAGS}>
    $<$<CONFIG:DEBUG>: ${DF_DBG_BUILD_FLAGS} ${DF_STD_LIB_CXX_FLAGS} ${DF_DBG_WARN_FLAGS} ${DF_TCMALLOC_FLAGS}>
)

target_link_options(${DF_TESTS} PRIVATE
    $<$<CONFIG:RELEASE>: -s ${DF_STD_LIB_CXX_LINKER_FLAGS} ${DF_TCMALLOC_LINKER_FLAGS}>
    $<$<CONFIG:DEBUG>: ${DF_STD_LIB_CXX_LINKER_FLAGS} ${DF_TCMALLOC_LINKER_FLAGS}>
)

set_target_properties(${DF_TESTS} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
)

include(GoogleTest)
gtest_discover_tests(${DF_TESTS})
