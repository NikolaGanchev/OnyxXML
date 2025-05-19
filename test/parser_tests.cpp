#include "catch2/catch_all.hpp"
#include "templater.h"

#include <chrono>

TEST_CASE("DomParser works") {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<html><head></head></html>";

    GenericNode output{
        "html", false,
        GenericNode("head", false)
    };

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser works with text") {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<html><body><div> Hello<span></span>World! </div></body></html>";
    
    GenericNode output{
        "html", false,
        GenericNode("body", false,
            GenericNode("div", false,
                Text(" Hello"), GenericNode("span", false), Text("World! ")))
    };

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("dom parser works with a single attribute") {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<html theme=\"dark\"><body><div> Hello<span></span>World! </div></body></html>";
    
    GenericNode output{
        "html", false,
        Attribute("theme", "dark"),
        GenericNode("body", false,
            GenericNode("div", false,
                Text(" Hello"), GenericNode("span", false), Text("World! ")))
    };

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}


TEST_CASE("DomParser works with many attributes") {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<html theme=\"dark\" lang='en'><body><div> Hello<span></span>World! </div></body></html>";
    
    GenericNode output{
        "html", false,
        Attribute("theme", "dark"),
        Attribute("lang", "en"),
        GenericNode("body", false,
            GenericNode("div", false,
                Text(" Hello"), GenericNode("span", false), Text("World! ")))
    };

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("DomParser parses complex html") {
    using namespace Templater::tags;
    using namespace Templater::parser;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    GenericNode output{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        
        GenericNode("head", false,
            GenericNode("meta", true, 
                Attribute("charset", "UTF-8")),
            GenericNode("meta", true, 
                Attribute("name", "viewport"),
                Attribute("content", "width=device-width, initial-scale=1.0")),
            GenericNode("title", false, 
                Text("Complex Test Page")),
            GenericNode("link", true,
                Attribute("rel", "stylesheet"),
                Attribute("href", "/styles/main.css"))
        ),
        
        GenericNode("body", false, 
            GenericNode("header", false, 
                GenericNode("nav", false, 
                    GenericNode("ul", false, 
                        GenericNode("li", false, 
                            GenericNode("a", false,
                                Attribute("href", "#home"),
                                Text("Home")
                            )
                        ),
                        GenericNode("li", false, 
                            GenericNode("a", false,
                                Attribute("href", "#about"),
                                Text("About Us")
                            )
                        )
                    )
                )
            ),
            
            GenericNode("main", false,
                GenericNode("section", false, 
                    Attribute("id", "introduction"),
                    GenericNode("h1", false, Text("Introduction")),
                    GenericNode("p", false, Text("Welcome to the complex HTML structure test case.")),
                    GenericNode("p", false, Text("This test includes various nested elements, attributes, and content.")),
                    GenericNode("form", false, 
                        Attribute("name", "contact-form"),
                        GenericNode("label", false,
                            Attribute("for", "name"),
                            Text("Your Name:")
                        ),
                        GenericNode("input", true, 
                            Attribute("type", "text"),
                            Attribute("id", "name"),
                            Attribute("name", "name")
                        ),
                        GenericNode("label", false, 
                            Attribute("for", "email"),
                            Text("Your Email:")
                        ),
                        GenericNode("input", true, 
                            Attribute("type", "email"),
                            Attribute("id", "email"),
                            Attribute("name", "email")
                        ),
                        GenericNode("button", false, 
                            Attribute("type", "submit"),
                            Text("Submit")
                        )
                    )
                ),
                
                GenericNode("section", false, 
                    Attribute("id", "features"),
                    GenericNode("h2", false, Text("Features")),
                    GenericNode("ul", false, 
                        GenericNode("li", false, Text("Feature 1")),
                        GenericNode("li", false, Text("Feature 2")),
                        GenericNode("li", false, Text("Feature 3"))
                    ),
                    GenericNode("p", false, Text("These are the key features of the application."))
                )
            ),
            
            GenericNode("footer", false,
                GenericNode("p", false, Text("© 2025 Complex HTML Test Page")),
                GenericNode("a", false,
                    Attribute("href", "https://www.example.com"),
                    Text("Privacy Policy")
                )
            )
        )
    };

    std::string input = "<html lang=\"en\" theme=\"dark\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>Complex Test Page</title><link rel=\"stylesheet\" href=\"/styles/main.css\"/></head><body><header><nav><ul><li><a href=\"#home\">Home</a></li><li><a href=\"#about\">About Us</a></li></ul></nav></header><main><section id=\"introduction\"><h1>Introduction</h1><p>Welcome to the complex HTML structure test case.</p><p>This test includes various nested elements, attributes, and content.</p><form name=\"contact-form\"><label for=\"name\">Your Name:</label><input type=\"text\" id=\"name\" name=\"name\"/><label for=\"email\">Your Email:</label><input type=\"email\" id=\"email\" name=\"email\"/><button type=\"submit\">Submit</button></form></section><section id=\"features\"><h2>Features</h2><ul><li>Feature 1</li><li>Feature 2</li><li>Feature 3</li></ul><p>These are the key features of the application.</p></section></main><footer><p>© 2025 Complex HTML Test Page</p><a href=\"https://www.example.com\">Privacy Policy</a></footer></body></html>";

    auto t1 = high_resolution_clock::now();
        ParseResult pr = DomParser::parse(input);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;

    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));

    INFO(time.count());
    REQUIRE(time.count() < 0);
}

TEST_CASE("dom parser works with comments") {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<html theme=\"dark\"><!--This is a comment!--><body><div> Hello<span></span>World! </div></body></html>";
    
    GenericNode output{
        "html", false,
        Attribute("theme", "dark"),
        Comment("This is a comment!"),
        GenericNode("body", false,
            GenericNode("div", false,
                Text(" Hello"), GenericNode("span", false), Text("World! ")))
    };

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("dom parser works with processing instructions", "[Comment]" ) {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<root lang=\"en\"><?templater doSomething 5 > 4 somethingElse?></root>";

    GenericNode output{
        "root", false,
        Attribute("lang", "en"),
        ProcessingInstruction("templater", "doSomething 5 > 4 somethingElse")
    };

    ParseResult pr = DomParser::parse(input);

    INFO(output.serialize());
    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}