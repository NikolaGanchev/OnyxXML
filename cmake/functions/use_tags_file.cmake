function(use_tags_file compileTarget tags_path cross_compilation)
    if(NOT tags_path)
        message(FATAL_ERROR "A tags file path must be passed.")
    endif()

    if(EXISTS "${tags_path}")
        set(GENERATED_TAGS_DIRECTORY_ROOT "${CMAKE_BINARY_DIR}/generated_tags")
        set(GENERATED_TAGS_DIRECTORY "${GENERATED_TAGS_DIRECTORY_ROOT}/output")
        set(DYNAMIC_TAGS_DIRECTORY "${GENERATED_TAGS_DIRECTORY}/dynamic")
        set(COMPILE_TAGS_DIRECTORY "${GENERATED_TAGS_DIRECTORY}/compile")

        set(GENERATED_TAGS_H "${DYNAMIC_TAGS_DIRECTORY}/tags.h")
        set(GENERATED_TAGS_CPP "${DYNAMIC_TAGS_DIRECTORY}/tags.cpp")
        set(GENERATED_TAGS_COMPILE_H "${COMPILE_TAGS_DIRECTORY}/tags.h")

        target_include_directories("${compileTarget}" 
            PUBLIC
             "${GENERATED_TAGS_DIRECTORY}")

        target_sources("${compileTarget}"
            PRIVATE
                "${GENERATED_TAGS_CPP}")

        if (cross_compilation)
            if(NOT DEFINED OnyxXML_XML_PYTHON_CODEGEN_SCRIPT OR NOT EXISTS "${OnyxXML_XML_PYTHON_CODEGEN_SCRIPT}")
                message(FATAL_ERROR "OnyxXML: Python codegen script not found at '${OnyxXML_XML_PYTHON_CODEGEN_SCRIPT}'. Ensure OnyxXML is installed correctly.")
            endif()

            add_custom_command(
                OUTPUT "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMAND "python" "${OnyxXML_XML_PYTHON_CODEGEN_SCRIPT}" "${tags_path}" "${DYNAMIC_TAGS_DIRECTORY}" "${COMPILE_TAGS_DIRECTORY}"
                DEPENDS "${tags_path}" "${OnyxXML_XML_PYTHON_CODEGEN_SCRIPT}"
                COMMENT "Running the generate executable to generate the xml tags files"
                VERBATIM
            )

            add_custom_target(
                generate_files ALL
                DEPENDS "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMENT "Ensuring generated files are up-to-date"
            )
    
            add_dependencies("${compileTarget}" generate_files)
        else()
            if(NOT DEFINED OnyxXML_XML_CPP_CODEGEN_SOURCE OR NOT EXISTS "${OnyxXML_XML_CPP_CODEGEN_SOURCE}")
                message(FATAL_ERROR "OnyxXML: C++ codegen script not found at '${OnyxXML_XML_CPP_CODEGEN_SOURCE}'. Ensure OnyxXML is installed correctly.")
            endif()
            add_executable(generate "${OnyxXML_XML_CPP_CODEGEN_SOURCE}")

            add_custom_command(
                OUTPUT "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMAND generate "${tags_path}" "${DYNAMIC_TAGS_DIRECTORY}" "${COMPILE_TAGS_DIRECTORY}"
                DEPENDS generate "${tags_path}" "${OnyxXML_XML_CPP_CODEGEN_SOURCE}"
                COMMENT "Running the generate executable to generate the xml tags files"
                VERBATIM
            )

            add_custom_target(
                generate_files ALL
                DEPENDS "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMENT "Ensuring generated files are up-to-date"
            )
    
            add_dependencies("${compileTarget}" generate_files)
    
            add_dependencies(generate_files generate)
        endif()
        
    else()
        message(FATAL_ERROR "A tags file path must be passed.")
    endif()
endfunction()