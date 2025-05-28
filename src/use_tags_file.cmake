function(use_tags_file tags_path cross_compilation)
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

        target_include_directories(samplelib 
            PUBLIC
             "${GENERATED_TAGS_DIRECTORY}")

        target_sources(samplelib
            PRIVATE
                "${GENERATED_TAGS_CPP}")

        if (cross_compilation)
            add_custom_command(
                OUTPUT "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMAND "python" "${GENERATED_TAGS_DIRECTORY_ROOT}/generate_tags_classes.py" "${tags_path}" "${DYNAMIC_TAGS_DIRECTORY}" "${COMPILE_TAGS_DIRECTORY}"
                DEPENDS "${tags_path}" "${GENERATED_TAGS_DIRECTORY_ROOT}/generate_tags_classes.py"
                COMMENT "Running the generate executable to generate the html tags files"
                VERBATIM
            )

            add_custom_target(
                generate_files ALL
                DEPENDS "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMENT "Ensuring generated files are up-to-date"
            )
    
            add_dependencies(samplelib generate_files)
        else()
            add_executable(generate "${GENERATED_TAGS_DIRECTORY_ROOT}/generate_tags_classes.cpp")

            add_custom_command(
                OUTPUT "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMAND generate "${tags_path}" "${DYNAMIC_TAGS_DIRECTORY}" "${COMPILE_TAGS_DIRECTORY}"
                DEPENDS generate "${tags_path}" "${GENERATED_TAGS_DIRECTORY_ROOT}/generate_tags_classes.cpp"
                COMMENT "Running the generate executable to generate the html tags files"
                VERBATIM
            )

            add_custom_target(
                generate_files ALL
                DEPENDS "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
                COMMENT "Ensuring generated files are up-to-date"
            )
    
            add_dependencies(samplelib generate_files)
    
            add_dependencies(generate_files generate)
        endif()
        
    else()
        message(FATAL_ERROR "A tags file path must be passed.")
    endif()
endfunction()