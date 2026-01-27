#include <limits>

#include "catch2/catch_all.hpp"
#include "onyx.h"

TEST_CASE("xpath string function correctly converts string to string") {
    using namespace onyx::dynamic::xpath;

    std::string empty = "";
    std::string test = "test";
    REQUIRE(functions::string(XPathObject(empty)) == "");
    REQUIRE(functions::string(XPathObject(test)) == "test");
}

TEST_CASE("xpath string function correctly converts bool to string") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(functions::string(XPathObject(true)) == "true");
    REQUIRE(functions::string(XPathObject(false)) == "false");
}

TEST_CASE("xpath string function correctly converts double to string") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(functions::string(XPathObject(std::numeric_limits<double>::quiet_NaN())) == "NaN");
    REQUIRE(functions::string(XPathObject(+0.0)) == "0");
    REQUIRE(functions::string(XPathObject(-0.0)) == "0");
    REQUIRE(functions::string(XPathObject(std::numeric_limits<double>::infinity())) == "Infinity");
    REQUIRE(functions::string(XPathObject(-std::numeric_limits<double>::infinity())) == "-Infinity");
    REQUIRE(functions::string(XPathObject(5.0000)) == "5");
    REQUIRE(functions::string(XPathObject(-5.0000)) == "-5");
    REQUIRE(functions::string(XPathObject(1.0)) == "1");
    REQUIRE(functions::string(XPathObject(1.234567)) == "1.234567");
    REQUIRE(functions::string(XPathObject(0.234567)) == "0.234567");
    REQUIRE(functions::string(XPathObject(00.234567)) == "0.234567");
    REQUIRE(functions::string(XPathObject(00.034567)) == "0.034567");
}

TEST_CASE("xpath string function correctly converts nodeset to string") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    std::vector<Node*> nodes;
    XPathObject obj(nodes);

    Text node1("Hello World");
    std::vector<Node*> nodes1 = { &node1 };
    XPathObject obj1(nodes1);

    cdiv node2{
        span(Text("Deep")),
        Text(" Text")
    };
    std::vector<Node*> nodes2 = { &node2 };
    XPathObject obj2(nodes2);

    GenericNode node31("div", false, Attribute("href", "test"));
    AttributeViewNode node32(&node31, 0);
    std::vector<Node*> nodes3 = { &node32, &node31 };
    XPathObject obj3(nodes3);

    REQUIRE(functions::string(obj) == "");
    REQUIRE(functions::string(obj1) == "Hello World");
    REQUIRE(functions::string(obj2) == "Deep Text");
    REQUIRE(functions::string(obj3) == "test");
}

TEST_CASE("xpath boolean function correctly converts boolean to boolean") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(functions::boolean(XPathObject(true)));
    REQUIRE(!functions::boolean(XPathObject(false)));
}

TEST_CASE("xpath boolean function correctly converts string to boolean") {
    using namespace onyx::dynamic::xpath;

    std::string falsy = "";
    std::string truthy = " ";
    std::string truthy1 = " test ";
    std::string truthy2 = "false";
    REQUIRE(!functions::boolean(XPathObject(falsy)));
    REQUIRE(functions::boolean(XPathObject(truthy)));
    REQUIRE(functions::boolean(XPathObject(truthy1)));
    REQUIRE(functions::boolean(XPathObject(truthy2)));
}

TEST_CASE("xpath boolean function correctly converts number to boolean") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(!functions::boolean(XPathObject(-0.0)));
    REQUIRE(!functions::boolean(XPathObject(0.0)));
    REQUIRE(!functions::boolean(XPathObject(std::numeric_limits<double>::quiet_NaN())));
    REQUIRE(functions::boolean(XPathObject(std::numeric_limits<double>::infinity())));
    REQUIRE(functions::boolean(XPathObject(-std::numeric_limits<double>::infinity())));
    REQUIRE(functions::boolean(XPathObject(-1.0)));
    REQUIRE(functions::boolean(XPathObject(0.00000001)));
    REQUIRE(functions::boolean(XPathObject(5.67890)));
}

