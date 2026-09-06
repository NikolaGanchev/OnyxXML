#include <chrono>

#include "catch2/catch_all.hpp"
#include "onyx.h"

TEST_CASE("Index is added correctly", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid, Attribute("id", "0"),
                GenericNode("div", NonVoid, Attribute("id", "1"),
                            GenericNode("div", NonVoid, Attribute("id", "2"))),
                GenericNode("div", NonVoid, Attribute("id", "3")),
                GenericNode("div", NonVoid, Attribute("id", "4"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index handles multiple matches correctly", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid, Attribute("class", "container"),
                GenericNode("div", NonVoid, Attribute("class", "item")),
                GenericNode("div", NonVoid, Attribute("class", "item")),
                GenericNode("div", NonVoid, Attribute("class", "item"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto result = index.getByValue("item");

    REQUIRE(result.size() == 3);
    for (const auto& elem : result) {
        CHECK(elem->getAttributeValue("class") == "item");
    }
}

TEST_CASE("Index returns empty when no match found", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"html", NonVoid,
                    GenericNode("body", NonVoid,
                                GenericNode("div", NonVoid,
                                            Attribute("class", "container")))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto result = index.getByValue("nonexistent");
    REQUIRE(result.empty());
}

TEST_CASE("Index works with nested attributes", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode(
            "body", NonVoid,
            GenericNode("section", NonVoid, Attribute("data-type", "main"),
                        GenericNode("div", NonVoid,
                                    Attribute("data-type", "nested"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "data-type");

    auto result = index.getByValue("nested");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("data-type") == "nested");
}

TEST_CASE("Index updates correctly when attributes change", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"div", NonVoid, Attribute("id", "test")};
    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "id");

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
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"div", NonVoid};
    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");

    auto result = index.getByValue("new-class");
    REQUIRE(result.empty());

    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "span", NonVoid, Attribute("class", "new-class"));
    obj.addChild(std::move(child));

    result = index.getByValue("new-class");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("class") == "new-class");
}

TEST_CASE("Index updates correctly when children are added using move",
          "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"div", NonVoid};
    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "class");
    auto result = index.getByValue("new-class");
    REQUIRE(result.empty());

    obj.addChild(GenericNode("span", NonVoid, Attribute("class", "new-class")));

    result = index.getByValue("new-class");
    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("class") == "new-class");
}

TEST_CASE(
    "Index updates correctly when attributes are modified using operator []",
    "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"div", NonVoid};
    obj["id"] = "original";

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "id");

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
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode parent{"div", NonVoid};
    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "span", NonVoid, Attribute("class", "removable"));
    Node* childRef = parent.addChild(std::move(child));

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&parent, "class");
    REQUIRE(index.getByValue("removable").size() == 1);

    parent.removeChild(childRef);
    REQUIRE(index.getByValue("removable").empty());
}

TEST_CASE("Children keep their own indices when removed", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "span", NonVoid, Attribute("id", "child"));
    index::AttributeNameIndex childIndex =
        index::createIndex<index::AttributeNameIndex>(child.get(), "id");

    REQUIRE(childIndex.getByValue("child").size() == 1);

    GenericNode parent{"div", NonVoid};
    index::AttributeNameIndex parentIndex =
        index::createIndex<index::AttributeNameIndex>(&parent, "id");

    Node* childRef = parent.addChild(std::move(child));
    NodeHandle child2 = parent.removeChild(childRef);

    REQUIRE(child2);
    REQUIRE(childIndex.getByValue("child").size() == 1);
}

TEST_CASE("Index is created with createIndexPointer ", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid, Attribute("id", "0"),
                GenericNode("div", NonVoid, Attribute("id", "1"),
                            GenericNode("div", NonVoid, Attribute("id", "2"))),
                GenericNode("div", NonVoid, Attribute("id", "3")),
                GenericNode("div", NonVoid, Attribute("id", "4"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex* index =
        index::createIndexPointer<index::AttributeNameIndex>(&obj, "id");

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");

    delete index;
}

TEST_CASE("Index is created with createIndexUniquePointer ", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid, Attribute("id", "0"),
                GenericNode("div", NonVoid, Attribute("id", "1"),
                            GenericNode("div", NonVoid, Attribute("id", "2"))),
                GenericNode("div", NonVoid, Attribute("id", "3")),
                GenericNode("div", NonVoid, Attribute("id", "4"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    std::unique_ptr<index::AttributeNameIndex> index =
        index::createIndexUniquePointer<index::AttributeNameIndex>(&obj, "id");

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index is created with createIndexSharedPointer ", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid, Attribute("id", "0"),
                GenericNode("div", NonVoid, Attribute("id", "1"),
                            GenericNode("div", NonVoid, Attribute("id", "2"))),
                GenericNode("div", NonVoid, Attribute("id", "3")),
                GenericNode("div", NonVoid, Attribute("id", "4"))))};

    REQUIRE(obj.getChildrenCount() > 0);

    std::shared_ptr<index::AttributeNameIndex> index =
        index::createIndexSharedPointer<index::AttributeNameIndex>(&obj, "id");

    auto result = index->getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");
}

TEST_CASE("Index is invalidated correctly", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    std::shared_ptr<index::AttributeNameIndex> ptr;

    {
        GenericNode obj{
            "html",
            NonVoid,
            Attribute("lang", "en"),
            Attribute("theme", "dark"),
            GenericNode("head", NonVoid),
            GenericNode(
                "body", NonVoid,
                GenericNode(
                    "div", NonVoid, Attribute("id", "0"),
                    GenericNode(
                        "div", NonVoid, Attribute("id", "1"),
                        GenericNode("div", NonVoid, Attribute("id", "2"))),
                    GenericNode("div", NonVoid, Attribute("id", "3")),
                    GenericNode("div", NonVoid, Attribute("id", "4"))))};

        ptr = index::createIndexSharedPointer<index::AttributeNameIndex>(&obj,
                                                                         "id");
    }

    auto result = ptr->getByValue("3");

    CHECK(result.size() == 0);
    CHECK(!ptr->isValid());
}

TEST_CASE("Node operations work after an index is removed", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid, Attribute("id", "0"),
                GenericNode("div", NonVoid, Attribute("id", "1"),
                            GenericNode("div", NonVoid, Attribute("id", "2"))),
                GenericNode("div", NonVoid, Attribute("id", "3")),
                GenericNode("div", NonVoid, Attribute("id", "4"))))};

    {
        index::AttributeNameIndex index =
            index::createIndex<index::AttributeNameIndex>(&obj, "id");
        auto result = index.getByValue("3");

        REQUIRE(result.size() == 1);
        CHECK(result[0]->getAttributeValue("id") == "3");
    }

    obj.setAttributeValue("theme", "light");

    REQUIRE(obj.operator[]("theme") == "light");
}

