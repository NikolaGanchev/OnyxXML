#pragma once

#include "arena.h"
#include "compile/attribute.h"
#include "compile/base_serializable_node.h"
#include "compile/cdata.h"
#include "compile/comment.h"
#include "compile/doctype.h"
#include "compile/evaluated_document.h"
#include "compile/generic_node.h"
#include "compile/placeholder.h"
#include "compile/processing_instruction.h"
#include "compile/text.h"
#include "compile/xml_declaration.h"
#include "document.h"
#include "index.h"
#include "indices/attribute_name_index.h"
#include "indices/tag_index.h"
#include "indices/tag_name_index.h"
#include "literals.h"
#include "node_handle.h"
#include "nodes/_dangerous_raw_text_node.h"
#include "nodes/attribute_view_node.h"
#include "nodes/cdata_node.h"
#include "nodes/comment_node.h"
#include "nodes/doctype_node.h"
#include "nodes/empty_node.h"
#include "nodes/foreach_node.h"
#include "nodes/generic_node.h"
#include "nodes/if_node.h"
#include "nodes/namespace_view_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/root_view_node.h"
#include "nodes/text_node.h"
#include "nodes/xml_declaration_node.h"
#include "parse/dom_parser.h"
#include "parse/sax_parser.h"
#include "text.h"
#include "void_node.h"
#include "xpath/compiler.h"
#include "xpath/functions.h"
#include "xpath/lexer.h"
#include "xpath/parser.h"
#include "xpath/virtual_machine.h"
#include "xpath/xpath_object.h"
#include "xpath/xpath_query.h"

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
namespace xpath = dynamic::xpath;

namespace dynamic::tags {
using onyx::dynamic::Arena;
using onyx::dynamic::Attribute;
using onyx::dynamic::Node;
using onyx::dynamic::NodeHandle;
using onyx::dynamic::NonOwning;
using enum onyx::dynamic::tags::GenericNode::Type;
}  // namespace dynamic::tags

namespace compile::ctags {
using onyx::compile::Bind;
using onyx::compile::Document;
using onyx::compile::EvaluatedDocument;
using onyx::compile::instantiate;
using onyx::compile::PlaceholderBinding;
}  // namespace compile::ctags
}  // namespace onyx