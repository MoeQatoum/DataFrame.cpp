cmake_minimum_required(VERSION 3.27)

set(LIB_DF dataframe CACHE INTERNAL "Project Name")

project(
    ${LIB_DF}
    VERSION 0.1.0
    DESCRIPTION "simple dataframe, in C++"
    LANGUAGES CXX
)

include(cmake/CompilerCheck.cmake)

include(GNUInstallDirs)

add_subdirectory(libdataframe)

if(DF_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(DF_BUILD_BENCH_MARKS)
    add_subdirectory(benchmark)
endif()

if(DF_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(DF_INSTALL)
    include(CMakePackageConfigHelpers)

    set(config_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_DF}-${PROJECT_VERSION})

    write_basic_package_version_file(${LIB_DF}ConfigVersion.cmake COMPATIBILITY ExactVersion)

    configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/${LIB_DF}Config.cmake.in
        ${PROJECT_BINARY_DIR}/${LIB_DF}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_DF}-${PROJECT_VERSION}
        INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
    )

    write_basic_package_version_file(
        ${LIB_DF}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        EXPORT ${LIB_DF}_Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_DF}-${PROJECT_VERSION}
    )

    install(FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_DF}-${PROJECT_VERSION}
    )

    if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()

    set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
    set(CPACK_PACKAGE_CHECKSUM SHA256)
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
    include(CPack)
endif(DF_INSTALL)
