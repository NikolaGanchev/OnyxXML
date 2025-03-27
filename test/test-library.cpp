#include "catch2/catch_all.hpp"
#include "templater.h"

#include <chrono>

TEST_CASE("HTML is generated", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false)
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head></head>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("serialise() arguments override global indentation rules", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false)
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n    <head></head>\n</html>";

    CHECK(expected == obj.serialise("    "));
}

TEST_CASE("Vector constructor works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::vector<Attribute> attributes;
    std::vector<std::shared_ptr<Object>> children;

    attributes.emplace_back("id", "list");
    for (int i = 1; i <= 3; i++) {
        children.push_back(std::make_shared<dtags::li>(Text(std::to_string(i))));
    }

    dtags::ul obj{attributes, children};

    std::string expected = "<ul id=\"list\">\n\t<li>\n\t\t1\n\t</li>\n\t<li>\n\t\t2\n\t</li>\n\t<li>\n\t\t3\n\t</li>\n</ul>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("Complex test case generates html", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        
        GenericObject("head", false,
            GenericObject("meta", true, 
                Attribute("charset", "UTF-8")),
            GenericObject("meta", true, 
                Attribute("name", "viewport"),
                Attribute("content", "width=device-width, initial-scale=1.0")),
            GenericObject("title", false, 
                Text("Complex Test Page")),
            GenericObject("link", true,
                Attribute("rel", "stylesheet"),
                Attribute("href", "/styles/main.css"))
        ),
        
        GenericObject("body", false, 
            GenericObject("header", false, 
                GenericObject("nav", false, 
                    GenericObject("ul", false, 
                        GenericObject("li", false, 
                            GenericObject("a", false,
                                Attribute("href", "#home"),
                                Text("Home")
                            )
                        ),
                        GenericObject("li", false, 
                            GenericObject("a", false,
                                Attribute("href", "#about"),
                                Text("About Us")
                            )
                        )
                    )
                )
            ),
            
            GenericObject("main", false,
                GenericObject("section", false, 
                    Attribute("id", "introduction"),
                    GenericObject("h1", false, Text("Introduction")),
                    GenericObject("p", false, Text("Welcome to the complex HTML structure test case.")),
                    GenericObject("p", false, Text("This test includes various nested elements, attributes, and content.")),
                    GenericObject("form", false, 
                        Attribute("name", "contact-form"),
                        GenericObject("label", false,
                            Attribute("for", "name"),
                            Text("Your Name:")
                        ),
                        GenericObject("input", true, 
                            Attribute("type", "text"),
                            Attribute("id", "name"),
                            Attribute("name", "name")
                        ),
                        GenericObject("label", false, 
                            Attribute("for", "email"),
                            Text("Your Email:")
                        ),
                        GenericObject("input", true, 
                            Attribute("type", "email"),
                            Attribute("id", "email"),
                            Attribute("name", "email")
                        ),
                        GenericObject("button", false, 
                            Attribute("type", "submit"),
                            Text("Submit")
                        )
                    )
                ),
                
                GenericObject("section", false, 
                    Attribute("id", "features"),
                    GenericObject("h2", false, Text("Features")),
                    GenericObject("ul", false, 
                        GenericObject("li", false, Text("Feature 1")),
                        GenericObject("li", false, Text("Feature 2")),
                        GenericObject("li", false, Text("Feature 3"))
                    ),
                    GenericObject("p", false, Text("These are the key features of the application."))
                )
            ),
            
            GenericObject("footer", false,
                GenericObject("p", false, Text("© 2025 Complex HTML Test Page")),
                GenericObject("a", false,
                    Attribute("href", "https://www.example.com"),
                    Text("Privacy Policy")
                )
            )
        )
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#about\">\n\t\t\t\t\t\t\tAbout Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes various nested elements, attributes, and content.\n\t\t\t\t</p>\n\t\t\t\t<form name=\"contact-form\">\n\t\t\t\t\t<label for=\"name\">\n\t\t\t\t\t\tYour Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</form>\n\t\t\t</section>\n\t\t\t<section id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are the key features of the application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test Page\n\t\t\t</p>\n\t\t\t<a href=\"https://www.example.com\">\n\t\t\t\tPrivacy Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    CHECK(expected == obj.serialise());
}

