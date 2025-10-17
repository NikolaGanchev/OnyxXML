#include <chrono>

#include "catch2/catch_all.hpp"
#include "onyx.h"

TEST_CASE("DomParser works") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<html><head></head></html>";

    GenericNode output{"html", false, GenericNode("head", false)};

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with text") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html><body><div> Hello<span></span>World! </div></body></html>";

    GenericNode output{
        "html", false,
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with a single attribute") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\"><body><div> Hello<span></span>World! "
        "</div></body></html>";

    GenericNode output{
        "html", false, Attribute("theme", "dark"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with many attributes") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\" lang='en'><body><div> Hello<span></span>World! "
        "</div></body></html>";

    GenericNode output{
        "html", false, Attribute("theme", "dark"), Attribute("lang", "en"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser expands entities in text") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\" lang='en'><body><div> 4 &#60; 5; "
        "</div></body></html>";

    GenericNode output{
        "html", false, Attribute("theme", "dark"), Attribute("lang", "en"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" 4 < 5; ")))};

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser expands entities in attribute values") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark&apos;\" lang='en'><body><div> "
        "Hello<span></span>World! </div></body></html>";

    GenericNode output{
        "html", false, Attribute("theme", "dark'"), Attribute("lang", "en"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
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

    auto t1 = high_resolution_clock::now();
    ParseResult pr = DomParser::parse(input);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;

    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));

    INFO(time.count());
}

TEST_CASE("DomParser works with comments") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<html theme=\"dark\"><!--This is a comment!--><body><div> "
        "Hello<span></span>World! </div></body></html>";

    GenericNode output{
        "html", false, Attribute("theme", "dark"),
        Comment("This is a comment!"),
        GenericNode("body", false,
                    GenericNode("div", false, Text(" Hello"),
                                GenericNode("span", false), Text("World! ")))};

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with processing instructions") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<root lang=\"en\"><?templater doSomething 5 > 4 "
        "somethingElse?></root>";

    GenericNode output{
        "root", false, Attribute("lang", "en"),
        ProcessingInstruction("templater", "doSomething 5 > 4 somethingElse")};

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with CDATA") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<root lang=\"en\"><![CDATA[<someElement> This is some literal text, "
        "in which & and <, > can safely be written!]]></root>";

    GenericNode output{"root", false, Attribute("lang", "en"),
                       CData("<someElement> This is some literal text, in "
                             "which & and <, > can safely be written!")};

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with XML declarations") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.0\"?>";

    XmlDeclaration output("1.0", "UTF-8", false, false, false, false);

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    INFO(dynamic_cast<XmlDeclaration*>(pr.root)->getStandalone());
    INFO(dynamic_cast<XmlDeclaration*>(pr.root)->getEncoding());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with XML declarations with encoding") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.1\" encoding=\"ISO-8859-1\"?>";

    XmlDeclaration output("1.1", "ISO-8859-1", true, false, false, false);

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with XML declarations with standalone") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input = "<?xml version=\"1.0\" standalone=\"yes\"?>";

    XmlDeclaration output("1.0", "UTF-8", false, true, true, false);

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with DOCTYPE") {
    using namespace onyx::tags;
    using namespace onyx::parser;

    std::string input =
        "<!DOCTYPE html><html theme=\"dark\"><body><div> "
        "Hello<span></span>World! </div></body></html>";

    EmptyNode output{
        Doctype("html"),
        GenericNode("html", false, Attribute("theme", "dark"),
                    GenericNode("body", false,
                                GenericNode("div", false, Text(" Hello"),
                                            GenericNode("span", false),
                                            Text("World! "))))};

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
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

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser throws \"Invalid end after tag open\"") {
    using namespace onyx::parser;

    std::string input = "<tag>   \n\t   ";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Unclosed tags left");
}

TEST_CASE("DomParser throws \"Premature end of document after <\"") {
    using namespace onyx::parser;

    std::string input = "<";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Premature end of document");
}

TEST_CASE("DomParser throws \"Premature end in processing instruction\"") {
    using namespace onyx::parser;

    std::string input = "<?";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Premature end of document");
}

TEST_CASE("DomParser throws \"Empty processing instruction tag name\"") {
    using namespace onyx::parser;

    std::string input = "<? >";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Invalid tag name");
}

TEST_CASE(
    "DomParser throws \"XML declaration is only allowed at the first position "
    "in the prologue\"") {
    using namespace onyx::parser;

    std::string input = "<tag></tag><?xml ?>";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "XML declaration is only allowed at the first position "
                        "in the prologue");
}

