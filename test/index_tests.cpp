#include "catch2/catch_all.hpp"
#include "templater.h"
#include <chrono>

TEST_CASE("Index is added correctly", "[Index]" ) {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

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

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index handles multiple matches correctly", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("class", "container"),
                GenericNode("div", false, Attribute("class", "item")),
                GenericNode("div", false, Attribute("class", "item")),
                GenericNode("div", false, Attribute("class", "item"))
            )
        )
    };

    REQUIRE(obj.getChildrenCount() > 0);
    
    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto result = index.getByValue("item");

    REQUIRE(result.size() == 3);
    for (const auto& elem : result) {
        CHECK(elem->getAttributeValue("class") == "item");
    }
}

TEST_CASE("Index returns empty when no match found", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("class", "container"))
        )
    };

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto result = index.getByValue("nonexistent");
    REQUIRE(result.empty());
}

TEST_CASE("Index works with nested attributes", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("section", false, Attribute("data-type", "main"),
                GenericNode("div", false, Attribute("data-type", "nested"))
            )
        )
    };

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "data-type");

    auto result = index.getByValue("nested");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("data-type") == "nested");
}

TEST_CASE("Index updates correctly when attributes change", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{"div", false, Attribute("id", "test")};
    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("test");
    REQUIRE(result.size() == 1);

    obj.setAttributeValue("id", "updated");
    result = index.getByValue("test");
    REQUIRE(result.empty());

    result = index.getByValue("updated");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "updated");

    obj.removeAttribute("id");

    result = index.getByValue("updated");
    CHECK(result.size() == 0);
}

TEST_CASE("Index updates correctly when children are added", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{"div", false};
    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto result = index.getByValue("new-class");
    REQUIRE(result.empty());

    std::unique_ptr<Node> child = std::make_unique<GenericNode>("span", false, Attribute("class", "new-class"));
    obj.addChild(std::move(child));

    result = index.getByValue("new-class");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("class") == "new-class");
}

TEST_CASE("Index updates correctly when children are added using move", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{"div", false};
    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "class");
    auto result = index.getByValue("new-class");
    REQUIRE(result.empty());

    obj.addChild(GenericNode("span", false, Attribute("class", "new-class")));

    result = index.getByValue("new-class");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("class") == "new-class");
}

TEST_CASE("Index updates correctly when attributes are modified using operator []", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{"div", false};
    obj["id"] = "original";

    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("original");
    REQUIRE(result.size() == 1);

    obj["id"] = "changed";
    result = index.getByValue("original");
    REQUIRE(result.empty());

    result = index.getByValue("changed");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "changed");
}

TEST_CASE("Children are properly removed from parent indices", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode parent{"div", false};
    std::unique_ptr<Node> child = std::make_unique<GenericNode>("span", false, Attribute("class", "removable"));
    Node* childRef = parent.addChild(std::move(child));

    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&parent, "class");
    REQUIRE(index.getByValue("removable").size() == 1);

    parent.removeChild(childRef);
    REQUIRE(index.getByValue("removable").empty());
}

TEST_CASE("Children keep their own indices when removed", "[Index]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    std::unique_ptr<Node> child = std::make_unique<GenericNode>("span", false, Attribute("id", "child"));
    index::AttributeNameIndex childIndex = index::createIndex<index::AttributeNameIndex>(child.get(), "id");

    REQUIRE(childIndex.getByValue("child").size() == 1);

    GenericNode parent{"div", false};
    index::AttributeNameIndex parentIndex = index::createIndex<index::AttributeNameIndex>(&parent, "id");

    Node* childRef = parent.addChild(std::move(child));
    std::unique_ptr<Node> child2 = parent.removeChild(childRef);

    REQUIRE(child2);
    REQUIRE(childIndex.getByValue("child").size() == 1);
}

TEST_CASE("Index is created with createIndexPointer ", "[Index]" ) {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

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

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex* index = index::createIndexPointer<index::AttributeNameIndex>(&obj, "id");

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
    
    delete index;
}

TEST_CASE("Index is created with createIndexUniquePointer ", "[Index]" ) {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

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

    REQUIRE(obj.getChildrenCount() > 0);

    std::unique_ptr<index::AttributeNameIndex> index = index::createIndexUniquePointer<index::AttributeNameIndex>(&obj, "id");

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index is created with createIndexSharedPointer ", "[Index]" ) {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

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

    REQUIRE(obj.getChildrenCount() > 0);

    std::shared_ptr<index::AttributeNameIndex> index = index::createIndexSharedPointer<index::AttributeNameIndex>(&obj, "id");

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index is invalidated correctly", "[Index]" ) {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    std::shared_ptr<index::AttributeNameIndex> ptr;

    {
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

        ptr = index::createIndexSharedPointer<index::AttributeNameIndex>(&obj, "id");;
    }

    auto result = ptr->getByValue("3");

    CHECK(result.size() == 0);
    CHECK(!ptr->isValid());
}

