# Sample static library
add_library(samplelib STATIC)

set_target_properties(samplelib PROPERTIES OUTPUT_NAME "sample")

target_sources(
    samplelib
    PRIVATE
        "templater.cpp"
        "templater.h"
        "/generated/tags.h"
        "/generated/tags.cpp"
)

set(GENERATED_TAGS_H ${CMAKE_SOURCE_DIR}/src/generated/tags.h)
set(GENERATED_TAGS_CPP ${CMAKE_SOURCE_DIR}/src/generated/tags.cpp)

add_executable(generate ${CMAKE_SOURCE_DIR}/src/generate_tags_classes.cpp)

add_custom_command(
    OUTPUT ${GENERATED_TAGS_H} ${GENERATED_TAGS_CPP}
    COMMAND generate
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/
    DEPENDS generate ${CMAKE_SOURCE_DIR}/src/tags.txt ${CMAKE_SOURCE_DIR}/src/generate_tags_classes.cpp
    COMMENT "Running the generate executable to generate the html tags classes files"
    VERBATIM
)

add_custom_target(
    generate_files ALL
    DEPENDS ${GENERATED_TAGS_H} ${GENERATED_TAGS_CPP}
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