TEST_CASE("Indexing multiple occurrences of the same tag", "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode("body", NonVoid,
                    GenericNode("div", NonVoid, GenericNode("div", NonVoid),
                                GenericNode("span", NonVoid),
                                GenericNode("div", NonVoid)),
                    GenericNode("div", NonVoid))};

    index::TagNameIndex index =
        index::createIndex<index::TagNameIndex>(&obj, "div");
    auto result = index.get();
    REQUIRE(result.size() == 4);
}

TEST_CASE("Indexing nested elements with the same tag name", "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid,
                GenericNode("div", NonVoid,
                            GenericNode("div", NonVoid,
                                        GenericNode("div", NonVoid)))))};

    index::TagNameIndex index =
        index::createIndex<index::TagNameIndex>(&obj, "div");
    auto result = index.get();
    REQUIRE(result.size() == 4);
}

TEST_CASE("Indexing multiple different tag names", "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"html", NonVoid,
                    GenericNode("body", NonVoid,
                                GenericNode("section", NonVoid,
                                            GenericNode("article", NonVoid),
                                            GenericNode("div", NonVoid),
                                            GenericNode("article", NonVoid)),
                                GenericNode("section", NonVoid))};

    index::TagNameIndex sectionIndex =
        index::createIndex<index::TagNameIndex>(&obj, "section");
    REQUIRE(sectionIndex.get().size() == 2);

    index::TagNameIndex articleIndex =
        index::createIndex<index::TagNameIndex>(&obj, "article");
    REQUIRE(articleIndex.get().size() == 2);
}

TEST_CASE("Indexing when no elements match", "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"html", NonVoid,
                    GenericNode("body", NonVoid, GenericNode("header", NonVoid),
                                GenericNode("footer", NonVoid))};

    index::TagNameIndex index =
        index::createIndex<index::TagNameIndex>(&obj, "nav");
    auto result = index.get();
    REQUIRE(result.empty());
}

TEST_CASE("Removing a child updates the index", "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode("body", NonVoid,
                    GenericNode("div", NonVoid, Attribute("id", "1")),
                    GenericNode("div", NonVoid, Attribute("id", "2")),
                    GenericNode("div", NonVoid, Attribute("id", "3")))};

    index::TagNameIndex index =
        index::createIndex<index::TagNameIndex>(&obj, "div");
    REQUIRE(index.get().size() == 3);

    std::vector<Node*> toRemove = obj.getChildrenById("2");
    REQUIRE(toRemove.size() == 1);
    obj.removeChild(toRemove[0]);

    REQUIRE(index.get().size() == 2);
}

TEST_CASE("Indexing nested elements with multiple occurrences of the same tag",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode(
            "body", NonVoid,
            GenericNode(
                "div", NonVoid,
                GenericNode("div", NonVoid,
                            GenericNode("div", NonVoid,
                                        GenericNode("div", NonVoid)))))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    auto result = index.getByTagName("div");
    REQUIRE(result.size() == 4);
    result = index.getByTagName("body");
    REQUIRE(result.size() == 1);
}

TEST_CASE("Indexing multiple different tag names in a tree", "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"html", NonVoid,
                    GenericNode("body", NonVoid,
                                GenericNode("section", NonVoid,
                                            GenericNode("article", NonVoid),
                                            GenericNode("div", NonVoid),
                                            GenericNode("article", NonVoid)),
                                GenericNode("section", NonVoid))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);

    auto sectionResult = index.getByTagName("section");
    REQUIRE(sectionResult.size() == 2);

    auto articleResult = index.getByTagName("article");
    REQUIRE(articleResult.size() == 2);

    auto divResult = index.getByTagName("div");
    REQUIRE(divResult.size() == 1);
}

TEST_CASE("Indexing when no elements match", "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"html", NonVoid,
                    GenericNode("body", NonVoid, GenericNode("header", NonVoid),
                                GenericNode("footer", NonVoid))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    auto result = index.getByTagName("nav");
    REQUIRE(result.empty());
}

