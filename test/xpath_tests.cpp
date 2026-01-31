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

TEST_CASE("Virtual machine runs") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book1")),
            GenericNode("price", false, Text("10"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Book2")),
            GenericNode("price", false, Text("20")))
    );

    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));

    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    REQUIRE(res.asNodeset()[0] == &doc);
}

TEST_CASE("Virtual machine /store/book") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book1")),
            GenericNode("price", false, Text("10"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Book2")),
            GenericNode("price", false, Text("20")))
    );

    // 0="store", 1="book"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        // Select /store
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 8))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
    REQUIRE(nodeset[1]->getTagName() == "book");
    REQUIRE(nodeset[1]->getAttributeValue("id") == "2");
}

TEST_CASE("Virtual machine predicate /store/book[price > 15]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book1")),
            GenericNode("price", false, Text("10"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Book2")),
            GenericNode("price", false, Text("20")))
    );

    // 0="store", 1="book", 2="price", 3=15.0
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        .addData(XPathObject(std::string("price")))
        .addData(XPathObject(15.0))
        // Select /store
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0)) 
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 16)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1)) 
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 14)) // Jump to UNION
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::GREATER_THAN))
                .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
}

TEST_CASE("Virtual machine attribute Test /store/book[@id='1']") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book1")),
            GenericNode("price", false, Text("10"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Book2")),
            GenericNode("price", false, Text("20")))
    );

    // 0="store", 1="book", 2="id", 3="1"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        .addData(XPathObject(std::string("id")))
        .addData(XPathObject(std::string("1")))
        // Select /store
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0)) 
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 16)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 14))
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))  // "id"
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))  // "1"
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
                .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("Virtual machine empty /store/book/author") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book1"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Book2")))
    );

    // 0="store", 1="book", 2="author"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        .addData(XPathObject(std::string("author")))
        // 1. Select /store
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0)) 
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 13)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 11))
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2)) // "author"
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
                .addInstruction(Instruction(OPCODE::LOOP_UNION))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().empty());
}

TEST_CASE("Virtual machine math /store/book[price div 2 < 15]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("price", false, Text("20"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("price", false, Text("40")))
    );

    // 0="store", 1="book", 2="price", 3=2.0, 4=15.0
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        .addData(XPathObject(std::string("price")))
        .addData(XPathObject(2.0))
        .addData(XPathObject(15.0))
        // Select /store/book
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0)) 
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 20)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 18))
                // Select 'price'
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2)) 
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
                // Load 2
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3)) 
                .addInstruction(Instruction(OPCODE::CALCULATE, CALCULATE_MODE::DIVIDE))
                // Load 15
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::LESS_THAN))
                .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("Virtual machine function composition book[not(starts-with(title, 'Second'))]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Second Book")))
    );

    // 0="store", 1="book", 2="title", 3="Second"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        .addData(XPathObject(std::string("title")))
        .addData(XPathObject(std::string("Second")))
        // Select /store/book
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0)) 
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 20)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 18))
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::STARTS_WITH_2))
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::NOT_1))
                .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("Virtual machine sum function /store[sum(book/price) > 50]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("store", false, Attribute("id", "1"),
            GenericNode("book", false, GenericNode("price", false, Text("10"))),
            GenericNode("book", false, GenericNode("price", false, Text("20")))
        ),
        GenericNode("store", false, Attribute("id", "2"),
            GenericNode("book", false, GenericNode("price", false, Text("40"))),
            GenericNode("book", false, GenericNode("price", false, Text("30")))
        )
    );

    // 0="store", 1="book", 2="price", 3=50.0, 4="root"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("store")))
        .addData(XPathObject(std::string("book")))
        .addData(XPathObject(std::string("price")))
        .addData(XPathObject(50.0))
        .addData(XPathObject(std::string("root")))
        // Select /root/store
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4)) 
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 24)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0)) // store
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 22))
                
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1)) // book
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
                
                .addInstruction(Instruction(OPCODE::LOOP_ENTER, 16)) 
                    .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                    .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2)) // price
                    .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
                    .addInstruction(Instruction(OPCODE::LOOP_UNION))
                .addInstruction(Instruction(OPCODE::LOOP_NEXT))
                
                // Call sum()
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::SUM_1))
                
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3)) // 50.0
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::GREATER_THAN))
                .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "store");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
}

