#include <chrono>

#include "catch2/catch_all.hpp"
#include "onyx.h"

TEST_CASE("HTML is generated", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{"html", false, Attribute("lang", "en"),
                    Attribute("theme", "dark"), GenericNode("head", false)};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head></head>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("Attribute remove works", "[Node]") {
    using namespace onyx::tags;

    GenericNode obj{"html", false, Attribute("lang", "en"),
                    Attribute("theme", "dark"), GenericNode("head", false)};

    obj.removeAttribute("lang");

    CHECK(!obj.hasAttribute("lang"));
}

TEST_CASE("Constructor throws on Attribute repetition", "[Node]") {
    using namespace onyx::tags;

    REQUIRE_THROWS_WITH(GenericNode("div", false, Attribute("name", "1"), Attribute("name", "2")), "Adding duplicate Attribute");
}

TEST_CASE("Vector constructor works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::vector<Attribute> attributes;
    std::vector<NodeHandle> children;

    attributes.emplace_back("id", "list");
    for (int i = 1; i <= 3; i++) {
        children.emplace_back(std::make_unique<li>(Text(std::to_string(i))));
    }

    ul obj{attributes, std::move(children)};

    std::string expected =
        "<ul "
        "id=\"list\">\n\t<li>\n\t\t1\n\t</li>\n\t<li>\n\t\t2\n\t</"
        "li>\n\t<li>\n\t\t3\n\t</li>\n</ul>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("Complex test case generates pretty html", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
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

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<meta "
        "charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, "
        "initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex "
        "Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" "
        "rel=\"stylesheet\"/>\n\t</"
        "head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<"
        "li>\n\t\t\t\t\t\t<a "
        "href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</"
        "li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a "
        "href=\"#about\">\n\t\t\t\t\t\t\tAbout "
        "Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</"
        "nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section "
        "id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</"
        "h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure "
        "test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes "
        "various nested elements, attributes, and "
        "content.\n\t\t\t\t</p>\n\t\t\t\t<form "
        "name=\"contact-form\">\n\t\t\t\t\t<label "
        "for=\"name\">\n\t\t\t\t\t\tYour "
        "Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" "
        "type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour "
        "Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" "
        "name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button "
        "type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</"
        "form>\n\t\t\t</section>\n\t\t\t<section "
        "id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</"
        "h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature "
        "1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature "
        "2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature "
        "3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are "
        "the key features of the "
        "application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</"
        "main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test "
        "Page\n\t\t\t</p>\n\t\t\t<a "
        "href=\"https://www.example.com\">\n\t\t\t\tPrivacy "
        "Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("Complex test case generates non-pretty html", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
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

    std::string expected =
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
    CHECK(expected == obj.serialize());
}

TEST_CASE("GenericNode can't be given children if void", "[GenericNode]") {
    using namespace onyx::tags;

    REQUIRE_THROWS(GenericNode{"img", true, GenericNode{"div", false}});

    std::unique_ptr<Node> d = std::make_unique<GenericNode>("div", false);

    std::vector<NodeHandle> vec;
    vec.push_back(std::move(d));

    REQUIRE_THROWS(GenericNode{"img", true, {}, std::move(vec)});
}

TEST_CASE("Node::addChild() throws if used on a void node",
          "[Node::addChild]") {
    using namespace onyx::tags;

    GenericNode image{"img", true};

    REQUIRE_THROWS(image.addChild(GenericNode{"div", false}));

    std::unique_ptr<Node> d = std::make_unique<GenericNode>("div", false);

    REQUIRE_THROWS(image.addChild(std::move(d)));
}

TEST_CASE("Children return by tag name works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("id", "0"),
                GenericNode("div", false, Attribute("id", "1"),
                            GenericNode("div", false, Attribute("id", "2"))),
                GenericNode("div", false, Attribute("id", "3")),
                GenericNode("div", false, Attribute("id", "4"))))};

    auto children = obj.getChildrenByTagName("div");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }
}

TEST_CASE("Children return by id works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("id", "0"),
                GenericNode("div", false, Attribute("id", "1"),
                            GenericNode("div", false, Attribute("id", "2"),
                                        GenericNode("p", false,
                                                    Attribute("id", "11")))),
                GenericNode("div", false, Attribute("id", "3")),
                GenericNode("div", false, Attribute("id", "4"))))};

    auto children = obj.getChildrenById("11");

    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
    CHECK(children[0]->getTagName() == "p");
}

TEST_CASE("Children return by name works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("name", "d"), Attribute("id", "0"),
                GenericNode(
                    "div", false, Attribute("name", "d"), Attribute("id", "1"),
                    GenericNode("div", false, Attribute("name", "d"),
                                Attribute("id", "2"),
                                GenericNode("p", false, Attribute("name", "p"),
                                            Attribute("id", "11")))),
                GenericNode("div", false, Attribute("name", "d"),
                            Attribute("id", "3")),
                GenericNode("div", false, Attribute("name", "d"),
                            Attribute("id", "4"))))};

    auto children = obj.getChildrenByName("d");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByName("p");

    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Children return by class name works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("class", "d"), Attribute("id", "0"),
                GenericNode(
                    "div", false, Attribute("class", "d"), Attribute("id", "1"),
                    GenericNode("div", false, Attribute("class", "d"),
                                Attribute("id", "2"),
                                GenericNode("p", false, Attribute("class", "p"),
                                            Attribute("id", "11")))),
                GenericNode("div", false, Attribute("class", "d"),
                            Attribute("id", "3")),
                GenericNode("div", false, Attribute("class", "d"),
                            Attribute("id", "4"))))};

    auto children = obj.getChildrenByClassName("d");

    REQUIRE(children.size() == 5);

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByClassName("p");

    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Children return by attribute works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("class", "d"), Attribute("id", "0"),
                GenericNode(
                    "div", false, Attribute("class", "d"), Attribute("id", "1"),
                    GenericNode("div", false, Attribute("class", "d"),
                                Attribute("id", "2"),
                                GenericNode("p", false, Attribute("class", "p"),
                                            Attribute("id", "11")))),
                GenericNode("div", false, Attribute("class", "d"),
                            Attribute("id", "3")),
                GenericNode("div", false, Attribute("class", "d"),
                            Attribute("id", "4"))))};

    auto children = obj.getChildrenByAttribute("class", "d");

    REQUIRE(children.size() == 5);

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }
}

TEST_CASE("Children return by attribute name works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{
        "html",
        false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false),
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("class", "a"), Attribute("id", "0"),
                GenericNode(
                    "div", false, Attribute("class", "b"), Attribute("id", "1"),
                    GenericNode("div", false, Attribute("class", "c"),
                                Attribute("id", "2"),
                                GenericNode("p", false, Attribute("class", "d"),
                                            Attribute("id", "3")))),
                GenericNode("div", false, Attribute("class", "e"),
                            Attribute("id", "4")),
                GenericNode("div", false, Attribute("class", "f"),
                            Attribute("id", "5"))))};

    auto children = obj.getChildrenByAttributeName("class");

    REQUIRE(children.size() == 6);

    for (int i = 0; i < 6; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }
}

TEST_CASE("Child add works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{"html",
                    false,
                    Attribute("lang", "en"),
                    Attribute("theme", "dark"),
                    GenericNode("head", false),
                    GenericNode("body", false)};

    auto body = obj.getChildrenByTagName("body");

    std::unique_ptr<Node> child =
        std::make_unique<GenericNode>("div", false, Attribute("id", "1"));

    body[0]->addChild(std::move(child));

    auto children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());
    CHECK(children[0]->getParentNode() == body[0]);
}