TEST_CASE("GenericObject can't be given children if void", "[GenericObject]") {
    using namespace Templater::dynamic::dtags;

    REQUIRE_THROWS(GenericObject{"img", true, GenericObject{"div", false}});

    std::shared_ptr<Object> d = std::make_shared<GenericObject>("div", false);
    
    REQUIRE_THROWS(GenericObject{"img", true, {}, { d }});
}

TEST_CASE("Object::addChild() throws if used on a void object", "[Object::addChild]") {
    using namespace Templater::dynamic::dtags;

    GenericObject image{"img", true};

    REQUIRE_THROWS(image.addChild(GenericObject{"div", false}));

    std::shared_ptr<Object> d = std::make_shared<GenericObject>("div", false);

    REQUIRE_THROWS(image.addChild(d));
}

TEST_CASE("Children return by tag name works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("id", "0"), 
                GenericObject("div", false, Attribute("id", "1"),
                    GenericObject("div", false, Attribute("id", "2"))),
            GenericObject("div", false, Attribute("id", "3")),
            GenericObject("div", false, Attribute("id", "4")))
    )};

    auto children = obj.getChildrenByTagName("div");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }
}

TEST_CASE("Children return by id works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("id", "0"), 
                GenericObject("div", false, Attribute("id", "1"),
                    GenericObject("div", false, Attribute("id", "2"),
                        GenericObject("p", false, Attribute("id", "11")))),
            GenericObject("div", false, Attribute("id", "3")),
            GenericObject("div", false, Attribute("id", "4")))
    )};

    auto children = obj.getChildrenById("11");

    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
    CHECK(children[0]->getTagName() == "p");
}

TEST_CASE("Children return by name works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("name", "d"), Attribute("id", "0"), 
                GenericObject("div", false, Attribute("name", "d"), Attribute("id", "1"), 
                    GenericObject("div", false, Attribute("name", "d"), Attribute("id", "2"), 
                        GenericObject("p", false, Attribute("name", "p"), Attribute("id", "11")))),
            GenericObject("div", false, Attribute("name", "d"), Attribute("id", "3")),
            GenericObject("div", false, Attribute("name", "d"), Attribute("id", "4")))
    )};

    auto children = obj.getChildrenByName("d");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByName("p");
    
    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Children return by class name works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("class", "d"), Attribute("id", "0"), 
                GenericObject("div", false, Attribute("class", "d"), Attribute("id", "1"), 
                    GenericObject("div", false, Attribute("class", "d"), Attribute("id", "2"), 
                        GenericObject("p", false, Attribute("class", "p"), Attribute("id", "11")))),
            GenericObject("div", false, Attribute("class", "d"), Attribute("id", "3")),
            GenericObject("div", false, Attribute("class", "d"), Attribute("id", "4")))
    )};

    auto children = obj.getChildrenByClassName("d");
    
    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByClassName("p");
    
    REQUIRE(children.size() == 1);

    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Child add works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false)};

    auto children = obj.getChildrenByTagName("body");

    std::shared_ptr<Object> child = std::make_shared<GenericObject>(
        "div", false, Attribute("id", "1")
    );

    children[0]->addChild(child);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0].get()->isInTree());
}

TEST_CASE("Child remove works", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::shared_ptr<Object> child = std::make_shared<GenericObject>(
        "div", false, Attribute("id", "1")
    );

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, {}, {child})};

    CHECK(child->isInTree());

    auto children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0]->isInTree());

    bool result = obj.removeChild(children[0]);

    CHECK(result);

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 0);
    CHECK(!child->isInTree());
}

TEST_CASE("Children get properly disowned upon parent destruction", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::shared_ptr<GenericObject> child = std::make_shared<GenericObject>(
        "div", false, Attribute("id", "1")
    );

    {
        GenericObject obj{
            "html", false,
            Attribute("lang", "en"),
            Attribute("theme", "dark"),
            GenericObject("head", false),
            GenericObject("body", false, {}, {child})};
    
        CHECK(child->isInTree());    
    }
    
    CHECK(!child->isInTree());  
}

TEST_CASE("Operator [] works for attribute access", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false)};

    CHECK(obj["lang"] == "en");

    obj["theme"] = "light";

    CHECK(obj["theme"] == "light"); 
    CHECK(obj.getAttributeValue("theme") == "light");   
}

TEST_CASE("Operator += works for child add", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false)};

    auto children = obj.getChildrenByTagName("body");

    std::shared_ptr<Object> child = std::make_shared<GenericObject>(
        "div", false, Attribute("id", "1")
    );

    REQUIRE(children.size() == 1);
    *(children[0].get()) += child;

    children = obj.getChildrenById("1");

    REQUIRE(children.size() == 1);
    CHECK(children[0].get()->isInTree());
}