TEST_CASE("Virtual machine attributes /root/item/@id") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("item", false, Attribute("id", "a")),
        GenericNode("item", false, Attribute("id", "b"), Attribute("other", "x"))
    );

    // 0="root", 1="item", 2="id"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("root")))
        .addData(XPathObject(std::string("item")))
        .addData(XPathObject(std::string("id")))
        
        // Select /root
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
        
        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 13)) 
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1)) // "item"
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 11))
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2)) // "id"
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
                .addInstruction(Instruction(OPCODE::LOOP_UNION))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getXPathType() == Node::XPathType::ATTRIBUTE);
    REQUIRE(nodeset[0]->getStringValue() == "a"); 
    REQUIRE(nodeset[1]->getXPathType() == Node::XPathType::ATTRIBUTE);
    REQUIRE(nodeset[1]->getStringValue() == "b"); 
}

TEST_CASE("Virtual machine booleans item[@x='1' and (@y='2' or @z='3')]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("item", false, Attribute("id", "1"), Attribute("x", "1"), Attribute("y", "2")),
        GenericNode("item", false, Attribute("id", "2"), Attribute("x", "1"), Attribute("z", "3")),
        GenericNode("item", false, Attribute("id", "3"), Attribute("x", "1"), Attribute("y", "9")),
        GenericNode("item", false, Attribute("id", "4"), Attribute("x", "0"), Attribute("y", "2"))
    );

    // 0="root", 1="item", 2="x", 3="1", 4="y", 5="2", 6="z", 7="3"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("root")))
        .addData(XPathObject(std::string("item")))
        .addData(XPathObject(std::string("x")))
        .addData(XPathObject(std::string("1")))
        .addData(XPathObject(std::string("y")))
        .addData(XPathObject(std::string("2")))
        .addData(XPathObject(std::string("z")))
        .addData(XPathObject(std::string("3")))

        // Select /root/item
        .addInstruction(Instruction(OPCODE::LOAD_ROOT))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
        .addInstruction(Instruction(OPCODE::LOOP_ENTER, 33)) // Skip to HALT
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 27))
                
                // Check @x == '1'
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
                // If False, Jump to FAIL
                .addInstruction(Instruction(OPCODE::JUMP_F, 26)) 

                // Check (@y='2' OR @z='3')
                
                // Check @y == '2'
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 5))
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
                // If True, Jump to SUCCESS
                .addInstruction(Instruction(OPCODE::JUMP_T, 28)) 

                // Check @z == '3'
                .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 6))
                .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
                .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 7))
                .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
                // If True, Jump to SUCCESS
                .addInstruction(Instruction(OPCODE::JUMP_T, 28))

                // FAIL (26)
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::_FALSE_0))
                .addInstruction(Instruction(OPCODE::JUMP, 29)) // Jump to Test

                // SUCCESS (28)
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::_TRUE_0))

                // TEST (29)
                .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))

        .addInstruction(Instruction(OPCODE::LOOP_NEXT))
        .addInstruction(Instruction(OPCODE::HALT)) // 33
        .build();


    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getAttributes()[0].getValue()== "1");
    REQUIRE(nodeset[1]->getAttributes()[0].getValue() == "2");
}