TEST_CASE("Child remove works", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::unique_ptr<Node> child =
        std::make_unique<GenericNode>("div", false, Attribute("id", "1"));

    std::vector<NodeHandle> vec;
    vec.push_back(std::move(child));

    GenericNode obj{"html",
                    false,
                    Attribute("lang", "en"),
                    Attribute("theme", "dark"),
                    GenericNode("head", false),
                    GenericNode("body", false, {}, std::move(vec))};

    auto children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());

    NodeHandle result = obj.removeChild(children[0]);

    REQUIRE(result);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 0);
    CHECK(!result->isInTree());
}

TEST_CASE("Operator [] works for attribute access", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{"html",
                    false,
                    Attribute("lang", "en"),
                    Attribute("theme", "dark"),
                    GenericNode("head", false),
                    GenericNode("body", false)};

    CHECK(obj["lang"] == "en");

    obj["theme"] = "light";

    CHECK(obj["theme"] == "light");
    CHECK(obj.getAttributeValue("theme") == "light");
}

TEST_CASE("Operator += works for child add", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    GenericNode obj{"html",
                    false,
                    Attribute("lang", "en"),
                    Attribute("theme", "dark"),
                    GenericNode("head", false),
                    GenericNode("body", false)};

    auto children = obj.getChildrenByTagName("body");

    std::unique_ptr<Node> child =
        std::make_unique<GenericNode>("div", false, Attribute("id", "1"));

    REQUIRE(children.size() == 1);
    *(children[0]) += std::move(child);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());
}

TEST_CASE("Complex html with dynamic tags", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    html obj{
        Attribute("lang", "en"), Attribute("theme", "dark"),

        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"),
                 Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            clink(Attribute("rel", "stylesheet"),
                  Attribute("href", "/styles/main.css"))),

        body(
            header(
                nav(ul(li(a(Attribute("href", "#home"), Text("Home"))),
                       li(a(Attribute("href", "#about"), Text("About Us")))))),

            main(
                section(
                    Attribute("id", "introduction"), h1(Text("Introduction")),
                    p(Text("Welcome to the complex HTML structure test case.")),
                    p(Text("This test includes various nested elements, "
                           "attributes, and content.")),
                    form(Attribute("name", "contact-form"),
                         label(Attribute("for", "name"), Text("Your Name:")),
                         input(Attribute("type", "text"),
                               Attribute("id", "name"),
                               Attribute("name", "name")),
                         label(Attribute("for", "email"), Text("Your Email:")),
                         input(Attribute("type", "email"),
                               Attribute("id", "email"),
                               Attribute("name", "email")),
                         button(Attribute("type", "submit"), Text("Submit")))),

                section(
                    Attribute("id", "features"), h2(Text("Features")),
                    ul(li(Text("Feature 1")), li(Text("Feature 2")),
                       li(Text("Feature 3"))),
                    p(Text("These are the key features of the application.")))),

            footer(p(Text("© 2025 Complex HTML Test Page")),
                   a(Attribute("href", "https://www.example.com"),
                     Text("Privacy Policy"))))};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<meta "
        "charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, "
        "initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex "
        "Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" "
        "rel=\"stylesheet\"/>\n\t</"
        "head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<"
        "li>\n\t\t\t\t\t\t<a "
        "href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</"
        "li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a "
        "href=\"#about\">\n\t\t\t\t\t\t\tAbout "
        "Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</"
        "nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section "
        "id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</"
        "h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure "
        "test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes "
        "various nested elements, attributes, and "
        "content.\n\t\t\t\t</p>\n\t\t\t\t<form "
        "name=\"contact-form\">\n\t\t\t\t\t<label "
        "for=\"name\">\n\t\t\t\t\t\tYour "
        "Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" "
        "type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour "
        "Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" "
        "name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button "
        "type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</"
        "form>\n\t\t\t</section>\n\t\t\t<section "
        "id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</"
        "h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature "
        "1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature "
        "2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature "
        "3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are "
        "the key features of the "
        "application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</"
        "main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test "
        "Page\n\t\t\t</p>\n\t\t\t<a "
        "href=\"https://www.example.com\">\n\t\t\t\tPrivacy "
        "Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("Empty html tree has size 1", "[Node::size()]") {
    using namespace onyx::tags;
    html root;
    REQUIRE(root.size() == 1);
}

TEST_CASE("Html tree with one child has size 2", "[Node::size()]") {
    using namespace onyx::tags;
    html root;
    root.addChild(body());
    REQUIRE(root.size() == 2);
}

TEST_CASE("Html tree with 5 nodes has size 6", "[Node::size()]") {
    using namespace onyx::tags;
    html root{p(), p(), p(), p(), p()};
    REQUIRE(root.size() == 6);
}

TEST_CASE("Html tree with 5001 nodes has size 5001", "[Node::size()]") {
    using namespace onyx::tags;
    html root;
    for (int i = 0; i < 1000; i++) {
        root.addChild(section(cdiv{p(Text("Text")), p()}));
    }
    REQUIRE(root.size() == 5001);
}

