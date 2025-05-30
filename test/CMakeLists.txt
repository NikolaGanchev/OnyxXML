# Executable target for the unit tests
add_executable(unit-tests)

target_link_libraries(
    unit-tests
    PRIVATE
        OnyxXML
        Catch2::Catch2WithMain
)

target_sources(
    unit-tests
    PRIVATE
        "node_tests.cpp"
        "text_tests.cpp"
        "index_tests.cpp"
        "parser_tests.cpp"
)

include(../cmake/use_tags_file.cmake)

use_tags_file(unit-tests "${CMAKE_CURRENT_SOURCE_DIR}/tags.txt" OFF)

# Automatically register all tests
catch_discover_tests(unit-tests)