TEST_CASE("Removing a child element updates the index", "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid,
        GenericNode("body", NonVoid,
                    GenericNode("div", NonVoid, Attribute("id", "1")),
                    GenericNode("div", NonVoid, Attribute("id", "2")),
                    GenericNode("div", NonVoid, Attribute("id", "3")))};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    REQUIRE(index.getByTagName("div").size() == 3);

    std::vector<Node*> toRemove = obj.getChildrenById("2");
    REQUIRE(toRemove.size() == 1);
    obj.removeChild(toRemove[0]);

    REQUIRE(index.getByTagName("div").size() == 2);
}

TEST_CASE("Indexing when no elements are present", "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    EmptyNode obj{};

    index::TagIndex index = index::createIndex<index::TagIndex>(&obj);
    auto result = index.getByTagName("div");
    REQUIRE(result.empty());
}

TEST_CASE("Indexing nested elements with different tag names", "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{"html", NonVoid,
                    GenericNode("body", NonVoid, GenericNode("header", NonVoid),
                                GenericNode("section", NonVoid,
                                            GenericNode("article", NonVoid),
                                            GenericNode("footer", NonVoid)))};

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

onyx::dynamic::tags::GenericNode getComplexTree() {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),

        GenericNode(
            "head", NonVoid,
            GenericNode("meta", Void, Attribute("charset", "UTF-8")),
            GenericNode(
                "meta", Void, Attribute("name", "viewport"),
                Attribute("content", "width=device-width, initial-scale=1.0")),
            GenericNode("title", NonVoid, Text("Complex Test Page")),
            GenericNode("link", Void, Attribute("rel", "stylesheet"),
                        Attribute("href", "/styles/main.css"))),

        GenericNode(
            "body", NonVoid,
            GenericNode(
                "header", NonVoid,
                GenericNode(
                    "nav", NonVoid,
                    GenericNode(
                        "ul", NonVoid,
                        GenericNode("li", NonVoid, Attribute("class", "item"),
                                    GenericNode("a", NonVoid,
                                                Attribute("href", "#home"),
                                                Text("Home"))),
                        GenericNode("li", NonVoid, Attribute("class", "item"),
                                    GenericNode("a", NonVoid,
                                                Attribute("href", "#about"),
                                                Text("About Us")))))),

            GenericNode(
                "main", NonVoid,
                GenericNode(
                    "section", NonVoid, Attribute("id", "introduction"),
                    GenericNode("h1", NonVoid, Text("Introduction")),
                    GenericNode("p", NonVoid,
                                Text("Welcome to the complex HTML structure "
                                     "test case.")),
                    GenericNode("p", NonVoid,
                                Text("This test includes various nested "
                                     "elements, attributes, and content.")),
                    GenericNode(
                        "form", NonVoid, Attribute("name", "contact-form"),
                        GenericNode("label", NonVoid, Attribute("for", "name"),
                                    Text("Your Name:")),
                        GenericNode("input", Void, Attribute("type", "text"),
                                    Attribute("id", "name"),
                                    Attribute("name", "name")),
                        GenericNode("label", NonVoid, Attribute("for", "email"),
                                    Text("Your Email:")),
                        GenericNode("input", Void, Attribute("type", "email"),
                                    Attribute("id", "email"),
                                    Attribute("name", "email")),
                        GenericNode("button", NonVoid,
                                    Attribute("type", "submit"),
                                    Text("Submit")))),

                GenericNode(
                    "section", NonVoid, Attribute("id", "features"),
                    GenericNode("h2", NonVoid, Text("Features")),
                    GenericNode(
                        "ul", NonVoid,
                        GenericNode("li", NonVoid, Attribute("class", "item"),
                                    Text("Feature 1")),
                        GenericNode("li", NonVoid, Attribute("class", "item"),
                                    Text("Feature 2")),
                        GenericNode("li", NonVoid, Attribute("class", "item"),
                                    Text("Feature 3"))),
                    GenericNode("p", NonVoid,
                                Text("These are the key features of the "
                                     "application.")))),

            GenericNode(
                "footer", NonVoid,
                GenericNode("p", NonVoid,
                            Text("© 2025 Complex HTML Test Page")),
                GenericNode("a", NonVoid,
                            Attribute("href", "https://www.example.com"),
                            Text("Privacy Policy"))))};

    return obj;
}

TEST_CASE("Index move constructor works", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode("body", NonVoid,
                    GenericNode("div", NonVoid, Attribute("id", "0")),
                    GenericNode("div", NonVoid, Attribute("id", "3")),
                    GenericNode("div", NonVoid, Attribute("id", "4")))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");

    index::AttributeNameIndex index2{std::move(index)};

    REQUIRE(!index.isValid());

    auto result2 = index2.getByValue("3");

    REQUIRE(result2.size() == 1);

    obj.addChild(GenericNode("div", NonVoid, Attribute("id", "3")));

    auto result3 = index2.getByValue("3");

    REQUIRE(result3.size() == 2);
}

TEST_CASE("Index move assignment operator works", "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html", NonVoid, Attribute("lang", "en"), Attribute("theme", "dark"),
        GenericNode("body", NonVoid,
                    GenericNode("div", NonVoid, Attribute("id", "0")),
                    GenericNode("div", NonVoid, Attribute("id", "3")),
                    GenericNode("div", NonVoid, Attribute("id", "4")))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");

    index::AttributeNameIndex index2 = std::move(index);

    REQUIRE(!index.isValid());

    auto result2 = index2.getByValue("3");

    REQUIRE(result2.size() == 1);

    obj.addChild(GenericNode("div", NonVoid, Attribute("id", "3")));

    auto result3 = index2.getByValue("3");

    REQUIRE(result3.size() == 2);
}