TEST_CASE("DomParser throws \"Missing space after PI target\"") {
    using namespace onyx::parser;

    std::string input = "<?pi?>";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "No space between processing instruction target and "
                        "processing instruction content");
}

TEST_CASE("DomParser throws \"Unterminated processing instruction\"") {
    using namespace onyx::parser;

    std::string input = "<?pi content";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Invalid processing instruction without ending");
}

TEST_CASE("DomParser throws \"Unterminated comment\"") {
    using namespace onyx::parser;

    std::string input = "<!-- comment";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Invalid comment without ending");
}

TEST_CASE("DomParser throws \"-- inside comment\"") {
    using namespace onyx::parser;

    std::string input = "<!-- comment-- ->";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "-- inside of comment not allowed");
}

TEST_CASE("DomParser throws \"Empty tag name\"") {
    using namespace onyx::parser;

    std::string input = "<>";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Invalid tag name");
}

TEST_CASE("DomParser throws \"Invalid attribute name\"") {
    using namespace onyx::parser;

    std::string input = "<tag =\"value\">";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Invalid non-closing tag");
}

TEST_CASE("DomParser throws \"No equals after attribute name\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr>";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "No = after attribute");
}

TEST_CASE("DomParser throws \"Premature end at attribute\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Premature end at attribute");
}

TEST_CASE("DomParser throws \"No quote after attribute equals\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=value>";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "No quote (\" or ') after attribute =");
}

TEST_CASE("DomParser throws \"Improperly closed attribute value\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Improperly closed attribute value");
}

TEST_CASE("DomParser throws \"No whitespace after closing attribute quote\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val\"x>";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "No whitespace after attribute closing quote");
}

TEST_CASE("DomParser throws \"Premature end after attribute\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val\"";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "No whitespace after attribute closing quote");
}

TEST_CASE("DomParser throws \"Double closing tag\"") {
    using namespace onyx::parser;

    std::string input = "</tag/>";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Trying to double-close closing tag");
}

TEST_CASE("DomParser throws \"Invalid tag close after /\"") {
    using namespace onyx::parser;

    std::string input = "<tag/ ";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Invalid tag close - must have > after /");
}

TEST_CASE("DomParser throws \"No tag close\"") {
    using namespace onyx::parser;

    std::string input = "<tag attr=\"val\" x";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "No = after attribute");
}

TEST_CASE("DomParser throws \"Closing unopened tag\"") {
    using namespace onyx::parser;

    std::string input = "<a></b>";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Closing unopened tag");
}

TEST_CASE("DomParser throws \"Closing non-existent tags\"") {
    using namespace onyx::parser;

    std::string input = "</a>";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Closing unopened tag");
}

TEST_CASE("DomParser throws \"Unclosed tags left\"") {
    using namespace onyx::parser;

    std::string input = "<a><b></b>";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Unclosed tags left");
}

TEST_CASE("DomParser throws \"Premature end of CDATA section\"") {
    using namespace onyx::parser;

    std::string input = "<![CD";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Premature end of CDATA section");
}

TEST_CASE("DomParser throws \"Invalid CDATA without ending\"") {
    using namespace onyx::parser;

    std::string input = "<![CDATA[asdfasfasfasfasfasfasda";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Invalid CDATA without ending");
}

TEST_CASE("DomParser throws \"Premature end of DOCTYPE section\"") {
    using namespace onyx::parser;

    std::string input = "<!DOC";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Premature end of DOCTYPE section");
}

