#pragma once

#include "arena.h"
#include "compile/compile_attribute.h"
#include "compile/compile_cdata.h"
#include "compile/compile_comment.h"
#include "compile/compile_doctype.h"
#include "compile/compile_generic_node.h"
#include "compile/compile_placeholder.h"
#include "compile/compile_processing_instruction.h"
#include "compile/compile_text.h"
#include "compile/compile_xml_declaration.h"
#include "compile/placeholder_document.h"
#include "document.h"
#include "index.h"
#include "indices/attribute_name_index.h"
#include "indices/cache_index.h"
#include "indices/tag_index.h"
#include "indices/tag_name_index.h"
#include "node_handle.h"
#include "nodes/_dangerous_raw_text_node.h"
#include "nodes/cdata_node.h"
#include "nodes/comment_node.h"
#include "nodes/doctype_node.h"
#include "nodes/empty_node.h"
#include "nodes/foreach_node.h"
#include "nodes/generic_node.h"
#include "nodes/if_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/text_node.h"
#include "nodes/xml_declaration_node.h"
#include "nodes/attribute_view_node.h"
#include "parse/dom_parser.h"
#include "parse/sax_parser.h"
#include "text.h"
#include "void_node.h"

#if __has_include("dynamic/tags.h")
#include "dynamic/tags.h"
#endif

#if __has_include("compile/tags.h")
#include "compile/tags.h"
#endif

namespace onyx {
namespace tags = dynamic::tags;
namespace ctags = compile::ctags;
namespace index = dynamic::index;
namespace text = dynamic::text;
namespace parser = dynamic::parser;

namespace dynamic::tags {
using onyx::dynamic::Arena;
using onyx::dynamic::Attribute;
using onyx::dynamic::Node;
using onyx::dynamic::NodeHandle;
using onyx::dynamic::NonOwning;
}  // namespace dynamic::tags

namespace compile::ctags {
using onyx::compile::Document;
using onyx::compile::PlaceholderDocument;
}  // namespace compile::ctags
}  // namespace onyx