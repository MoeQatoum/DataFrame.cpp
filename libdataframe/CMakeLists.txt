
set(DF_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "DataFrame module dir")
set(DF_SRC_DIR ${DF_DIR}/src CACHE PATH "DataFrame module src dir")
set(DF_INCLUDE_DIR ${DF_DIR}/include CACHE PATH "DataFrame module include dir")

file(GLOB_RECURSE DATA_FRAME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/**.hpp
)

add_library(${LIB_DF} INTERFACE)

target_include_directories(${LIB_DF} INTERFACE
    $<BUILD_INTERFACE:${DF_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(${LIB_DF} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
)

if(DF_INSTALL)
    install(
        TARGETS ${LIB_DF}
        EXPORT ${LIB_DF}_Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        DIRECTORY ${DF_INCLUDE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.hpp"
        REGEX "/[^.]+$"
    )
endif(DF_INSTALL)
