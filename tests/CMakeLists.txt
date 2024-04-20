
set(DF_TESTS dataframe-tests CACHE INTERNAL "")

project(
    ${DF_TESTS}
    VERSION 0.1.0
    DESCRIPTION "dataframe tests"
    LANGUAGES CXX
)

set(INSTALL_GTEST OFF)
include(CTest)

if(DF_UPDATE_SUBMODULES)
    include(${CMAKE_SOURCE_DIR}/cmake/UpdateSubmodules.cmake)
endif()

add_subdirectory(third_party/GoogleTest)

file(GLOB_RECURSE DF_TESTS_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(${DF_TESTS} ${DF_TESTS_SOURCES})

target_link_libraries(${DF_TESTS} PRIVATE
    GTest::gtest_main
    dataframe
)

include(${CMAKE_SOURCE_DIR}/cmake/CompilerCheck.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/buildFlags.cmake)

target_compile_options(${DF_TESTS} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: ${LIB_DF_REL_BUILD_FLAGS} ${LIB_DF_REL_WARN_FLAGS} ${TCMALLOC_FLAGS}>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>: ${LIB_DF_DBG_BUILD_FLAGS} ${LIB_DF_DBG_WARN_FLAGS} ${TCMALLOC_FLAGS}>>
)

target_link_options(${DF_TESTS} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: -s ${TCMALLOC_LINKER_FLAGS}>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>: ${TCMALLOC_LINKER_FLAGS}>>
)

set_target_properties(${DF_TESTS} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
)

include(GoogleTest)
gtest_discover_tests(${DF_TESTS})