TEST_CASE("Virtual machine union Operator //div | //span") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("div", false,
            GenericNode("span", false, Text("A"))
        ),
        GenericNode("section", false,
            GenericNode("span", false, Text("B")),
            GenericNode("div", false, Text("C"))
        )
    );

    // 0="div", 1="span"
    std::unique_ptr<Program> pr = Program::Builder()
        .addData(XPathObject(std::string("div")))
        .addData(XPathObject(std::string("span")))
        
        // Context Node
        .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
        
        // Select //div
        .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE)) 
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::DESCENDANT))
        
        // Select //span
        .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
        .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
        .addInstruction(Instruction(OPCODE::SELECT, AXIS::DESCENDANT))
        .addInstruction(Instruction(OPCODE::UNION))
        
        .addInstruction(Instruction(OPCODE::HALT))
        .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 4);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "div");
    REQUIRE(nodeset[0]->getChildrenCount() == 1);
    REQUIRE(nodeset[0]->getFirstChild()->getTagName() == "span");
    REQUIRE(nodeset[1]->getTagName() == "span");
    REQUIRE(nodeset[1]->getChildrenCount() == 1);
    REQUIRE(nodeset[1]->getFirstChild()->serialize() == "A");
    REQUIRE(nodeset[2]->getTagName() == "span");
    REQUIRE(nodeset[2]->getChildrenCount() == 1);
    REQUIRE(nodeset[2]->getFirstChild()->serialize() == "B");
    REQUIRE(nodeset[3]->getTagName() == "div");
    REQUIRE(nodeset[3]->getChildrenCount() == 1);
    REQUIRE(nodeset[3]->getFirstChild()->serialize() == "C");
}

void requireToken(onyx::xpath::Lexer& lexer, 
    onyx::xpath::Lexer::TokenType expectedType, 
    const std::string& expectedValue = "") {
    const auto& token = lexer.nextToken();
    REQUIRE(token.getType() == expectedType);
    if (!expectedValue.empty()) {
        REQUIRE(token.getValue() == expectedValue);
    }
}

TEST_CASE("XPath Lexer basic test") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("/book/title | //author");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "book");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "title");
    requireToken(lexer, Lexer::TokenType::PIPE);
    requireToken(lexer, Lexer::TokenType::DOUBLE_SLASH);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "author");
}

TEST_CASE("XPath Lexer integers and decimals") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("123 45.67");
    Lexer lexer(cursor);
    
    requireToken(lexer, Lexer::TokenType::NUMBER, "123");
    requireToken(lexer, Lexer::TokenType::NUMBER, "45.67");
}

TEST_CASE("XPath Lexer dot ambiguities") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor(". .. .5");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::DOT);
    requireToken(lexer, Lexer::TokenType::TWO_DOTS);
    requireToken(lexer, Lexer::TokenType::NUMBER, ".5");
}

TEST_CASE("XPath Lexer Dot attached to path") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("./book");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::DOT);
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "book");
}

TEST_CASE("XPath Lexer * disambiguation as nametest") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("child::* / *");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::AXIS_NAME, "child");
    requireToken(lexer, Lexer::TokenType::DOUBLE_COLON);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "*");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "*");
}

TEST_CASE("XPath Lexer * disambiguation as multiplication")  {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("price * 5");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NAME_TEST, "price");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "*"); 
    requireToken(lexer, Lexer::TokenType::NUMBER, "5");
}
    
TEST_CASE("XPath Lexer * as namespace wildcard") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("svg:*");
    Lexer lexer(cursor);
    
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "svg:*");
}

TEST_CASE("XPath Lexer operator keywords as operators") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("10 div 5 mod 2");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NUMBER, "10");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "div");
    requireToken(lexer, Lexer::TokenType::NUMBER, "5");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "mod");
    requireToken(lexer, Lexer::TokenType::NUMBER, "2");
}

TEST_CASE("XPath Lexer operator keywords as nametests") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("div/mod");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NAME_TEST, "div");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "mod");
}

TEST_CASE("XPath Lexer div div div edge case") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("div div div");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NAME_TEST, "div");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "div");
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "div");
}

TEST_CASE("XPath Lexer node types") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("child::node()/child::text()/child::comment()");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::AXIS_NAME, "child");
    requireToken(lexer, Lexer::TokenType::DOUBLE_COLON);
    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "node()");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::AXIS_NAME, "child");
    requireToken(lexer, Lexer::TokenType::DOUBLE_COLON);
    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "text()");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::AXIS_NAME, "child");
    requireToken(lexer, Lexer::TokenType::DOUBLE_COLON);
    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "comment()");
}

