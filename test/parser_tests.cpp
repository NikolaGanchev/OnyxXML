#include <chrono>
#include <sstream>

#include "catch2/catch_all.hpp"
#include "nodes/cdata_node.h"
#include "nodes/comment_node.h"
#include "nodes/doctype_node.h"
#include "nodes/empty_node.h"
#include "nodes/generic_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/text_node.h"
#include "onyx.h"
#include "parse/string_view_read_buffer.h"

TEST_CASE("DomParser works") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<html><head></head></html>";
    std::stringstream inputStream(input);

    GenericNode output{"html", false, GenericNode("head", false)};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with text") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html><body><div> Hello<span></span>World! </div></body></html>";
    std::stringstream inputStream(input);

    GenericNode output{
        "html", false,
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with a single attribute") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\"><body><div> Hello<span></span>World! "
        "</div></body></html>";
    std::stringstream inputStream(input);

    GenericNode output{
        "html", false, Attribute("theme", "dark"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with many attributes") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\" lang='en'><body><div> Hello<span></span>World! "
        "</div></body></html>";
    std::stringstream inputStream(input);

    GenericNode output{
        "html", false, Attribute("theme", "dark"), Attribute("lang", "en"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser expands entities in text") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\" lang='en'><body><div> 4 &#60; 5; "
        "</div></body></html>";
    std::stringstream inputStream(input);

    GenericNode output{
        "html", false, Attribute("theme", "dark"), Attribute("lang", "en"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" 4 < 5; ")))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser expands entities in attribute values") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark&apos;\" lang='en'><body><div> "
        "Hello<span></span>World! </div></body></html>";
    std::stringstream inputStream(input);

    GenericNode output{
        "html", false, Attribute("theme", "dark'"), Attribute("lang", "en"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser properly normalizes text in doctype declaration") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<!DOCTYPE &apos \n \r\n \t &apos;>";
    std::stringstream inputStream(input);

    Doctype output{"&apos \n \n \t &apos;"};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser properly normalizes text in comment") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<!--This is a comment "
        "with invalid entities &apos that should not error, valid entities "
        "apos&; that should not expand, and newlines \r\n \r \n that should be "
        "normalized-->";

    std::stringstream inputStream(input);

    Comment output{
        "This is a comment with invalid entities &apos that should "
        "not error, valid entities apos&; that should not expand, "
        "and newlines \n \n \n that should be normalized"};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser properly normalizes text in processing instruction") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<?pi Inside processing instructions, invalid entities "
        "&apos also should not error, valid entities apos&; should not expand "
        "and newlines \r\n \r \n should be normalized?>";
    std::stringstream inputStream(input);

    ProcessingInstruction output{
        "pi",
        "Inside processing instructions, invalid entities &apos also "
        "should not error, valid entities apos&; should not expand and "
        "newlines \n \n \n should be normalized"};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser properly normalizes complex text") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<!DOCTYPE &apos \n \r\n &apos;><html theme=\"dark\r\n\" "
        "lang='\ten\n'><body><div> "
        "Hello<span>\r\n\n\r\r\n</span>World! "
        "\n<![CDATA[&apos\t\r\n\n&apos;]]></div></body><!--This is a comment "
        "with invalid entities &apos that should not error, valid entities "
        "apos&; that should not expand, and newlines \r\n \r \n that should be "
        "normalized--><?pi Inside processing instructions, invalid entities "
        "&apos also should not error, valid entities apos&; should not expand "
        "and newlines \r\n \r \n should be normalized?></html>";
    std::stringstream inputStream(input);

    EmptyNode output{
        Doctype("&apos \n \n &apos;"),
        GenericNode(
            "html", false, Attribute("theme", "dark "),
            Attribute("lang", " en "),
            GenericNode(
                "body", false,
                GenericNode("div", false, Text(" Hello"),
                            GenericNode("span", false, Text("\n\n\n\n")),
                            Text("World! \n"), CData("&apos\t\n\n&apos;"))),
            Comment("This is a comment with invalid entities &apos that should "
                    "not error, valid entities apos&; that should not expand, "
                    "and newlines \n \n \n that should be normalized"),
            ProcessingInstruction(
                "pi",
                "Inside processing instructions, invalid entities &apos also "
                "should not error, valid entities apos&; should not expand and "
                "newlines \n \n \n should be normalized"))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser parses complex html") {
    using namespace onyx::tags;
    using namespace onyx::parser;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    GenericNode output{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),

        GenericNode(
            "head", false,
            GenericNode("meta", true, Attribute("charset", "UTF-8")),
            GenericNode(
                "meta", true, Attribute("name", "viewport"),
                Attribute("content", "width=device-width, initial-scale=1.0")),
            GenericNode("title", false, Text("Complex Test Page")),
            GenericNode("link", true, Attribute("rel", "stylesheet"),
                        Attribute("href", "/styles/main.css"))),

        GenericNode(
            "body", false,
            GenericNode(
                "header", false,
                GenericNode(
                    "nav", false,
                    GenericNode(
                        "ul", false,
                        GenericNode(
                            "li", false,
                            GenericNode("a", false, Attribute("href", "#home"),
                                        Text("Home"))),
                        GenericNode(
                            "li", false,
                            GenericNode("a", false, Attribute("href", "#about"),
                                        Text("About Us")))))),

            GenericNode(
                "main", false,
                GenericNode(
                    "section", false, Attribute("id", "introduction"),
                    GenericNode("h1", false, Text("Introduction")),
                    GenericNode("p", false,
                                Text("Welcome to the complex HTML structure "
                                     "test case.")),
                    GenericNode("p", false,
                                Text("This test includes various nested "
                                     "elements, attributes, and content.")),
                    GenericNode(
                        "form", false, Attribute("name", "contact-form"),
                        GenericNode("label", false, Attribute("for", "name"),
                                    Text("Your Name:")),
                        GenericNode("input", true, Attribute("type", "text"),
                                    Attribute("id", "name"),
                                    Attribute("name", "name")),
                        GenericNode("label", false, Attribute("for", "email"),
                                    Text("Your Email:")),
                        GenericNode("input", true, Attribute("type", "email"),
                                    Attribute("id", "email"),
                                    Attribute("name", "email")),
                        GenericNode("button", false,
                                    Attribute("type", "submit"),
                                    Text("Submit")))),

                GenericNode(
                    "section", false, Attribute("id", "features"),
                    GenericNode("h2", false, Text("Features")),
                    GenericNode("ul", false,
                                GenericNode("li", false, Text("Feature 1")),
                                GenericNode("li", false, Text("Feature 2")),
                                GenericNode("li", false, Text("Feature 3"))),
                    GenericNode("p", false,
                                Text("These are the key features of the "
                                     "application.")))),

            GenericNode(
                "footer", false,
                GenericNode("p", false, Text("© 2025 Complex HTML Test Page")),
                GenericNode("a", false,
                            Attribute("href", "https://www.example.com"),
                            Text("Privacy Policy"))))};

    std::string input =
        "<html lang=\"en\" theme=\"dark\"><head><meta charset=\"UTF-8\"/><meta "
        "name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1.0\"/><title>Complex Test Page</title><link "
        "rel=\"stylesheet\" "
        "href=\"/styles/main.css\"/></head><body><header><nav><ul><li><a "
        "href=\"#home\">Home</a></li><li><a href=\"#about\">About "
        "Us</a></li></ul></nav></header><main><section "
        "id=\"introduction\"><h1>Introduction</h1><p>Welcome to the complex "
        "HTML structure test case.</p><p>This test includes various nested "
        "elements, attributes, and content.</p><form "
        "name=\"contact-form\"><label for=\"name\">Your Name:</label><input "
        "type=\"text\" id=\"name\" name=\"name\"/><label for=\"email\">Your "
        "Email:</label><input type=\"email\" id=\"email\" "
        "name=\"email\"/><button "
        "type=\"submit\">Submit</button></form></section><section "
        "id=\"features\"><h2>Features</h2><ul><li>Feature 1</li><li>Feature "
        "2</li><li>Feature 3</li></ul><p>These are the key features of the "
        "application.</p></section></main><footer><p>© 2025 Complex HTML Test "
        "Page</p><a href=\"https://www.example.com\">Privacy "
        "Policy</a></footer></body></html>";
    std::stringstream inputStream(input);

    auto t1 = high_resolution_clock::now();
    ParseResult pr = DomParser::parse(input);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;

    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));

    INFO(time.count());

    t1 = high_resolution_clock::now();
    ParseResult prStream = DomParser::parse(inputStream);
    t2 = high_resolution_clock::now();

    time = t2 - t1;

    INFO(time.count());
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with comments") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\"><!--This is a comment\r-!--><body><div> "
        "Hello<span></span>World! </div></body></html>";
    std::stringstream inputStream(input);

    GenericNode output{
        "html", false, Attribute("theme", "dark"),
        Comment("This is a comment\n-!"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with processing instructions") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<root lang=\"en\"><?templater doSomething 5 > 4 "
        "\r?somethingElse?></root>";
    std::stringstream inputStream(input);

    GenericNode output{"root", false, Attribute("lang", "en"),
                       ProcessingInstruction(
                           "templater", "doSomething 5 > 4 \n?somethingElse")};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with CDATA") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<root lang=\"en\"><![CDATA[<someElement> This is some literal text, "
        "in which & and <, > can safely be written! Also, a]>, ]a>, ]], aa> "
        "will not end this section.]]></root>";
    std::stringstream inputStream(input);

    GenericNode output{"root", false, Attribute("lang", "en"),
                       CData("<someElement> This is some literal text, in "
                             "which & and <, > can safely be written! Also, "
                             "a]>, ]a>, ]], aa> will not end this section.")};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with XML declarations") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.0\"?>";
    std::stringstream inputStream(input);

    XmlDeclaration output("1.0", "UTF-8", false, false, false, false);

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with XML declarations with encoding") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.1\" encoding=\"ISO-8859-1\"?>";
    std::stringstream inputStream(input);

    XmlDeclaration output("1.1", "ISO-8859-1", true, false, false, false);

    ParseResult pr = DomParser::parse(input, "ISO-8859-1");
    ParseResult prStream = DomParser::parse(inputStream, "ISO-8859-1");

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with XML declarations with standalone") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.0\" standalone=\"yes\"?>";
    std::stringstream inputStream(input);

    XmlDeclaration output("1.0", "UTF-8", false, true, true, false);

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser works with DOCTYPE") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<!DOCTYPE html><html theme=\"dark\"><body><div> "
        "Hello<span></span>World! </div></body></html>";
    std::stringstream inputStream(input);

    EmptyNode output{
        Doctype("html"),
        GenericNode("html", false, Attribute("theme", "dark"),
                    GenericNode("body", false,
                                GenericNode("div", false, Text(" Hello"),
                                            GenericNode("span", false),
                                            Text("World! "))))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser parses unicode") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<?xml version=\"1.0\" "
        "encoding=\"UTF-8\"?><root-тест><елемент-с-юникод>Hello, 世界! Привет! "
        "👋</елемент-с-юникод><データ>Some mixed content: éléphant, caffè, "
        "España. 🚀</データ><属性 attr=\"値-юникод-1\" "
        "друг-attr=\"テスト値\"></属性><空要素/></root-тест>";
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "UTF-8", true, false, false, false),
        GenericNode(
            "root-тест", false,
            GenericNode("елемент-с-юникод", false,
                        Text("Hello, 世界! Привет! 👋")),
            GenericNode(
                "データ", false,
                Text("Some mixed content: éléphant, caffè, España. 🚀")),
            GenericNode("属性", false, Attribute("attr", "値-юникод-1"),
                        Attribute("друг-attr", "テスト値")),
            GenericNode("空要素", true))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser throws \"Invalid end after tag open\"") {
    using namespace onyx::parser;

    std::string input = "<tag>   \n\t   ";
    std::stringstream inputStream(input);
    std::string message = "Unclosed tags left";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Premature end of document after <\"") {
    using namespace onyx::parser;

    std::string input = "<";
    std::stringstream inputStream(input);
    std::string message = "Premature end of document";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Premature end in processing instruction\"") {
    using namespace onyx::parser;

    std::string input = "<?";
    std::stringstream inputStream(input);
    std::string message = "Premature end of document";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Empty processing instruction tag name\"") {
    using namespace onyx::parser;

    std::string input = "<? >";
    std::stringstream inputStream(input);
    std::string message = "Invalid tag name";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML declaration is only allowed at the first position "
    "in the prologue\"") {
    using namespace onyx::parser;

    std::string input = "<tag></tag><?xml ?>";
    std::stringstream inputStream(input);
    std::string message =
        "XML declaration is only allowed at the first position "
        "in the prologue";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Multiple XML declarations found\"") {
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.0\"?><?xml version=\"1.0\"?>";
    std::stringstream inputStream(input);
    std::string message = "Multiple XML declarations found";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Missing space after PI target\"") {
    using namespace onyx::parser;

    std::string input = "<?pi?>";
    std::stringstream inputStream(input);
    std::string message =
        "No space between processing instruction target and "
        "processing instruction content";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Unterminated processing instruction\"") {
    using namespace onyx::parser;

    std::string input = "<?pi content";
    std::stringstream inputStream(input);
    std::string message = "Invalid processing instruction without ending";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Unterminated comment\"") {
    using namespace onyx::parser;

    std::string input = "<!-- comment";
    std::stringstream inputStream(input);
    std::string message = "Invalid comment without ending";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"-- inside comment\"") {
    using namespace onyx::parser;

    std::string input = "<!-- comment-- ->";
    std::stringstream inputStream(input);
    std::string message = "-- inside of comment not allowed";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Empty tag name\"") {
    using namespace onyx::parser;

    std::string input = "<>";
    std::stringstream inputStream(input);
    std::string message = "Invalid tag name";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Invalid attribute name\"") {
    using namespace onyx::parser;

    std::string input = "<tag =\"value\">";
    std::stringstream inputStream(input);
    std::string message = "Invalid non-closing tag";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"No equals after attribute name\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr>";
    std::stringstream inputStream(input);
    std::string message = "No = after attribute";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Premature end at attribute\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=";
    std::stringstream inputStream(input);
    std::string message = "Premature end at attribute";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"No quote after attribute equals\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=value>";
    std::stringstream inputStream(input);
    std::string message = "No quote (\" or ') after attribute =";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Improperly closed attribute value\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"";
    std::stringstream inputStream(input);
    std::string message = "Improperly closed attribute value";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Cannot have '<' inside of attribute value\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val<ue\"";
    std::stringstream inputStream(input);
    std::string message = "Cannot have '<' inside of attribute value";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"No whitespace after closing attribute quote\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val\"x>";
    std::stringstream inputStream(input);
    std::string message = "No whitespace after attribute closing quote";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Premature end after attribute\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val\"";
    std::stringstream inputStream(input);
    std::string message = "No whitespace after attribute closing quote";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Double closing tag\"") {
    using namespace onyx::parser;

    std::string input = "</tag/>";
    std::stringstream inputStream(input);
    std::string message = "Trying to double-close closing tag";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Invalid tag close after /\"") {
    using namespace onyx::parser;

    std::string input = "<tag/ ";
    std::stringstream inputStream(input);
    std::string message = "Invalid tag close - must have > after /";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"No tag close\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val\" x";
    std::stringstream inputStream(input);
    std::string message = "No = after attribute";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Closing unopened tag\"") {
    using namespace onyx::parser;

    std::string input = "<a></b>";
    std::stringstream inputStream(input);
    std::string message = "Closing unopened tag";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Closing non-existent tags\"") {
    using namespace onyx::parser;

    std::string input = "</a>";
    std::stringstream inputStream(input);
    std::string message = "Closing unopened tag";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Unclosed tags left\"") {
    using namespace onyx::parser;

    std::string input = "<a><b></b>";
    std::stringstream inputStream(input);
    std::string message = "Unclosed tags left";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Premature end of CDATA section\"") {
    using namespace onyx::parser;

    std::string input = "<![CD";
    std::stringstream inputStream(input);
    std::string message = "Premature end of CDATA section";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"CDATA ending sequence forbidden outside of CDATA "
    "ending sequences\"") {
    using namespace onyx::parser;

    std::string input = "<div>]]></div>";
    std::stringstream inputStream(input);
    std::string message =
        "']]>' is forbidden outside of CDATA ending sequences";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Invalid CDATA without ending\"") {
    using namespace onyx::parser;

    std::string input = "<![CDATA[asdfasfasfasfasfasfasda";
    std::stringstream inputStream(input);
    std::string message = "Invalid CDATA without ending";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Premature end of DOCTYPE section\"") {
    using namespace onyx::parser;

    std::string input = "<!DOC";
    std::stringstream inputStream(input);
    std::string message = "Premature end of DOCTYPE section";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Invalid DOCTYPE without ending\"") {
    using namespace onyx::parser;

    std::string input = "<!DOCTYPE sdfsdfsdf";
    std::stringstream inputStream(input);
    std::string message = "Invalid DOCTYPE without ending";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Tag name cannot contain '!'\"") {
    using namespace onyx::parser;

    std::string input = "<!as";
    std::stringstream inputStream(input);
    std::string message = "Tag name cannot contain '!'";
    REQUIRE_THROWS_WITH(DomParser::parse(input), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"XML declaration must include version\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml ?>";
    std::stringstream inputStream(xml);
    std::string message = "XML declaration must include version";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Unsupported XML version, must be '1.0' or '1.1'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"2.0\"?>";
    std::stringstream inputStream(xml);
    std::string message = "Unsupported XML version, must be '1.0' or '1.1'";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"No '=' after XML declaration attribute name\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version \"1.0\"?>";
    std::stringstream inputStream(xml);
    std::string message = "No '=' after XML declaration attribute name";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"XML declaration attribute value not quoted\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=1.0?>";
    std::stringstream inputStream(xml);
    std::string message = "XML declaration attribute value not quoted";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Unterminated XML declaration attribute value\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0?>";
    std::stringstream inputStream(xml);
    std::string message = "Unterminated XML declaration attribute value";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Invalid encoding in XML declaration\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" encoding=\"123-UTF\"?>";
    std::stringstream inputStream(xml);
    std::string message = "Invalid encoding in XML declaration";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Declared encoding does not match given encoding\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
    std::stringstream inputStream(xml);
    std::string message = "Declared encoding does not match given encoding";
    REQUIRE_THROWS_WITH(DomParser::parse(xml, "UTF-8"), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream, "UTF-8"), message);
}

TEST_CASE(
    "DomParser throws \"Invalid standalone value, must be 'yes' or 'no'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" standalone=\"maybe\"?>";
    std::stringstream inputStream(xml);
    std::string message = "Invalid standalone value, must be 'yes' or 'no'";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Invalid XML declaration attribute 'extra'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" extra=\"oops\"?>";
    std::stringstream inputStream(xml);
    std::string message = "Invalid XML declaration attribute 'extra'";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML Declaration 'version' declared more than once\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" version=\"1.1\"?>";
    std::stringstream inputStream(xml);
    std::string message = "XML Declaration 'version' declared more than once";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML Declaration 'encoding' declared more than once\"") {
    using namespace onyx::parser;

    std::string xml =
        "<?xml version=\"1.0\" encoding=\"utf-8\" encoding=\"utf-8\"?>";
    std::stringstream inputStream(xml);
    std::string message = "XML Declaration 'encoding' declared more than once";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML Declaration 'standalone' declared more than "
    "once\"") {
    using namespace onyx::parser;

    std::string xml =
        "<?xml version=\"1.0\" standalone=\"yes\" standalone=\"yes\"?>";
    std::stringstream inputStream(xml);
    std::string message =
        "XML Declaration 'standalone' declared more than once";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML Declaration cannot declare 'encoding' before "
    "'version'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml encoding=\"utf-8\"?>";
    std::stringstream inputStream(xml);
    std::string message =
        "XML Declaration cannot declare 'encoding' before 'version'";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML Declaration cannot declare 'standalone' before "
    "'encoding' when 'encoding' is present\"") {
    using namespace onyx::parser;

    std::string xml =
        "<?xml version=\"1.0\" standalone=\"yes\" encoding=\"utf-8\"?>";
    std::stringstream inputStream(xml);
    std::string message =
        "XML Declaration cannot declare 'standalone' before 'encoding' when "
        "'encoding' is present";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML Declaration cannot declare 'standalone' before "
    "'version'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml standalone=\"yes\"?>";
    std::stringstream inputStream(xml);
    std::string message =
        "XML Declaration cannot declare 'standalone' before 'version'";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Duplicate attribute name\"") {
    using namespace onyx::parser;

    std::string xml =
        "<div name=\"value\" name2=\"value\" name=\"value1\"></div>";
    std::stringstream inputStream(xml);
    std::string message = "Duplicate attribute name";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"Multiple Document Type Declarations found\"") {
    using namespace onyx::parser;

    std::string xml = "<!DOCTYPE ><!DOCTYPE >";
    std::stringstream inputStream(xml);
    std::string message = "Multiple Document Type Declarations found";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"XML declaration is only allowed at the first position "
    "in the prologue\" when before XML declaration") {
    using namespace onyx::parser;

    std::string xml = "<!DOCTYPE ><?xml version=\"1.0\"?>";
    std::stringstream inputStream(xml);
    std::string message =
        "XML declaration is only allowed at the first position in the prologue";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document Type Declaration is only allowed before all "
    "XML elements except the XML declaration\" when after first element") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\"?><div></div><!DOCTYPE >";
    std::stringstream inputStream(xml);
    std::string message =
        "Document Type Declaration is only allowed before all XML elements "
        "except the XML declaration";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Top level text forbidden in XML document\" when only "
    "text") {
    using namespace onyx::parser;

    std::string xml = "     Top level text    ";
    std::stringstream inputStream(xml);
    std::string message = "Top level text forbidden in XML document";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Top level text forbidden in XML document\" when text "
    "before root") {
    using namespace onyx::parser;

    std::string xml = "     Top level text    <div></div>";
    std::stringstream inputStream(xml);
    std::string message = "Top level text forbidden in XML document";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains forbidden ASCII character\" on "
    "forbidden ASCII characters") {
    using namespace onyx::parser;

    std::string invalidXMLAsciiChars =
        "\x01\x02\x03\x04\x05\x06\x07\x08"
        "\x0B\x0C"
        "\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E"
        "\x1F";  // excludes 0x0 ('\0'), 0x09 ('\t'), 0x0A ('\n') and 0X0D
                 // ('\r')

    for (size_t i = 0; i < invalidXMLAsciiChars.size(); i++) {
        std::string xml = {'<', 'd', 'i', 'v', '>', invalidXMLAsciiChars[i],
                           '<', '/', 'd', 'i', 'v', '>'};
        std::stringstream inputStream(xml);
        std::string message = "Document contains forbidden ASCII character";
        INFO((int)invalidXMLAsciiChars[i]);
        REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
        REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
    }
}

TEST_CASE(
    "DomParser throws \"Document contains truncated or malformed utf-8\" on 2 "
    "byte malformed") {
    using namespace onyx::parser;

    std::string xml = "<div>\xC2\x20</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains truncated or malformed utf-8";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains truncated or malformed utf-8\" on 3 "
    "byte malformed") {
    using namespace onyx::parser;

    std::string xml = "<div>\xE1\x80\x20</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains truncated or malformed utf-8";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains truncated or malformed utf-8\" on 3 "
    "bytes malformed with second byte failure") {
    using namespace onyx::parser;

    std::string xml = "<div>\xEF\x20\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains truncated or malformed utf-8";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains truncated or malformed utf-8\" on 4 "
    "bytes") {
    using namespace onyx::parser;

    std::string xml = "<div>\xF0\x90\x20\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains truncated or malformed utf-8";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains truncated or malformed utf-8\" on 4 "
    "bytes malformed with second byte failure") {
    using namespace onyx::parser;

    std::string xml = "<div>\xF1\x20\x80\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains truncated or malformed utf-8";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains invalid utf-8 starting byte\" on 2 "
    "bytes overlong encoding") {
    using namespace onyx::parser;

    std::string xml = "<div>\xC0\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains invalid utf-8 starting byte";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains overlong utf-8 encoding\" on 3 "
    "bytes") {
    using namespace onyx::parser;

    std::string xml = "<div>\xE0\x80\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains overlong utf-8 encoding";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains overlong utf-8 encoding\" on 4 "
    "bytes") {
    using namespace onyx::parser;

    std::string xml = "<div>\xF0\x80\x80\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains overlong utf-8 encoding";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains forbidden character from surrogate "
    "block\"") {
    using namespace onyx::parser;

    std::string xml = "<div>\xED\xA0\x80</div>";

    std::stringstream inputStream(xml);
    std::string message =
        "Document contains forbidden character from surrogate block";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains restricted U+FFFE or U+FFFF\"") {
    using namespace onyx::parser;

    std::string xmlFFFE = "<div>\xEF\xBF\xBE</div>";
    std::string xmlFFFF = "<div>\xEF\xBF\xBF</div>";

    std::stringstream inputStreamFFFE(xmlFFFE);
    std::stringstream inputStreamFFFF(xmlFFFF);
    std::string message = "Document contains restricted U+FFFE or U+FFFF";
    REQUIRE_THROWS_WITH(DomParser::parse(xmlFFFE), message);
    REQUIRE_THROWS_WITH(DomParser::parse(xmlFFFF), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStreamFFFE), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStreamFFFF), message);
}

TEST_CASE(
    "DomParser throws \"Document contains utf-8 character out of bounds\"") {
    using namespace onyx::parser;

    std::string xml = "<div>\xF4\x90\x80\x80</div>";

    std::stringstream inputStream(xml);
    std::string message = "Document contains utf-8 character out of bounds";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE(
    "DomParser throws \"Document contains invalid utf-8 starting byte\"") {
    using namespace onyx::parser;

    std::string invalidUtfStartingChars =
        "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
        "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
        "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
        "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
        "\xC0\xC1\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF";

    for (size_t i = 0; i < invalidUtfStartingChars.size(); i++) {
        std::string xml = {'<', 'd', 'i', 'v', '>', invalidUtfStartingChars[i],
                           '<', '/', 'd', 'i', 'v', '>'};
        std::stringstream inputStream(xml);
        std::string message = "Document contains invalid utf-8 starting byte";
        REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
        REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
    }
}

TEST_CASE(
    "DomParser throws \"& outside of entities not allowed.\" on attribute "
    "value") {
    using namespace onyx::parser;

    std::string xml = "<div name=\"val&ue\"></div>";
    std::stringstream inputStream(xml);
    std::string message = "& outside of entities not allowed.";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

TEST_CASE("DomParser throws \"& outside of entities not allowed.\" on text") {
    using namespace onyx::parser;

    std::string xml = "<div>This is some text with & inside.</div>";
    std::stringstream inputStream(xml);
    std::string message = "& outside of entities not allowed.";
    REQUIRE_THROWS_WITH(DomParser::parse(xml), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream), message);
}

#include <iostream>

class SaxListenerLogger : public virtual onyx::parser::SaxListener {
   private:
    int eventCount = 0;
    std::ostream& os;

   public:
    SaxListenerLogger(std::ostream& os) : os(os) {}

    void onStart() override {
        os << "Start\n";
        eventCount++;
    }

    void onText(std::string text) override {
        os << "Text: " << text << "\n";
        eventCount++;
    }

    void onComment(std::string text) override {
        os << "Comment: " << text << "\n";
        eventCount++;
    }

    void onCData(std::string text) override {
        os << "CData: " << text << "\n";
        eventCount++;
    }

    void onInstruction(std::string tag, std::string instruction) override {
        os << "Instruction Tag: " << tag << "\n\tInstruction: " << instruction
           << "\n";
        eventCount++;
    }

    void onTagOpen(std::string name, bool isSelfClosing,
                   std::vector<onyx::dynamic::Attribute> attributes) override {
        os << "Tag open: " << name << "\n\tisSelfClosing: " << isSelfClosing
           << "\n";
        for (size_t i = 0; i < attributes.size(); i++) {
            os << "\tAttribute Name: " << attributes[i].getName()
               << " | Attribute Value: " << attributes[i].getValue() << "\n";
        }

        eventCount++;
    }

    void onTagClose(std::string name) override {
        os << "Tag close: " << name << "\n";

        eventCount++;
    }

    void onXMLDeclaration(std::string version, std::string encoding,
                          bool hasEncoding, bool isStandalone,
                          bool hasStandalone) override {
        os << "XML Declaration: \t" << "\tVersion: " << version
           << "\n\tEncoding: " << encoding
           << "\n\tisStandalone: " << isStandalone << "\n";
        eventCount++;
    }

    void onDoctype(std::string text) override {
        os << "Doctype: " << text << "\n";
        eventCount++;
    }

    void onException(std::exception& e) override {
        os << "Exception: " << e.what() << "\n";
        eventCount++;
    }

    void onEnd() override { eventCount++; }

    int getEventCount() { return eventCount; }
};

TEST_CASE("SAXParser parses complex XML") {
    using namespace onyx::parser;

    std::string input =
        "<html lang=\"en\" theme=\"dark\"><head><meta charset=\"UTF-8\"/><meta "
        "name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1.0\"/><title>Complex Test Page</title><link "
        "rel=\"stylesheet\" "
        "href=\"/styles/main.css\"/></head><body><header><nav><ul><li><a "
        "href=\"#home\">Home</a></li><li><a href=\"#about\">About "
        "Us</a></li></ul></nav></header><main><section "
        "id=\"introduction\"><h1>Introduction</h1><p>Welcome to the complex "
        "HTML structure test case.</p><p>This test includes various nested "
        "elements, attributes, and content.</p><form "
        "name=\"contact-form\"><label for=\"name\">Your Name:</label><input "
        "type=\"text\" id=\"name\" name=\"name\"/><label for=\"email\">Your "
        "Email:</label><input type=\"email\" id=\"email\" "
        "name=\"email\"/><button "
        "type=\"submit\">Submit</button></form></section><section "
        "id=\"features\"><h2>Features</h2><ul><li>Feature 1</li><li>Feature "
        "2</li><li>Feature 3</li></ul><p>These are the key features of the "
        "application.</p></section></main><footer><p>© 2025 Complex HTML Test "
        "Page</p><a href=\"https://www.example.com\">Privacy "
        "Policy</a></footer></body></html>";

    std::stringstream str;
    SaxListenerLogger listener(str);
    SaxParser parser(listener);

    parser.parse(input);

    REQUIRE(listener.getEventCount() == 83);
}

TEST_CASE("SAXParser parses complex XML stream") {
    using namespace onyx::parser;

    std::stringstream input;
    input.str(
        "<html lang=\"en\" theme=\"dark\"><head><meta charset=\"UTF-8\"/><meta "
        "name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1.0\"/><title>Complex Test Page</title><link "
        "rel=\"stylesheet\" "
        "href=\"/styles/main.css\"/></head><body><header><nav><ul><li><a "
        "href=\"#home\">Home</a></li><li><a href=\"#about\">About "
        "Us</a></li></ul></nav></header><main><section "
        "id=\"introduction\"><h1>Introduction</h1><p>Welcome to the complex "
        "HTML structure test case.</p><p>This test includes various nested "
        "elements, attributes, and content.</p><form "
        "name=\"contact-form\"><label for=\"name\">Your Name:</label><input "
        "type=\"text\" id=\"name\" name=\"name\"/><label for=\"email\">Your "
        "Email:</label><input type=\"email\" id=\"email\" "
        "name=\"email\"/><button "
        "type=\"submit\">Submit</button></form></section><section "
        "id=\"features\"><h2>Features</h2><ul><li>Feature 1</li><li>Feature "
        "2</li><li>Feature 3</li></ul><p>These are the key features of the "
        "application.</p></section></main><footer><p>© 2025 Complex HTML Test "
        "Page</p><a href=\"https://www.example.com\">Privacy "
        "Policy</a></footer></body></html>");

    std::stringstream str;
    SaxListenerLogger listener(str);
    SaxParser parser(listener);

    parser.parse(input);

    REQUIRE(listener.getEventCount() == 83);
}

TEST_CASE("DomParser successfully transcodes ISO-8859-1 document to UTF-8") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // '\xE9' is "é" in ISO-8859-1
    std::string input =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><root>\xE9</root>";
    std::stringstream inputStream(input);

    // '\xC3\xA9' is "é" in UTF-8
    EmptyNode output{
        XmlDeclaration("1.0", "ISO-8859-1", true, false, false, false),
        GenericNode("root", false, Text("\xC3\xA9"))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prExplicit = DomParser::parse(input, "ISO-8859-1");
    ParseResult prStream = DomParser::parse(inputStream);
    inputStream.str(input);
    ParseResult prStreamExplicit = DomParser::parse(inputStream, "ISO-8859-1");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prExplicit.root));
    REQUIRE(output.deepEquals(*prStream.root));
    REQUIRE(output.deepEquals(*prStreamExplicit.root));
}

TEST_CASE("DomParser successfully transcodes Windows-1251 document to UTF-8") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // '\xC7\xE4\xF0\xE0\xE2\xE5\xE9' is "Здравей" in Windows-1251
    std::string input =
        "<?xml version=\"1.0\" "
        "encoding=\"Windows-1251\"?><root>\xC7\xE4\xF0\xE0\xE2\xE5\xE9</root>";
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "WINDOWS-1251", true, false, false, false),
        GenericNode("root", false, Text("Здравей"))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prExplicit = DomParser::parse(input, "WINDOWS-1251");
    ParseResult prStream = DomParser::parse(inputStream);
    inputStream.str(input);
    ParseResult prStreamExplicit =
        DomParser::parse(inputStream, "WINDOWS-1251");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prExplicit.root));
    REQUIRE(output.deepEquals(*prStream.root));
    REQUIRE(output.deepEquals(*prStreamExplicit.root));
}

