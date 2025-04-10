#include "catch2/catch_all.hpp"
#include "templater.h"

#include <chrono>

TEST_CASE("HTML is generated", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false)
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head></head>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("Attribute remove works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false)
    };

    obj.removeAttribute("lang");

    CHECK(!obj.hasAttribute("lang"));
}

TEST_CASE("serialise() arguments override global indentation rules", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false)
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n    <head></head>\n</html>";

    CHECK(expected == obj.serialise("    "));
}

TEST_CASE("Vector constructor works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::vector<Attribute> attributes;
    std::vector<std::unique_ptr<Node>> children;

    attributes.emplace_back("id", "list");
    for (int i = 1; i <= 3; i++) {
        children.push_back(std::make_unique<dtags::li>(Text(std::to_string(i))));
    }

    dtags::ul obj{attributes, std::move(children)};

    std::string expected = "<ul id=\"list\">\n\t<li>\n\t\t1\n\t</li>\n\t<li>\n\t\t2\n\t</li>\n\t<li>\n\t\t3\n\t</li>\n</ul>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("Complex test case generates html", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
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

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#about\">\n\t\t\t\t\t\t\tAbout Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes various nested elements, attributes, and content.\n\t\t\t\t</p>\n\t\t\t\t<form name=\"contact-form\">\n\t\t\t\t\t<label for=\"name\">\n\t\t\t\t\t\tYour Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</form>\n\t\t\t</section>\n\t\t\t<section id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are the key features of the application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test Page\n\t\t\t</p>\n\t\t\t<a href=\"https://www.example.com\">\n\t\t\t\tPrivacy Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("GenericNode can't be given children if void", "[GenericNode]") {
    using namespace Templater::dynamic::dtags;

    REQUIRE_THROWS(GenericNode{"img", true, GenericNode{"div", false}});

    std::unique_ptr<Node> d = std::make_unique<GenericNode>("div", false);
    
    std::vector<std::unique_ptr<Node>> vec;
    vec.push_back(std::move(d));

    REQUIRE_THROWS(GenericNode{"img", true, {}, std::move(vec)});
}

TEST_CASE("Node::addChild() throws if used on a void node", "[Node::addChild]") {
    using namespace Templater::dynamic::dtags;

    GenericNode image{"img", true};

    REQUIRE_THROWS(image.addChild(GenericNode{"div", false}));

    std::unique_ptr<Node> d = std::make_unique<GenericNode>("div", false);

    REQUIRE_THROWS(image.addChild(std::move(d)));
}

TEST_CASE("Children return by tag name works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("id", "0"), 
                GenericNode("div", false, Attribute("id", "1"),
                    GenericNode("div", false, Attribute("id", "2"))),
            GenericNode("div", false, Attribute("id", "3")),
            GenericNode("div", false, Attribute("id", "4")))
    )};

    auto children = obj.getChildrenByTagName("div");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }
}

TEST_CASE("Children return by id works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("id", "0"), 
                GenericNode("div", false, Attribute("id", "1"),
                    GenericNode("div", false, Attribute("id", "2"),
                        GenericNode("p", false, Attribute("id", "11")))),
            GenericNode("div", false, Attribute("id", "3")),
            GenericNode("div", false, Attribute("id", "4")))
    )};

    auto children = obj.getChildrenById("11");

    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
    CHECK(children[0]->getTagName() == "p");
}

TEST_CASE("Children return by name works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("name", "d"), Attribute("id", "0"), 
                GenericNode("div", false, Attribute("name", "d"), Attribute("id", "1"), 
                    GenericNode("div", false, Attribute("name", "d"), Attribute("id", "2"), 
                        GenericNode("p", false, Attribute("name", "p"), Attribute("id", "11")))),
            GenericNode("div", false, Attribute("name", "d"), Attribute("id", "3")),
            GenericNode("div", false, Attribute("name", "d"), Attribute("id", "4")))
    )};

    auto children = obj.getChildrenByName("d");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByName("p");
    
    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Children return by class name works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("class", "d"), Attribute("id", "0"), 
                GenericNode("div", false, Attribute("class", "d"), Attribute("id", "1"), 
                    GenericNode("div", false, Attribute("class", "d"), Attribute("id", "2"), 
                        GenericNode("p", false, Attribute("class", "p"), Attribute("id", "11")))),
            GenericNode("div", false, Attribute("class", "d"), Attribute("id", "3")),
            GenericNode("div", false, Attribute("class", "d"), Attribute("id", "4")))
    )};

    auto children = obj.getChildrenByClassName("d");
    
    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByClassName("p");
    
    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Child add works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false)};

    auto children = obj.getChildrenByTagName("body");

    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "div", false, Attribute("id", "1")
    );

    children[0]->addChild(std::move(child));

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());
}