TEST_CASE("Node operations work after an index is removed", "[Index]" ) {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

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

    {
        index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "id");
        auto result = index.getByValue("3");
    
        REQUIRE(result.size() == 1);
        CHECK(result[0]->getAttributeValue("id") == "3");
    }

    obj.setAttributeValue("theme", "light");

    REQUIRE(obj.operator[]("theme") == "light");
}

TEST_CASE("Indexing multiple occurrences of the same tag", "[TagNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, 
                GenericNode("div", false),
                GenericNode("span", false),
                GenericNode("div", false)),
            GenericNode("div", false))};
    
    index::TagNameIndex index = index::createIndex<index::TagNameIndex>(&obj, "div");
    auto result = index.get();
    REQUIRE(result.size() == 4);
}

TEST_CASE("Indexing nested elements with the same tag name", "[TagNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, 
                GenericNode("div", false, 
                    GenericNode("div", false, 
                        GenericNode("div", false)))))};
    
    index::TagNameIndex index = index::createIndex<index::TagNameIndex>(&obj, "div");
    auto result = index.get();
    REQUIRE(result.size() == 4);
}

TEST_CASE("Indexing multiple different tag names", "[TagNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("section", false, 
                GenericNode("article", false),
                GenericNode("div", false),
                GenericNode("article", false)),
            GenericNode("section", false))};
    
    index::TagNameIndex sectionIndex = index::createIndex<index::TagNameIndex>(&obj, "section");
    REQUIRE(sectionIndex.get().size() == 2);
    
    index::TagNameIndex articleIndex = index::createIndex<index::TagNameIndex>(&obj, "article");
    REQUIRE(articleIndex.get().size() == 2);
}

TEST_CASE("Indexing when no elements match", "[TagNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("header", false),
            GenericNode("footer", false))};
    
    index::TagNameIndex index = index::createIndex<index::TagNameIndex>(&obj, "nav");
    auto result = index.get();
    REQUIRE(result.empty());
}

TEST_CASE("Removing a child updates the index", "[TagNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("id", "1")),
            GenericNode("div", false, Attribute("id", "2")),
            GenericNode("div", false, Attribute("id", "3")))};
    
    index::TagNameIndex index = index::createIndex<index::TagNameIndex>(&obj, "div");
    REQUIRE(index.get().size() == 3);
    
    std::vector<Node*> toRemove = obj.getChildrenById("2");
    REQUIRE(toRemove.size() == 1);
    obj.removeChild(toRemove[0]);
    
    REQUIRE(index.get().size() == 2);
}

TEST_CASE("Indexing nested elements with multiple occurrences of the same tag", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, 
                GenericNode("div", false, 
                    GenericNode("div", false, 
                        GenericNode("div", false)))))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    auto result = index.getByTagName("div");
    REQUIRE(result.size() == 4);
    result = index.getByTagName("body");
    REQUIRE(result.size() == 1);
}

TEST_CASE("Indexing multiple different tag names in a tree", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("section", false, 
                GenericNode("article", false),
                GenericNode("div", false),
                GenericNode("article", false)),
            GenericNode("section", false))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    
    auto sectionResult = index.getByTagName("section");
    REQUIRE(sectionResult.size() == 2);
    
    auto articleResult = index.getByTagName("article");
    REQUIRE(articleResult.size() == 2);
    
    auto divResult = index.getByTagName("div");
    REQUIRE(divResult.size() == 1);
}

TEST_CASE("Indexing when no elements match", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("header", false),
            GenericNode("footer", false))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    auto result = index.getByTagName("nav");
    REQUIRE(result.empty());
}

TEST_CASE("Removing a child element updates the index", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("div", false, Attribute("id", "1")),
            GenericNode("div", false, Attribute("id", "2")),
            GenericNode("div", false, Attribute("id", "3")))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    REQUIRE(index.getByTagName("div").size() == 3);
    
    std::vector<Node*> toRemove = obj.getChildrenById("2");
    REQUIRE(toRemove.size() == 1);
    obj.removeChild(toRemove[0]);
    
    REQUIRE(index.getByTagName("div").size() == 2);
}

TEST_CASE("Indexing when no elements are present", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    EmptyNode obj{};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    auto result = index.getByTagName("div");
    REQUIRE(result.empty());
}

TEST_CASE("Indexing nested elements with different tag names", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        GenericNode("body", false, 
            GenericNode("header", false),
            GenericNode("section", false,
                GenericNode("article", false),
                GenericNode("footer", false)))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);

    auto headerResult = index.getByTagName("header");
    REQUIRE(headerResult.size() == 1);

    auto footerResult = index.getByTagName("footer");
    REQUIRE(footerResult.size() == 1);

    auto sectionResult = index.getByTagName("section");
    REQUIRE(sectionResult.size() == 1);

    auto articleResult = index.getByTagName("article");
    REQUIRE(articleResult.size() == 1);
}