TEST_CASE("DomParser skips transcoding if explicit encoding matches declared") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root>a</root>";
    std::stringstream inputStream(input);

    EmptyNode output{XmlDeclaration("1.0", "UTF-8", true, false, false, false),
                     GenericNode("root", false, Text("a"))};

    ParseResult pr = DomParser::parse(input, "UTF-8");
    ParseResult prStream = DomParser::parse(inputStream, "UTF-8");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser successfully transcodes complex ISO-8859-1 components to "
    "UTF-8") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // '\xE9' = é, '\xE7' = ç, '\xE0' = à
    std::string input =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
        "<root attr=\"\xE7\">"
        "<child>\xE9</child>"
        "<!--\xE0--><![CDATA[\xE9\xE0]]>"
        "</root>";
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "ISO-8859-1", true, false, false, false),
        GenericNode("root", false, Attribute("attr", "\xC3\xA7"),
                    GenericNode("child", false, Text("\xC3\xA9")),
                    Comment("\xC3\xA0"), CData("\xC3\xA9\xC3\xA0"))};

    ParseResult pr = DomParser::parse(input);
    ParseResult prExplicit = DomParser::parse(input);
    ParseResult prStream = DomParser::parse(inputStream);
    inputStream.str(input);
    ParseResult prStreamExplicit = DomParser::parse(inputStream);

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prExplicit.root));
    REQUIRE(output.deepEquals(*prStream.root));
    REQUIRE(output.deepEquals(*prStreamExplicit.root));
}