TEST_CASE("Child remove works", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "div", false, Attribute("id", "1")
    );

    std::vector<std::unique_ptr<Node>> vec;
    vec.push_back(std::move(child));

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false, {}, std::move(vec))};

    auto children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());

    std::unique_ptr<Node> result = obj.removeChild(children[0]);

    REQUIRE(result);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 0);
    CHECK(!result->isInTree());
}

TEST_CASE("Operator [] works for attribute access", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false)};

    CHECK(obj["lang"] == "en");

    obj["theme"] = "light";

    CHECK(obj["theme"] == "light"); 
    CHECK(obj.getAttributeValue("theme") == "light");   
}

TEST_CASE("Operator += works for child add", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode("body", false)};

    auto children = obj.getChildrenByTagName("body");

    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "div", false, Attribute("id", "1")
    );

    REQUIRE(children.size() == 1);
    *(children[0]) += std::move(child);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());
}

TEST_CASE("Complex html with dynamic tags", "[Node]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    html obj{
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        
        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"), 
                Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            clink(Attribute("rel", "stylesheet"),
                Attribute("href", "/styles/main.css"))
        ),
        
        body(
            header(
                nav(
                    ul(
                        li(
                            a(
                                Attribute("href", "#home"),
                                Text("Home")
                            )
                        ),
                        li(
                            a(
                                Attribute("href", "#about"),
                                Text("About Us")
                            )
                        )
                    )
                )
            ),
            
            main(
                section(
                    Attribute("id", "introduction"),
                    h1(Text("Introduction")),
                    p(Text("Welcome to the complex HTML structure test case.")),
                    p(Text("This test includes various nested elements, attributes, and content.")),
                    form(
                        Attribute("name", "contact-form"),
                        label(
                            Attribute("for", "name"),
                            Text("Your Name:")
                        ),
                        input(
                            Attribute("type", "text"),
                            Attribute("id", "name"),
                            Attribute("name", "name")
                        ),
                        label(
                            Attribute("for", "email"),
                            Text("Your Email:")
                        ),
                        input(
                            Attribute("type", "email"),
                            Attribute("id", "email"),
                            Attribute("name", "email")
                        ),
                        button(
                            Attribute("type", "submit"),
                            Text("Submit")
                        )
                    )
                ),
                
                section(
                    Attribute("id", "features"),
                    h2(Text("Features")),
                    ul(
                        li(Text("Feature 1")),
                        li(Text("Feature 2")),
                        li(Text("Feature 3"))
                    ),
                    p(Text("These are the key features of the application."))
                )
            ),
            
            footer(
                p(Text("© 2025 Complex HTML Test Page")),
                a(
                    Attribute("href", "https://www.example.com"),
                    Text("Privacy Policy")
                )
            )
        )
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#about\">\n\t\t\t\t\t\t\tAbout Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes various nested elements, attributes, and content.\n\t\t\t\t</p>\n\t\t\t\t<form name=\"contact-form\">\n\t\t\t\t\t<label for=\"name\">\n\t\t\t\t\t\tYour Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</form>\n\t\t\t</section>\n\t\t\t<section id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are the key features of the application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test Page\n\t\t\t</p>\n\t\t\t<a href=\"https://www.example.com\">\n\t\t\t\tPrivacy Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("Empty html tree has size 1", "[Node::size()]") {
    using namespace Templater::dynamic::dtags;
    html root;
    REQUIRE(root.size() == 1);
}