TEST_CASE("Index move assignment operator cleans up memory properly",
          "[Index]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    GenericNode obj{
        "html",
        NonVoid,
        Attribute("lang", "en"),
        Attribute("theme", "dark"),
        GenericNode("head", NonVoid),
        GenericNode("body", NonVoid,
                    GenericNode("div", NonVoid, Attribute("class", "0")),
                    GenericNode("div", NonVoid, Attribute("id", "3")),
                    GenericNode("div", NonVoid, Attribute("id", "4")))};

    REQUIRE(obj.getChildrenCount() > 0);

    index::AttributeNameIndex index =
        index::createIndex<index::AttributeNameIndex>(&obj, "id");

    auto result = index.getByValue("3");

    REQUIRE(result.size() == 1);
    CHECK(result[0]->getAttributeValue("id") == "3");

    {
        index::AttributeNameIndex indexClass =
            index::createIndex<index::AttributeNameIndex>(&obj, "class");
        auto resultClass = indexClass.getByValue("0");
        REQUIRE(resultClass.size() == 1);
        CHECK(resultClass[0]->getAttributeValue("class") == "0");
        index = std::move(indexClass);
    }

    auto result2 = index.getByValue("0");
    REQUIRE(result2.size() == 1);
    CHECK(result2[0]->getAttributeValue("class") == "0");

    obj.addChild(GenericNode("div", NonVoid, Attribute("class", "0")));
    obj.addChild(GenericNode("div", NonVoid, Attribute("id", "3")));

    auto result3 = index.getByValue("0");

    REQUIRE(result3.size() == 2);
    CHECK(result3[0]->getAttributeValue("class") == "0");
}

TEST_CASE(
    "AttributeNameIndex unprefixed index strictly matches attributes with no "
    "namespace",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/nsA";
    const std::string nsB = "http://example.com/nsB";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("node", NonVoid, Attribute("id", "target")),
                    GenericNode("node", NonVoid, Attribute("a:id", "target")),
                    GenericNode("node", NonVoid, Attribute("b:id", "target")),
                    GenericNode("node", NonVoid, Attribute("id", "other"))};

    auto index = index::createIndex<index::AttributeNameIndex>(&doc, "id");
    auto results = index.getByValue("target");

    REQUIRE(results.size() == 1);
    CHECK(results[0]->hasAttribute("id"));
    CHECK(!results[0]->hasAttribute("a:id"));
}

TEST_CASE(
    "AttributeNameIndex prefixed URI index matches only attributes mapped to "
    "that specific URI",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/nsA";
    const std::string nsB = "http://example.com/nsB";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("node", NonVoid, Attribute("id", "target")),
                    GenericNode("node", NonVoid, Attribute("a:id", "target")),
                    GenericNode("node", NonVoid, Attribute("b:id", "target")),
                    GenericNode("node", NonVoid, Attribute("id", "other"))};

    auto indexA =
        index::createIndex<index::AttributeNameIndex>(&doc, nsA, "id");
    auto resultsA = indexA.getByValue("target");

    REQUIRE(resultsA.size() == 1);
    CHECK(resultsA[0]->hasAttribute("a:id"));

    auto indexB =
        index::createIndex<index::AttributeNameIndex>(&doc, nsB, "id");
    auto resultsB = indexB.getByValue("target");

    REQUIRE(resultsB.size() == 1);
    CHECK(resultsB[0]->hasAttribute("b:id"));
}

TEST_CASE(
    "AttributeNameIndex AnyNamespace index matches all attributes across all "
    "namespaces",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/nsA";
    const std::string nsB = "http://example.com/nsB";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("node", NonVoid, Attribute("id", "target")),
                    GenericNode("node", NonVoid, Attribute("a:id", "target")),
                    GenericNode("node", NonVoid, Attribute("b:id", "target")),
                    GenericNode("node", NonVoid, Attribute("id", "other"))};

    auto indexAny = index::createIndex<index::AttributeNameIndex>(
        &doc, index::AnyNamespace, "id");
    auto results = indexAny.getByValue("target");

    REQUIRE(results.size() == 3);
}

TEST_CASE(
    "AttributeNameIndex indexes default xmlns declarations as unprefixed "
    "attributes",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string defaultNs = "http://example.com/default";

    GenericNode doc{
        "root", NonVoid, Attribute("xmlns", defaultNs),
        GenericNode("child", NonVoid, Attribute("xmlns", defaultNs)),
        GenericNode("other", NonVoid,
                    Attribute("xmlns", "http://example.com/other"))};

    auto index = index::createIndex<index::AttributeNameIndex>(&doc, "xmlns");
    auto results = index.getByValue(defaultNs);

    REQUIRE(results.size() == 2);
    CHECK(results[0]->getAttributeValue("xmlns") == defaultNs);
    CHECK(results[1]->getAttributeValue("xmlns") == defaultNs);
}

TEST_CASE(
    "AttributeNameIndex indexes prefixed xmlns declarations by local prefix "
    "name",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string targetNs = "http://example.com/custom";

    GenericNode doc{
        "root", NonVoid, Attribute("xmlns:custom", targetNs),
        GenericNode("child1", NonVoid, Attribute("xmlns:custom", targetNs)),
        GenericNode("child2", NonVoid, Attribute("xmlns:other", targetNs))};

    auto indexByUri = index::createIndex<index::AttributeNameIndex>(
        &doc, "http://www.w3.org/2000/xmlns/", "custom");
    auto uriResults = indexByUri.getByValue(targetNs);

    REQUIRE(uriResults.size() == 2);

    auto indexAny = index::createIndex<index::AttributeNameIndex>(
        &doc, index::AnyNamespace, "custom");
    auto anyResults = indexAny.getByValue(targetNs);

    REQUIRE(anyResults.size() == 2);
}