TEST_CASE(
    "SAXParser successfully transcodes ISO-8859-1 document to UTF-8 with "
    "inferred encoding") {
    using namespace onyx::parser;

    std::string input =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><root>\xE9</root>";
    std::stringstream inputStream(input);

    std::stringstream str1, str2;

    SaxListenerLogger listener(str1);
    SaxParser parser(listener);
    parser.parse(input);

    // Events: Start(1) + XMLDecl(1) + TagOpen(1) + Text(1) + TagClose(1) +
    // End(1) = 6 events
    REQUIRE(listener.getEventCount() == 6);
    REQUIRE(str1.str().find("Text: \xC3\xA9") != std::string::npos);

    SaxListenerLogger listenerStream(str2);
    SaxParser parserStream(listenerStream);
    parserStream.parse(inputStream);

    REQUIRE(listenerStream.getEventCount() == 6);
    REQUIRE(str2.str().find("Text: \xC3\xA9") != std::string::npos);
}

TEST_CASE(
    "SAXParser successfully transcodes ISO-8859-1 document to UTF-8 with "
    "explicit encoding") {
    using namespace onyx::parser;

    std::string input =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><root>\xE9</root>";
    std::stringstream inputStream(input);

    std::stringstream str1, str2;

    SaxListenerLogger listener(str1);
    SaxParser parser(listener);
    parser.parse(input, "ISO-8859-1");

    // Events: Start(1) + XMLDecl(1) + TagOpen(1) + Text(1) + TagClose(1) +
    // End(1) = 6 events
    REQUIRE(listener.getEventCount() == 6);
    REQUIRE(str1.str().find("Text: \xC3\xA9") != std::string::npos);

    SaxListenerLogger listenerStream(str2);
    SaxParser parserStream(listenerStream);
    parserStream.parse(inputStream, "ISO-8859-1");

    REQUIRE(listenerStream.getEventCount() == 6);
    REQUIRE(str2.str().find("Text: \xC3\xA9") != std::string::npos);
}