TEST_CASE("Html tree with one child has size 2", "[Node::size()]") {
    using namespace Templater::dynamic::dtags;
    html root;
    root.addChild(body());
    REQUIRE(root.size() == 2);
}

TEST_CASE("Html tree with 5 nodes has size 6", "[Node::size()]") {
    using namespace Templater::dynamic::dtags;
    html root{
        p(),
        p(),
        p(),
        p(),
        p()
    };
    REQUIRE(root.size() == 6);
}

TEST_CASE("Html tree with 5001 nodes has size 5001", "[Node::size()]") {
    using namespace Templater::dynamic::dtags;
    html root;
    for (int i = 0; i < 1000; i++) {
        root.addChild(section(
            dtags::div{
                p(Text("Text")),
                p()
            }
        ));
    }
    REQUIRE(root.size() == 5001);
}

std::string generateRandomString(size_t length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789><\"\'";
    std::string result;
    result.reserve(length);
    static int seed = 42;
    
    std::srand(seed);
    for (size_t i = 0; i < length; ++i) {
        result += chars[std::rand() % chars.size()];
    }
    seed++;
    return result;
}

void addChildren(Templater::dynamic::Node* root, int level) {
    using namespace Templater::dynamic::dtags;

    std::unique_ptr<section> s = std::make_unique<section>();
    if (level > 0) {
            addChildren(s.get(), level-1);
        }

    root->addChild(std::move(s));
}

TEST_CASE("3000 tags serialise in under 50ms", "[Node]") {
    using namespace Templater::dynamic::dtags;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    section root{};

    for (int i = 0; i < 500; i++) {
        std::unique_ptr<Node> paragraph = std::make_unique<p>();
        for (int i = 0; i < 10; i++) {
            paragraph->operator[](generateRandomString(10)) = generateRandomString(10);
        }
        paragraph->addChild(Text(generateRandomString(200)));
        addChildren(paragraph.get(), 3);
        root.addChild(std::move(paragraph));
    }

    auto t1 = high_resolution_clock::now();
    root.serialise("\t", true);
    auto t2 = high_resolution_clock::now();
    
    duration<double, std::milli> time = t2 - t1;
    INFO(root.size());

    REQUIRE(time.count() < 50);
}

TEST_CASE("Template html runtime api serialises correctly", "[Node]" ) {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    std::string doc1 = Document<
        html<
            Attribute<"lang", "en">,
            head<>,
            body<Text<"Hello world!">>
        >
    >::value("\t", true);

    std::string expected = "<html lang=\"en\">\n\t<head></head>\n\t<body>\n\t\tHello world!\n\t</body>\n</html>";

    CHECK(doc1 == expected);
}

TEST_CASE("Template html runtime api enforces given indentation rules", "[Node]" ) {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            ctags::Attribute<"theme", "light">,
            ctags::Attribute<"lang", "en">,
            head<>,
            body<ctags::Text<"Hello world!">>
        >
    >;

    std::string expected = "<html lang=\"en\" theme=\"light\">\n    <head></head>\n    <body>\n        Hello world!\n    </body>\n</html>";

    CHECK(doc::value("    ", true) == expected);
}

TEST_CASE("HTML fragment using template runtime api serialises correctly", "[Node]" ) {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    std::string doc3 = Document<
        li<Text<"1">>,
        li<Text<"2">>,
        li<Text<"3">>
    >::value("\t", true);;
    
    std::string expected = "<li>\n\t1\n</li>\n<li>\n\t2\n</li>\n<li>\n\t3\n</li>";

    CHECK(doc3 == expected);
}