TEST_CASE("xpath boolean function correctly converts nodeset to boolean") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    std::vector<Node*> empty;
    std::vector<Node*> nonEmpty;

    tags::GenericNode node("node", false);

    nonEmpty.push_back(&node);

    REQUIRE(!functions::boolean(XPathObject(empty)));
    REQUIRE(functions::boolean(XPathObject(nonEmpty)));
}

TEST_CASE("xpath number function correctly converts number to number") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(functions::number(XPathObject(1.0)) == 1.0);
    REQUIRE(functions::number(XPathObject(0.0)) == 0.0);
    REQUIRE(functions::number(XPathObject(std::isnan(std::numeric_limits<double>::quiet_NaN()))));
    REQUIRE(functions::number(XPathObject(std::numeric_limits<double>::infinity())) == std::numeric_limits<double>::infinity());
    REQUIRE(functions::number(XPathObject(-std::numeric_limits<double>::infinity())) == -std::numeric_limits<double>::infinity());
    REQUIRE(functions::number(XPathObject(0.000001)) == 0.000001);
}

TEST_CASE("xpath number function correctly converts boolean to number") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(functions::number(XPathObject(true)) == 1.0);
    REQUIRE(functions::number(XPathObject(false)) == 0.0);
}

TEST_CASE("xpath number function correctly converts string to number") {
    using namespace onyx::dynamic::xpath;

    std::string test1 = "";
    std::string test2 = " ";
    std::string test3 = "  ";
    std::string test4 = "  something 1 ";
    std::string test5 = "  1";
    std::string test6 = "  1.0";
    std::string test7 = "  1.013131031";
    std::string test8 = "  1.013131031.12";
    std::string test9 = "  1.013131031  ";
    std::string test10 = " 1.013131031.12  ";
    std::string test11 = " -5  ";
    std::string test12 = " -5.23  ";
    std::string test13 = " .23  ";
    std::string test14 = " 0.23  ";
    std::string test15 = " 0000.23000  ";

    REQUIRE(std::isnan(functions::number(XPathObject(test1))));
    REQUIRE(std::isnan(functions::number(XPathObject(test2))));
    REQUIRE(std::isnan(functions::number(XPathObject(test3))));
    REQUIRE(std::isnan(functions::number(XPathObject(test4))));
    REQUIRE(functions::number(XPathObject(test5)) == 1.0);
    REQUIRE(functions::number(XPathObject(test6)) == 1.0);
    REQUIRE(functions::number(XPathObject(test7)) == 1.013131031);
    REQUIRE(std::isnan(functions::number(XPathObject(test8))));
    REQUIRE(functions::number(XPathObject(test9)) == 1.013131031);
    REQUIRE(std::isnan(functions::number(XPathObject(test10))));
    REQUIRE(functions::number(XPathObject(test11)) == -5.0);
    REQUIRE(functions::number(XPathObject(test12)) == -5.23);
    REQUIRE(functions::number(XPathObject(test13)) == 0.23);
    REQUIRE(functions::number(XPathObject(test14)) == 0.23);
    REQUIRE(functions::number(XPathObject(test15)) == 0.23);
}

TEST_CASE("xpath number function correctly converts nodeset to number") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    using namespace onyx::dynamic::tags;
    
    std::vector<Node*> nodes;
    XPathObject obj(nodes);

    Text node1("0.0");
    std::vector<Node*> nodes1 = { &node1 };
    XPathObject obj1(nodes1);

    cdiv node2{
        span(Text("1")),
        Text(".2")
    };
    std::vector<Node*> nodes2 = { &node2 };
    XPathObject obj2(nodes2);

    GenericNode node31("div", false, Attribute("href", "2.3"));
    AttributeViewNode node32(&node31, 0);
    std::vector<Node*> nodes3 = { &node32, &node31 };
    XPathObject obj3(nodes3);

    cdiv node4{
        span(Text("1")),
        Text(".2 test")
    };
    std::vector<Node*> nodes4 = { &node4 };
    XPathObject obj4(nodes4);

    REQUIRE(std::isnan(functions::number(obj)));
    REQUIRE(functions::number(obj1) == 0.0);
    REQUIRE(functions::number(obj2) == 1.2);
    REQUIRE(functions::number(obj3) == 2.3);
    REQUIRE(std::isnan(functions::number(obj4)));
}