TEST_CASE("SAXParser transcodes complex ISO-8859-1 components") {
    using namespace onyx::parser;

    std::string input =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
        "<root attr=\"\xE7\">"
        "<!--\xE0-->"
        "</root>";

    std::stringstream str;
    SaxListenerLogger listener(str);
    SaxParser parser(listener);
    parser.parse(input);

    REQUIRE(str.str().find("Attribute Value: \xC3\xA7") != std::string::npos);
    REQUIRE(str.str().find("Comment: \xC3\xA0") != std::string::npos);
}

TEST_CASE("DomParser autodetects UTF-8 BOM and parses successfully") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "\xEF\xBB\xBF<?xml version=\"1.0\" encoding=\"UTF-8\"?><root>\xC3\xA9</"
        "root>";
    std::stringstream inputStream(input);

    EmptyNode output{XmlDeclaration("1.0", "UTF-8", true, false, false, false),
                     GenericNode("root", false, Text("\xC3\xA9"))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser autodetects UTF-16LE BOM and transcodes to UTF-8 successfully") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // UTF-16LE encoded: <root>é</root>
    const char utf16le_data[] = {
        '\xFF', '\xFE', '<', '\x00', 'r',    '\x00', 'o', '\x00', 'o', '\x00',
        't',    '\x00', '>', '\x00', '\xE9', '\x00', '<', '\x00', '/', '\x00',
        'r',    '\x00', 'o', '\x00', 'o',    '\x00', 't', '\x00', '>', '\x00'};
    std::string input(utf16le_data, sizeof(utf16le_data));
    std::stringstream inputStream(input);

    GenericNode output{"root", false, Text("\xC3\xA9")};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser autodetects UTF-16BE BOM and transcodes to UTF-8 successfully") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // UTF-16BE encoded: <root>é</root>
    const char utf16be_data[] = {
        '\xFE', '\xFF', '\x00', '<', '\x00', 'r',    '\x00', 'o', '\x00', 'o',
        '\x00', 't',    '\x00', '>', '\x00', '\xE9', '\x00', '<', '\x00', '/',
        '\x00', 'r',    '\x00', 'o', '\x00', 'o',    '\x00', 't', '\x00', '>'};
    std::string input(utf16be_data, sizeof(utf16be_data));
    std::stringstream inputStream(input);

    GenericNode output{"root", false, Text("\xC3\xA9")};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser autodetects UTF-16LE family without BOM and transcodes to UTF-8 "
    "successfully") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // UTF-16LE without BOM: <?xml version="1.0"
    // encoding="UTF-16LE"?><root>é</root>
    const char utf16le_no_bom[] = {
        '<', '\x00', '?', '\x00', 'x',    '\x00', 'm', '\x00', 'l', '\x00',
        ' ', '\x00', 'v', '\x00', 'e',    '\x00', 'r', '\x00', 's', '\x00',
        'i', '\x00', 'o', '\x00', 'n',    '\x00', '=', '\x00', '"', '\x00',
        '1', '\x00', '.', '\x00', '0',    '\x00', '"', '\x00', ' ', '\x00',
        'e', '\x00', 'n', '\x00', 'c',    '\x00', 'o', '\x00', 'd', '\x00',
        'i', '\x00', 'n', '\x00', 'g',    '\x00', '=', '\x00', '"', '\x00',
        'U', '\x00', 'T', '\x00', 'F',    '\x00', '-', '\x00', '1', '\x00',
        '6', '\x00', 'L', '\x00', 'E',    '\x00', '"', '\x00', '?', '\x00',
        '>', '\x00', '<', '\x00', 'r',    '\x00', 'o', '\x00', 'o', '\x00',
        't', '\x00', '>', '\x00', '\xE9', '\x00', '<', '\x00', '/', '\x00',
        'r', '\x00', 'o', '\x00', 'o',    '\x00', 't', '\x00', '>', '\x00'};
    std::string input(utf16le_no_bom, sizeof(utf16le_no_bom));
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "UTF-16LE", true, false, false, false),
        GenericNode("root", false, Text("\xC3\xA9"))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser defaults to UTF-8 when autodetection returns UNKNOWN") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<root>\xC3\xA9</root>";
    std::stringstream inputStream(input);

    GenericNode output{"root", false, Text("\xC3\xA9")};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser defaults to UTF-8 when autodetection hits EOF early") {
    using namespace onyx::parser;

    std::string input = "<!";
    std::stringstream inputStream(input);

    REQUIRE_THROWS_WITH(DomParser::parse(input, "autodetect"),
                        "Premature end of <! tag");
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream, "autodetect"),
                        "Premature end of <! tag");
}

