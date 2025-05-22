# Sample static library
add_library(samplelib STATIC)

set_target_properties(samplelib PROPERTIES OUTPUT_NAME "sample")

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
        "nodes/processing_instruction_node.cpp"
        "nodes/cdata_node.cpp"
        "nodes/foreach_node.cpp"
        "nodes/if_node.cpp"
        "parse/dom_parser.cpp"
        "node_handle.cpp"
        "arena.cpp"
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/generate_tags_classes.cpp" "${CMAKE_BINARY_DIR}/generated_tags/generate_tags_classes.cpp" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/generate_tags_classes.py" "${CMAKE_BINARY_DIR}/generated_tags/generate_tags_classes.py" COPYONLY)

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