TEST_CASE("CacheIndex works", "[CacheIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

    GenericNode obj{
        "html", false,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", false)
    };

    std::string expected = "<html lang=\"en\" theme=\"dark\">\n\t<head></head>\n</html>";

    index::CacheIndex index = index::createIndex<index::CacheIndex>(&obj);

    std::string serialized = index.cache(&GenericNode::serializePretty, "\t", true);

    CHECK(expected == serialized);

    CHECK(expected == index.getCached(&GenericNode::serializePretty, "\t", true));

    std::string serialized2 = index.cache(&GenericNode::serializePretty, "\t\t", true);
    
    CHECK(expected == index.getCached(&GenericNode::serializePretty, "\t", true));
    CHECK(expected != index.getCached(&GenericNode::serializePretty, "\t\t", true));

    auto head = obj.getChildrenByTagName("head");

    REQUIRE(head.size() == 1);
    obj.removeChild(head[0]);

    CHECK(!(index.isCached(&GenericNode::serializePretty, "\t", true)));
    CHECK(!(index.isCached(&GenericNode::serializePretty, "\t\t", true)));
    
    CHECK_THROWS(index.getCached(&GenericNode::serializePretty, "\t", true));
}

Templater::dynamic::dtags::GenericNode getComplexTree() {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;

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
                            Attribute("class", "item"),
                            GenericNode("a", false,
                                Attribute("href", "#home"),
                                Text("Home")
                            )
                        ),
                        GenericNode("li", false, 
                            Attribute("class", "item"),
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
                        GenericNode("li", false, Attribute("class", "item"), Text("Feature 1")),
                        GenericNode("li", false, Attribute("class", "item"), Text("Feature 2")),
                        GenericNode("li", false, Attribute("class", "item"), Text("Feature 3"))
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

    return obj;
}

TEST_CASE("AttributeNameIndex is faster than querying the tree", "[AttributeNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


    GenericNode obj = getComplexTree();

    REQUIRE(obj.getChildrenCount() > 0);
    
    index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto t1 = high_resolution_clock::now();
    auto result = index.getByValue("item");
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> timeIndex = t2 - t1;

    auto t21 = high_resolution_clock::now();
    auto result2 = obj.getChildrenByClassName("item");
    auto t22 = high_resolution_clock::now();

    REQUIRE(result2.size() == result.size());

    duration<double, std::milli> timeParse = t22 - t21;

    REQUIRE(timeIndex.count() < timeParse.count());
}


TEST_CASE("TagNameIndex is faster than querying the tree", "[TagNameIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    GenericNode obj = getComplexTree();

    REQUIRE(obj.getChildrenCount() > 0);
    
    index::TagNameIndex index = index::createIndex<index::TagNameIndex>(&obj, "p");

    auto t1 = high_resolution_clock::now();
    auto result = index.get();
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> timeIndex = t2 - t1;

    auto t21 = high_resolution_clock::now();
    auto result2 = obj.getChildrenByTagName("p");
    auto t22 = high_resolution_clock::now();

    REQUIRE(result2.size() == result.size());

    duration<double, std::milli> timeParse = t22 - t21;

    REQUIRE(timeIndex.count() < timeParse.count());
}


TEST_CASE("TagIndex is faster than querying the tree", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    GenericNode obj = getComplexTree();

    REQUIRE(obj.getChildrenCount() > 0);
    
    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);

    auto t1 = high_resolution_clock::now();
    auto result = index.getByTagName("p");
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> timeIndex = t2 - t1;

    auto t21 = high_resolution_clock::now();
    auto result2 = obj.getChildrenByTagName("p");
    auto t22 = high_resolution_clock::now();

    REQUIRE(result2.size() == result.size());

    duration<double, std::milli> timeParse = t22 - t21;

    REQUIRE(timeIndex.count() < timeParse.count());
}


TEST_CASE("CacheIndex is faster than querying the tree", "[TagIndex]") {
    using namespace Templater::dynamic::dtags;
    using namespace Templater::dynamic;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    GenericNode obj = getComplexTree();

    REQUIRE(obj.getChildrenCount() > 0);
    
    index::CacheIndex index = index::createIndex<index::CacheIndex>(&obj);

    std::string result = index.cache(&Node::serializePretty, "\t", true);

    auto t1 = high_resolution_clock::now();
    result = index.cache(&Node::serializePretty, "\t", true);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> timeIndex = t2 - t1;

    auto t21 = high_resolution_clock::now();
    std::string result2 = obj.serializePretty("\t", true);
    auto t22 = high_resolution_clock::now();

    REQUIRE(result2 == result);

    duration<double, std::milli> timeQuery = t22 - t21;

    REQUIRE(timeIndex.count() < timeQuery.count());
}