TEST_CASE("XPath Lexer function calls") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("count(book)");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::FUNCTION_NAME, "count");
    requireToken(lexer, Lexer::TokenType::OPENING_PAREN);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "book");
    requireToken(lexer, Lexer::TokenType::CLOSING_PAREN);
}

TEST_CASE("XPath Lexer processing instruction special case") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("processing-instruction('xml-stylesheet')");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "processing-instruction('xml-stylesheet')");
}

TEST_CASE("XPath Lexer empty processing instruction") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("processing-instruction()");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "processing-instruction()");
}

TEST_CASE("XPath Lexer literals") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("'single' \"double\"");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::LITERAL, "single");
    requireToken(lexer, Lexer::TokenType::LITERAL, "double");
}

TEST_CASE("XPath Lexer variables") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("$varName $my-var");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::VARIABLE_REFERENCE, "varName");
    requireToken(lexer, Lexer::TokenType::VARIABLE_REFERENCE, "my-var");
}

TEST_CASE("XPath Lexer real predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("book[price > -10 and @instock]");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NAME_TEST, "book");
    requireToken(lexer, Lexer::TokenType::OPENING_BRACKET);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "price");
    requireToken(lexer, Lexer::TokenType::OPERATOR, ">");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "-");
    requireToken(lexer, Lexer::TokenType::NUMBER, "10");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "and");
    requireToken(lexer, Lexer::TokenType::AT);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "instock");
    requireToken(lexer, Lexer::TokenType::CLOSING_BRACKET);
}

TEST_CASE("XPath Lexer complex predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("div div mod * .5 | child::* * 5 and node() or processing-instruction(' ) ') / .. // .");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NAME_TEST, "div");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "div");
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "mod");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "*");
    requireToken(lexer, Lexer::TokenType::NUMBER, ".5");
    requireToken(lexer, Lexer::TokenType::PIPE);
    requireToken(lexer, Lexer::TokenType::AXIS_NAME, "child");
    requireToken(lexer, Lexer::TokenType::DOUBLE_COLON);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "*");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "*");
    requireToken(lexer, Lexer::TokenType::NUMBER, "5");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "and");
    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "node()");
    requireToken(lexer, Lexer::TokenType::OPERATOR, "or");
    requireToken(lexer, Lexer::TokenType::NODE_TYPE, "processing-instruction(' ) ')");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::TWO_DOTS);
    requireToken(lexer, Lexer::TokenType::DOUBLE_SLASH);
    requireToken(lexer, Lexer::TokenType::DOT);
    requireToken(lexer, Lexer::TokenType::END);
}

std::unique_ptr<onyx::xpath::Parser::AstNode> parse(const std::string& expression) {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor(expression.c_str());
    Lexer lexer(cursor);
    Parser parser(lexer);
    return parser.buildAST();
}

TEST_CASE("XPath Parser basic element selection") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para");
    
    // Should be child::para
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "child");
    CHECK(step->test == "para");
    CHECK(step->predicates.empty());
}

TEST_CASE("XPath Parser wildcard selection") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("*");

    // Should be child::* (NameTest)
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "child");
    CHECK(step->test == "*");
}

TEST_CASE("XPath Parser text node selection") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("text()");

    // Should be child::text() (NodeType)
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "child");
    CHECK(step->test == "text()"); 
}

TEST_CASE("XPath Parser attribute selection abbreviation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("@name");

    // Should be attribute::name
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "attribute");
    CHECK(step->test == "name");
}

TEST_CASE("XPath Parser attribute wildcard") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("@*");

    // Should be attribute::*
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "attribute");
    CHECK(step->test == "*");
}

TEST_CASE("XPath Parser numeric predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[1]");

    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->test == "para");
    REQUIRE(step->predicates.size() == 1);
    
    Parser::Number* num = dynamic_cast<Parser::Number*>(step->predicates[0].get());
    REQUIRE(num != nullptr);
    CHECK(num->num == "1");
}

