# Sample static library
add_library(samplelib STATIC)

set_target_properties(samplelib PROPERTIES OUTPUT_NAME "sample")

target_sources(
    samplelib
    PRIVATE
        "html_object.cpp"
        "html_object.h"
        "dynamic/tags.h"
        "dynamic/tags.cpp"
        "document.h"
        "compile/tags.h"
)

set(GENERATED_TAGS_H "${CMAKE_CURRENT_SOURCE_DIR}/dynamic/tags.h")
set(GENERATED_TAGS_CPP "${CMAKE_CURRENT_SOURCE_DIR}/dynamic/tags.cpp")
set(GENERATED_TAGS_COMPILE_H "${CMAKE_CURRENT_SOURCE_DIR}/compile/tags.h")
set(GENERATED_COMPILE_H "${CMAKE_CURRENT_SOURCE_DIR}/compile/documents.h")

add_executable(generate "${CMAKE_CURRENT_SOURCE_DIR}/generate_tags_classes.cpp")

add_custom_command(
    OUTPUT "${GENERATED_TAGS_H}" "${GENERATED_TAGS_CPP}" "${GENERATED_TAGS_COMPILE_H}"
    COMMAND generate
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