TEST_CASE("Complex html with dynamic tags", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    html obj{
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        
        head(
            meta(Attribute("charset", "UTF-8")),
            meta(Attribute("name", "viewport"), 
                Attribute("content", "width=device-width, initial-scale=1.0")),
            title(Text("Complex Test Page")),
            link(Attribute("rel", "stylesheet"),
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


TEST_CASE("Empty html tree has size 1", "[Object::size()]") {
    using namespace Templater::dynamic::dtags;
    html root;
    REQUIRE(root.size() == 1);
}

TEST_CASE("Html tree with one child has size 2", "[Object::size()]") {
    using namespace Templater::dynamic::dtags;
    html root;
    root.addChild(body());
    REQUIRE(root.size() == 2);
}

TEST_CASE("Html tree with 5 nodes has size 6", "[Object::size()]") {
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

TEST_CASE("Html tree with 5001 nodes has size 5001", "[Object::size()]") {
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

void addChildren(std::shared_ptr<Templater::dynamic::Object> root, int level) {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<section> s = std::make_shared<section>();
    if (level > 0) {
        addChildren(s, level-1);
    }

    root->addChild(s);
}

TEST_CASE("3000 tags serialise in under 50ms", "[Object]") {
    using namespace Templater::dynamic::dtags;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    section root{};

    for (int i = 0; i < 500; i++) {
        std::shared_ptr<Object> paragraph = std::make_shared<p>();
        for (int i = 0; i < 10; i++) {
            paragraph->operator[](generateRandomString(10)) = generateRandomString(10);
        }
        paragraph->addChild(Text(generateRandomString(200)));
        addChildren(paragraph, 3);
        root.addChild(paragraph);
    }

    auto t1 = high_resolution_clock::now();
    root.serialise("\t", true);
    auto t2 = high_resolution_clock::now();
    
    duration<double, std::milli> time = t2 - t1;
    INFO(root.size());

    REQUIRE(time.count() < 50);
}

TEST_CASE("Compile-time html serialises correctly", "[Object]" ) {
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

TEST_CASE("Compile-time html enforces given indentation rules", "[Object]" ) {
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

TEST_CASE("HTML fragment using Document serialises correctly", "[Object]" ) {
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


TEST_CASE("Complex html with constant tags", "[Object]" ) {
    using namespace Templater::compile;
    using namespace Templater::compile::ctags;

    std::string doc4 = Document<
        html<
            Attribute<"lang", "en">,
            head<
                title<Text<"Test Page">>,
                meta<Attribute<"charset", "UTF-8">>,
                meta<Attribute<"name", "viewport">, Attribute<"content", "width=device-width, initial-scale=1.0">>,
                link<Attribute<"rel", "stylesheet">, Attribute<"href", "styles.css">>
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

TEST_CASE("Escapes complex html", "[escape]" ) {
    using namespace Templater::dynamic::text;

    std::string textToEscape = "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    std::string escaped = "&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;";

    CHECK(escaped == escape(textToEscape));
}

TEST_CASE("Empty string remains unchanged", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "";
    std::string expected = "";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Reserved HTML characters are properly escaped", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "&<>\"'";
    std::string expected = "&amp;&lt;&gt;&quot;&#39;";
    REQUIRE(escape(input) == expected);
}

TEST_CASE("Non-escaping ASCII characters remain unchanged", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "Hello, World!";
    std::string expected = "Hello, World!";
    REQUIRE(escape(input) == expected);
}

TEST_CASE("Single non-ASCII character is converted to a numeric entity when multi-byte escaping is enabled", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "café";
    std::string expected = "caf&#xe9;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Single non-ASCII character is not converted to a numeric entity when multi-byte escaping is disabled", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "café";
    std::string expected = "café";
    REQUIRE(escape(input, false) == expected);
}

TEST_CASE("Emoji (4-byte sequence) is converted to a numeric entity", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "😊";
    std::string expected = "&#x1f60a;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Mixed content with ASCII, reserved characters, and multi-byte sequences", "[escape]") {
    using namespace Templater::dynamic::text;
    std::string input = "Hello <world> & café 😊";
    std::string expected = "Hello &lt;world&gt; &amp; caf&#xe9; &#x1f60a;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Escapes 1 million characters in under 150ms", "[escape]") {
    using namespace Templater::dynamic::text;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::string input = 
    R"~(<script>alert('Hacked!');</script><img src=x onerror=alert(1)><a href="javascript:alert('XSS')">Click me</a>🌀✨🔥💀🎉🚀👾🤖👻<div style="background:url(javascript:alert('XSS'))">Test</div>
<p onclick="alert('XSS')">Click here</p>&lt;iframe src="javascript:alert('XSS')"&gt;&lt;/iframe&gt;©®™✓😈💣💥🌍📢💾🔑🎵💡🏆🤯🥳<svg onload=alert('XSS')></svg>
💖🧡💛💚💙💜🖤🤍🤎💢💬🗯️🔥👀🚨⚠️<input type="text" value="&lt;script&gt;alert('XSS')&lt;/script&gt;">☀️🌧️🌩️🌈☃️🌊🦄🎭🎨🎤🎮🎻🔮📱💻🖥️🖨️⌨️🖱️🖲️🎥📸📹🔍🔎💰💳💎
&lt;math&gt;&lt;mtext&gt;&lt;script&gt;alert(1)&lt;/script&gt;&lt;/mtext&gt;&lt;/math&gt;🎵🎶🎼🥁🎷🎸🎺🎻🪕📢📣🔊🔔🛎️🔕🏆🥇🥈🥉🏅🎖️🎗️🎟️🎫🔑🔐🗝️💡💰💴💵💶💷💳💸💎
𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂𐍈𐌰𐌼𐍃𐍂)~";

    for (int i = 0; i < 10; i++) {
        input += input;
    } 

    INFO(input.size());

    auto t1 = high_resolution_clock::now();
    std::string result = escape(input, true);
    auto t2 = high_resolution_clock::now();
    
    duration<double, std::milli> time = t2 - t1;

    REQUIRE(time.count() < 150);
}

TEST_CASE("Escape 1 million character safe string in under 100ms", "[escape]") {
    using namespace Templater::dynamic::text;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::string input(1'000'000, 'a');

    INFO(input.size());

    auto t1 = high_resolution_clock::now();
    std::string result = escape(input, true);
    auto t2 = high_resolution_clock::now();
    
    duration<double, std::milli> time = t2 - t1;

    REQUIRE(time.count() < 100);
}

TEST_CASE("Text properly escapes html", "[dynamic::dtags::Text]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::string textToEscape = "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    dtags::div d{Text(textToEscape)};

    std::string expected = "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serialise() == expected);
}

TEST_CASE("Text properly escapes unicode when multi-byte escaping is enabled", "[dynamic::dtags::Text]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::string textToEscape = "<div class=\"content\"><h1>😀Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    dtags::div d{Text(textToEscape, true)};

    std::string expected = "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;&#x1f600;Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serialise() == expected);
}

TEST_CASE("Text does not escape unicode when multi-byte escaping is disabled", "[dynamic::dtags::Text]" ) {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::string textToEscape = "<div class=\"content\"><h1>😀Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    dtags::div d{Text(textToEscape, false)};

    std::string expected = "<div>\n\t&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;😀Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;\n</div>";

    CHECK(d.serialise() == expected);
}

TEST_CASE("Index is added correctly", "[Object]" ) {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("id", "0"), 
                GenericObject("div", false, Attribute("id", "1"),
                    GenericObject("div", false, Attribute("id", "2"))),
            GenericObject("div", false, Attribute("id", "3")),
            GenericObject("div", false, Attribute("id", "4")))
    ));

    REQUIRE(obj->getChildrenCount() > 0);

    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "id");
    index->init();

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index handles multiple matches correctly", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>(
        "html", false,
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("class", "container"),
                GenericObject("div", false, Attribute("class", "item")),
                GenericObject("div", false, Attribute("class", "item")),
                GenericObject("div", false, Attribute("class", "item"))
            )
        )
    );

    REQUIRE(obj->getChildrenCount() > 0);

    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "class");
    index->init();

    auto result = index->getByValue("item");

    REQUIRE(result.size() == 3);
    for (const auto& elem : result) {
        CHECK(elem->getAttributeValue("class") == "item");
    }
}

TEST_CASE("Index returns empty when no match found", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>(
        "html", false,
        GenericObject("body", false, 
            GenericObject("div", false, Attribute("class", "container"))
        )
    );

    REQUIRE(obj->getChildrenCount() > 0);

    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "class");
    index->init();

    auto result = index->getByValue("nonexistent");
    REQUIRE(result.empty());
}