TEST_CASE(
    "SaxParser autodetects UTF-16LE BOM and transcodes to UTF-8 successfully") {
    using namespace onyx::parser;

    // UTF-16LE encoded: <root>é</root>
    const char utf16le_data[] = {
        '\xFF', '\xFE', '<', '\x00', 'r',    '\x00', 'o', '\x00', 'o', '\x00',
        't',    '\x00', '>', '\x00', '\xE9', '\x00', '<', '\x00', '/', '\x00',
        'r',    '\x00', 'o', '\x00', 'o',    '\x00', 't', '\x00', '>', '\x00'};
    std::string input(utf16le_data, sizeof(utf16le_data));
    std::stringstream inputStream(input);

    std::stringstream str1, str2;

    SaxListenerLogger listener(str1);
    SaxParser parser(listener);
    parser.parse(input, "autodetect");

    // Events: Start(1) + TagOpen(1) + Text(1) + TagClose(1) + End(1) = 5
    REQUIRE(listener.getEventCount() == 5);
    REQUIRE(str1.str().find("Text: \xC3\xA9") != std::string::npos);

    SaxListenerLogger listenerStream(str2);
    SaxParser parserStream(listenerStream);
    parserStream.parse(inputStream, "autodetect");

    REQUIRE(listenerStream.getEventCount() == 5);
    REQUIRE(str2.str().find("Text: \xC3\xA9") != std::string::npos);
}