TEST_CASE("DomParser throws \"Invalid DOCTYPE without ending\"") {
    using namespace onyx::parser;

    std::string input = "<!DOCTYPE sdfsdfsdf";
    REQUIRE_THROWS_WITH(DomParser::parse(input),
                        "Invalid DOCTYPE without ending");
}

TEST_CASE("DomParser throws \"Tag name cannot contain '!'\"") {
    using namespace onyx::parser;

    std::string input = "<!as";
    REQUIRE_THROWS_WITH(DomParser::parse(input), "Tag name cannot contain '!'");
}

TEST_CASE("DomParser throws \"XML declaration must include version\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml encoding=\"UTF-8\"?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "XML declaration must include version");
}

TEST_CASE(
    "DomParser throws \"Unsupported XML version, must be '1.0' or '1.1'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"2.0\"?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "Unsupported XML version, must be '1.0' or '1.1'");
}

TEST_CASE("DomParser throws \"No '=' after XML declaration attribute name\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version \"1.0\"?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "No '=' after XML declaration attribute name");
}

TEST_CASE("DomParser throws \"XML declaration attribute value not quoted\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=1.0?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "XML declaration attribute value not quoted");
}

TEST_CASE("DomParser throws \"Unterminated XML declaration attribute value\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "Unterminated XML declaration attribute value");
}

TEST_CASE("DomParser throws \"Invalid encoding in XML declaration\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" encoding=\"123-UTF\"?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "Invalid encoding in XML declaration");
}

TEST_CASE(
    "DomParser throws \"Invalid standalone value, must be 'yes' or 'no'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" standalone=\"maybe\"?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "Invalid standalone value, must be 'yes' or 'no'");
}

TEST_CASE("DomParser throws \"Invalid XML declaration attribute 'extra'\"") {
    using namespace onyx::parser;

    std::string xml = "<?xml version=\"1.0\" extra=\"oops\"?>";
    REQUIRE_THROWS_WITH(DomParser::parse(xml),
                        "Invalid XML declaration attribute 'extra'");
}

#include <iostream>

class SaxListenerLogger : public virtual onyx::parser::SaxListener {
   private:
    int eventCount = 0;

   public:
    void onStart() override {
        std::cout << "Start\n";
        eventCount++;
    }

    void onText(std::string text) override {
        std::cout << "Text: " << text << "\n";
        eventCount++;
    }

    void onComment(std::string text) override {
        std::cout << "Comment: " << text << "\n";
        eventCount++;
    }

    void onCData(std::string text) override {
        std::cout << "CData: " << text << "\n";
        eventCount++;
    }

    void onInstruction(std::string tag, std::string instruction) override {
        std::cout << "Instruction Tag: " << tag
                  << "\n\tInstruction: " << instruction << "\n";
        eventCount++;
    }

    void onTagOpen(std::string name, bool isSelfClosing,
                   std::vector<onyx::dynamic::Attribute> attributes) override {
        std::cout << "Tag open: " << name
                  << "\n\tisSelfClosing: " << isSelfClosing << "\n";
        for (size_t i = 0; i < attributes.size(); i++) {
            std::cout << "\tAttribute Name: " << attributes[i].getName()
                      << " | Attribute Value: " << attributes[i].getValue()
                      << "\n";
        }

        eventCount++;
    }

    void onTagClose(std::string name) override {
        std::cout << "Tag close: " << name << "\n";

        eventCount++;
    }

    void onXMLDeclaration(std::string version, std::string encoding,
                          bool hasEncoding, bool isStandalone,
                          bool hasStandalone) override {
        std::cout << "XML Declaration: \t" << "\tVersion: " << version
                  << "\n\tEncoding: " << encoding
                  << "\n\tisStandalone: " << isStandalone << "\n";
        eventCount++;
    }

    void onDoctype(std::string text) override {
        std::cout << "Doctype: " << text << "\n";
        eventCount++;
    }

    void onException(std::exception& e) override {
        std::cout << "Exception: " << e.what() << "\n";
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

    SaxListenerLogger listener;
    SaxParser parser(listener);

    parser.parse(input);
    REQUIRE(listener.getEventCount() == 0);
}