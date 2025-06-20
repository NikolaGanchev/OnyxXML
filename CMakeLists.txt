cmake_minimum_required (VERSION 3.24)

project ("OnyxXML" VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(ENABLE_DOCUMENTATION "Specifies whether to enable building the project documentation" ON)


if (NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Has parent
    if (NOT OnyxXML_BUILD_TESTING) 
        set(BUILD_TESTING OFF)
    endif()
    if (DEFINED OnyxXML_SOURCE_DIR)
        list(APPEND CMAKE_MODULE_PATH "${OnyxXML_SOURCE_DIR}/cmake/functions")
        set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
    endif()
    set(OnyxXML_XML_CPP_CODEGEN_SOURCE "${PROJECT_SOURCE_DIR}/src/generate_tags_classes.cpp" PARENT_SCOPE)
    set(OnyxXML_XML_PYTHON_CODEGEN_SCRIPT "${PROJECT_SOURCE_DIR}/src/generate_tags_classes.py" PARENT_SCOPE)
else()
    set(OnyxXML_XML_CPP_CODEGEN_SOURCE "${PROJECT_SOURCE_DIR}/src/generate_tags_classes.cpp")
    set(OnyxXML_XML_PYTHON_CODEGEN_SCRIPT "${PROJECT_SOURCE_DIR}/src/generate_tags_classes.py")
endif()

#
# Tell MSVC to build using multiple processes.
# This may speed up compilation time significantly.
# For more information check:
# https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes?view=msvc-170
#
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

# Makes it easier to display some useful info
include(CMakePrintHelpers)

# Uncomment the line below, if you want to specify additional
# locations to be searched by find_package and include.
# For example, a local cmake/ direcory within the project, etc.
# list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Display some useful information
cmake_print_variables(CMAKE_MODULE_PATH)
cmake_print_variables(CMAKE_PREFIX_PATH)



################################################################################
#
# Unit testing
#

# Configure the project for testing with CTest/CDash
# Automatically adds the BUILD_TESTING option and sets it to ON
# If BUILD_TESTING is ON, automatically calls enable_testing().
# Check the following resources for more info:
#   https://cmake.org/cmake/help/latest/module/CTest.html
#   https://cmake.org/cmake/help/latest/command/enable_testing.html
#   https://cmake.org/cmake/help/latest/manual/ctest.1.html
include(CTest)


# Make Catch2 available
if(BUILD_TESTING)

  message(STATUS "Make Catch2 available...")

  if(EXISTS ${CMAKE_SOURCE_DIR}/lib/Catch2)

    # If Catch2's repo has been cloned to the /lib directory, use that    
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/Catch2)
  
  else()

    # Try to either find a local installation of Catch2,
    # or download it from its repository.
    #
    # You can find more information on how FetchContent works and
    # what is the order of locations being searched in these sources:
    #
    # Using Dependencies Guide
    #   https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#guide:Using%20Dependencies%20Guide
    # FetchContent examples:
    #   https://cmake.org/cmake/help/latest/module/FetchContent.html#fetchcontent-find-package-integration-examples
    # If necessary, set up FETCHCONTENT_TRY_FIND_PACKAGE_MODE. Check:
    #   https://cmake.org/cmake/help/latest/module/FetchContent.html#variable:FETCHCONTENT_TRY_FIND_PACKAGE_MODE
    # For Catch2's own documentation on CMake integration check:
    #   https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
    
    include(FetchContent)

    # FIND_PACKAGE_ARGS makes it so that CMake first tries to find
    # CMake with find_package() and if it is NOT found, it will
    # be retrieved from its repository.
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
        FIND_PACKAGE_ARGS
    )

    FetchContent_MakeAvailable(Catch2)

    # The line below was necessary when Catch2 was obtained with FetchContent,
    # as described here:
    #   https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md)
    # This does not seem to be the case anymore.
    # list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

  endif()

  # Include the Catch module, which provides catch_discover_tests
  include(Catch)

  # Status messages
  cmake_print_variables(Catch2_DIR)
  cmake_print_variables(catch2_SOURCE_DIR)
  cmake_print_variables(Catch2_SOURCE_DIR)
  cmake_print_variables(CMAKE_MODULE_PATH)

endif()



################################################################################
#
# Targets
#

# Add the src/ directory to the include path of all targets
include_directories("include/onyxxml")
include_directories("include/")

# Executable and library targets
add_subdirectory(src)

# Unit testing
if(BUILD_TESTING)
  add_subdirectory(test)
endif()


if(ENABLE_DOCUMENTATION)
    message(STATUS "Documentation building is enabled")
    add_subdirectory("docs")
endif()

target_include_directories(OnyxXML
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

include(GNUInstallDirs)

install(TARGETS OnyxXML
    EXPORT OnyxXMLTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/onyxxml
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/docs/html/
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install(EXPORT OnyxXMLTargets
    FILE OnyxXMLTargets.cmake
    NAMESPACE OnyxXML::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OnyxXML
)

install(FILES
    cmake/functions/use_tags_file.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OnyxXML
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generate_tags_classes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generate_tags_classes.py
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OnyxXML/scripts
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/OnyxXMLConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OnyxXML
)