TEST_CASE("AttributeNameIndex resolves inherited and shadowed prefix bindings",
          "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsV1 = "http://example.com/v1";
    const std::string nsV2 = "http://example.com/v2";

    GenericNode doc{
        "root", NonVoid, Attribute("xmlns:p", nsV1),
        GenericNode("parent", NonVoid,
                    GenericNode("child1", NonVoid, Attribute("p:tag", "found")),
                    GenericNode("child2", NonVoid, Attribute("xmlns:p", nsV2),
                                Attribute("p:tag", "found")))};

    auto indexV1 =
        index::createIndex<index::AttributeNameIndex>(&doc, nsV1, "tag");
    auto resultsV1 = indexV1.getByValue("found");
    REQUIRE(resultsV1.size() == 1);
    CHECK(resultsV1[0]->getTagName() == "child1");

    auto indexV2 =
        index::createIndex<index::AttributeNameIndex>(&doc, nsV2, "tag");
    auto resultsV2 = indexV2.getByValue("found");
    REQUIRE(resultsV2.size() == 1);
    CHECK(resultsV2[0]->getTagName() == "child2");
}

TEST_CASE(
    "AttributeNameIndex updates correctly when namespaced attributes mutate",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns)};
    auto index = index::createIndex<index::AttributeNameIndex>(&doc, ns, "key");

    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "item", NonVoid, Attribute("p:key", "initial"));
    Node* childRef = doc.addChild(std::move(child));

    REQUIRE(index.getByValue("initial").size() == 1);

    childRef->setAttributeValue("p:key", "updated");
    REQUIRE(index.getByValue("initial").empty());
    REQUIRE(index.getByValue("updated").size() == 1);

    childRef->removeAttribute("p:key");
    REQUIRE(index.getByValue("updated").empty());
}

TEST_CASE("AttributeNameIndex updates when namespace prefix bindings change",
          "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsOld = "http://example.com/old";
    const std::string nsNew = "http://example.com/new";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", nsOld)};
    std::unique_ptr<Node> child = std::make_unique<GenericNode>(
        "item", NonVoid, Attribute("p:attr", "val"));
    Node* childRef = doc.addChild(std::move(child));

    auto indexOld =
        index::createIndex<index::AttributeNameIndex>(&doc, nsOld, "attr");
    auto indexNew =
        index::createIndex<index::AttributeNameIndex>(&doc, nsNew, "attr");

    REQUIRE(indexOld.getByValue("val").size() == 1);
    REQUIRE(indexNew.getByValue("val").empty());

    doc.setAttributeValue("xmlns:p", nsNew);

    REQUIRE(indexOld.getByValue("val").empty());
    REQUIRE(indexNew.getByValue("val").size() == 1);
}

TEST_CASE(
    "AttributeNameIndex move constructor preserves URI and AnyNamespace "
    "configurations",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:a", ns),
                    GenericNode("node", NonVoid, Attribute("a:code", "alpha"))};

    auto indexURI =
        index::createIndex<index::AttributeNameIndex>(&doc, ns, "code");
    auto indexAny = index::createIndex<index::AttributeNameIndex>(
        &doc, index::AnyNamespace, "code");

    index::AttributeNameIndex movedURI{std::move(indexURI)};
    index::AttributeNameIndex movedAny{std::move(indexAny)};

    REQUIRE(!indexURI.isValid());
    REQUIRE(!indexAny.isValid());

    REQUIRE(movedURI.getByValue("alpha").size() == 1);
    REQUIRE(movedAny.getByValue("alpha").size() == 1);

    doc.addChild(GenericNode("node", NonVoid, Attribute("a:code", "beta")));

    REQUIRE(movedURI.getByValue("beta").size() == 1);
    REQUIRE(movedAny.getByValue("beta").size() == 1);
}

TEST_CASE(
    "AttributeNameIndex move assignment preserves URI behaviors and updates "
    "cleanly",
    "[AttributeNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:a", ns),
                    GenericNode("node", NonVoid, Attribute("a:code", "alpha"))};

    auto indexURI =
        index::createIndex<index::AttributeNameIndex>(&doc, ns, "code");
    auto indexUnprefixed =
        index::createIndex<index::AttributeNameIndex>(&doc, "code");

    indexUnprefixed = std::move(indexURI);
    REQUIRE(!indexURI.isValid());
    REQUIRE(indexUnprefixed.getByValue("alpha").size() == 1);

    doc.addChild(GenericNode("node", NonVoid, Attribute("a:code", "alpha")));
    REQUIRE(indexUnprefixed.getByValue("alpha").size() == 2);
}

TEST_CASE(
    "TagNameIndex unprefixed index strictly matches tags with no namespace",
    "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:p", ns),
                    GenericNode("item", NonVoid),
                    GenericNode("p:item", NonVoid),
                    GenericNode("item", NonVoid),
                    GenericNode("other", NonVoid)};

    auto index = index::createIndex<index::TagNameIndex>(&doc, "item");
    auto results = index.get();

    REQUIRE(results.size() == 2);
    for (auto* node : results) {
        CHECK(node->getTagName() == "item");
        auto elemNs = node->getNamespaceName();
        CHECK((!elemNs.has_value() || elemNs->empty()));
    }
}