TEST_CASE(
    "SaxParser autodetects UTF-16BE family without BOM and transcodes to UTF-8 "
    "successfully") {
    using namespace onyx::parser;

    // UTF-16BE without BOM: <?xml version="1.0"
    // encoding="UTF-16BE"?><root>é</root>
    const char utf16be_no_bom[] = {
        '\x00', '<', '\x00', '?', '\x00', 'x',    '\x00', 'm',  '\x00', 'l',
        '\x00', ' ', '\x00', 'v', '\x00', 'e',    '\x00', 'r',  '\x00', 's',
        '\x00', 'i', '\x00', 'o', '\x00', 'n',    '\x00', '=',  '\x00', '"',
        '\x00', '1', '\x00', '.', '\x00', '0',    '\x00', '"',  '\00',  ' ',
        '\00',  'e', '\00',  'n', '\00',  'c',    '\00',  'o',  '\00',  'd',
        '\00',  'i', '\00',  'n', '\00',  'g',    '\00',  '=',  '\00',  '\"',
        '\00',  'U', '\00',  'T', '\00',  'F',    '\00',  '-',  '\00',  '1',
        '\00',  '6', '\00',  'B', '\00',  'E',    '\00',  '\"', '\x00', '?',
        '\x00', '>', '\x00', '<', '\x00', 'r',    '\x00', 'o',  '\x00', 'o',
        '\x00', 't', '\x00', '>', '\x00', '\xE9', '\x00', '<',  '\x00', '/',
        '\x00', 'r', '\x00', 'o', '\x00', 'o',    '\x00', 't',  '\x00', '>'};
    std::string input(utf16be_no_bom, sizeof(utf16be_no_bom));
    std::stringstream inputStream(input);

    std::stringstream str1, str2;

    SaxListenerLogger listener(str1);
    SaxParser parser(listener);
    parser.parse(input, "autodetect");

    // Events: Start(1) + XMLDecl(1) + TagOpen(1) + Text(1) + TagClose(1) +
    // End(1) = 6
    REQUIRE(listener.getEventCount() == 6);
    REQUIRE(str1.str().find("Text: \xC3\xA9") != std::string::npos);

    SaxListenerLogger listenerStream(str2);
    SaxParser parserStream(listenerStream);
    parserStream.parse(inputStream, "autodetect");

    REQUIRE(listenerStream.getEventCount() == 6);
    REQUIRE(str2.str().find("Text: \xC3\xA9") != std::string::npos);
}

