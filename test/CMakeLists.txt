# Executable target for the unit tests
add_executable(OnyxXML_unit-tests)

target_link_libraries(
    OnyxXML_unit-tests
    PRIVATE
        OnyxXML
        Catch2::Catch2WithMain
)

target_sources(
    OnyxXML_unit-tests
    PRIVATE
        "node_tests.cpp"
        "text_tests.cpp"
        "index_tests.cpp"
        "parser_tests.cpp"
)

include(../cmake/functions/use_tags_file.cmake)

use_tags_file(OnyxXML_unit-tests "${CMAKE_CURRENT_SOURCE_DIR}/tags.txt" ON)

# Automatically register all tests
catch_discover_tests(OnyxXML_unit-tests)