TEST_CASE("Index works with nested attributes", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>(
        "html", false,
        GenericObject("body", false, 
            GenericObject("section", false, Attribute("data-type", "main"),
                GenericObject("div", false, Attribute("data-type", "nested"))
            )
        )
    );

    REQUIRE(obj->getChildrenCount() > 0);

    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "data-type");
    index->init();

    auto result = index->getByValue("nested");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("data-type") == "nested");
}

TEST_CASE("Index updates correctly when attributes change", "[Object]") {
    using namespace Templater::dynamic::dtags;
    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>("div", false, Attribute("id", "test"));
    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "id");
    index->init();

    auto result = index->getByValue("test");
    REQUIRE(result.size() == 1);

    obj->setAttributeValue("id", "updated");
    result = index->getByValue("test");
    REQUIRE(result.empty());

    result = index->getByValue("updated");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "updated");
}

TEST_CASE("Index updates correctly when children are added", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>("div", false);
    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "class");
    index->init();

    auto result = index->getByValue("new-class");
    REQUIRE(result.empty());

    std::shared_ptr<Object> child = std::make_shared<GenericObject>("span", false, Attribute("class", "new-class"));
    obj->addChild(child);

    result = index->getByValue("new-class");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("class") == "new-class");
}

TEST_CASE("Index updates correctly when children are added using move", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>("div", false);
    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "class");
    index->init();

    auto result = index->getByValue("new-class");
    REQUIRE(result.empty());

    obj->addChild(GenericObject("span", false, Attribute("class", "new-class")));

    result = index->getByValue("new-class");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("class") == "new-class");
}