TEST_CASE("Complex templated runtime api html with constant tags", "[Node]" ) {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    std::string doc4 = Document<
        html<
            Attribute<"lang", "en">,
            head<
                title<Text<"Test Page">>,
                meta<Attribute<"charset", "UTF-8">>,
                meta<Attribute<"name", "viewport">, Attribute<"content", "width=device-width, initial-scale=1.0">>,
                clink<Attribute<"rel", "stylesheet">, Attribute<"href", "styles.css">>
            >,
            body<
                h1<Text<"Welcome to the Test Page">>,
                p<Text<"This is a paragraph demonstrating compile-time HTML generation.">>,
                cdiv<
                    Attribute<"class", "container">,
                    p<Text<"Inside a div element.">>,
                    ul<
                        li<Text<"Item 1">>,
                        li<Text<"Item 2">>,
                        li<Text<"Item 3">>
                    >
                >,
                form<
                    Attribute<"action", "/submit">,
                    Attribute<"method", "post">,
                    label<Attribute<"for", "name">, Text<"Name: ">>,
                    input<Attribute<"type", "text">, Attribute<"id", "name">, Attribute<"name", "name">>,
                    br<>,
                    label<Attribute<"for", "email">, Text<"Email: ">>,
                    input<Attribute<"type", "email">, Attribute<"id", "email">, Attribute<"name", "email">>,
                    br<>,
                    button<Attribute<"type", "submit">, Text<"Submit">>
                >
            >
        >
    >::value("\t", true);
    
    std::string expected = "<html lang=\"en\">\n\t<head>\n\t\t<title>\n\t\t\tTest Page\n\t\t</title>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, initial-scale=1.0\" name=\"viewport\"/>\n\t\t<link href=\"styles.css\" rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<h1>\n\t\t\tWelcome to the Test Page\n\t\t</h1>\n\t\t<p>\n\t\t\tThis is a paragraph demonstrating compile-time HTML generation.\n\t\t</p>\n\t\t<div class=\"container\">\n\t\t\t<p>\n\t\t\t\tInside a div element.\n\t\t\t</p>\n\t\t\t<ul>\n\t\t\t\t<li>\n\t\t\t\t\tItem 1\n\t\t\t\t</li>\n\t\t\t\t<li>\n\t\t\t\t\tItem 2\n\t\t\t\t</li>\n\t\t\t\t<li>\n\t\t\t\t\tItem 3\n\t\t\t\t</li>\n\t\t\t</ul>\n\t\t</div>\n\t\t<form action=\"/submit\" method=\"post\">\n\t\t\t<label for=\"name\">\n\t\t\t\tName: \n\t\t\t</label>\n\t\t\t<input id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t<br/>\n\t\t\t<label for=\"email\">\n\t\t\t\tEmail: \n\t\t\t</label>\n\t\t\t<input id=\"email\" name=\"email\" type=\"email\"/>\n\t\t\t<br/>\n\t\t\t<button type=\"submit\">\n\t\t\t\tSubmit\n\t\t\t</button>\n\t\t</form>\n\t</body>\n</html>";

    CHECK(expected == doc4);
}


TEST_CASE("HTML is correctly serialised") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            Attribute<"lang", "en">,
            head<>,
            body<Text<"Hello world!">>
        >
    >;

    CHECK(std::string(doc::serialise().data()) == "<html lang=\"en\"><head></head><body>Hello world!</body></html>");
}

TEST_CASE("Attributes are serialised correctly") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            Attribute<"theme", "dark">,
            Attribute<"lang", "en">,
            head<>,
            body<>
        >
    >;

    CHECK(std::string(doc::serialise().data()) == "<html theme=\"dark\" lang=\"en\"><head></head><body></body></html>");
}

TEST_CASE("Text nodes are serialised correctly") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            head<>,
            body<Text<"This is a test!">>
        >
    >;

    CHECK(std::string(doc::serialise().data()) == "<html><head></head><body>This is a test!</body></html>");
}

TEST_CASE("Void tags are serialised correctly") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            head<>,
            body<
                img<Attribute<"src", "img.jpg">>
            >
        >
    >;

    CHECK(std::string(doc::serialise().data()) == "<html><head></head><body><img src=\"img.jpg\" /></body></html>");
}

TEST_CASE("UTF-8 text strings are cut off properly") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            head<>,
            body<Text<"Grüße">> // Contains UTF-8 ü
        >
    >;

    auto serialized = std::string(doc::serialise().data());
    CHECK(serialized == "<html><head></head><body>Grüße</body></html>");
}

TEST_CASE("Empty document serialises correctly") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<>;
    CHECK(std::string(doc::serialise().data()) == "");
}

