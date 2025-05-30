include("${CMAKE_CURRENT_LIST_DIR}/OnyxXMLTargets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/use_tags_file.cmake")

set(OnyxXML_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include")
set(OnyxXML_LIBRARIES OnyxXML::OnyxXML)

set(OnyxXML_XML_PYTHON_CODEGEN_SCRIPT
    "${CMAKE_CURRENT_LIST_DIR}/scripts/generate_tags_classes.py"
)

set(OnyxXML_XML_CPP_CODEGEN_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/scripts/generate_tags_classes.cpp"
)