TEST_CASE("Index updates correctly when attributes are modified using operator []", "[Object]") {
    using namespace Templater::dynamic::dtags;

    Object::setIndentationSequence("\t");
    Object::setSortAttributes(true);

    std::shared_ptr<Object> obj = std::make_shared<GenericObject>("div", false);
    (*obj)["id"] = "original";

    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(obj, "id");
    index->init();

    auto result = index->getByValue("original");
    REQUIRE(result.size() == 1);

    (*obj)["id"] = "changed";
    result = index->getByValue("original");
    REQUIRE(result.empty());

    result = index->getByValue("changed");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "changed");
}


TEST_CASE("Children are properly removed from parent indices", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> parent = std::make_shared<GenericObject>("div", false);
    std::shared_ptr<Object> child = std::make_shared<GenericObject>("span", false, Attribute("class", "removable"));
    parent->addChild(child);

    std::shared_ptr<index::AttributeNameIndex> index = std::make_shared<index::AttributeNameIndex>(parent, "class");
    index->init();
    REQUIRE(index->getByValue("removable").size() == 1);

    parent->removeChild(child);
    REQUIRE(index->getByValue("removable").empty());
}

TEST_CASE("Children keep their own indices when removed", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> child = std::make_shared<GenericObject>("span", false, Attribute("id", "child"));
    std::shared_ptr<index::AttributeNameIndex> childIndex = std::make_shared<index::AttributeNameIndex>(child, "id");
    childIndex->init();
    REQUIRE(childIndex->getByValue("child").size() == 1);

    std::shared_ptr<Object> parent = std::make_shared<GenericObject>("div", false);
    std::shared_ptr<index::AttributeNameIndex> parentIndex = std::make_shared<index::AttributeNameIndex>(parent, "id");
    parentIndex->init();
    parent->addChild(child);
    parent->removeChild(child);

    REQUIRE(childIndex->getByValue("child").size() == 1);
}

TEST_CASE("Children keep their own indices when removed upon parent destruction", "[Object]") {
    using namespace Templater::dynamic::dtags;

    std::shared_ptr<Object> child = std::make_shared<GenericObject>("span", false, Attribute("id", "child"));
    std::shared_ptr<index::AttributeNameIndex> childIndex = std::make_shared<index::AttributeNameIndex>(child, "id");
    childIndex->init();
    REQUIRE(childIndex->getByValue("child").size() == 1);
    {
        std::shared_ptr<Object> parent = std::make_shared<GenericObject>("div", false);
        std::shared_ptr<index::AttributeNameIndex> parentIndex = std::make_shared<index::AttributeNameIndex>(parent, "id");
        parentIndex->init();
        parent->addChild(child);
    }

    REQUIRE(childIndex->getByValue("child").size() == 1);
}

