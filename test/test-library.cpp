#include "catch2/catch_all.hpp"
#include "templater.h"

#include <chrono>

using namespace Templater::html;

TEST_CASE("HTML is generated", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false)
    );

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head></head>\n</html>";

    INFO ("The generated html is \n" << obj.serialise() );

    CHECK(expected == obj.serialise());
}

TEST_CASE("serialise() arguments override global identation rules", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false)
    );

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n    <head></head>\n</html>";

    INFO ("The generated html is \n" << obj.serialise("    ") );

    CHECK(expected == obj.serialise("    "));
}

TEST_CASE("Vector constructor works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    std::vector<Attribute> attributes;
    std::vector<std::shared_ptr<Object>> children;

    attributes.emplace_back("id", "list");
    for (int i = 1; i <= 3; i++) {
        children.push_back(dtags::li(Text(std::to_string(i))).clone());
    }

    dtags::ul obj = dtags::ul(attributes, children);

    std::string expected = "<ul id=\"list\">\n\t<li>\n\t\t1\n\t</li>\n\t<li>\n\t\t2\n\t</li>\n\t<li>\n\t\t3\n\t</li>\n</ul>";

    INFO ("The generated html is \n" << obj.serialise() );

    CHECK(expected == obj.serialise());
}

TEST_CASE("Complex test case generates html", "[Object]" ) {

    GenericObject obj = GenericObject(
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
    );

    std::string expected = "<html theme=\"dark\" lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#about\">\n\t\t\t\t\t\t\tAbout Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes various nested elements, attributes, and content.\n\t\t\t\t</p>\n\t\t\t\t<form name=\"contact-form\">\n\t\t\t\t\t<label for=\"name\">\n\t\t\t\t\t\tYour Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</form>\n\t\t\t</section>\n\t\t\t<section id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are the key features of the application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test Page\n\t\t\t</p>\n\t\t\t<a href=\"https://www.example.com\">\n\t\t\t\tPrivacy Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    INFO ("The generated html is \n" << obj.serialise() );

    CHECK(expected == obj.serialise());
}

TEST_CASE("Children return by tag name works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
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

    auto children = obj.getChildrenByTagName("div");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }
}

TEST_CASE("Children return by id works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
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
    ));

    auto children = obj.getChildrenById("11");

    CHECK(children[0]->getAttributeValue("id") == std::to_string(11));
    CHECK(children[0]->getTagName() == "p");
}

TEST_CASE("Children return by name works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
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
    ));

    auto children = obj.getChildrenByName("d");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByName("p");
    
    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Children return by class name works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
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
    ));

    auto children = obj.getChildrenByClassName("d");

    for (int i = 0; i < 5; i++) {
        CHECK(children[i]->getAttributeValue("id") == std::to_string(i));
    }

    children = obj.getChildrenByClassName("p");
    
    CHECK(children[0]->getAttributeValue("id") == "11");
}

TEST_CASE("Child add works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false));

    auto children = obj.getChildrenByTagName("body");

    GenericObject child = GenericObject(
        "div", false, Attribute("id", "1")
    );

    children[0]->addChild(child);

    children = obj.getChildrenById("1");

    CHECK(children[0].get()->isInTree());
    CHECK(*(children[0].get()) == child);
}

TEST_CASE("Child remove works", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject child = GenericObject(
        "div", false, Attribute("id", "1")
    );

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, child));

    CHECK(child.isInTree());

    auto children = obj.getChildrenById("1");

    CHECK(children.size() == 1);
    CHECK(children[0]->isInTree());

    bool result = obj.removeChild(children[0]);

    CHECK(result);

    children = obj.getChildrenById("1");

    CHECK(children.size() == 0);
    CHECK(!child.isInTree());
}

TEST_CASE("Children get properly disowned upon parent destruction", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject child = GenericObject(
        "div", false, Attribute("id", "1")
    );

    {
        GenericObject obj = GenericObject(
            "html", false,
            Attribute("lang", "en"),
            Attribute("theme", "dark"),
            GenericObject("head", false),
            GenericObject("body", false, child));
    
        CHECK(child.isInTree());    
    }
    
    CHECK(!child.isInTree());  
}

