
if(NOT DF_UPDATE_SUBMODULES)
    return()
endif()

find_package(Git QUIET REQUIRED)

if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE EXISTING_SUBMODULES
        RESULT_VARIABLE RETURN_CODE)

    message(STATUS "Updating git submodules:\n${EXISTING_SUBMODULES}")

    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --remote --merge
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE RETURN_CODE)

    if(NOT RETURN_CODE EQUAL "0")
        message(WARNING "Cannot update submodules. git command failed with ${RETURN_CODE}")
        return()
    endif()

    message(STATUS "Git submodules updated successfully.")
endif(GIT_FOUND)