std::string generateRandomString(size_t length) {
    const std::string chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789><\"\'";
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

void addChildren(onyx::dynamic::Node* root, int level) {
    using namespace onyx::tags;

    std::unique_ptr<section> s = std::make_unique<section>();
    if (level > 0) {
        addChildren(s.get(), level - 1);
    }

    root->addChild(std::move(s));
}

TEST_CASE("3000 tags serialize in under 50ms", "[Node]") {
    using namespace onyx::tags;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    section root{};

    for (int i = 0; i < 500; i++) {
        std::unique_ptr<Node> paragraph = std::make_unique<p>();
        for (int i = 0; i < 10; i++) {
            paragraph->operator[](generateRandomString(10)) =
                generateRandomString(10);
        }
        paragraph->addChild(Text(generateRandomString(200)));
        addChildren(paragraph.get(), 3);
        root.addChild(std::move(paragraph));
    }

    auto t1 = high_resolution_clock::now();
    root.serialize();
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;
    INFO(root.size());

    REQUIRE(time.count() < 50);
}

TEST_CASE("Template html runtime api serializes correctly", "[Node]") {
    using namespace onyx::ctags;

    std::string doc1 = Document<html<Attribute<"lang", "en">, head<>,
                                     body<Text<"Hello world!">>>>::dynamicTree()
                           ->serializePretty("\t", true);

    std::string expected =
        "<html lang=\"en\">\n\t<head></head>\n\t<body>\n\t\tHello "
        "world!\n\t</body>\n</html>";

    CHECK(doc1 == expected);
}

TEST_CASE("Template html runtime api enforces given indentation rules",
          "[Node]") {
    using namespace onyx::ctags;

    using doc =
        Document<html<Attribute<"theme", "light">, Attribute<"lang", "en">,
                      head<>, body<Text<"Hello world!">>>>;

    std::string expected =
        "<html lang=\"en\" theme=\"light\">\n    <head></head>\n    <body>\n   "
        "     Hello world!\n    </body>\n</html>";

    CHECK(doc::dynamicTree()->serializePretty("    ", true) == expected);
}

TEST_CASE("HTML fragment using template runtime api serializes correctly",
          "[Node]") {
    using namespace onyx::ctags;

    std::string doc3 =
        Document<li<Text<"1">>, li<Text<"2">>, li<Text<"3">>>::dynamicTree()
            ->serializePretty("\t", true);
    ;

    std::string expected =
        "<li>\n\t1\n</li>\n<li>\n\t2\n</li>\n<li>\n\t3\n</li>";

    CHECK(doc3 == expected);
}

TEST_CASE(
    "GenericNode fragment using template runtime api serializes correctly",
    "[Node]") {
    using namespace onyx::ctags;

    std::string doc3 =
        Document<GenericNode<"li", false, Text<"1">>,
                 GenericNode<"li", false, Text<"2">>,
                 GenericNode<"li", false, Text<"3">>>::dynamicTree()
            ->serializePretty("\t", true);
    ;

    std::string expected =
        "<li>\n\t1\n</li>\n<li>\n\t2\n</li>\n<li>\n\t3\n</li>";

    CHECK(doc3 == expected);
}


TEST_CASE("Special templated runtime api tags are serialized correctly") {
    using namespace onyx::ctags;
    std::string doc = Document<
        XmlDeclaration<"1.0", "UTF-8", "no">,
        ProcessingInstruction<"xml-stylesheet", "type=\"text/xsl\" href=\"style.xsl\"">,
        DOCTYPE<"library SYSTEM \"library.dtd\"">,
        GenericNode<"library", false,
            GenericNode<"book", false, Attribute<"id","bk101">,
                GenericNode<"title", false, Text<"The Great Adventure">>,
                GenericNode<"author", false, Text<"John Doe">>,
                GenericNode<"genre", false, CDATA<"Fantasy & Adventure">>,
                ProcessingInstruction<"editor", "instructions=\"Review for historical accuracy\"">,
                GenericNode<"publication_year", false, Text<"2023">>
            >
        >
    >::dynamicTree()->serializePretty("\t", false);

    CHECK(doc ==
          "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"style.xsl\"?>\n<!DOCTYPE library SYSTEM \"library.dtd\">\n<library>\n\t<book id=\"bk101\">\n\t\t<title>\n\t\t\tThe Great Adventure\n\t\t</title>\n\t\t<author>\n\t\t\tJohn Doe\n\t\t</author>\n\t\t<genre>\n\t\t\t<![CDATA[Fantasy & Adventure]]>\n\t\t</genre>\n\t\t<?editor instructions=\"Review for historical accuracy\"?>\n\t\t<publication_year>\n\t\t\t2023\n\t\t</publication_year>\n\t</book>\n</library>");
}

TEST_CASE("Complex templated runtime api html with constant tags", "[Node]") {
    using namespace onyx::ctags;

    std::string doc4 = Document<html<
        Attribute<"lang", "en">,
        head<
            title<Text<"Test Page">>, meta<Attribute<"charset", "UTF-8">>,
            meta<Attribute<"name", "viewport">,
                 Attribute<"content", "width=device-width, initial-scale=1.0">>,
            clink<Attribute<"rel", "stylesheet">,
                  Attribute<"href", "styles.css">>>,
        body<
            h1<Text<"Welcome to the Test Page">>,
            p<Text<"This is a paragraph demonstrating compile-time HTML "
                   "generation.">>,
            cdiv<
                Attribute<"class", "container">,
                p<Text<"Inside a div element.">>,
                ul<li<Text<"Item 1">>, li<Text<"Item 2">>, li<Text<"Item 3">>>>,
            form<Attribute<"action", "/submit">, Attribute<"method", "post">,
                 label<Attribute<"for", "name">, Text<"Name: ">>,
                 input<Attribute<"type", "text">, Attribute<"id", "name">,
                       Attribute<"name", "name">>,
                 br<>, label<Attribute<"for", "email">, Text<"Email: ">>,
                 input<Attribute<"type", "email">, Attribute<"id", "email">,
                       Attribute<"name", "email">>,
                 br<>, button<Attribute<"type", "submit">, Text<"Submit">>>>>>::
                           dynamicTree()
                               ->serializePretty("\t", true);

    std::string expected =
        "<html lang=\"en\">\n\t<head>\n\t\t<title>\n\t\t\tTest "
        "Page\n\t\t</title>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta "
        "content=\"width=device-width, initial-scale=1.0\" "
        "name=\"viewport\"/>\n\t\t<link href=\"styles.css\" "
        "rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<h1>\n\t\t\tWelcome to "
        "the Test Page\n\t\t</h1>\n\t\t<p>\n\t\t\tThis is a paragraph "
        "demonstrating compile-time HTML generation.\n\t\t</p>\n\t\t<div "
        "class=\"container\">\n\t\t\t<p>\n\t\t\t\tInside a div "
        "element.\n\t\t\t</p>\n\t\t\t<ul>\n\t\t\t\t<li>\n\t\t\t\t\tItem "
        "1\n\t\t\t\t</li>\n\t\t\t\t<li>\n\t\t\t\t\tItem "
        "2\n\t\t\t\t</li>\n\t\t\t\t<li>\n\t\t\t\t\tItem "
        "3\n\t\t\t\t</li>\n\t\t\t</ul>\n\t\t</div>\n\t\t<form "
        "action=\"/submit\" method=\"post\">\n\t\t\t<label "
        "for=\"name\">\n\t\t\t\tName: \n\t\t\t</label>\n\t\t\t<input "
        "id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t<br/>\n\t\t\t<label "
        "for=\"email\">\n\t\t\t\tEmail: \n\t\t\t</label>\n\t\t\t<input "
        "id=\"email\" name=\"email\" "
        "type=\"email\"/>\n\t\t\t<br/>\n\t\t\t<button "
        "type=\"submit\">\n\t\t\t\tSubmit\n\t\t\t</button>\n\t\t</form>\n\t</"
        "body>\n</html>";

    CHECK(expected == doc4);
}

TEST_CASE("HTML is correctly serialized") {
    using namespace onyx::ctags;

    using doc = Document<
        html<Attribute<"lang", "en">, head<>, body<Text<"Hello world!">>>>;

    CHECK(doc::toString() ==
          "<html lang=\"en\"><head></head><body>Hello world!</body></html>");
}

TEST_CASE("Attributes are serialized correctly") {
    using namespace onyx::ctags;

    using doc = Document<html<Attribute<"theme", "dark">,
                              Attribute<"lang", "en">, head<>, body<>>>;

    CHECK(doc::toString() ==
          "<html theme=\"dark\" lang=\"en\"><head></head><body></body></html>");
}

TEST_CASE("Text nodes are serialized correctly") {
    using namespace onyx::ctags;

    using doc = Document<html<head<>, body<Text<"This is a test!">>>>;

    CHECK(doc::toString() ==
          "<html><head></head><body>This is a test!</body></html>");
}

TEST_CASE("Void tags are serialized correctly") {
    using namespace onyx::ctags;

    using doc = Document<html<head<>, body<img<Attribute<"src", "img.jpg">>>>>;

    CHECK(doc::toString() ==
          "<html><head></head><body><img src=\"img.jpg\" /></body></html>");
}

TEST_CASE("Comment tags are serialized correctly") {
    using namespace onyx::ctags;

    using doc = Document<html<head<>, Comment<"This is a comment!">,
                              body<img<Attribute<"src", "img.jpg">>>>>;

    CHECK(doc::toString() ==
          "<html><head></head><!--This is a comment!--><body><img "
          "src=\"img.jpg\" /></body></html>");
}

TEST_CASE("GenericNode non-void compile tag is serialized correctly") {
    using namespace onyx::ctags;

    using doc = Document<html<
        head<>, GenericNode<"body", false, img<Attribute<"src", "img.jpg">>>>>;

    CHECK(doc::toString() ==
          "<html><head></head><body><img src=\"img.jpg\" /></body></html>");
}

TEST_CASE("GenericNode void compile tag is serialized correctly") {
    using namespace onyx::ctags;

    using doc = Document<html<
        head<>, body<GenericNode<"img", true, Attribute<"src", "img.jpg">>>>>;

    CHECK(doc::toString() ==
          "<html><head></head><body><img src=\"img.jpg\" /></body></html>");
}

TEST_CASE("Special compile-time tags are serialized correctly") {
    using namespace onyx::ctags;
    using doc = Document<
        XmlDeclaration<"1.0", "UTF-8", "no">,
        ProcessingInstruction<"xml-stylesheet", "type=\"text/xsl\" href=\"style.xsl\"">,
        DOCTYPE<"library SYSTEM \"library.dtd\"">,
        GenericNode<"library", false,
            GenericNode<"book", false, Attribute<"id","bk101">,
                GenericNode<"title", false, Text<"The Great Adventure">>,
                GenericNode<"author", false, Text<"John Doe">>,
                GenericNode<"genre", false, CDATA<"Fantasy & Adventure">>,
                ProcessingInstruction<"editor", "instructions=\"Review for historical accuracy\"">,
                GenericNode<"publication_year", false, Text<"2023">>
            >
        >
    >;

    CHECK(doc::toString() ==
          "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><?xml-stylesheet type=\"text/xsl\" href=\"style.xsl\"?><!DOCTYPE library SYSTEM \"library.dtd\"><library><book id=\"bk101\"><title>The Great Adventure</title><author>John Doe</author><genre><![CDATA[Fantasy & Adventure]]></genre><?editor instructions=\"Review for historical accuracy\"?><publication_year>2023</publication_year></book></library>");
}

TEST_CASE("UTF-8 text strings are cut off properly") {
    using namespace onyx::ctags;

    using doc = Document<html<head<>,
                              body<Text<"Grüße">>  // Contains UTF-8 ü
                              >>;

    CHECK(doc::toString() == "<html><head></head><body>Grüße</body></html>");
}

TEST_CASE("Empty document serializes correctly") {
    using namespace onyx::ctags;

    using doc = Document<>;
    CHECK(doc::toString() == "");
}

TEST_CASE("Deeply nested elements serialize correctly") {
    using namespace onyx::ctags;

    using doc =
        Document<html<head<>, body<cdiv<Attribute<"class", "container">,
                                        cdiv<Text<"Nested content">>>>>>;

    CHECK(doc::toString() ==
          "<html><head></head><body><div class=\"container\"><div>Nested "
          "content</div></div></body></html>");
}

TEST_CASE("Multiple text nodes are serialized in sequence") {
    using namespace onyx::ctags;

    using doc = Document<html<head<>, body<Text<"Hello ">, Text<"world!">>>>;

    CHECK(doc::toString() ==
          "<html><head></head><body>Hello world!</body></html>");
}

TEST_CASE("Text properly escapes html", "[dynamic::tags::Text]") {
    using namespace onyx::tags;

    std::string textToEscape =
        "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My "
        "Awesome Website</span></h1><p>Today's date is: "
        "<script>alert('Hacked!');</script></p><a "
        "href=\"https://example.com?param=<script>evil()</script>\">Click "
        "here</a><p>&copy; 2025 My Awesome Website</p></div>";

    cdiv d{Text(textToEscape)};

    std::string expected =
        "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to "
        "&lt;span style=&quot;color: red;&quot;&gt;My Awesome "
        "Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: "
        "&lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a "
        "href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/"
        "script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My "
        "Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serializePretty("\t", true) == expected);
}

TEST_CASE("Attribute properly escapes html", "[dynamic::Attribute]") {
    using namespace onyx::tags;

    std::string textToEscape =
        "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My "
        "Awesome Website</span></h1><p>Today's date is: "
        "<script>alert('Hacked!');</script></p><a "
        "href=\"https://example.com?param=<script>evil()</script>\">Click "
        "here</a><p>&copy; 2025 My Awesome Website</p></div>";

    cdiv d{Attribute("textToEscape", textToEscape, true)};

    std::string expected =
        "<div textToEscape=\"&lt;div "
        "class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to &lt;span "
        "style=&quot;color: red;&quot;&gt;My Awesome "
        "Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: "
        "&lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a "
        "href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/"
        "script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My "
        "Awesome Website&lt;/p&gt;&lt;/div&gt;\"></div>";

    CHECK(d.serialize() == expected);
}

TEST_CASE("Attribute properly escapes multi-byte", "[dynamic::Attribute]") {
    using namespace onyx::tags;

    cdiv d{Attribute("textToEscape", "😊", true, true)};

    std::string expected =
        "<div textToEscape=\"&#x1f60a;\"></div>";

    CHECK(d.serialize() == expected);
}

TEST_CASE("Attribute does not escape values when shouldEscape() is false", "[dynamic::Attribute]") {
    using namespace onyx::tags;

    cdiv d{Attribute("textToEscape", "\"", false)};

    std::string expected =
        "<div textToEscape=\"\"\"></div>";

    CHECK(d.serialize() == expected);
}

TEST_CASE("Text properly escapes unicode when multi-byte escaping is enabled",
          "[dynamic::tags::Text]") {
    using namespace onyx::tags;

    std::string textToEscape =
        "<div class=\"content\"><h1>😀Welcome to <span style=\"color: "
        "red;\">My Awesome Website</span></h1><p>Today's date is: "
        "<script>alert('Hacked!');</script></p><a "
        "href=\"https://example.com?param=<script>evil()</script>\">Click "
        "here</a><p>&copy; 2025 My Awesome Website</p></div>";

    cdiv d{Text(textToEscape, true)};

    std::string expected =
        "<div>\n\t&lt;div "
        "class=&quot;content&quot;&gt;&lt;h1&gt;&#x1f600;Welcome to &lt;span "
        "style=&quot;color: red;&quot;&gt;My Awesome "
        "Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: "
        "&lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a "
        "href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/"
        "script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My "
        "Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serializePretty("\t", true) == expected);
}

TEST_CASE("Text does not escape unicode when multi-byte escaping is disabled",
          "[dynamic::tags::Text]") {
    using namespace onyx::tags;

    std::string textToEscape =
        "<div class=\"content\"><h1>😀Welcome to <span style=\"color: "
        "red;\">My Awesome Website</span></h1><p>Today's date is: "
        "<script>alert('Hacked!');</script></p><a "
        "href=\"https://example.com?param=<script>evil()</script>\">Click "
        "here</a><p>&copy; 2025 My Awesome Website</p></div>";

    cdiv d{Text(textToEscape, false)};

    std::string expected =
        "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;😀Welcome to "
        "&lt;span style=&quot;color: red;&quot;&gt;My Awesome "
        "Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: "
        "&lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a "
        "href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/"
        "script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My "
        "Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serializePretty("\t", true) == expected);
}

TEST_CASE("XML comments are generated") {
    using namespace onyx::tags;

    GenericNode obj{"html", false, Attribute("lang", "en"),
                    Attribute("theme", "dark"),
                    GenericNode("head", false, Comment("A comment."))};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<!--A "
        "comment.-->\n\t</head>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("XML comments are escaped") {
    using namespace onyx::tags;

    GenericNode obj{
        "html", false, Attribute("lang", "en"), Attribute("theme", "dark"),
        GenericNode("head", false,
                    Comment("A comment. - --><dangerous>sequence."))};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<!--A comment. - "
        "&#x2d;&#x2d;><dangerous>sequence.-->\n\t</head>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("XML CDATA sections are generated") {
    using namespace onyx::tags;

    GenericNode obj{
        "html", false, Attribute("lang", "en"), Attribute("theme", "dark"),
        GenericNode("head", false,
                    CData("A CDATA section can contain arbitrary characters, "
                          "like >, <, =, \", \' and even &!"))};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<![CDATA[A CDATA "
        "section can contain arbitrary characters, like >, <, =, \", \' and "
        "even &!]]>\n\t</head>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("XML CDATA sections are escaped") {
    using namespace onyx::tags;

    GenericNode obj{
        "html", false, Attribute("lang", "en"), Attribute("theme", "dark"),
        GenericNode(
            "head", false,
            CData("A CDATA section is only forbidden from containing ]]>!"))};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<![CDATA[A CDATA "
        "section is only forbidden from containing "
        "&#x5d;&#x5d;&#x3e;!]]>\n\t</head>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("XML processing instructions are generated") {
    using namespace onyx::tags;

    GenericNode obj{
        "root", false, Attribute("lang", "en"),
        ProcessingInstruction("templater", "doSomething 5 > 4 somethingElse")};

    std::string expected =
        "<root lang=\"en\"><?templater doSomething 5 > 4 "
        "somethingElse?></root>";

    CHECK(expected == obj.serialize());
}

TEST_CASE("XML processing instructions are escaped") {
    using namespace onyx::tags;

    GenericNode obj{"root", false, Attribute("lang", "en"),
                    ProcessingInstruction(
                        "templater", "doSomething 5 > 4 ?> somethingElse")};

    std::string expected =
        "<root lang=\"en\"><?templater doSomething 5 > 4 &#x3f;&#x3e; "
        "somethingElse?></root>";

    CHECK(expected == obj.serialize());
}

TEST_CASE("DOCTYPEs are generated") {
    using namespace onyx::tags;

    EmptyNode obj{Doctype("some_list SYSTEM \"example.dtd\""),
                  GenericNode("html", false, Attribute("lang", "en"),
                              GenericNode("head", false))};

    std::string expected =
        "<!DOCTYPE some_list SYSTEM \"example.dtd\"><html "
        "lang=\"en\"><head></head></html>";

    CHECK(expected == obj.serialize());
}

TEST_CASE(
    "XmlDeclaration constructor with version, encoding, standalone "
    "(hadEncoding=true, hadStandalone=true)") {
    using namespace onyx::tags;

    XmlDeclaration decl("1.0", "ISO-8859-1", true);
    REQUIRE(decl.getVersionInfo() == "1.0");
    REQUIRE(decl.getEncoding() == "ISO-8859-1");
    REQUIRE(decl.getStandalone() == true);
}

TEST_CASE(
    "XmlDeclaration constructor with only version (defaults encoding and "
    "standalone)") {
    using namespace onyx::tags;

    XmlDeclaration decl("1.1");
    REQUIRE(decl.getVersionInfo() == "1.1");
    REQUIRE(decl.getEncoding() == "UTF-8");
    REQUIRE(decl.getStandalone() == false);
}

TEST_CASE(
    "XmlDeclaration constructor with version, encoding, hasEncoding, "
    "standalone, hasStandalone") {
    using namespace onyx::tags;

    XmlDeclaration decl("1.0", "UTF-16", false, true, false);
    REQUIRE(decl.getVersionInfo() == "1.0");
    REQUIRE(decl.getEncoding() == "UTF-16");
    REQUIRE(decl.getStandalone() == true);
}

TEST_CASE("XmlDeclaration copy constructor duplicates values") {
    using namespace onyx::tags;

    XmlDeclaration original("1.0", "UTF-8", true);
    XmlDeclaration copy(original);
    REQUIRE(copy.getVersionInfo() == original.getVersionInfo());
    REQUIRE(copy.getEncoding() == original.getEncoding());
    REQUIRE(copy.getStandalone() == original.getStandalone());
}

TEST_CASE("XmlDeclaration move constructor transfers values") {
    using namespace onyx::tags;

    XmlDeclaration original("1.0", "ISO-8859-1", true);
    XmlDeclaration moved(std::move(original));
    REQUIRE(moved.getVersionInfo() == "1.0");
    REQUIRE(moved.getEncoding() == "ISO-8859-1");
    REQUIRE(moved.getStandalone() == true);
}

TEST_CASE("XmlDeclaration move assignment operator transfers values") {
    using namespace onyx::tags;

    XmlDeclaration original("1.0", "ISO-8859-1", true);
    XmlDeclaration target("1.1");
    target = std::move(original);
    REQUIRE(target.getVersionInfo() == "1.0");
    REQUIRE(target.getEncoding() == "ISO-8859-1");
    REQUIRE(target.getStandalone() == true);
}

TEST_CASE("XmlDeclaration serialize with encoding and standalone attributes") {
    using namespace onyx::tags;

    XmlDeclaration decl("1.0", "ISO-8859-1", true);
    std::string serialized = decl.serialize();
    REQUIRE(
        serialized ==
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>");
}

TEST_CASE(
    "XmlDeclaration serialize with no encoding and no standalone (defaults)") {
    using namespace onyx::tags;

    XmlDeclaration decl("1.0");
    std::string serialized = decl.serialize();
    REQUIRE(serialized == "<?xml version=\"1.0\"?>");
}

TEST_CASE("XmlDeclaration serialize with encoding but standalone false") {
    using namespace onyx::tags;

    XmlDeclaration decl("1.0", "UTF-8", false);
    std::string serialized = decl.serialize();
    REQUIRE(serialized ==
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
}

TEST_CASE(
    "XmlDeclaration special serialize with encoding and standalone "
    "attributes") {
    using namespace onyx::tags;

    EmptyNode root(XmlDeclaration("1.0", "ISO-8859-1", true),
                   GenericNode("node", true));
    std::string serialized = root.serialize();
    REQUIRE(serialized ==
            "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" "
            "standalone=\"yes\"?><node/>");
}

TEST_CASE(
    "XmlDeclaration special serialize with no encoding and no standalone "
    "(defaults)") {
    using namespace onyx::tags;

    EmptyNode root(XmlDeclaration("1.0"), GenericNode("node", true));
    std::string serialized = root.serialize();
    REQUIRE(serialized == "<?xml version=\"1.0\"?><node/>");
}

TEST_CASE(
    "XmlDeclaration special serialize with encoding but standalone false") {
    using namespace onyx::tags;

    EmptyNode root(XmlDeclaration("1.0", "UTF-8", false),
                   GenericNode("node", true));
    std::string serialized = root.serialize();
    REQUIRE(
        serialized ==
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><node/>");
}

TEST_CASE("__DangerousRawText works", "[DangerousRawText]") {
    using namespace onyx::tags;

    GenericNode obj{
        "html", false, Attribute("lang", "en"), Attribute("theme", "dark"),
        GenericNode("head", false,
                    __DangerousRawText("<h1> Injected title! </h1>"))};

    std::string expected =
        "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<h1> Injected title! "
        "</h1>\n\t</head>\n</html>";

    CHECK(expected == obj.serializePretty("\t", true));
}

TEST_CASE("Node deepCopy() works", "[Node]") {
    using namespace onyx::tags;

    html obj{
        Attribute("lang", "en"), Attribute("theme", "dark"),

        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"),
                 Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            clink(Attribute("rel", "stylesheet"),
                  Attribute("href", "/styles/main.css"))),

        body(
            header(
                nav(ul(li(a(Attribute("href", "#home"), Text("Home"))),
                       li(a(Attribute("href", "#about"), Text("About Us")))))),

            main(
                section(
                    Attribute("id", "introduction"), h1(Text("Introduction")),
                    p(Text("Welcome to the complex HTML structure test case.")),
                    p(Text("This test includes various nested elements, "
                           "attributes, and content.")),
                    form(Attribute("name", "contact-form"),
                         label(Attribute("for", "name"), Text("Your Name:")),
                         input(Attribute("type", "text"),
                               Attribute("id", "name"),
                               Attribute("name", "name")),
                         label(Attribute("for", "email"), Text("Your Email:")),
                         input(Attribute("type", "email"),
                               Attribute("id", "email"),
                               Attribute("name", "email")),
                         button(Attribute("type", "submit"), Text("Submit")))),

                section(
                    Attribute("id", "features"), h2(Text("Features")),
                    ul(li(Text("Feature 1")), li(Text("Feature 2")),
                       li(Text("Feature 3"))),
                    p(Text("These are the key features of the application.")))),

            footer(p(Text("© 2025 Complex HTML Test Page")),
                   a(Attribute("href", "https://www.example.com"),
                     Text("Privacy Policy"))))};

    std::unique_ptr<Node> root = obj.deepCopy();
    REQUIRE(root);
    CHECK(root->serializePretty("\t", true) == obj.serializePretty("\t", true));
    CHECK(root.get() != &obj);
}

TEST_CASE("Node deepCopy creates a new instance with identical attributes",
          "[Node]") {
    using namespace onyx::tags;

    GenericNode obj{"html", false, Attribute("class", "test"),
                    Attribute("id", "1"), html(Attribute("class", "test"))};

    // Create a deep copy of the original node
    auto copy = obj.deepCopy();

    // Verify the copied node has the same attributes but is not the same
    // instance
    REQUIRE(copy != nullptr);
    REQUIRE(copy->getAttributes().size() == obj.getAttributes().size());
    REQUIRE(copy->getAttributes()[0] == obj.getAttributes()[0]);
    REQUIRE(copy->getAttributes()[1] == obj.getAttributes()[1]);

    // Verify the children are deeply copied (not just the pointer)
    REQUIRE(copy->getChildren().size() == obj.getChildren().size());
    REQUIRE(copy->getChildren()[0] != obj.getChildren()[0]);
    REQUIRE(copy->getChildren()[0]->getAttributes()[0] ==
            obj.getChildren()[0]->getAttributes()[0]);
}

TEST_CASE("Node deepCopy with empty node", "[Node]") {
    using namespace onyx::tags;

    p obj{};

    auto copy = obj.deepCopy();

    REQUIRE(copy != nullptr);
    REQUIRE(copy->getChildren().empty());
    REQUIRE(copy->getAttributes().empty());
}

TEST_CASE("Node deepCopy does not share internal state", "[Node]") {
    using namespace onyx::tags;

    html obj{Attribute("class", "testNode"), p(Attribute("id", "child1"))};

    auto copy = obj.deepCopy();

    obj.setAttributeValue("class", "modifiedClass");
    REQUIRE(obj.getAttributeValue("class") == "modifiedClass");
    REQUIRE(copy->getAttributeValue("class") == "testNode");
}

TEST_CASE("Node deepEquals() works", "[Node]") {
    using namespace onyx::tags;

    html obj{
        Attribute("lang", "en"), Attribute("theme", "dark"),

        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"),
                 Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            clink(Attribute("rel", "stylesheet"),
                  Attribute("href", "/styles/main.css"))),

        body(
            header(
                nav(ul(li(a(Attribute("href", "#home"), Text("Home"))),
                       li(a(Attribute("href", "#about"), Text("About Us")))))),

            main(
                section(
                    Attribute("id", "introduction"), h1(Text("Introduction")),
                    p(Text("Welcome to the complex HTML structure test case.")),
                    p(Text("This test includes various nested elements, "
                           "attributes, and content.")),
                    form(Attribute("name", "contact-form"),
                         label(Attribute("for", "name"), Text("Your Name:")),
                         input(Attribute("type", "text"),
                               Attribute("id", "name"),
                               Attribute("name", "name")),
                         label(Attribute("for", "email"), Text("Your Email:")),
                         input(Attribute("type", "email"),
                               Attribute("id", "email"),
                               Attribute("name", "email")),
                         button(Attribute("type", "submit"), Text("Submit")))),

                section(
                    Attribute("id", "features"), h2(Text("Features")),
                    ul(li(Text("Feature 1")), li(Text("Feature 2")),
                       li(Text("Feature 3"))),
                    p(Text("These are the key features of the application.")))),

            footer(p(Text("© 2025 Complex HTML Test Page")),
                   a(Attribute("href", "https://www.example.com"),
                     Text("Privacy Policy"))))};

    std::unique_ptr<Node> root = obj.deepCopy();
    REQUIRE(root);
    CHECK(root->deepEquals(obj));
    auto childrenWithHrefHome = root->getChildrenByAttribute("href", "#home");
    REQUIRE(childrenWithHrefHome.size() == 1);
    root->removeChild(childrenWithHrefHome[0]);
    CHECK(!root->deepEquals(obj));
}

TEST_CASE("Single Node has depth 0", "[Node]") {
    using namespace onyx::tags;

    cdiv root;
    REQUIRE(root.depth() == 0);
}

TEST_CASE("One level of children increases depth", "[Node]") {
    using namespace onyx::tags;

    cdiv root{p()};
    REQUIRE(root.depth() == 1);
}

TEST_CASE("Nested children increase depth", "[Node]") {
    using namespace onyx::tags;

    body root{cdiv(section(article()))};

    REQUIRE(root.depth() == 3);
}

TEST_CASE("Deepest of multiple branches determines the depth of a tree",
          "[Node]") {
    using namespace onyx::tags;

    main root{nav(), section(article(p()))};

    REQUIRE(root.depth() == 3);
}

TEST_CASE("leafCount() works", "[Node]") {
    using namespace onyx::tags;

    main root{nav(), cdiv(),
              section(article(p(), p(), span(), p(), span(), img()))};

    REQUIRE(root.leafCount() == 8);
}

TEST_CASE("leafCount() returns 1 on empty tree", "[Node]") {
    using namespace onyx::tags;

    main root{};

    REQUIRE(root.leafCount() == 1);
}

TEST_CASE("Node move properly handle indices", "[Node]") {
    using namespace onyx;
    using namespace onyx::tags;

    GenericNode obj{
        "html", false,
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("class", "container"),
                GenericNode("div", false, Attribute("class", "item")),
                GenericNode("div", false, Attribute("class", "item")),
                GenericNode("div", false, Attribute("class", "item"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");

    REQUIRE(index.getRoot() == &obj);

    GenericNode obj1(std::move(obj));
    for (auto& child : obj1.getChildren()) {
        REQUIRE(child->getParentNode() == &obj1);
    }
    REQUIRE(index.getRoot() == &obj1);
}

TEST_CASE("Nodes get removed from indices upon destruction in non-owning trees",
          "[Node]") {
    using namespace onyx;
    using namespace onyx::tags;

    GenericNode obj{NonOwning, "html", false};

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");

    REQUIRE(index.getRoot() == &obj);

    Node* child = new GenericNode(NonOwning, "div", false);
    child->setAttributeValue("class", "item");

    obj.addChild(child);

    REQUIRE(index.getByValue("item").size() == 1);

    delete child;

    REQUIRE(index.getByValue("item").size() == 0);
}

TEST_CASE("Nodes get removed from parents upon destruction in non-owning trees",
          "[Node]") {
    using namespace onyx;
    using namespace onyx::tags;

    GenericNode obj{NonOwning, "html", false};

    Node* child = new GenericNode(NonOwning, "div", false);
    child->setAttributeValue("class", "item");

    obj.addChild(child);

    REQUIRE(obj.getChildrenCount() == 1);

    delete child;

    REQUIRE(obj.getChildrenCount() == 0);
}

TEST_CASE("Node move assignment properly disowns resources", "[Node]") {
    using namespace onyx;
    using namespace onyx::tags;

    GenericNode obj{
        "html", false,
        GenericNode(
            "body", false,
            GenericNode(
                "div", false, Attribute("class", "container"),
                GenericNode("div", false, Attribute("class", "item")),
                GenericNode("div", false, Attribute("class", "item")),
                GenericNode("div", false, Attribute("class", "item"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");

    REQUIRE(index.getRoot() == &obj);

    obj = std::move(GenericNode("html", false));
    REQUIRE_FALSE(index.isValid());
}

TEST_CASE("ForEach Node unique_ptr constructor works") {
    using namespace onyx::tags;

    ul normalConstructed{li(Text("1")), li(Text("2")), li(Text("3")),
                         li(Text("4")), li(Text("5"))};

    ul forEachConstructed{ForEach{
        std::array<int, 5>{1, 2, 3, 4, 5},
        [](int index, const std::array<int, 5>& container) {
            return std::make_unique<li>(Text(std::to_string(container[index])));
        }}};

    REQUIRE(normalConstructed.serialize() == forEachConstructed.serialize());
}

TEST_CASE("ForEach Node unique_ptr constructor works with different types") {
    using namespace onyx::tags;

    cdiv normalConstructed{section(Text("1")), cdiv(Text("2")),
                           section(Text("3")), cdiv(Text("4")),
                           section(Text("5"))};

    cdiv forEachConstructed{ForEach{
        std::array<int, 5>{1, 2, 3, 4, 5},
        [](int index,
           const std::array<int, 5>& container) -> std::unique_ptr<Node> {
            if (container[index] % 2 == 0) {
                return std::make_unique<cdiv>(
                    Text(std::to_string(container[index])));
            } else {
                return std::make_unique<section>(
                    Text(std::to_string(container[index])));
            }
        }}};

    REQUIRE(normalConstructed.serialize() == forEachConstructed.serialize());
}

TEST_CASE("ForEach Node type-locked constructor works") {
    using namespace onyx::tags;

    ul normalConstructed{li(Text("1")), li(Text("2")), li(Text("3")),
                         li(Text("4")), li(Text("5"))};

    ul forEachConstructed{
        ForEach{std::array<int, 5>{1, 2, 3, 4, 5},
                [](int index, const std::array<int, 5>& container) {
                    return li(Text(std::to_string(container[index])));
                }}};

    REQUIRE(normalConstructed.serialize() == forEachConstructed.serialize());
}

TEST_CASE("ForEach Node works with strings") {
    using namespace onyx::tags;

    cdiv obscured{
        span(Text("o")), span(Text("b")), span(Text("s")), span(Text("c")),
        span(Text("u")), span(Text("r")), span(Text("e")), span(Text("d")),
    };

    cdiv forEachConstructed{ForEach{
        std::string("obscured"), [](int index, const std::string& container) {
            return span(Text(std::string(1, container[index])));
        }}};

    REQUIRE(obscured.serialize() == forEachConstructed.serialize());
}

TEST_CASE("ForEach Node iterator constructor works") {
    using namespace onyx::tags;

    std::vector<std::string> text = {"Text 1", "Text 2", "Text 3", "Text 4",
                                     "Text 5"};

    ul normalConstructed{li(Text(text[0])), li(Text(text[1])),
                         li(Text(text[2])), li(Text(text[3])),
                         li(Text(text[4]))};

    ul forEachConstructed{ForEach{text.begin(), text.end(), [](auto iterator) {
                                      return li(Text(*iterator));
                                  }}};

    REQUIRE(normalConstructed.serialize() == forEachConstructed.serialize());
}

TEST_CASE("ForEach Node no step range constructor works") {
    using namespace onyx::tags;

    ul normalConstructed{li(Text("1")), li(Text("2")), li(Text("3")),
                         li(Text("4")), li(Text("5"))};

    ul forEachConstructed{ForEach{
        1, 5, [](int index) { return li(Text(std::to_string(index))); }}};

    REQUIRE(normalConstructed.serialize() == forEachConstructed.serialize());
}

TEST_CASE("ForEach Node step range constructor works") {
    using namespace onyx::tags;

    ul normalConstructed{li(Text("1")), li(Text("3")), li(Text("5"))};

    ul forEachConstructed{ForEach{
        1, 5, 2, [](int index) { return li(Text(std::to_string(index))); }}};

    REQUIRE(normalConstructed.serialize() == forEachConstructed.serialize());
}

TEST_CASE("If Node chooses correctly") {
    using namespace onyx::tags;

    cdiv nodeTrue{If(5 > 2, Text("True"), Text("False"))};

    cdiv nodeFalse{If(3 > 5, Text("True"), Text("False"))};

    REQUIRE(nodeTrue.serialize() == "<div>True</div>");
    REQUIRE(nodeFalse.serialize() == "<div>False</div>");
}

TEST_CASE("If Node works with different Node argument types") {
    using namespace onyx::tags;

    std::string expected = "<div>True</div>";

    SECTION("Node&& and Node&&") {
        cdiv nodeTrue{If(5 > 2, Text("True"), Text("False"))};

        REQUIRE(nodeTrue.serialize() == expected);
    }
    SECTION("Node&& and std::unique_ptr<Node>") {
        std::unique_ptr<Node> text = std::make_unique<Text>("False");
        cdiv nodeTrue{If(5 > 2, Text("True"), std::move(text))};

        REQUIRE(nodeTrue.serialize() == expected);
    }
    SECTION("std::unique_ptr<Node> and Node&&") {
        std::unique_ptr<Node> text = std::make_unique<Text>("True");
        cdiv nodeTrue{If(5 > 2, std::move(text), Text("False"))};

        REQUIRE(nodeTrue.serialize() == expected);
    }
    SECTION("std::unique_ptr<Node> and std::unique_ptr<Node>") {
        std::unique_ptr<Node> textTrue = std::make_unique<Text>("True");
        std::unique_ptr<Node> textFalse = std::make_unique<Text>("True");
        cdiv nodeTrue{If(5 > 2, std::move(textTrue), std::move(textFalse))};

        REQUIRE(nodeTrue.serialize() == expected);
    }
}

TEST_CASE("Child replace works", "[Node]") {
    using namespace onyx::tags;

    std::unique_ptr<Node> child =
        std::make_unique<GenericNode>("div", false, Attribute("id", "1"));

    std::vector<NodeHandle> vec;
    vec.push_back(std::move(child));

    GenericNode obj{"html",
                    false,
                    Attribute("lang", "en"),
                    Attribute("theme", "dark"),
                    GenericNode("head", false),
                    GenericNode("body", false, {}, std::move(vec))};

    auto children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->isInTree());

    Node* parent = children[0]->getParentNode();

    std::unique_ptr<Node> child2 =
        std::make_unique<GenericNode>("div", false, Attribute("id", "2"));

    NodeHandle result = obj.replaceChild(children[0], std::move(child2));

    REQUIRE(result);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 0);
    CHECK(!result->isInTree());

    children = obj.getChildrenById("2");
    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->getParentNode() == parent);
}

TEST_CASE("Compile api dynamic bindings work") {
    using namespace onyx;
    using namespace onyx::ctags;

    tags::cdiv cd{tags::Text{"Hello!"}};

    using doc = PlaceholderDocument<
        html<head<>, body<Placeholder<"cd">, Placeholder<"ab">>>>;

    tags::ul valueToBind{tags::li(tags::Text("1")), tags::li(tags::Text("2")),
                         tags::li(tags::Text("3")), tags::li(tags::Text("4")),
                         tags::li(tags::Text("5"))};

    tags::section valueToBind2{tags::article{tags::p{}, tags::p{}, tags::span{},
                                             tags::p{}, tags::span{},
                                             tags::img{}}};

    std::string expected =
        "<html><head></head><body><ul><li>1</li><li>2</li><li>3</li><li>4</"
        "li><li>5</li></ul><section><article><p></p><p></p><span></span><p></"
        "p><span></span><img/></article></section></body></html>";

    REQUIRE(doc::serializeWithPlaceholders("cd", valueToBind, "ab",
                                           valueToBind2) == expected);

    REQUIRE(
        doc::dynamicTreeWithPlaceholders("cd", valueToBind, "ab", valueToBind2)
            ->serialize() == expected);
}

TEST_CASE("Owning NodeHandle reports owning and retains pointer",
          "[NodeHandle]") {
    using namespace onyx::tags;

    auto up = std::make_unique<GenericNode>("div", false);
    Node* raw = up.get();

    NodeHandle h(std::move(up));

    REQUIRE(h.owning() == true);
    REQUIRE(h.get() == raw);
}

TEST_CASE("Can construct owning NodeHandle with raw pointer", "[NodeHandle]") {
    using namespace onyx::tags;

    GenericNode* node = new GenericNode("span", false);

    NodeHandle h(node, true);

    REQUIRE(h.owning() == true);
    REQUIRE(h.get() == node);
}

TEST_CASE("Non-owning NodeHandle reports non-owning and retains pointer",
          "[NodeHandle]") {
    using namespace onyx::tags;

    GenericNode* node = new GenericNode("span", false);

    NodeHandle h(node, false);

    REQUIRE(h.owning() == false);
    REQUIRE(h.get() == node);

    // Clean up manually since non‑owning handle won't delete
    delete node;
}

TEST_CASE("releaseRaw transfers pointer and resets handle", "[NodeHandle]") {
    using namespace onyx::tags;
    auto up = std::make_unique<GenericNode>("p", false);
    Node* raw = up.get();

    NodeHandle h(std::move(up));

    REQUIRE(h.owning() == true);
    REQUIRE(h.get() == raw);

    Node* released = h.release();
    REQUIRE(released == raw);
    REQUIRE(h.get() == nullptr);
    REQUIRE(h.owning() == false);

    delete released;
}

TEST_CASE("toUnique on owning handle yields unique_ptr and becomes non-owning",
          "[NodeHandle]") {
    using namespace onyx::tags;

    auto up = std::make_unique<GenericNode>("section", false);
    Node* raw = up.get();
    NodeHandle h(std::move(up));

    REQUIRE(h.owning() == true);
    auto u2 = h.toUnique();

    REQUIRE(u2.get() == raw);

    REQUIRE(h.owning() == false);
}

TEST_CASE("toUnique on non-owning handle throws logic_error", "[NodeHandle]") {
    using namespace onyx::tags;
    GenericNode* node = new GenericNode("header", false);

    NodeHandle h(node, false);

    REQUIRE(h.owning() == false);
    REQUIRE_THROWS_AS(h.toUnique(), std::logic_error);

    delete node;
}

TEST_CASE("Move constructor transfers pointer and ownership", "[NodeHandle]") {
    using namespace onyx::tags;
    auto up = std::make_unique<GenericNode>("footer", false);
    Node* raw = up.get();

    NodeHandle h1(std::move(up));
    REQUIRE(h1.owning() == true);

    NodeHandle h2(std::move(h1));
    REQUIRE(h2.owning() == true);
    REQUIRE(h2.get() == raw);
    REQUIRE(h1.get() == nullptr);
    REQUIRE(h1.owning() == false);
}

TEST_CASE("Move assignment transfers pointer and ownership", "[NodeHandle]") {
    using namespace onyx::tags;
    auto up1 = std::make_unique<GenericNode>("article", false);
    Node* raw = up1.get();

    NodeHandle h1(std::move(up1));
    REQUIRE(h1.owning() == true);

    GenericNode dummy("div", false);
    NodeHandle h2(&dummy, false);
    REQUIRE(h2.owning() == false);

    h2 = std::move(h1);

    REQUIRE(h2.owning() == true);
    REQUIRE(h2.get() == raw);
    REQUIRE(h1.get() == nullptr);
    REQUIRE(h1.owning() == false);
}

TEST_CASE("Non-owning Node does not destroy its children") {
    using namespace onyx::tags;

    Node* body = new GenericNode(NonOwning, "body", false);
    {
        GenericNode root(NonOwning, "html", false);

        root.addChild(body);
    }

    REQUIRE(body->getTagName() == "body");

    delete body;
}

TEST_CASE(
    "Mixing owning and non-owning Nodes with move constructor causes "
    "exception") {
    using namespace onyx::tags;

    // This will leak unless move constructor properly cleans up memory
    REQUIRE_THROWS(GenericNode("html", false, GenericNode("body", false),
                               GenericNode(NonOwning, "div", false)));
}

TEST_CASE("Mixing owning and non-owning Nodes causes exception") {
    using namespace onyx::tags;

    GenericNode root("html", false);
    Node* body = new GenericNode(NonOwning, "body", false);
    REQUIRE_THROWS(root.addChild(body));

    delete body;
}

TEST_CASE("Vector constructor throws when mixing ownership modes", "[Node]") {
    using namespace onyx::tags;

    Node::setIndentationSequence("\t");
    Node::setSortAttributes(true);

    std::vector<Attribute> attributes;
    std::vector<NodeHandle> children;

    attributes.emplace_back("id", "list");
    for (int i = 1; i <= 3; i++) {
        children.emplace_back(std::make_unique<cdiv>(Text(std::to_string(i))));
    }

    Node* nonOwned = new GenericNode("div", false);

    children.emplace_back(nonOwned);

    REQUIRE_THROWS(body{attributes, std::move(children)});

    delete nonOwned;
}

TEST_CASE("Arena allocates and constructs nodes", "[Arena]") {
    using namespace onyx::tags;
    Arena::Builder builder;
    builder.preallocate<GenericNode>().preallocate<Text>();

    Arena arena = builder.build();

    auto* parent = arena.allocate<GenericNode>("html", false);
    auto* child = arena.allocate<Text>("hello world");

    REQUIRE(parent->getTagName() == "html");
    REQUIRE(child->getTagName() == ".text");

    parent->addChild(child);

    REQUIRE(parent->getChildrenCount() == 1);
    REQUIRE(parent->getChildren()[0]->getTagName() == ".text");
    REQUIRE(static_cast<Text*>(parent->getChildren()[0])->getText() ==
            "hello world");
}

TEST_CASE("Arena cleans up memory", "[Arena]") {
    using namespace onyx::tags;
    Arena::Builder builder;
    builder.preallocate<GenericNode>();
    for (int i = 0; i < 5; ++i) {
        builder.preallocate<Text>();
    }
    builder.preallocate<Text>();

    EmptyNode parent{NonOwning};

    {
        Arena arena = builder.build();
        Node* ptr = parent.addChild(arena.allocate<GenericNode>("html", false));
        for (int i = 0; i < 5; ++i) {
            parent.addChild(arena.allocate<Text>("abc"));
        }
        ptr->addChild(arena.allocate<Text>("abc"));
    }

    REQUIRE(parent.getChildrenCount() == 0);
}

TEST_CASE("Arena throws on over allocation", "[Arena]") {
    using namespace onyx::tags;

    Arena arena(2);

    REQUIRE_THROWS(arena.allocate<Text>(""));
}