TEST_CASE(
    "TagNameIndex prefixed URI index matches only tags mapped to specific "
    "namespace",
    "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/a";
    const std::string nsB = "http://example.com/b";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("item", NonVoid),
                    GenericNode("a:item", NonVoid),
                    GenericNode("a:item", NonVoid),
                    GenericNode("b:item", NonVoid)};

    auto indexA = index::createIndex<index::TagNameIndex>(&doc, nsA, "item");
    auto resultsA = indexA.get();
    REQUIRE(resultsA.size() == 2);
    for (auto* node : resultsA) {
        CHECK(node->getTagName() == "item");
        CHECK(node->getNamespaceName() == nsA);
    }

    auto indexB = index::createIndex<index::TagNameIndex>(&doc, nsB, "item");
    auto resultsB = indexB.get();
    REQUIRE(resultsB.size() == 1);
    CHECK(resultsB[0]->getTagName() == "item");
    CHECK(resultsB[0]->getNamespaceName() == nsB);
}

TEST_CASE(
    "TagNameIndex AnyNamespace index matches tags regardless of namespace "
    "binding",
    "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/a";
    const std::string nsB = "http://example.com/b";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("item", NonVoid),
                    GenericNode("a:item", NonVoid),
                    GenericNode("b:item", NonVoid),
                    GenericNode("other", NonVoid)};

    auto indexAny = index::createIndex<index::TagNameIndex>(
        &doc, index::AnyNamespaceTag{}, "item");
    auto results = indexAny.get();

    REQUIRE(results.size() == 3);
}

TEST_CASE("TagNameIndex matches tags inheriting default namespaces",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string defaultNs = "http://example.com/default";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns", defaultNs),
                    GenericNode("item", NonVoid),
                    GenericNode("item", NonVoid),
                    GenericNode("container", NonVoid, Attribute("xmlns", ""),
                                GenericNode("item", NonVoid))};

    auto indexDefaultNs =
        index::createIndex<index::TagNameIndex>(&doc, defaultNs, "item");
    auto defaultResults = indexDefaultNs.get();
    REQUIRE(defaultResults.size() == 2);

    auto indexNoNs = index::createIndex<index::TagNameIndex>(&doc, "item");
    auto noNsResults = indexNoNs.get();
    REQUIRE(noNsResults.size() == 1);
}

TEST_CASE("TagNameIndex resolves shadowed namespace bindings correctly",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsV1 = "http://example.com/v1";
    const std::string nsV2 = "http://example.com/v2";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", nsV1),
                    GenericNode("p:entry", NonVoid),
                    GenericNode("sub", NonVoid, Attribute("xmlns:p", nsV2),
                                GenericNode("p:entry", NonVoid))};

    auto indexV1 = index::createIndex<index::TagNameIndex>(&doc, nsV1, "entry");
    auto resultsV1 = indexV1.get();
    REQUIRE(resultsV1.size() == 1);
    CHECK(resultsV1[0]->getNamespaceName() == nsV1);

    auto indexV2 = index::createIndex<index::TagNameIndex>(&doc, nsV2, "entry");
    auto resultsV2 = indexV2.get();
    REQUIRE(resultsV2.size() == 1);
    CHECK(resultsV2[0]->getNamespaceName() == nsV2);
}

TEST_CASE("TagNameIndex updates correctly when setTagName is invoked",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns)};
    auto index = index::createIndex<index::TagNameIndex>(&doc, ns, "target");

    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("p:initial", NonVoid);
    GenericNode* childPtr = child.get();
    doc.addChild(std::move(child));

    REQUIRE(index.get().empty());

    childPtr->setTagName("target");
    REQUIRE(index.get().size() == 1);
    CHECK(index.get()[0] == childPtr);

    childPtr->setTagName("renamed");
    REQUIRE(index.get().empty());
}

TEST_CASE("TagNameIndex updates correctly when setNamespacePrefix is invoked",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/a";
    const std::string nsB = "http://example.com/b";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB)};

    auto indexA = index::createIndex<index::TagNameIndex>(&doc, nsA, "node");
    auto indexB = index::createIndex<index::TagNameIndex>(&doc, nsB, "node");
    auto indexNoNs = index::createIndex<index::TagNameIndex>(&doc, "node");

    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("node", NonVoid);
    GenericNode* childPtr = child.get();
    doc.addChild(std::move(child));

    REQUIRE(indexNoNs.get().size() == 1);
    REQUIRE(indexA.get().empty());
    REQUIRE(indexB.get().empty());

    childPtr->setNamespacePrefix("a");
    REQUIRE(indexNoNs.get().empty());
    REQUIRE(indexA.get().size() == 1);
    REQUIRE(indexB.get().empty());

    childPtr->setNamespacePrefix("b");
    REQUIRE(indexNoNs.get().empty());
    REQUIRE(indexA.get().empty());
    REQUIRE(indexB.get().size() == 1);

    childPtr->setNamespacePrefix("");
    REQUIRE(indexNoNs.get().size() == 1);
    REQUIRE(indexA.get().empty());
    REQUIRE(indexB.get().empty());
}

TEST_CASE("TagNameIndex updates when ancestor namespace bindings mutate",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsOld = "http://example.com/old";
    const std::string nsNew = "http://example.com/new";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", nsOld)};
    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("p:item", NonVoid);
    doc.addChild(std::move(child));

    auto indexOld =
        index::createIndex<index::TagNameIndex>(&doc, nsOld, "item");
    auto indexNew =
        index::createIndex<index::TagNameIndex>(&doc, nsNew, "item");

    REQUIRE(indexOld.get().size() == 1);
    REQUIRE(indexNew.get().empty());

    doc.setAttributeValue("xmlns:p", nsNew);

    REQUIRE(indexOld.get().empty());
    REQUIRE(indexNew.get().size() == 1);
}

