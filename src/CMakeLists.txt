cmake_minimum_required (VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
        "nodes/xml_declaration_node.cpp"
        "nodes/doctype_node.cpp"
)