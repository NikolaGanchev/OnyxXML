# OnyxXML

OnyxXML is a C++ library designed to streamline XML document construction, parsing, and indexing without sacrificing performance or safety. By combining dynamic tree building, compile-time generation, and a hybrid approach, along with advanced dynamic query and editing tools, OnyxXML offers unparalleled flexibility. Whether you need a fully static document generated at compile time or a richly dynamic structure in your application, OnyxXML delivers with zero runtime recursion, thorough memory safety, and an intuitive API.

## Table of Contents

1. [Overview](#overview)
2. [Installation](#installation)
   - [As an Installed Package](#as-an-installed-package)
   - [Via FetchContent](#via-fetchcontent)
3. [Tag Definitions Generation using CMake](#tag-definitions-generation-using-cmake)
4. [Documentation](#documentation)
5. [Usage Examples](#usage-examples)
   - [Dynamic API](#dynamic-api)
   - [Indexing API](#indexing-api)
   - [Compile-Time API](#compile-time-api)
   - [Hybrid API](#hybrid-api)
   - [Placeholders](#placeholders)
   - [Control Constructs (ForEach, If)](#control-constructs)
   - [Non-Owning Nodes](#non-owning-nodes)
   - [Arena Allocator](#arena-allocator)
   - [DOM Parser](#dom-parser)
   - [GenericNode API](#genericnode-api)
   - [Text Handling](#text-handling)
   - [Other Provided Nodes](#other-provided-nodes)
6. [License](#license)
7. [Roadmap](#roadmap)

## Overview

OnyxXML blends modern C++ best practices with rigorous testing to guarantee safe XML handling at any scale.&#x20;

The library’s dynamic API lets you assemble document trees at runtime with a clear, XML-centric syntax, while its compile-time module generates fully formed XML strings without incurring any runtime overhead. For scenarios requiring both static and dynamic segments, the hybrid API constructs a compile-time blueprint that you can later manipulate dynamically. Advanced features such as indexing, non-owning nodes, and arena-based allocation ensure you maintain high performance without manual memory management.

## Installation

You can install the library system‑wide or embed it directly using CMake’s FetchContent module. OnyxXML requires a C++20‑compliant compiler and CMake (>=3.24). 

### As an Installed Package

After building and installing OnyxXML on your machine, add the following to your `CMakeLists.txt`:

```cmake
find_package(OnyxXML REQUIRED)

add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp PRIVATE OnyxXML::OnyxXML)
```

This approach places OnyxXML in your system’s CMake package registry. The `use_tags_file` function is automatically included.

### Via FetchContent

If you prefer to pull OnyxXML alongside your source code, use CMake’s FetchContent feature:

```cmake
include(FetchContent)

FetchContent_Declare(
  OnyxXML
  GIT_REPOSITORY https://github.com/NikolaGanchev/OnyxXML
)
FetchContent_MakeAvailable(OnyxXML)

# Optionally import tag definition generation function
include(use_tags_file)

add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp PRIVATE OnyxXML)
```

## Tag Definitions Generation using CMake

Tag definitions are generated at build time from a simple CSV-like file and injected into your include paths automatically. Invoke them with:

```cmake
use_tags_file(<target> <path> [cross-compile])
```

By default, `use_tags_file` builds a `.cpp` script along with the library, and that executable is used for the generation. This is done to avoid 3rd party dependencies. 
In cross-compilation cases, the target compiler's system is different from the host system's, so this method does not work.
In this case, a functionally equivalent Python fallback is provided. Set cross-compile to `ON` when calling `use_tags_file` to trigger this behavior. 
This, of course, requires that Python is installed on the host system. 

### Tag Definition File Format

The file is CSV-like and must contain the following columns (with headers in the first row):

- **canonical**: The tag name as used in XML (e.g., `product`, `item`).
- **is\_void**: `1` if the tag is self-closing (void), or `0` otherwise.
- **dynamic**: The dynamic API class name to generate, or empty to skip.
- **compile**: The compile-time API class name, or empty to skip.

The last 2 columns are optional when writing out tag names.

Example:

```csv
canonical,is_void,dynamic,compile
product,0
separator,1
auto,0,dauto,cauto
```

## Documentation
The library is documented via Doxygen comments. The HTML documentation is built upon building the library. If the library is built independently (which also happens when added via `FetchContent`), the docs are in `<build-folder>/docs/html`. If the library is installed, they will be in `<install-folder>/share/doc/OnyxXML`.

## Usage Examples

### Dynamic API

```cpp
using namespace onyx::tags;

auto catalog = catalog(
    Attribute("version","1.0"),
    product(
        Attribute("id","123"),
        name(Text("Gadget")),
        price(Text("19.99")),
        description(Text("A versatile gadget."))
    ),
    product(
        Attribute("id","124"),
        name(Text("Widget")),
        price(Text("29.99"))
    )
);

std::string xmlOutput = catalog.serializePretty("\t", true);
```

This example builds a product catalog at runtime using tag helpers generated from your XML dialect.

### Indexing API

OnyxXML provides four built-in index types for efficient tree queries and caching. You can also extend the indexing API to define custom indices.

#### AttributeNameIndex

```cpp
using namespace onyx::tags;
using namespace onyx::dynamic;

// Build a server settings document
GenericNode serverConfig("server", false,
    Attribute("host","localhost"),
    database(
        user(Attribute("name","admin")),
        user(Attribute("name","guest"))
    )
);

// Index by "name" attribute
auto attrIdx = index::createIndex<index::AttributeNameIndex>(&serverConfig, "name");
auto users = attrIdx.getByValue("guest");
REQUIRE(users.size() == 1);
CHECK(users[0]->getAttributeValue("name") == "guest");
```

#### TagIndex

```cpp
using namespace onyx::dynamic;

// Create an order document
order orderDoc(
    item(Text("Foo")),
    item(Text("Bar")),
    shipment()
);

// Index tags by name
auto tagIdx = index::createIndex<index::TagIndex>(&orderDoc);
auto items = tagIdx.getByTagName("item");
REQUIRE(items.size() == 2);
auto shipments = tagIdx.getByTagName("shipment");
REQUIRE(shipments.size() == 1);
```

#### TagNameIndex

```cpp
using namespace onyx::dynamic;

// Build an inventory list
inventory inv(
    product(Text("Foo")),
    product(Text("Bar")),
    product(Text("Baz"))
);

// Index specifically "product" tags
auto nameIdx = index::createIndex<index::TagNameIndex>(&inv, "product");
auto products = nameIdx.get();
REQUIRE(products.size() == 3);
```

#### CacheIndex

```cpp
using namespace onyx::tags;
using namespace onyx::dynamic;

// Simple XML with attributes
GenericNode docRoot("document", false,
    Attribute("lang","en"),
    Attribute("status","draft"),
    metadata()
);

// Cache serialization
auto cacheIdx = index::createIndex<index::CacheIndex>(&docRoot);
std::string first = cacheIdx.cache(&GenericNode::serializePretty, "\t", true);
// second will be the cached result from the previous call
// The cache will reset if docRoot is edited in any way
std::string second = cacheIdx.cache(&GenericNode::serializePretty, "\t", true);
CHECK(first == second);
```

You may define custom indices by inheriting from `Node::Index` and implementing its methods. Neither storage methods nor query methods are provided by the base class.
Refer to the built-in Doxygen documentation, [`include/onyxxml/index.h`](include/onyxxml/index.h), [`include/onyxxml/indices`](include/onyxxml/indices), 
[`src/indices`](src/indices/) for more details and examples.

### Compile-Time API

```cpp
using namespace onyx::ctags;
using MyDoc = Document<
    catalog<
        product<Attribute<"id","001">,
            name<Text<"Gizmo">>,
            price<Text<"9.99">>
        >
    >
>;

constexpr auto xml = MyDoc::serialize();
static_assert(std::string(xml.data()).find("<price>9.99</price>") != std::string::npos);
```

All serialization logic is resolved at compile time, ensuring zero-cost runtime performance.

### Hybrid API

```cpp
using namespace onyx::ctags;
auto dynamicItems = Document<
    entry<Text<"Alpha">>,
    entry<Text<"Beta">>
>::dynamicTree();

dynamicItems->addChild(entry(Text("Gamma")));
std::string xml = dynamicItems->serialize();
```

Start with a static definition and then adjust dynamically. This is mostly an alternative syntax for the Dynamic API, as they do the same thing under the hood.

### Placeholders

```cpp
using namespace onyx::ctags;
using Template = PlaceholderDocument<
    report<Placeholder<"HEADER">, Placeholder<"BODY">>
>;

tags::header header(tags::title(tags::Text("Monthly Report")));
tags::section body(tags::entry(Text("Data")));

std::string output = Template::serializeWithPlaceholders(
    "HEADER", header,
    "BODY", body
);
```

Placeholders allow seamless mixing of static templates and dynamic content.

### Control Constructs

```cpp
// ForEach example
categories(
  ForEach(std::vector<std::string>{"Gold","Silver","Bronze"}, [](auto& val){
      return tags::level(Text(val));
  })
);

// If example
statusBlock(
  If(isActive, tags::status(Text("Active")), tags::status(Text("Inactive")))
);
```

Encode loops and conditions directly in your XML structure. Works only with owning nodes (as non-owning nodes do not support the nested constructor API).
Alternative iterator-based and index-based constructors are available for ForEach. Refer to [`include/onyxxml/nodes/foreach_node.h`](include/onyxxml/nodes/foreach_node.h).
Returning pointers instead of fixed types for polymorphism is also available.

### Non-Owning Nodes

```cpp
using namespace onyx::tags;

// Construct nodes without ownership semantics
GenericNode root(NonOwning, "items", false);
GenericNode* leaf = new GenericNode(NonOwning, "item", false);
root.addChild(leaf);

// Caller is responsible for leaf’s lifetime; root will not delete it
```

Non-owning nodes grant fine-grained control over Node lifetimes, useful in environments where you manage memory externally.

### Arena Allocator

```cpp
using namespace onyx::tags;

// Preconfigure the arena with expected types
Arena::Builder builder;
builder.preallocate<GenericNode>().preallocate<Text>();
Arena arena = builder.build();

// Allocate many nodes efficiently
GenericNode* container = arena.allocate<GenericNode>("container", false);
Text* entry = arena.allocate<Text>("entry data");
container->addChild(entry);
```

The arena allocator bulk-allocates objects of specified types in contiguous memory, yielding faster allocation and deallocation by releasing all at once when the arena goes out of scope. All nodes held by the Arena are NonOwning.

### DOM Parser

```cpp
#include "dom_parser.h"
ParseResult document = DomParser::parse(xmlString);
```

The DOM Parser is non-recursive, syntax-preserving, and throws detailed exceptions on invalid XML. Per the standard, it is non-validating - meaning it does not validate content according to DTDs. Due to security concerns, it does not parse DTDs at all, nor does it parse or expand user-defined entities. It also has no explicit namespace support. It is otherwise standard compliant, including encoding handling, character reference and entity expansion and newline behavior. The DOM parser currently represents every element as a `GenericNode`. `ParseResult` contains a private Arena and a public `ParseResult::root` Node\*, the root of the underlying non-owning tree. All memory is handled by ParseResult and is released when it goes out of scope.

### GenericNode API

`GenericNode` serves as the universal node type used by the DOM parser and dynamic API when no specific tag class is generated. It accepts a tag name and a void flag as the first two arguments and the remaining arguments are as per normal Node constructors.

```cpp
using namespace onyx::dynamic;

GenericNode custom("customTag", false,
    Attribute("key", "value"),
    GenericNode("child", true)
);

std::string output = custom.serialize();
```

`GenericNode` provides a fallback for any tag not covered by the generated definitions.

A compile-time `GenericNode` struct also exists:

```cpp
using namespace onyx::ctags;
using MyDoc = Document<
    GenericNode<"catalog", false,
        product<Attribute<"id","001">,
            name<Text<"Gizmo">>,
            price<Text<"9.99">>
        >
    >
>;
```

### Text Handling
The `Text` node escapes XML-sensitive characters by default. It has an optional second boolean argument (`escapeMultiByte`), which, when set to `true`, converts Unicode characters (e.g., emojis) into their numeric entity references; by default (`false`), original Unicode is preserved. This is included for legacy systems, where Unicode may cause security vulnerabilities. The `Attribute` class escapes by default, which can be turned off using a constructor parameter and has the same Unicode escaping functionality which is false by default and can be turned on using a second constructor parameter.

```cpp
using namespace onyx::dynamic;

GenericNode cdiv("div", false,
    Text("😊", true) // escapeMultiByte is true
);

REQUIRE(cdiv.serialize() == "<div>&#x1f60a;</div>");
```

If you need raw text, you may use the `__DangerousRawText` Node.

It is also important to note that the compile-time `Text` and `Attribute` structs do not provide any escaping. What you write is what you get.


### Other Provided Nodes

Nodes are provided for some special XML constructs - `CDATA`, `DOCTYPE`, processing instructions, XML declaration, comments. Equivalents are also available for the Compile API. 
`EmptyNode` is also provided for the Dynamic API. It acts as a dummy root for fragments that have multiple sibling roots. This role is served by `Document` in the Compile API.

## License

OnyxXML is distributed under the Apache License 2.0. See [LICENSE](LICENSE) for details.

## Roadmap

- SAX parser
- Runtime and compile-time schema validation
- Sanitization
- XPath support