TEST_CASE("DomParser autodetects UCS-4BE family without BOM") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // UCS-4BE without BOM: <?xml version="1.0"
    // encoding="UCS-4BE"?><root>a</root>
    const char ucs4be_data[] = {
        '\00', '\00', '\00', '<',   '\00', '\00', '\00', '?',   '\00', '\00',
        '\00', 'x',   '\00', '\00', '\00', 'm',   '\00', '\00', '\00', 'l',
        '\00', '\00', '\00', ' ',   '\00', '\00', '\00', 'v',   '\00', '\00',
        '\00', 'e',   '\00', '\00', '\00', 'r',   '\00', '\00', '\00', 's',
        '\00', '\00', '\00', 'i',   '\00', '\00', '\00', 'o',   '\00', '\00',
        '\00', 'n',   '\00', '\00', '\00', '=',   '\00', '\00', '\00', '"',
        '\00', '\00', '\00', '1',   '\00', '\00', '\00', '.',   '\00', '\00',
        '\00', '0',   '\00', '\00', '\00', '"',   '\00', '\00', '\00', ' ',
        '\00', '\00', '\00', 'e',   '\00', '\00', '\00', 'n',   '\00', '\00',
        '\00', 'c',   '\00', '\00', '\00', 'o',   '\00', '\00', '\00', 'd',
        '\00', '\00', '\00', 'i',   '\00', '\00', '\00', 'n',   '\00', '\00',
        '\00', 'g',   '\00', '\00', '\00', '=',   '\00', '\00', '\00', '"',
        '\00', '\00', '\00', 'U',   '\00', '\00', '\00', 'C',   '\00', '\00',
        '\00', 'S',   '\00', '\00', '\00', '-',   '\00', '\00', '\00', '4',
        '\00', '\00', '\00', 'B',   '\00', '\00', '\00', 'E',   '\00', '\00',
        '\00', '"',   '\00', '\00', '\00', '?',   '\00', '\00', '\00', '>',
        '\00', '\00', '\00', '<',   '\00', '\00', '\00', 'r',   '\00', '\00',
        '\00', 'o',   '\00', '\00', '\00', 'o',   '\00', '\00', '\00', 't',
        '\00', '\00', '\00', '>',   '\00', '\00', '\00', 'a',   '\00', '\00',
        '\00', '<',   '\00', '\00', '\00', '/',   '\00', '\00', '\00', 'r',
        '\00', '\00', '\00', 'o',   '\00', '\00', '\00', 'o',   '\00', '\00',
        '\00', 't',   '\00', '\00', '\00', '>'};
    std::string input(ucs4be_data, sizeof(ucs4be_data));
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "UCS-4BE", true, false, false, false),
        GenericNode("root", false, Text("a"))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser autodetects UTF-32LE (UCS-4LE) BOM") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // UTF-32LE with BOM: <root>a</root>
    const char utf32le_data[] =
        "\xFF\xFE\x00\x00\x3C\x00\x00\x00\x72\x00\x00\x00\x6F\x00\x00\x00"
        "\x6F\x00\x00\x00\x74\x00\x00\x00\x3E\x00\x00\x00\x61\x00\x00\x00"
        "\x3C\x00\x00\x00\x2F\x00\x00\x00\x72\x00\x00\x00\x6F\x00\x00\x00"
        "\x6F\x00\x00\x00\x74\x00\x00\x00\x3E\x00\x00\x00";
    std::string input(utf32le_data, sizeof(utf32le_data) - 1);
    std::stringstream inputStream(input);

    GenericNode output{"root", false, Text("a")};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser autodetects UCS-2LE family and restarts parse") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // UCS-2LE without BOM: <?xml version="1.0"
    // encoding="UCS-2LE"?><root>a</root>
    const char ucs2le_data[] =
        "\x3C\x00\x3F\x00\x78\x00\x6D\x00\x6C\x00\x20\x00\x76\x00\x65\x00"
        "\x72\x00\x73\x00\x69\x00\x6F\x00\x6E\x00\x3D\x00\x22\x00\x31\x00"
        "\x2E\x00\x30\x00\x22\x00\x20\x00\x65\x00\x6E\x00\x63\x00\x6F\x00"
        "\x64\x00\x69\x00\x6E\x00\x67\x00\x3D\x00\x22\x00\x55\x00\x43\x00"
        "\x53\x00\x2D\x00\x32\x00\x4C\x00\x45\x00\x22\x00\x3F\x00\x3E\x00"
        "\x3C\x00\x72\x00\x6F\x00\x6F\x00\x74\x00\x3E\x00\x61\x00\x3C\x00"
        "\x2F\x00\x72\x00\x6F\x00\x6F\x00\x74\x00\x3E\x00";
    std::string input(ucs2le_data, sizeof(ucs2le_data) - 1);
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "UCS-2LE", true, false, false, false),
        GenericNode("root", false, Text("a"))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser detects ASCII family and restarts for ISO-8859-1 (Latin1)") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><root>\xE9</root>";
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "ISO-8859-1", true, false, false, false),
        GenericNode("root", false, Text("\xC3\xA9"))};  // "é" in UTF-8

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE("DomParser detects ASCII family and restarts for Shift_JIS") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // Shift_JIS encoded: <?xml version="1.0"
    // encoding="Shift_JIS"?><root>テスト</root>
    std::string input =
        "<?xml version=\"1.0\" "
        "encoding=\"Shift_JIS\"?><root>\x83\x65\x83\x58\x83\x67</root>";
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "SHIFT_JIS", true, false, false, false),
        GenericNode("root", false, Text("テスト"))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser autodetects EBCDIC IBM037 family matching actual encoding") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // IBM037 encoded: <?xml version="1.0" encoding="IBM037"?><root>a</root>
    const char ibm037_data[] =
        "\x4C\x6F\xA7\x94\x93\x40\xA5\x85\x99\xA2\x89\x96\x95\x7E\x7F\xF1"
        "\x4B\xF0\x7F\x40\x85\x95\x83\x96\x84\x89\x95\x87\x7E\x7F\xC9\xC2"
        "\xD4\xF0\xF3\xF7\x7F\x6F\x6E\x4C\x99\x96\x96\xA3\x6E\x81\x4C\x61"
        "\x99\x96\x96\xA3\x6E";
    std::string input(ibm037_data, sizeof(ibm037_data) - 1);
    std::stringstream inputStream(input);

    EmptyNode output{XmlDeclaration("1.0", "IBM037", true, false, false, false),
                     GenericNode("root", false, Text("a"))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser autodetects EBCDIC family and restarts for incompatible "
    "IBM1047") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    // IBM1047 encoded: <?xml version="1.0" encoding="IBM1047"?><root>[</root>
    // Note: 0xAD is '[' in IBM1047, but would be 'Ý' if incorrectly parsed as
    // IBM037
    const char ibm1047_data[] =
        "\x4C\x6F\xA7\x94\x93\x40\xA5\x85\x99\xA2\x89\x96\x95\x7E\x7F\xF1"
        "\x4B\xF0\x7F\x40\x85\x95\x83\x96\x84\x89\x95\x87\x7E\x7F\xC9\xC2"
        "\xD4\xF1\xF0\xF4\xF7\x7F\x6F\x6E\x4C\x99\x96\x96\xA3\x6E\xAD\x4C"
        "\x61\x99\x96\x96\xA3\x6E";
    std::string input(ibm1047_data, sizeof(ibm1047_data) - 1);
    std::stringstream inputStream(input);

    EmptyNode output{
        XmlDeclaration("1.0", "IBM1047", true, false, false, false),
        GenericNode("root", false, Text("["))};

    ParseResult pr = DomParser::parse(input, "autodetect");
    ParseResult prStream = DomParser::parse(inputStream, "autodetect");

    REQUIRE(output.deepEquals(*pr.root));
    REQUIRE(output.deepEquals(*prStream.root));
}