TEST_CASE("XPathObject throws when trying to cast non-nodeset to nodeset") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    REQUIRE_THROWS_WITH(XPathObject(1.0).asNodeset(), "Tried to cast non-nodeset to nodeset");
}

TEST_CASE("XPathObject boolean vs boolean comparison") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(XPathObject(true) == XPathObject(true));
    REQUIRE(XPathObject(false) == XPathObject(false));
    REQUIRE(XPathObject(true) != XPathObject(false));
    REQUIRE_FALSE(XPathObject(true) == XPathObject(false));
}

TEST_CASE("XPathObject boolean vs other comparison") {
    using namespace onyx::dynamic::xpath;
    
    REQUIRE(XPathObject(true) == XPathObject(1.0));
    REQUIRE(XPathObject(false) == XPathObject(0.0));
    REQUIRE(XPathObject(true) != XPathObject(0.0)); 
    REQUIRE(XPathObject(true) == XPathObject(std::string("true"))); 
    REQUIRE(XPathObject(false) == XPathObject(std::string(""))); 
    REQUIRE(XPathObject(true) == XPathObject(std::string("false")));
    REQUIRE(XPathObject(true) <= XPathObject(1.0));
    REQUIRE(XPathObject(true) >= XPathObject(1.0));
    REQUIRE(XPathObject(false) < XPathObject(1.0));
}

TEST_CASE("XPathObject number vs string comparison") {
    using namespace onyx::dynamic::xpath;
    
    REQUIRE(XPathObject(1.0) == XPathObject(std::string("1.0")));
    REQUIRE(XPathObject(1.0) == XPathObject(std::string("  1  ")));
    REQUIRE(XPathObject(0.5) == XPathObject(std::string(".5")));
    REQUIRE(XPathObject(std::string("02")) == XPathObject(2.0));
}

TEST_CASE("XPathObject number vs number comparison") {
    using namespace onyx::dynamic::xpath;
    double nan = std::numeric_limits<double>::quiet_NaN();

    REQUIRE(XPathObject(1.0) == XPathObject(1.0));
    REQUIRE(XPathObject(1.0) != XPathObject(0.8));
    REQUIRE(XPathObject(1.0) > XPathObject(0.9));
    REQUIRE(XPathObject(0.5) < XPathObject(1.0));
    REQUIRE(XPathObject(2.0) >= XPathObject(2.0));
    REQUIRE(XPathObject(2.0) >= XPathObject(1.9));
    REQUIRE(XPathObject(2.0) <= XPathObject(2.0));
    REQUIRE(XPathObject(2.0) <= XPathObject(2.1));

    REQUIRE(XPathObject(nan) != XPathObject(nan));
    REQUIRE_FALSE(XPathObject(nan) == XPathObject(nan));
    REQUIRE_FALSE(XPathObject(nan) < XPathObject(1.0));
    REQUIRE_FALSE(XPathObject(nan) > XPathObject(1.0));
    REQUIRE_FALSE(XPathObject(nan) <= XPathObject(1.0));
    REQUIRE_FALSE(XPathObject(nan) >= XPathObject(1.0));
}

TEST_CASE("XPathObject string vs string comparison") {
    using namespace onyx::dynamic::xpath;
    
    REQUIRE(XPathObject(std::string("foo")) == XPathObject(std::string("foo")));
    REQUIRE(XPathObject(std::string("foo")) != XPathObject(std::string("Foo")));
    REQUIRE(XPathObject(std::string("10")) > XPathObject(std::string("2"))); 
    REQUIRE_FALSE(XPathObject(std::string("10")) < XPathObject(std::string("2")));
    REQUIRE_FALSE(XPathObject(std::string("abc")) < XPathObject(std::string("def")));
    REQUIRE_FALSE(XPathObject(std::string("abc")) > XPathObject(std::string("def")));
    REQUIRE_FALSE(XPathObject(std::string("abc")) == XPathObject(std::string("def")));
}

