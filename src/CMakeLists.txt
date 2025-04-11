# Sample static library
add_library(samplelib STATIC)

set_target_properties(samplelib PROPERTIES OUTPUT_NAME "sample")

set(GENERATED_TAGS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated_tags")
set(DYNAMIC_TAGS_DIRECTORY "${GENERATED_TAGS_DIRECTORY}/dynamic")
set(COMPILE_TAGS_DIRECTORY "${GENERATED_TAGS_DIRECTORY}/compile")

set(GENERATED_TAGS_H "${DYNAMIC_TAGS_DIRECTORY}/tags.h")
set(GENERATED_TAGS_CPP "${DYNAMIC_TAGS_DIRECTORY}/tags.cpp")
set(GENERATED_TAGS_COMPILE_H "${COMPILE_TAGS_DIRECTORY}/tags.h")

target_include_directories(samplelib 
    PUBLIC
     "${GENERATED_TAGS_DIRECTORY}")

target_sources(
    samplelib
    PRIVATE
        "attribute.cpp"
        "node.cpp"
        "text.cpp"
        "index.cpp"
        "indices/attribute_name_index.cpp"
        "indices/tag_name_index.cpp"
        "indices/tag_index.cpp"
        "indices/cache_index.cpp"
        "void_node.cpp"
        "nodes/text_node.cpp"
        "nodes/empty_node.cpp"
        "nodes/generic_node.cpp"
        "nodes/comment_node.cpp"
        "nodes/_dangerous_raw_text_node.cpp"
        "${GENERATED_TAGS_CPP}"
)

add_executable(generate "${CMAKE_CURRENT_SOURCE_DIR}/generate_tags_classes.cpp")

add_custom_command(
    OUTPUT "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
    COMMAND generate "${CMAKE_CURRENT_SOURCE_DIR}/tags.txt" "${DYNAMIC_TAGS_DIRECTORY}" "${COMPILE_TAGS_DIRECTORY}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS generate "${CMAKE_CURRENT_SOURCE_DIR}/tags.txt" "${CMAKE_CURRENT_SOURCE_DIR}/generate_tags_classes.cpp"
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

add_executable(application)

set_target_properties(application PROPERTIES OUTPUT_NAME "html-templater")

target_link_libraries(
    application
    PRIVATE
        samplelib
)

target_sources(
    application
    PRIVATE
        "main.cpp"
)