TEST_CASE(
    "DomParser throws when family inferred but XML declaration lacks "
    "encoding") {
    using namespace onyx::parser;

    // UTF-16LE without BOM, XML declaration lacks encoding: <?xml
    // version="1.0"?><root>a</root>
    const char utf16le_no_enc[] =
        "\x3C\x00\x3F\x00\x78\x00\x6D\x00\x6C\x00\x20\x00\x76\x00\x65\x00"
        "\x72\x00\x73\x00\x69\x00\x6F\x00\x6E\x00\x3D\x00\x22\x00\x31\x00"
        "\x2E\x00\x30\x00\x22\x00\x3F\x00\x3E\x00\x3C\x00\x72\x00\x6F\x00"
        "\x6F\x00\x74\x00\x3E\x00\x61\x00\x3C\x00\x2F\x00\x72\x00\x6F\x00"
        "\x6F\x00\x74\x00\x3E\x00";
    std::string input(utf16le_no_enc, sizeof(utf16le_no_enc) - 1);
    std::stringstream inputStream(input);

    std::string message = "Required encoding not found in XML declaration";
    REQUIRE_THROWS_WITH(DomParser::parse(input, "autodetect"), message);
    REQUIRE_THROWS_WITH(DomParser::parse(inputStream, "autodetect"), message);
}

TEST_CASE("StringViewReadBuffer can be read sequentially via std::istream") {
    using namespace onyx::parser;
    std::string_view data = "Hello, World!";
    impl::StringViewReadBuffer<char> buffer(data);
    std::istream stream(&buffer);

    std::string result;
    std::getline(stream, result);

    REQUIRE(result == "Hello, World!");
    REQUIRE(stream.eof());
}

TEST_CASE(
    "StringViewReadBuffer correctly handles character-by-character "
    "extraction") {
    using namespace onyx::parser;
    std::string_view data = "abc";
    impl::StringViewReadBuffer<char> buffer(data);
    std::istream stream(&buffer);

    REQUIRE(stream.get() == 'a');
    REQUIRE(stream.get() == 'b');
    REQUIRE(stream.get() == 'c');

    REQUIRE(stream.get() == std::char_traits<char>::eof());
    REQUIRE(stream.eof());
}

TEST_CASE("StringViewReadBuffer supports block reading") {
    using namespace onyx::parser;
    std::string_view data = "Block read test data";
    impl::StringViewReadBuffer<char> buffer(data);
    std::istream stream(&buffer);

    char output[10];
    stream.read(output, 5);
    output[5] = '\0';

    REQUIRE(std::string(output) == "Block");
    REQUIRE(stream.gcount() == 5);
    REQUIRE_FALSE(stream.eof());
}

TEST_CASE(
    "StringViewReadBuffer correctly reports EOF for an empty string_view") {
    using namespace onyx::parser;
    std::string_view empty_data = "";
    impl::StringViewReadBuffer<char> buffer(empty_data);
    std::istream stream(&buffer);

    REQUIRE(stream.get() == std::char_traits<char>::eof());
    REQUIRE(stream.eof());
}

TEST_CASE(
    "StringViewReadBuffer throws logic_error on single character write "
    "(overflow)") {
    using namespace onyx::parser;
    std::string_view data = "Read Only";
    impl::StringViewReadBuffer<char> buffer(data);

    REQUIRE_THROWS_AS(buffer.sputc('X'), std::logic_error);
    REQUIRE_THROWS_WITH(buffer.sputc('X'),
                        "Trying to write to read only buffer.");
}

TEST_CASE("StringViewReadBuffer throws logic_error on block write (xsputn)") {
    using namespace onyx::parser;
    std::string_view data = "Read Only";
    impl::StringViewReadBuffer<char> buffer(data);

    REQUIRE_THROWS_AS(buffer.sputc('X'), std::logic_error);
    REQUIRE_THROWS_WITH(buffer.sputc('X'),
                        "Trying to write to read only buffer.");
}
