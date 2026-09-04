# OnyxXML

![CI](https://github.com/NikolaGanchev/OnyxXML/actions/workflows/ci.yml/badge.svg)
![Coverage Badge](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/NikolaGanchev/OnyxXML/badges/coverage-badge.json)

OnyxXML is a C++ XML library focused on API design as a first priority, hoping to bring expressive, modern and safe APIs while not sacrificing performance and compliance. This is achieved via modern C++ features, exhaustive and rigorous automated testing, and granular control over behavior. OnyxXML provides multiple standout features, such as a fluent runtime API for constructing XML trees, the ability to construct owning or non-owning trees, compile-time construction of XML, a highly generic fully-iterative parser backend to tailor to specific applications needs, coupled with multiple optimized frontends and a custom fully-iterative XPath 1.0 execution pipeline that compiles queries into virtual machine bytecode. OnyxXML aims to be completely recursion-free and memory-safe. OnyxXML currently has an extensive automated test suite containing over 500 tests and over 1700 assertions within them.

## Table of Contents

1. [Installation](#installation)
   - [As an Installed Package](#as-an-installed-package)
   - [Via FetchContent](#via-fetchcontent)
2. [Tag Definitions Generation using CMake](#tag-definitions-generation-using-cmake)
3. [Documentation](#documentation)
4. [Usage Examples](#usage-examples)
   - [Dynamic API](#dynamic-api)
   - [Indexing API](#indexing-api)
     - [AttributeNameIndex](#attributenameindex)
     - [TagIndex](#tagindex)
     - [TagNameIndex](#tagnameindex)
   - [Compile-Time API](#compile-time-api)
   - [Hybrid API](#hybrid-api)
   - [Placeholders](#placeholders)
   - [Control Constructs (ForEach, If)](#control-constructs)
   - [Non-Owning Nodes](#non-owning-nodes)
   - [Arena Allocator](#arena-allocator)
   - [Parser](#parser)
     - [DOM Parser](#dom-parser)
     - [SAX Parser](#sax-parser)
     - [Encoding support](#encoding-support)
     - [Creating a custom frontend](#creating-a-custom-frontend)
   - [GenericNode API](#genericnode-api)
   - [XPath 1.0 Support](#xpath-10-support)
   - [Text Handling](#text-handling)
   - [Other Provided Nodes](#other-provided-nodes)
5. [License](#license)
6. [Roadmap](#roadmap)

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
The library is documented via Doxygen comments. The HTML documentation is built upon building the library, but requires that Doxygen is installed on the system. If the library is built independently (which also happens when added via `FetchContent`), the docs are in `<build-folder>/docs/html`. If the library is installed, they will be in `<install-folder>/share/doc/OnyxXML`.

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

OnyxXML provides three built-in index types for efficient tree queries and caching. You can also extend the indexing API to define custom indices. For more information on creating a custom index, refer to [the index header](include/onyxxml/index.h).

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

You may define custom indices by inheriting from `Node::Index` and implementing its methods. Neither storage methods nor query methods are provided by the base class.
Refer to the built-in Doxygen documentation, [`include/onyxxml/index.h`](include/onyxxml/index.h). Refer to [`include/onyxxml/indices`](include/onyxxml/indices) and [`src/indices`](src/indices/) for more details and examples.

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

### Parser

The library provides a non-recursive, fast, configurable, policy-based XML parser backend. 

The parser has a high level of [XML 1.0 Specification](https://www.w3.org/TR/xml/) compliance, including standard-compliant text expansion (newlines and embedded entities), encoding support, implementation for the optional encoding autodetection support (Appendix F of the [XML 1.0 Specification](https://www.w3.org/TR/xml/)), strict validation of UTF-8 content, strict validation of content rules (such as `--` not being allowed on comments, or `]]>` not being allowed outside of CDATA sequences), strict validation of XML declarations. 

The parser can read isolated fragments of XML. This in turn means that by default, it allows documents without a single root. For example:
```xml
<tag></tag>
<another-tag></another-tag>
```
The above fragment will be parsed as valid, despite it not being a valid XML Document because it lacks a single root. However, XML declaration order and DOCTYPE order are enforced if available, so this will cause an exception:
```xml
<tag></tag>
<?xml version="1.0"?>
```
Furthermore, top level text is not allowed. This in turn means that the fragment:
```xml
Text
<tag></tag>
```
will cause an exception, and so would
```xml
An XML document
```
A configuration option can be added in the future to enforce root presence in documents but is not currently present.

The parser is non-validating as per the XML standard as it does not validate content according to a document's DOCTYPE Declaration. Furthermore, it currently does not parse DTDs at all, which may lead to the rejection of a valid document containing a DOCTYPE Declaration with a syntactically correct '>' character inside of it. Implementing support for parsing DOCTYPE declarations and validating mode is possible in the future and on the [Roadmap](#roadmap).

The parser backend is heavily templated and flexible, which makes it complex to set up. Considering this, for most users the provided frontends ([DOM Parser](#dom-parser) and [SAX Parser](#sax-parser)) are recommended. Both frontends use the underlying backend.

Users that wish to set up a custom frontend should skip to [Creating a custom frontend](#creating-a-custom-frontend).

#### DOM Parser

```cpp
#include "dom_parser.h"
ParseResult document = DomParser::parse(xmlString);
```

The DOM Parser is a frontend implementation of the backend parser that parses a DOM tree from provided XML. 
The DOM parser currently represents every element as a `GenericNode`. `ParseResult` contains a private Arena and a public `ParseResult::root` Node\*, the root of the underlying non-owning tree. All memory is handled by ParseResult and is released when it goes out of scope.

The DOM Parser supports both reading from a string in-memory and streams. Generally, parsing an in-memory string is faster, generally by about a factor of 2. However, streaming does not require the whole document to be loaded in-memory. 

#### SAX Parser

A SAX Parser is also provided. It uses the same underlying parser as the DOM Parser, as well as the same text handling functions and same validation logic. It is used via implementing the `SaxListener` interface and supplying the instance to the SAX Parser on construction.
```cpp
class ConcreteSaxListener : public SaxListener {
   public:
    void onStart() override {};
    void onText(std::string text) override {};
    void onComment(std::string text) override {};
    void onCData(std::string text) override {};
    void onInstruction(std::string tag, std::string instruction) override {};
    void onTagOpen(std::string name, bool isSelfClosing,
                   std::vector<Attribute> attributes) override {};
    void onTagClose(std::string name) override {};
    void onXMLDeclaration(std::string version, std::string encoding,
                          bool hasEncoding, bool isStandalone,
                          bool hasStandalone) override {};
    void onDoctype(std::string text) override {};
    void onException(std::exception& e) override {};
    void onEnd() override {};
};

ConcreteSaxListener listener;
SaxParser parser(listener);

parser.parse(xmlString);
```

The SAX Parser is generally slower than the DOM Parser due to the function call overhead. However, it allows massive savings in memory as the full DOM tree is not constructed. Also supports stream parsing.

#### Encoding support

By default, the parser can only parse UTF-8. Support for any other encodings, including the standard mandated UTF-16, requires the usage of `iconv`. 
To enable `iconv` via CMake, the `OnyxXML_USE_ICONV` option needs to be set, which will use CMake to find `iconv` on the host system. Furthermore, the `ICONV_AVAILABLE` compile definition needs to be defined. Otherwise, any attempt to transcode text at runtime will result in an exception. 

The set of encodings and available conversions is thus defined entirely by the set of encodings and conversions that the local build of `iconv` that CMake finds supports.

All predefined frontends expose the `encoding` argument as an additional optional argument. This means all overloads of `DomParser::parse` and all overloads of `SaxParser::parse`. The encoding argument is interpreted as follows:

- empty (default)

    If the encoding argument is the empty string (`""`), then the encoding is presumed to be UTF-8. The encoding is presumed to be **final**, which in turn means that if an `encoding` pseudo-attribute in the XML Declaration (if available) is set to a contradictory encoding, an exception will be thrown. This includes encodings that are compatible, such as UTF-8 and ASCII. A configuration option may be added in the future to control this behavior. If a document contains a UTF-8 Byte Order Mark (BOM) at the beginning, this configuration will not strip it and will most likely throw an exception.

- "autodetect"

    If the encoding argument is the string `"autodetect"`, then autodetection will follow per Appendix F of the [XML 1.0 Specification](https://www.w3.org/TR/xml/).

    First, an attempt will be made to read the encoding of the document from the beginning 4 bytes, as defined in the specification. If an encoding is extracted from a Byte Order Mark (BOM), that encoding will be treated as final, which is defined as in the previous section and the parsing process will proceed as normal.

    If no Byte Order Mark (BOM) is available, then an attempt will be made to infer the encoding from the first 4 bytes of the document as according to the specification. The attempt is guaranteed to yield a possible encoding *family*, where a family is a set of encodings that the specification details are possible encodings of the document given these 4 starting bytes.

    If the family is inferred, then it is strictly required that the document possess a valid XML Declaration that has an `encoding` pseudo-attribute. If the document does not possess a valid XML Declaration, or the declaration it possesses has not declared an `encoding` pseudo-attribute, an exception will be thrown.

    The value of the `encoding` pseudo-attribute will be the final encoding, as defined in the previous sections. The frontends do not validate whether the encoding that was found belongs to or is compatible with the autodetected encoding family.

    Autodetection uses only the first 4 bytes of the document. It is possible to craft payloads that mimic a wrong BOM or family detection sequence. Due to this, it is important to carefully consider usages of autodetection.

- other (non-empty, non-autodetect string)

    If the encoding argument is any other string that is not `""` and is not `"autodetect"`, it will be interpreted as the encoding of the document. The document will in turn be transcoded from that encoding to UTF-8 using `iconv`. This encoding will be treated as final, as defined in the previous sections.  

Take note that encoding handling behavior is specific to the frontends. The backend parser only consumes UTF-8 and any custom frontends may support or may not support encodings.

#### Creating a custom frontend

The backend parser is defined in [`include/onyxxml/parse/parser.h`](include/onyxxml/parse/parser.h). It is broken into a variety of documented functions that parse individual elements of an XML document such as text, comments, tags, attributes, XML Declarations, DOCTYPE Declarations, CDATA sequences. While these functions can be used, we mostly care for `parseBody` which acts as a dispatcher for them.

`parseBody` requires the following to start:

- Config

    A Config is a struct for configuration of the parser which satisfies the [`isParserConfig`](include/onyxxml/parse/is_parser_config.h) concept.

    Common configs are defined in [`include/onyxxml/parse/common_parser_configs.h`](include/onyxxml/parse/common_parser_configs.h).

    This argument is strictly passed as a typename.

- Cursor

    An instance of a Cursor is the source the parser will read from. A Cursor is any class (or struct) which satisfies the [`isCursor`](include/onyxxml/parse/is_cursor.h) concept.

- Policy

    An instance of a Policy is how the parser backend communicates with the outer world. As it parses, the parses dispatches events via the Policy methods, not unlike a SAX parser. The Policy chooses how to handle them. The Policy also provides to the cursor the types it requires, and offers methods for certain string related transformation. 
    
    For example, it is responsibility of the policy to correctly transform text per accordance with the specification via the `Policy::transformText` method, which the parser calls with an appropriate [`TextTransformationMode`](include/onyxxml/parse/text_transformation_mode.h). The functions `text::expandEntitiesAndNormalizeEol`, `text::expandText`, `text::expandAttributeValue` and `text::expandEOLOnly` defined in [`include/onyxxml/text.h`](include/onyxxml/text.h) are compliant and may be used.

    A Policy is any class (or struct) which satisfies the [`isParserPolicy`](include/onyxxml/parse/is_parser_policy.h) concept.

    The structs [`BaseParserPolicy`](include/onyxxml/parse/base_parser_policy.h) and [`BasicAutodetectionParserPolicy`](include/onyxxml/parse/basic_autodetection_parser_policy.h) may be used via CRTP.

- bool validateUTF8

    This bool is strictly runtime and indicates whether the parser backend should validate UTF-8 or not. If the source has been or is being transcoded, the transcoding layer may (and in the case of `iconv`, will) do validation, which removes the need for a second validation. 

Furthermore, if encoding support is desired, one may use the [`EncodingController`](include/onyxxml/parse/encoding_controller.h) class to simplify the complex state control for the different autodetection modes. More is specified in the documentation of [`EncodingController`](include/onyxxml/parse/encoding_controller.h).

In the end, a valid call to `parseBody` will look something like this:
```cpp
parseBody<ParseConfig>(pos, policy, validateUTF8);
```

Further examples can be read in [`dom_parser.cpp`](src/parse/dom_parser.cpp) and [`sax_parser.cpp`](src/parse/sax_parser.cpp).

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

### XPath 1.0 Support

OnyxXML includes a fully custom XPath 1.0 engine. The engine uses a custom pipeline to lex, parse and compile XPath queries into custom bytecode, which is then executed by a stack-based virtual machine. The engine is fully iterative. The engine also supports attribute nodes and the special XPath 1.0 root node via the classes `AttributeViewNode` and `RootViewNode`.

XML Namespaces are not currently supported by the XPath engine. The engine also does NOT explicitly handle UTF-8 content, especially in string functions.

Not all functions are supported. In particular, `name`, `local-name` and `lang` are not supported yet. `id` is implemented in a specification-compliant manner and returns the empty string, as the library does not support DTDs. However, this may be unexpected behavior for some users.

The pipeline should correctly execute valid XPath queries, but it is not guaranteed to reject all invalid ones. It has been reasonably tested against many wrong queries with missing brackets/parenthesis, invalid names, wrong syntax structure, but does not validate the complete XPath 1.0 grammar. This is because it currently lacks a semantic analysis step, which will likely be built in the future. In general, an invalid query will either be rejected or successfully complete with a reasonably expected result given the query. Queries can be rejected at different stages, such as the Lexer, Parser, Compiler or at execution. This means that an `XPathQuery` object may be constructed successfully using an invalid query, but fail at execution.

Outside of these constraints, the engine follows the XPath 1.0 specification as closely as possible.

```cpp
using namespace onyx::dynamic::xpath;
using namespace onyx::dynamic::tags;

GenericNode store("store", false,
    GenericNode("book", false, Attribute("category", "fiction"),
        GenericNode("title", false, Text("Book1")),
        GenericNode("price", false, Text("10"))
    ),
    GenericNode("book", false, Attribute("category", "code"),
        GenericNode("title", false, Text("Book2")),
        GenericNode("price", false, Text("55"))
    )
);

XPathQuery query("/store/book[price > 15]/title");
XPathQuery::Result result = query.execute(&store);

if (result.object.isNodeset()) {
    const std::vector<Node*>& nodes = result.object.asNodeset();
    // nodes contains the "title" element for Book2
    REQUIRE(result.object.asString() == "Book2"); // supports XPath type-casting rules and string-value
}
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

Nodes are provided for some special XML constructs - `CDATA`, `DOCTYPE`, processing instructions, XML declaration, comments. Equivalents are also available for the compile-time API. 
`EmptyNode` is also provided for the dynamic API. It acts as a dummy root for fragments that have multiple sibling roots. This role is served by `Document` in the compile-time API.

## License

OnyxXML is distributed under the Apache License 2.0. See [LICENSE](LICENSE) for details.

## Roadmap

- Namespaces
- Semantic analyzer for XPath
- DOCTYPE Declaration support
- Testing with the [`W3C XML Conformance Test Suites`](https://www.w3.org/XML/Test/)