TEST_CASE("TagNameIndex updates when nodes are removed or added dynamically",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns)};
    auto index = index::createIndex<index::TagNameIndex>(&doc, ns, "elem");

    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("p:elem", NonVoid);
    Node* childRef = doc.addChild(std::move(child));

    REQUIRE(index.get().size() == 1);

    doc.removeChild(childRef);
    REQUIRE(index.get().empty());

    doc.addChild(GenericNode("p:elem", NonVoid));
    REQUIRE(index.get().size() == 1);
}

TEST_CASE("TagNameIndex move constructor preserves namespace configuration",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns),
                    GenericNode("p:item", NonVoid)};

    auto indexNs = index::createIndex<index::TagNameIndex>(&doc, ns, "item");
    auto indexAny = index::createIndex<index::TagNameIndex>(
        &doc, index::AnyNamespaceTag{}, "item");

    index::TagNameIndex movedNs{std::move(indexNs)};
    index::TagNameIndex movedAny{std::move(indexAny)};

    REQUIRE(!indexNs.isValid());
    REQUIRE(!indexAny.isValid());

    REQUIRE(movedNs.get().size() == 1);
    REQUIRE(movedAny.get().size() == 1);

    doc.addChild(GenericNode("p:item", NonVoid));

    REQUIRE(movedNs.get().size() == 2);
    REQUIRE(movedAny.get().size() == 2);
}

TEST_CASE("TagNameIndex move assignment updates internal index state properly",
          "[TagNameIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns),
                    GenericNode("p:item", NonVoid)};

    auto indexNs = index::createIndex<index::TagNameIndex>(&doc, ns, "item");
    auto indexNoNs = index::createIndex<index::TagNameIndex>(&doc, "item");

    indexNoNs = std::move(indexNs);
    REQUIRE(!indexNs.isValid());
    REQUIRE(indexNoNs.get().size() == 1);

    doc.addChild(GenericNode("p:item", NonVoid));
    REQUIRE(indexNoNs.get().size() == 2);
}

TEST_CASE(
    "TagIndex unprefixed getByTagName strictly matches tags with no namespace",
    "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:p", ns),
                    GenericNode("item", NonVoid),
                    GenericNode("p:item", NonVoid),
                    GenericNode("item", NonVoid),
                    GenericNode("other", NonVoid)};

    auto index = index::createIndex<index::TagIndex>(&doc);
    auto results = index.getByTagName("item");

    REQUIRE(results.size() == 2);
    for (auto* node : results) {
        CHECK(node->getTagName() == "item");
        auto nodeNs = node->getNamespaceName();
        CHECK((!nodeNs.has_value() || nodeNs->empty()));
    }
}

TEST_CASE("TagIndex expanded name lookup matches exact URI and local name",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/a";
    const std::string nsB = "http://example.com/b";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("item", NonVoid),
                    GenericNode("a:item", NonVoid),
                    GenericNode("a:item", NonVoid),
                    GenericNode("b:item", NonVoid)};

    auto index = index::createIndex<index::TagIndex>(&doc);

    auto resultsA = index.getByTagName(nsA, "item");
    REQUIRE(resultsA.size() == 2);
    for (auto* node : resultsA) {
        CHECK(node->getTagName() == "item");
        CHECK(node->getNamespaceName() == nsA);
    }

    auto resultsB = index.getByTagName(nsB, "item");
    REQUIRE(resultsB.size() == 1);
    CHECK(resultsB[0]->getTagName() == "item");
    CHECK(resultsB[0]->getNamespaceName() == nsB);

    auto resultsUnprefixed = index.getByTagName("", "item");
    REQUIRE(resultsUnprefixed.size() == 1);
}

TEST_CASE("TagIndex getByLocalName matches across all namespaces",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/a";
    const std::string nsB = "http://example.com/b";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB),
                    GenericNode("item", NonVoid),
                    GenericNode("a:item", NonVoid),
                    GenericNode("b:item", NonVoid),
                    GenericNode("other", NonVoid)};

    auto index = index::createIndex<index::TagIndex>(&doc);
    auto results = index.getByLocalName("item");

    REQUIRE(results.size() == 3);
}

TEST_CASE(
    "TagIndex getByNamespace returns all elements belonging to target URI",
    "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/suite";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns:p", ns),
                    GenericNode("p:header", NonVoid),
                    GenericNode("p:body", NonVoid),
                    GenericNode("p:item", NonVoid),
                    GenericNode("footer", NonVoid)};

    auto index = index::createIndex<index::TagIndex>(&doc);
    auto results = index.getByNamespace(ns);

    REQUIRE(results.size() == 3);
    for (auto* node : results) {
        CHECK(node->getNamespaceName() == ns);
    }
}

TEST_CASE("TagIndex resolves elements inheriting default namespaces",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string defaultNs = "http://example.com/default";

    GenericNode doc{"root",
                    NonVoid,
                    Attribute("xmlns", defaultNs),
                    GenericNode("item", NonVoid),
                    GenericNode("item", NonVoid),
                    GenericNode("container", NonVoid, Attribute("xmlns", ""),
                                GenericNode("item", NonVoid))};

    auto index = index::createIndex<index::TagIndex>(&doc);

    auto defaultResults = index.getByTagName(defaultNs, "item");
    REQUIRE(defaultResults.size() == 2);

    auto noNsResults = index.getByTagName("item");
    REQUIRE(noNsResults.size() == 1);
}