TEST_CASE("Deeply nested elements serialize correctly") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            head<>,
            body<
                cdiv<
                    Attribute<"class", "container">,
                    cdiv<
                        Text<"Nested content">
                    >
                >
            >
        >
    >;

    CHECK(std::string(doc::serialise().data()) ==
          "<html><head></head><body><div class=\"container\"><div>Nested content</div></div></body></html>");
}

TEST_CASE("Multiple text nodes are serialised in sequence") {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    using doc = Document<
        html<
            head<>,
            body<
                Text<"Hello ">,
                Text<"world!">
            >
        >
    >;

    CHECK(std::string(doc::serialise().data()) == "<html><head></head><body>Hello world!</body></html>");
}

TEST_CASE("Text properly escapes html", "[dynamic::dtags::Text]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::string textToEscape = "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    dtags::div d{Text(textToEscape)};

    std::string expected = "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serialise() == expected);
}

TEST_CASE("Text properly escapes unicode when multi-byte escaping is enabled", "[dynamic::dtags::Text]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::string textToEscape = "<div class=\"content\"><h1>😀Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    dtags::div d{Text(textToEscape, true)};

    std::string expected = "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;&#x1f600;Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serialise() == expected);
}

TEST_CASE("Text does not escape unicode when multi-byte escaping is disabled", "[dynamic::dtags::Text]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::string textToEscape = "<div class=\"content\"><h1>😀Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    dtags::div d{Text(textToEscape, false)};

    std::string expected = "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;😀Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serialise() == expected);
}

TEST_CASE("HTML comments are generated", "[Comment]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false, Comment("A comment."))
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<!--A comment.-->\n\t</head>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("HTML comments are escaped", "[Comment]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false, Comment("A comment. --><dangerous>sequence."))
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<!--A comment. --&gt;&lt;dangerous&gt;sequence.-->\n\t</head>\n</html>";

    CHECK(expected == obj.serialise());
}


TEST_CASE("__DangerousRawText works", "[DangerousRawText]" ) {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false, __DangerousRawText("<h1> Injected title! </h1>"))
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<h1> Injected title! </h1>\n\t</head>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("Node deepCopy() works", "[Node]") {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    html obj{
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        
        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"), 
                Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            clink(Attribute("rel", "stylesheet"),
                Attribute("href", "/styles/main.css"))
        ),
        
        body(
            header(
                nav(
                    ul(
                        li(
                            a(
                                Attribute("href", "#home"),
                                Text("Home")
                            )
                        ),
                        li(
                            a(
                                Attribute("href", "#about"),
                                Text("About Us")
                            )
                        )
                    )
                )
            ),
            
            dtags::main(
                section(
                    Attribute("id", "introduction"),
                    h1(Text("Introduction")),
                    p(Text("Welcome to the complex HTML structure test case.")),
                    p(Text("This test includes various nested elements, attributes, and content.")),
                    form(
                        Attribute("name", "contact-form"),
                        label(
                            Attribute("for", "name"),
                            Text("Your Name:")
                        ),
                        input(
                            Attribute("type", "text"),
                            Attribute("id", "name"),
                            Attribute("name", "name")
                        ),
                        label(
                            Attribute("for", "email"),
                            Text("Your Email:")
                        ),
                        input(
                            Attribute("type", "email"),
                            Attribute("id", "email"),
                            Attribute("name", "email")
                        ),
                        button(
                            Attribute("type", "submit"),
                            Text("Submit")
                        )
                    )
                ),
                
                section(
                    Attribute("id", "features"),
                    h2(Text("Features")),
                    ul(
                        li(Text("Feature 1")),
                        li(Text("Feature 2")),
                        li(Text("Feature 3"))
                    ),
                    p(Text("These are the key features of the application."))
                )
            ),
            
            footer(
                p(Text("© 2025 Complex HTML Test Page")),
                a(
                    Attribute("href", "https://www.example.com"),
                    Text("Privacy Policy")
                )
            )
        )
    };

    std::unique_ptr<Node> root = obj.deepCopy();
    REQUIRE(root);
    CHECK(root->serialise() == obj.serialise());
    CHECK(root.get() != &obj);
}