TEST_CASE("XPathObject nodeset vs number comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text node10{"10"}; 
    Text node20{"20"}; 
    std::vector<Node*> nodes = { &node10, &node20 };
    XPathObject set(nodes);

    std::vector<Node*> empty_vec;
    XPathObject emptySet(empty_vec);

    REQUIRE(set == XPathObject(10.0));
    REQUIRE(set == XPathObject(20.0));
    REQUIRE(set != XPathObject(15.0));
    REQUIRE_FALSE(set == XPathObject(15.0));
    REQUIRE_FALSE(emptySet == XPathObject(0.0));
}

TEST_CASE("XPathObject nodeset vs number with NaN value comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text node2{"test"}; 
    std::vector<Node*> nodes2 = { &node2 };
    XPathObject set2(nodes2);

    REQUIRE(set2 != XPathObject(1.0));
    REQUIRE_FALSE(set2 == XPathObject(1.0)); 
}

TEST_CASE("XPathObject nodeset vs string comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    Text nodeA{"A"}; 
    Text nodeB{"B"}; 
    std::vector<Node*> nodes = { &nodeA, &nodeB };
    XPathObject set(nodes);
    
    std::vector<Node*> empty_vec;
    XPathObject emptySet(empty_vec);

    REQUIRE(set == XPathObject(std::string("A")));
    REQUIRE(set == XPathObject(std::string("B")));
    REQUIRE_FALSE(set == XPathObject(std::string("C")));
    REQUIRE_FALSE(emptySet == XPathObject(std::string("")));
}

TEST_CASE("XPathObject nodeset vs boolean comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    Text node1{"false"}; 
    Text node2{"0"}; 
    std::vector<Node*> nodes = { &node1, &node2 };
    XPathObject set(nodes);
    
    std::vector<Node*> empty_vec;
    XPathObject emptySet(empty_vec);
    
    REQUIRE(set == XPathObject(true));
    REQUIRE(set != XPathObject(false));
    REQUIRE(emptySet == XPathObject(false));
}

TEST_CASE("XPathObject nodeset vs nodeset intersection comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    Text node11{"1"}; 
    Text node12{"2"}; 
    std::vector<Node*> nodes1 = { &node11, &node12 };
    
    Text node21{"2"}; 
    Text node22{"3"}; 
    std::vector<Node*> nodes2 = { &node21, &node22 };

    XPathObject set1(nodes1);
    XPathObject set2(nodes2);
    
    REQUIRE(set1 == set2);
    REQUIRE(set1 != set2);
}

TEST_CASE("XPathObject nodeset vs nodeset subset comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    Text node1{"1"}; 
    std::vector<Node*> nodes1 = { &node1 };
    
    Text node21{"1"}; 
    Text node22{"2"}; 
    std::vector<Node*> nodes2 = { &node21, &node22 };

    XPathObject set1(nodes1);
    XPathObject set2(nodes2);

    REQUIRE(set1 == set2);
    REQUIRE(set1 != set2);
}

TEST_CASE("XPathObject nodeset vs nodeset single match comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    Text node1{"1"}; 
    std::vector<Node*> nodes1 = { &node1 };
    
    Text node2{"1"}; 
    std::vector<Node*> nodes2 = { &node2 };

    XPathObject set1(nodes1);
    XPathObject set2(nodes2);

    REQUIRE(set1 == set2);
    
    REQUIRE_FALSE(set1 != set2);
}

TEST_CASE("XPathObject nodeset vs nodeset relational operators") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text node5{"5"};
    Text node10{"10"};
    Text node20{"20"};
    
    std::vector<Node*> nodesA = { &node10, &node20 };
    XPathObject setA(nodesA);

    std::vector<Node*> nodesB = { &node5 };
    XPathObject setB(nodesB);

    std::vector<Node*> empty_vec;
    XPathObject emptySet(empty_vec);

    REQUIRE(setA > XPathObject(15.0)); 
    REQUIRE(setA < XPathObject(15.0)); 
    REQUIRE_FALSE(emptySet < XPathObject(10.0)); 

    REQUIRE(setA > setB);
    REQUIRE(setB < setA);
}

TEST_CASE("XPathObject nodeset vs nodeset not equal comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text nodeA{"foo"};
    Text nodeB{"bar"};
    std::vector<Node*> nodes = { &nodeA, &nodeB };
    XPathObject set(nodes);

    REQUIRE(set == XPathObject(std::string("foo")));
    REQUIRE(set != XPathObject(std::string("foo")));
}