TEST_CASE("XPath Parser function predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[last()]");

    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    REQUIRE(step->predicates.size() == 1);

    Parser::FunctionCall* func = dynamic_cast<Parser::FunctionCall*>(step->predicates[0].get());
    REQUIRE(func != nullptr);
    CHECK(func->name == "last");
    CHECK(func->args.empty());
}

TEST_CASE("XPath Parser relative path with wildcard") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("*/para");

    Parser::BinaryOp* binary = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(binary != nullptr);
    CHECK(binary->op == "/");

    Parser::Step* left = dynamic_cast<Parser::Step*>(binary->left.get());
    REQUIRE(left != nullptr);
    CHECK(left->test == "*");

    Parser::Step* right = dynamic_cast<Parser::Step*>(binary->right.get());
    REQUIRE(right != nullptr);
    CHECK(right->test == "para");
}

TEST_CASE("XPath Parser absolute path with predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("/doc/chapter[5]/section[2]");
    
    Parser::BinaryOp* op2 = dynamic_cast<Parser::BinaryOp*>(ast.get()); 
    REQUIRE(op2 != nullptr);
    
    Parser::Step* section = dynamic_cast<Parser::Step*>(op2->right.get());
    REQUIRE(section != nullptr);
    CHECK(section->test == "section");
    CHECK(section->predicates.size() == 1);

    Parser::BinaryOp* op1 = dynamic_cast<Parser::BinaryOp*>(op2->left.get());
    REQUIRE(op1 != nullptr);

    Parser::Step* chapter = dynamic_cast<Parser::Step*>(op1->right.get());
    REQUIRE(chapter != nullptr);
    CHECK(chapter->test == "chapter");
    CHECK(chapter->predicates.size() == 1);

    Parser::BinaryOp* op0 = dynamic_cast<Parser::BinaryOp*>(op1->left.get());
    REQUIRE(op0 != nullptr);
    
    CHECK(dynamic_cast<Parser::RootNode*>(op0->left.get()) != nullptr);
    
    Parser::Step* doc = dynamic_cast<Parser::Step*>(op0->right.get());
    CHECK(doc->test == "doc");
}

TEST_CASE("XPath Parser double slash expansion") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    // Should expand to chapter / descendant-or-self::node() / para
    std::unique_ptr<Parser::AstNode> ast = parse("chapter//para");

    Parser::BinaryOp* finalSlash = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(finalSlash != nullptr);
    
    Parser::Step* right = dynamic_cast<Parser::Step*>(finalSlash->right.get());
    CHECK(right->test == "para");

    Parser::BinaryOp* firstSlash = dynamic_cast<Parser::BinaryOp*>(finalSlash->left.get());
    REQUIRE(firstSlash != nullptr);

    Parser::Step* chapter = dynamic_cast<Parser::Step*>(firstSlash->left.get());
    CHECK(chapter->test == "chapter");

    Parser::Step* hidden = dynamic_cast<Parser::Step*>(firstSlash->right.get());
    REQUIRE(hidden != nullptr);
    CHECK(hidden->axis == "descendant-or-self");
    CHECK(hidden->test == "node()");
}

TEST_CASE("XPath Parser absolute double slash") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    // Root / descendant-or-self::node() / para
    std::unique_ptr<Parser::AstNode> ast = parse("//para");

    Parser::BinaryOp* finalSlash = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(finalSlash != nullptr);

    Parser::BinaryOp* firstSlash = dynamic_cast<Parser::BinaryOp*>(finalSlash->left.get());
    REQUIRE(firstSlash != nullptr);

    CHECK(dynamic_cast<Parser::RootNode*>(firstSlash->left.get()) != nullptr);

    Parser::Step* hidden = dynamic_cast<Parser::Step*>(firstSlash->right.get());
    CHECK(hidden->axis == "descendant-or-self");
}

TEST_CASE("XPath Parser context node abbreviation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse(".");
    
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "self");
    CHECK(step->test == "node()");
}

TEST_CASE("XPath Parser parent node abbreviation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("..");
    
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "parent");
    CHECK(step->test == "node()");
}

