#pragma once

#include "text.h"
#include "index.h"
#include "indices/attribute_name_index.h"
#include "indices/tag_name_index.h"
#include "indices/tag_index.h"
#include "indices/cache_index.h"
#include "void_node.h"
#include "nodes/text_node.h"
#include "nodes/generic_node.h"
#include "nodes/empty_node.h"
#include "nodes/comment_node.h"
#include "nodes/_dangerous_raw_text_node.h"
#include "nodes/foreach_node.h"
#include "compile/compile_attribute.h"
#include "compile/compile_comment.h"
#include "compile/compile_text.h"
#include "document.h"

#if __has_include("dynamic/tags.h")
    #include "dynamic/tags.h"
#endif

#if __has_include("compile/tags.h")
    #include "compile/tags.h"
#endif