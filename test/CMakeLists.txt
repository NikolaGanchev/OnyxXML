# Executable target for the unit tests
add_executable(OnyxXML-unit-tests)

target_link_libraries(
    OnyxXML-unit-tests
    PRIVATE
        OnyxXML
        Catch2::Catch2WithMain
)

target_sources(
    OnyxXML-unit-tests
    PRIVATE
        "node_tests.cpp"
        "text_tests.cpp"
        "index_tests.cpp"
        "parser_tests.cpp"
)

include(../cmake/use_tags_file.cmake)

use_tags_file(OnyxXML-unit-tests "${CMAKE_CURRENT_SOURCE_DIR}/tags.txt" OFF)

# Automatically register all tests
catch_discover_tests(OnyxXML-unit-tests)