TEST_CASE("XPath Parser parent attribute path") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("../@lang");

    Parser::BinaryOp* binary = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(binary != nullptr);

    Parser::Step* left = dynamic_cast<Parser::Step*>(binary->left.get());
    CHECK(left->axis == "parent");

    Parser::Step* right = dynamic_cast<Parser::Step*>(binary->right.get());
    CHECK(right->axis == "attribute");
    CHECK(right->test == "lang");
}

TEST_CASE("XPath Parser predicate with attribute equality") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[@type=\"warning\"]");

    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    REQUIRE(step->predicates.size() == 1);

    Parser::BinaryOp* eqOp = dynamic_cast<Parser::BinaryOp*>(step->predicates[0].get());
    REQUIRE(eqOp != nullptr);
    CHECK(eqOp->op == "=");

    Parser::Step* left = dynamic_cast<Parser::Step*>(eqOp->left.get());
    CHECK(left->axis == "attribute");
    CHECK(left->test == "type");

    Parser::Literal* right = dynamic_cast<Parser::Literal*>(eqOp->right.get());
    CHECK(right->value == "warning");
}

TEST_CASE("XPath Parser multiple predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[5][@type=\"warning\"]");
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    
    REQUIRE(step != nullptr);
    CHECK(step->predicates.size() == 2); 
    CHECK(dynamic_cast<Parser::Number*>(step->predicates[0].get()) != nullptr);
    CHECK(dynamic_cast<Parser::BinaryOp*>(step->predicates[1].get()) != nullptr);
}

TEST_CASE("XPath Parser predicate with child string value") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("chapter[title=\"Introduction\"]");

    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    Parser::BinaryOp* pred = dynamic_cast<Parser::BinaryOp*>(step->predicates[0].get());
    
    Parser::Step* titleStep = dynamic_cast<Parser::Step*>(pred->left.get());
    CHECK(titleStep->test == "title");
    CHECK(titleStep->axis == "child");
}

TEST_CASE("XPath Parser predicate existence test") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("chapter[title]");

    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    Parser::Step* predStep = dynamic_cast<Parser::Step*>(step->predicates[0].get());
    CHECK(predStep->test == "title");
}

TEST_CASE("XPath Parser boolean and in predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("employee[@secretary and @assistant]");

    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    Parser::BinaryOp* andOp = dynamic_cast<Parser::BinaryOp*>(step->predicates[0].get());
    
    REQUIRE(andOp != nullptr);
    CHECK(andOp->op == "and");

    Parser::Step* left = dynamic_cast<Parser::Step*>(andOp->left.get());
    CHECK(left->axis == "attribute");
    CHECK(left->test == "secretary");

    Parser::Step* right = dynamic_cast<Parser::Step*>(andOp->right.get());
    CHECK(right->axis == "attribute");
    CHECK(right->test == "assistant");
}

TEST_CASE("XPath Parser precedence step") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    std::unique_ptr<Parser::AstNode> ast = parse("ancestor::div[1]");
    Parser::Step* step = dynamic_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "ancestor");
    CHECK(step->test == "div");
    CHECK(step->predicates.size() == 1);
}

TEST_CASE("XPath Parser precedence grouping") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    std::unique_ptr<Parser::AstNode> ast = parse("(ancestor::div)[1]");
    Parser::FilterExpr* filter = dynamic_cast<Parser::FilterExpr*>(ast.get());
    REQUIRE(filter != nullptr);
    
    Parser::Step* step = dynamic_cast<Parser::Step*>(filter->subject.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "ancestor");
    CHECK(step->predicates.empty());

    REQUIRE(filter->predicates.size() == 1);
    CHECK(dynamic_cast<Parser::Number*>(filter->predicates[0].get())->num == "1");
}

TEST_CASE("XPath Parser arithmetic precedence") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("5 + 3 * 2");
    
    Parser::BinaryOp* plusOp = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(plusOp != nullptr);
    CHECK(plusOp->op == "+");

    Parser::BinaryOp* multOp = dynamic_cast<Parser::BinaryOp*>(plusOp->right.get());
    REQUIRE(multOp != nullptr);
    CHECK(multOp->op == "*");
}