TEST_CASE("TagIndex resolves shadowed prefix declarations across subtrees",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsV1 = "http://example.com/v1";
    const std::string nsV2 = "http://example.com/v2";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", nsV1),
                    GenericNode("p:node", NonVoid),
                    GenericNode("sub", NonVoid, Attribute("xmlns:p", nsV2),
                                GenericNode("p:node", NonVoid))};

    auto index = index::createIndex<index::TagIndex>(&doc);

    auto resultsV1 = index.getByTagName(nsV1, "node");
    REQUIRE(resultsV1.size() == 1);
    CHECK(resultsV1[0]->getNamespaceName() == nsV1);

    auto resultsV2 = index.getByTagName(nsV2, "node");
    REQUIRE(resultsV2.size() == 1);
    CHECK(resultsV2[0]->getNamespaceName() == nsV2);
}

TEST_CASE("TagIndex updates correctly when setTagName is called",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns)};
    auto index = index::createIndex<index::TagIndex>(&doc);

    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("p:initial", NonVoid);
    GenericNode* childPtr = child.get();
    doc.addChild(std::move(child));

    REQUIRE(index.getByTagName(ns, "initial").size() == 1);
    REQUIRE(index.getByTagName(ns, "renamed").empty());

    childPtr->setTagName("renamed");

    REQUIRE(index.getByTagName(ns, "initial").empty());
    REQUIRE(index.getByTagName(ns, "renamed").size() == 1);
    CHECK(index.getByTagName(ns, "renamed")[0] == childPtr);
}

TEST_CASE("TagIndex updates correctly when setNamespacePrefix is called",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsA = "http://example.com/a";
    const std::string nsB = "http://example.com/b";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:a", nsA),
                    Attribute("xmlns:b", nsB)};

    auto index = index::createIndex<index::TagIndex>(&doc);

    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("entry", NonVoid);
    GenericNode* childPtr = child.get();
    doc.addChild(std::move(child));

    REQUIRE(index.getByTagName("entry").size() == 1);
    REQUIRE(index.getByTagName(nsA, "entry").empty());
    REQUIRE(index.getByTagName(nsB, "entry").empty());

    childPtr->setNamespacePrefix("a");

    REQUIRE(index.getByTagName("entry").empty());
    REQUIRE(index.getByTagName(nsA, "entry").size() == 1);
    REQUIRE(index.getByTagName(nsB, "entry").empty());

    childPtr->setNamespacePrefix("b");

    REQUIRE(index.getByTagName("entry").empty());
    REQUIRE(index.getByTagName(nsA, "entry").empty());
    REQUIRE(index.getByTagName(nsB, "entry").size() == 1);

    childPtr->setNamespacePrefix("");

    REQUIRE(index.getByTagName("entry").size() == 1);
    REQUIRE(index.getByTagName(nsA, "entry").empty());
    REQUIRE(index.getByTagName(nsB, "entry").empty());
}

TEST_CASE("TagIndex updates when ancestor namespace bindings mutate",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string nsOld = "http://example.com/old";
    const std::string nsNew = "http://example.com/new";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", nsOld)};
    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("p:item", NonVoid);
    doc.addChild(std::move(child));

    auto index = index::createIndex<index::TagIndex>(&doc);

    REQUIRE(index.getByTagName(nsOld, "item").size() == 1);
    REQUIRE(index.getByTagName(nsNew, "item").empty());

    doc.setAttributeValue("xmlns:p", nsNew);

    REQUIRE(index.getByTagName(nsOld, "item").empty());
    REQUIRE(index.getByTagName(nsNew, "item").size() == 1);
}

TEST_CASE("TagIndex handles node addition and removal correctly",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns)};
    auto index = index::createIndex<index::TagIndex>(&doc);

    std::unique_ptr<GenericNode> child =
        std::make_unique<GenericNode>("p:data", NonVoid);
    Node* childRef = doc.addChild(std::move(child));

    REQUIRE(index.getByTagName(ns, "data").size() == 1);

    doc.removeChild(childRef);

    REQUIRE(index.getByTagName(ns, "data").empty());
    REQUIRE(index.getByNamespace(ns).empty());

    doc.addChild(GenericNode("p:data", NonVoid));
    REQUIRE(index.getByTagName(ns, "data").size() == 1);
}

TEST_CASE("TagIndex move constructor transfers maps and keeps tracking updates",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns),
                    GenericNode("p:item", NonVoid)};

    auto index = index::createIndex<index::TagIndex>(&doc);
    index::TagIndex movedIndex{std::move(index)};

    REQUIRE(!index.isValid());
    REQUIRE(movedIndex.getByTagName(ns, "item").size() == 1);

    doc.addChild(GenericNode("p:item", NonVoid));

    REQUIRE(movedIndex.getByTagName(ns, "item").size() == 2);
}

TEST_CASE("TagIndex move assignment transfers maps and updates correctly",
          "[TagIndex]") {
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    const std::string ns = "http://example.com/ns";

    GenericNode doc{"root", NonVoid, Attribute("xmlns:p", ns),
                    GenericNode("p:item", NonVoid)};

    GenericNode emptyDoc{"empty", NonVoid};

    auto indexMain = index::createIndex<index::TagIndex>(&doc);
    auto indexEmpty = index::createIndex<index::TagIndex>(&emptyDoc);

    indexEmpty = std::move(indexMain);

    REQUIRE(!indexMain.isValid());
    REQUIRE(indexEmpty.getByTagName(ns, "item").size() == 1);

    doc.addChild(GenericNode("p:item", NonVoid));
    REQUIRE(indexEmpty.getByTagName(ns, "item").size() == 2);
}