TEST_CASE("Node deepCopy creates a new instance with identical attributes", "[Node]") {
    using namespace Templater::dynamic::dtags;
    
    GenericNode obj{"html", false, 
        Attribute("class", "test"), 
        Attribute("id", "1"), 
        html(
            Attribute("class", "test"))
    };
    
    // Create a deep copy of the original node
    auto copy = obj.deepCopy();

    // Verify the copied node has the same attributes but is not the same instance
    REQUIRE(copy != nullptr);
    REQUIRE(copy->getAttributes().size() == obj.getAttributes().size());
    REQUIRE(copy->getAttributes()[0] == obj.getAttributes()[0]);
    REQUIRE(copy->getAttributes()[1] == obj.getAttributes()[1]);

    // Verify the children are deeply copied (not just the pointer)
    REQUIRE(copy->getChildren().size() == obj.getChildren().size());
    REQUIRE(copy->getChildren()[0] != obj.getChildren()[0]);
    REQUIRE(copy->getChildren()[0]->getAttributes()[0] == obj.getChildren()[0]->getAttributes()[0]);
}

TEST_CASE("Node deepCopy with empty node", "[Node]") {
    using namespace Templater::dynamic::dtags;

    p obj{};

    auto copy = obj.deepCopy();

    REQUIRE(copy != nullptr);
    REQUIRE(copy->getChildren().empty());
    REQUIRE(copy->getAttributes().empty());
}

TEST_CASE("Node deepCopy does not share internal state", "[Node]") {
    using namespace Templater::dynamic::dtags;
    
    html obj{
        Attribute("class", "testNode"), 
        p(Attribute("id", "child1"))
    };

    auto copy = obj.deepCopy();

    obj.setAttributeValue("class", "modifiedClass");
    REQUIRE(obj.getAttributeValue("class") == "modifiedClass");
    REQUIRE(copy->getAttributeValue("class") == "testNode");
}

TEST_CASE("Node deepEquals() works", "[Node]") {
    using namespace Templater::dynamic::dtags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    html obj{
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        
        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"), 
                Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            clink(Attribute("rel", "stylesheet"),
                Attribute("href", "/styles/main.css"))
        ),
        
        body(
            header(
                nav(
                    ul(
                        li(
                            a(
                                Attribute("href", "#home"),
                                Text("Home")
                            )
                        ),
                        li(
                            a(
                                Attribute("href", "#about"),
                                Text("About Us")
                            )
                        )
                    )
                )
            ),
            
            dtags::main(
                section(
                    Attribute("id", "introduction"),
                    h1(Text("Introduction")),
                    p(Text("Welcome to the complex HTML structure test case.")),
                    p(Text("This test includes various nested elements, attributes, and content.")),
                    form(
                        Attribute("name", "contact-form"),
                        label(
                            Attribute("for", "name"),
                            Text("Your Name:")
                        ),
                        input(
                            Attribute("type", "text"),
                            Attribute("id", "name"),
                            Attribute("name", "name")
                        ),
                        label(
                            Attribute("for", "email"),
                            Text("Your Email:")
                        ),
                        input(
                            Attribute("type", "email"),
                            Attribute("id", "email"),
                            Attribute("name", "email")
                        ),
                        button(
                            Attribute("type", "submit"),
                            Text("Submit")
                        )
                    )
                ),
                
                section(
                    Attribute("id", "features"),
                    h2(Text("Features")),
                    ul(
                        li(Text("Feature 1")),
                        li(Text("Feature 2")),
                        li(Text("Feature 3"))
                    ),
                    p(Text("These are the key features of the application."))
                )
            ),
            
            footer(
                p(Text("© 2025 Complex HTML Test Page")),
                a(
                    Attribute("href", "https://www.example.com"),
                    Text("Privacy Policy")
                )
            )
        )
    };

    std::unique_ptr<Node> root = obj.deepCopy();
    REQUIRE(root);
    CHECK(root->deepEquals(obj));
    auto childrenWithHrefHome = root->getChildrenByAttribute("href", "#home");
    REQUIRE(childrenWithHrefHome.size() == 1);
    root->removeChild(childrenWithHrefHome[0]);
    CHECK(!root->deepEquals(obj));
}