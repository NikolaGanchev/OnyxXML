#include "catch2/catch_all.hpp"
#include "templater.h"

using namespace Templater::dynamic;

TEST_CASE("Interning returns same pointer for same strings", "[Attribute]") {
    Attribute attr1("name", "value");
    Attribute attr2("name", "value");

    const std::string& name1 = attr1.getName();
    const std::string& name2 = attr2.getName();
    const std::string& value1 = attr1.getValue();
    const std::string& value2 = attr2.getValue();

    REQUIRE(&name1 == &name2);
    REQUIRE(&value1 == &value2);
}

TEST_CASE("Copy constructor interns same string", "[Attribute]") {
    Attribute attr1("name", "value");
    Attribute attrCopy(attr1);

    REQUIRE(&attr1.getName() == &attrCopy.getName());
    REQUIRE(&attr1.getValue() == &attrCopy.getValue());
}

TEST_CASE("Copy assignment operator interns same string", "[Attribute]") {
    Attribute attr1("name", "value");
    Attribute attr2("other", "otherValue");

    attr2 = attr1;

    REQUIRE(&attr1.getName() == &attr2.getName());
    REQUIRE(&attr1.getValue() == &attr2.getValue());
}

TEST_CASE("Move constructor transfers ownership", "[Attribute]") {
    Attribute attr1("name", "value");
    const std::string* originalNamePtr = &attr1.getName();
    const std::string* originalValuePtr = &attr1.getValue();

    Attribute attrMoved(std::move(attr1));

    REQUIRE(&attrMoved.getName() == originalNamePtr);
    REQUIRE(&attrMoved.getValue() == originalValuePtr);
}

TEST_CASE("Move assignment operator transfers ownership", "[Attribute]") {
    Attribute attr1("name", "value");
    Attribute attr2("other", "otherValue");

    const std::string* originalNamePtr = &attr1.getName();
    const std::string* originalValuePtr = &attr1.getValue();

    attr2 = std::move(attr1);

    REQUIRE(&attr2.getName() == originalNamePtr);
    REQUIRE(&attr2.getValue() == originalValuePtr);
}

TEST_CASE("Memory leak test", "[Attribute]") {
    for (int i = 0; i < 10000; ++i) {
        Attribute attr("name" + std::to_string(i % 10), "value" + std::to_string(i % 10));
    }
    
    REQUIRE(Attribute::getInternPoolSize() == 0);
}