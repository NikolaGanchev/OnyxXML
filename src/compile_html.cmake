set(COMPILE_TIME_DOCUMENT_COMPILATION_ENABLED "false" CACHE INTERNAL "True if compile time document compilation is enabled" FORCE)

add_custom_command(
    OUTPUT ${FOUND_DOCUMENTS_FILE}
    COMMAND ${CMAKE_COMMAND} -P "${FIND_DOCUMENTS_SCRIPT}"
    COMMENT "Finding documents"
    VERBATIM
)   

add_custom_target(
    find_documents_task
    DEPENDS ${FOUND_DOCUMENTS_FILE}
    COMMENT "Ensuring found documents are up to date"
    VERBATIM
)

# Function to enable compile-time HTML generation
function(register_compile_html_executable EXECUTABLE_TARGET)
        # Parse optional arguments
        set(OPTIONS "")
        set(ONE_VALUE_ARGS "")
        set(MULTI_VALUE_ARGS FILES)
        cmake_parse_arguments(PARSE_ARG "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})
    
        # Ensure target exists
        if(NOT TARGET ${EXECUTABLE_TARGET})
            message(FATAL_ERROR "Target '${EXECUTABLE_TARGET}' does not exist.")
        endif()
    
        # Get list of source files from the target
        get_target_property(USER_SOURCES ${EXECUTABLE_TARGET} SOURCES)
        if(NOT USER_SOURCES)
            message(WARNING "No source files found for target '${EXECUTABLE_TARGET}'.")
            set(ABSOLUTE_USER_SOURCES "")
        endif()
        if(USER_SOURCES)
        
            set(ABSOLUTE_USER_SOURCES "")
            foreach(FILE ${USER_SOURCES})
                get_filename_component(ABS_FILE ${FILE} ABSOLUTE)
                list(APPEND ABSOLUTE_USER_SOURCES ${ABS_FILE})
        endforeach()
    
        endif()
    
        # Get include directories from the target (to find header files)
        get_target_property(USER_INCLUDE_DIRS ${EXECUTABLE_TARGET} INCLUDE_DIRECTORIES)
        if(NOT USER_INCLUDE_DIRS)
            message(WARNING "No include directories found for target '${EXECUTABLE_TARGET}', using project root.")
            set(USER_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}") # Fallback
        endif()
    
        # If specific files are passed, use them instead
        if(PARSE_ARG_FILES)
            set(USER_FILES ${PARSE_ARG_FILES})
        else()
            set(USER_FILES ${ABSOLUTE_USER_SOURCES})
        endif()

        set(FOUND_DOCUMENTS_FILE "${CMAKE_BINARY_DIR}/found_documents.txt" CACHE INTERNAL "The location of the found documents for compile time evaluation" FORCE)
        set(FIND_DOCUMENTS_SCRIPT "${CMAKE_BINARY_DIR}/find_documents.cmake" CACHE INTERNAL "The location of the script that finds documents for compile time evalutation" FORCE)
    
        set(COMPILE_DOCUMENT_REGEX [=[COMPILE_DOCUMENT[ \n\r\t]*\([ \n\r\t]*(((\"[^\"]*\")|([^)\"]))+)[ \n\r\t]*\)]=])

        file(WRITE ${FIND_DOCUMENTS_SCRIPT} "")
        file(APPEND ${FIND_DOCUMENTS_SCRIPT} "file(WRITE \"${FOUND_DOCUMENTS_FILE}\" \"\")\n")
    
        foreach(FILE ${USER_FILES})
            file(APPEND ${FIND_DOCUMENTS_SCRIPT} "file(READ \"${FILE}\" CONTENTS)\n")
            file(APPEND ${FIND_DOCUMENTS_SCRIPT} "string(REGEX MATCHALL \"${COMPILE_DOCUMENT_REGEX}\" MATCHES \${CONTENTS})\n")
            file(APPEND ${FIND_DOCUMENTS_SCRIPT} "foreach(MATCH \${MATCHES})\n")
            file(APPEND ${FIND_DOCUMENTS_SCRIPT} "  file(APPEND \"${FOUND_DOCUMENTS_FILE}\" \"\${MATCH}\\n\")\n")
            file(APPEND ${FIND_DOCUMENTS_SCRIPT} "endforeach()\n")
        endforeach()

        # Set environment variable
        set(COMPILE_TIME_DOCUMENT_COMPILATION_ENABLED "true" CACHE INTERNAL "True if compile time document compilation is enabled" FORCE)
        message(STATUS "Enabled compile-time html generation. Found html stored in: ${FOUND_DOCUMENTS_FILE}")
        
        add_dependencies(${EXECUTABLE_TARGET} find_documents_task)
    # Expose html file location for use in the library
    set(COMPILE_TIME_DOCUMENTS_FILE ${FOUND_DOCUMENTS_FILE} PARENT_SCOPE)
endfunction()