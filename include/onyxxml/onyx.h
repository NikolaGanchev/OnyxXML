#pragma once

#include "onyxxml/arena.h"
#include "onyxxml/compile/attribute.h"
#include "onyxxml/compile/base_serializable_node.h"
#include "onyxxml/compile/cdata.h"
#include "onyxxml/compile/comment.h"
#include "onyxxml/compile/doctype.h"
#include "onyxxml/compile/evaluated_document.h"
#include "onyxxml/compile/generic_node.h"
#include "onyxxml/compile/placeholder.h"
#include "onyxxml/compile/processing_instruction.h"
#include "onyxxml/compile/text.h"
#include "onyxxml/compile/xml_declaration.h"
#include "onyxxml/document.h"
#include "onyxxml/index.h"
#include "onyxxml/indices/attribute_name_index.h"
#include "onyxxml/indices/tag_index.h"
#include "onyxxml/indices/tag_name_index.h"
#include "onyxxml/literals.h"
#include "onyxxml/node_handle.h"
#include "onyxxml/nodes/_dangerous_raw_text_node.h"
#include "onyxxml/nodes/attribute_view_node.h"
#include "onyxxml/nodes/cdata_node.h"
#include "onyxxml/nodes/comment_node.h"
#include "onyxxml/nodes/doctype_node.h"
#include "onyxxml/nodes/empty_node.h"
#include "onyxxml/nodes/foreach_node.h"
#include "onyxxml/nodes/generic_node.h"
#include "onyxxml/nodes/if_node.h"
#include "onyxxml/nodes/namespace_view_node.h"
#include "onyxxml/nodes/processing_instruction_node.h"
#include "onyxxml/nodes/root_view_node.h"
#include "onyxxml/nodes/text_node.h"
#include "onyxxml/nodes/xml_declaration_node.h"
#include "onyxxml/parse/dom_parser.h"
#include "onyxxml/parse/sax_parser.h"
#include "onyxxml/text.h"
#include "onyxxml/void_node.h"
#include "onyxxml/xpath/compiler.h"
#include "onyxxml/xpath/functions.h"
#include "onyxxml/xpath/lexer.h"
#include "onyxxml/xpath/parser.h"
#include "onyxxml/xpath/virtual_machine.h"
#include "onyxxml/xpath/xpath_object.h"
#include "onyxxml/xpath/xpath_query.h"

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