TEST_CASE("Children do not get disowned upon pointer to parent destruction", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject child = GenericObject(
        "div", false, Attribute("id", "1")
    );

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false, child));

    {
        GenericObject obj2 = obj; 
    }  
    
    CHECK(child.isInTree());  
}

TEST_CASE("Operator [] works for attribute access", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false));

    CHECK(obj["lang"] == "en");

    obj["theme"] = "light";

    CHECK(obj["theme"] == "light");    
}

TEST_CASE("Operator += works for child add", "[Object]" ) {
    Object::setIdentationSequence("\t");
    Object::setSortAttributes(true);

    GenericObject obj = GenericObject(
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericObject("head", false),
        GenericObject("body", false));

    auto children = obj.getChildrenByTagName("body");

    GenericObject child = GenericObject(
        "div", false, Attribute("id", "1")
    );

    *(children[0].get()) += child;

    children = obj.getChildrenById("1");

    CHECK(children[0].get()->isInTree());
    CHECK(*(children[0].get()) == child);
}


TEST_CASE("Complex html with tags", "[Object]" ) {
    using namespace Templater::html::dtags;

    html obj = html(
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
    );

    std::string expected = "<html theme=\"dark\" lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\"/>\n\t\t<meta content=\"width=device-width, initial-scale=1.0\" name=\"viewport\"/>\n\t\t<title>\n\t\t\tComplex Test Page\n\t\t</title>\n\t\t<link href=\"/styles/main.css\" rel=\"stylesheet\"/>\n\t</head>\n\t<body>\n\t\t<header>\n\t\t\t<nav>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#home\">\n\t\t\t\t\t\t\tHome\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\t<a href=\"#about\">\n\t\t\t\t\t\t\tAbout Us\n\t\t\t\t\t\t</a>\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t</nav>\n\t\t</header>\n\t\t<main>\n\t\t\t<section id=\"introduction\">\n\t\t\t\t<h1>\n\t\t\t\t\tIntroduction\n\t\t\t\t</h1>\n\t\t\t\t<p>\n\t\t\t\t\tWelcome to the complex HTML structure test case.\n\t\t\t\t</p>\n\t\t\t\t<p>\n\t\t\t\t\tThis test includes various nested elements, attributes, and content.\n\t\t\t\t</p>\n\t\t\t\t<form name=\"contact-form\">\n\t\t\t\t\t<label for=\"name\">\n\t\t\t\t\t\tYour Name:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"name\" name=\"name\" type=\"text\"/>\n\t\t\t\t\t<label for=\"email\">\n\t\t\t\t\t\tYour Email:\n\t\t\t\t\t</label>\n\t\t\t\t\t<input id=\"email\" name=\"email\" type=\"email\"/>\n\t\t\t\t\t<button type=\"submit\">\n\t\t\t\t\t\tSubmit\n\t\t\t\t\t</button>\n\t\t\t\t</form>\n\t\t\t</section>\n\t\t\t<section id=\"features\">\n\t\t\t\t<h2>\n\t\t\t\t\tFeatures\n\t\t\t\t</h2>\n\t\t\t\t<ul>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 1\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 2\n\t\t\t\t\t</li>\n\t\t\t\t\t<li>\n\t\t\t\t\t\tFeature 3\n\t\t\t\t\t</li>\n\t\t\t\t</ul>\n\t\t\t\t<p>\n\t\t\t\t\tThese are the key features of the application.\n\t\t\t\t</p>\n\t\t\t</section>\n\t\t</main>\n\t\t<footer>\n\t\t\t<p>\n\t\t\t\t© 2025 Complex HTML Test Page\n\t\t\t</p>\n\t\t\t<a href=\"https://www.example.com\">\n\t\t\t\tPrivacy Policy\n\t\t\t</a>\n\t\t</footer>\n\t</body>\n</html>";

    INFO ("The generated html is \n" << obj.serialise() );

    CHECK(expected == obj.serialise());
}

TEST_CASE("Static library check", "[Object]" ) {
    using namespace Templater::html::ctags;

    using doc = Document<html<
                            ctags::Attribute<"lang", "en">,
                            head<>,
                            body<ctags::Text<"Hello world!">>
    >>;

    std::string expected = "<html lang=\"en\">\n\t<head></head>\n\t<body>\n\t\tHello world!\n\t</body>\n</html>";

    CHECK(doc::value() == expected);
}