TEST_CASE("XPath Parser unary minus") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("-child::para");

    Parser::BinaryOp* subOp = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(subOp != nullptr);
    CHECK(subOp->op == "-");

    Parser::Number* zero = dynamic_cast<Parser::Number*>(subOp->left.get());
    CHECK(zero->num == "0");

    Parser::Step* step = dynamic_cast<Parser::Step*>(subOp->right.get());
    CHECK(step->test == "para");
}

TEST_CASE("XPath Parser complex union") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("book | cd");

    Parser::BinaryOp* pipeOp = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(pipeOp != nullptr);
    CHECK(pipeOp->op == "|");
}

TEST_CASE("XPath Parser nested functions") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("substring-before(@src, '?')");

    Parser::FunctionCall* func = dynamic_cast<Parser::FunctionCall*>(ast.get());
    REQUIRE(func != nullptr);
    CHECK(func->name == "substring-before");
    REQUIRE(func->args.size() == 2);

    Parser::Step* arg1 = dynamic_cast<Parser::Step*>(func->args[0].get()); // @src
    CHECK(arg1->axis == "attribute");

    Parser::Literal* arg2 = dynamic_cast<Parser::Literal*>(func->args[1].get()); // '?'
    CHECK(arg2->value == "?");
}

TEST_CASE("XPath Parser complex filter on path") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("book/author[name='Bob'][1]");
    
    Parser::BinaryOp* path = dynamic_cast<Parser::BinaryOp*>(ast.get());
    Parser::Step* authorStep = dynamic_cast<Parser::Step*>(path->right.get());
    
    REQUIRE(authorStep != nullptr);
    CHECK(authorStep->test == "author");
    CHECK(authorStep->predicates.size() == 2);
}

TEST_CASE("XPath Parser union precedence") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("book | cd | dvd");

    Parser::BinaryOp* topPipe = dynamic_cast<Parser::BinaryOp*>(ast.get());
    CHECK(dynamic_cast<Parser::Step*>(topPipe->right.get())->test == "dvd");

    Parser::BinaryOp* leftPipe = dynamic_cast<Parser::BinaryOp*>(topPipe->left.get());
    CHECK(dynamic_cast<Parser::Step*>(leftPipe->right.get())->test == "cd");
}

TEST_CASE("XPath Parser ambiguous operators and names") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("div div mod * - 5");

    Parser::BinaryOp* multOp = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(multOp != nullptr);
    CHECK(multOp->op == "*");

    Parser::BinaryOp* unaryNode = dynamic_cast<Parser::BinaryOp*>(multOp->right.get());
    REQUIRE(unaryNode != nullptr);
    CHECK(unaryNode->op == "-"); 
    CHECK(dynamic_cast<Parser::Number*>(unaryNode->left.get())->num == "0");
    CHECK(dynamic_cast<Parser::Number*>(unaryNode->right.get())->num == "5");

    Parser::BinaryOp* divOp = dynamic_cast<Parser::BinaryOp*>(multOp->left.get());
    REQUIRE(divOp != nullptr);
    CHECK(divOp->op == "div");

    Parser::Step* divName = dynamic_cast<Parser::Step*>(divOp->left.get());
    REQUIRE(divName != nullptr);
    CHECK(divName->axis == "child");
    CHECK(divName->test == "div");

    Parser::Step* modName = dynamic_cast<Parser::Step*>(divOp->right.get());
    REQUIRE(modName != nullptr);
    CHECK(modName->axis == "child");
    CHECK(modName->test == "mod");
}

TEST_CASE("XPath Parser ambiguous wildcard and multiply") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    auto ast = parse("* * *");

    Parser::BinaryOp* multOp = dynamic_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(multOp != nullptr);
    CHECK(multOp->op == "*");

    Parser::Step* left = dynamic_cast<Parser::Step*>(multOp->left.get());
    REQUIRE(left != nullptr);
    CHECK(left->test == "*");
    
    Parser::Step* right = dynamic_cast<Parser::Step*>(multOp->right.get());
    REQUIRE(right != nullptr);
    CHECK(right->test == "*");
}