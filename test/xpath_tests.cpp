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

TEST_CASE("xpath normalize-space function works") {
    using namespace onyx::dynamic::xpath;
    
    REQUIRE(functions::normalizeSpace(" \t\r\n ") == "");
    REQUIRE(functions::normalizeSpace("A\t\r\nB") == "A B");
    REQUIRE(functions::normalizeSpace("\t\nA") == "A");
    REQUIRE(functions::normalizeSpace("A\r ") == "A");
    REQUIRE(functions::normalizeSpace("A\r ") == "A");
    REQUIRE(functions::normalizeSpace("A\r ") == "A");
    REQUIRE(functions::normalizeSpace("A \u00A0 B") == "A \u00A0 B");
    REQUIRE(functions::normalizeSpace("\v") == "\v");
    REQUIRE(functions::normalizeSpace("\f") == "\f");
    REQUIRE(functions::normalizeSpace("") == "");
    REQUIRE(functions::normalizeSpace(" ") == "");
    REQUIRE(functions::normalizeSpace(" A ") == "A");
    REQUIRE(functions::normalizeSpace("A B") == "A B");
    REQUIRE(functions::normalizeSpace(" A B ") == "A B");
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
    REQUIRE(XPathObject(true) == XPathObject("true")); 
    REQUIRE(XPathObject(false) == XPathObject("")); 
    REQUIRE(XPathObject(true) == XPathObject("false"));
    REQUIRE(XPathObject(true) <= XPathObject(1.0));
    REQUIRE(XPathObject(true) >= XPathObject(1.0));
    REQUIRE(XPathObject(false) < XPathObject(1.0));
}

TEST_CASE("XPathObject number vs string comparison") {
    using namespace onyx::dynamic::xpath;
    
    REQUIRE(XPathObject(1.0) == XPathObject("1.0"));
    REQUIRE(XPathObject(1.0) == XPathObject("  1  "));
    REQUIRE(XPathObject(0.5) == XPathObject(".5"));
    REQUIRE(XPathObject("02") == XPathObject(2.0));
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
    
    REQUIRE(XPathObject("foo") == XPathObject("foo"));
    REQUIRE(XPathObject("foo") != XPathObject("Foo"));
    REQUIRE(XPathObject("10") > XPathObject("2")); 
    REQUIRE_FALSE(XPathObject("10") < XPathObject("2"));
    REQUIRE_FALSE(XPathObject("abc") < XPathObject("def"));
    REQUIRE_FALSE(XPathObject("abc") > XPathObject("def"));
    REQUIRE_FALSE(XPathObject("abc") == XPathObject("def"));
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

    REQUIRE(set == XPathObject("A"));
    REQUIRE(set == XPathObject("B"));
    REQUIRE_FALSE(set == XPathObject("C"));
    REQUIRE_FALSE(emptySet == XPathObject(""));
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

    REQUIRE(set == XPathObject("foo"));
    REQUIRE(set != XPathObject("foo"));
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
        .addData(XPathObject("store"))
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
        .addData(XPathObject("price"))
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
        .addData(XPathObject("id"))
        .addData(XPathObject("1"))
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
        .addData(XPathObject("author"))
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
        .addData(XPathObject("price"))
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
        .addData(XPathObject("title"))
        .addData(XPathObject("Second"))
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

TEST_CASE("Virtual machine sum function /root/store[sum(book/price) > 50]") {
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
        .addData(XPathObject("store"))
        .addData(XPathObject("book"))
        .addData(XPathObject("price"))
        .addData(XPathObject(50.0))
        .addData(XPathObject("root"))
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
        .addData(XPathObject("root"))
        .addData(XPathObject("item"))
        .addData(XPathObject("id"))
        
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
        .addData(XPathObject("root"))
        .addData(XPathObject("item"))
        .addData(XPathObject("x"))
        .addData(XPathObject("1"))
        .addData(XPathObject("y"))
        .addData(XPathObject("2"))
        .addData(XPathObject("z"))
        .addData(XPathObject("3"))

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
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::FALSE_0))
                .addInstruction(Instruction(OPCODE::JUMP, 29)) // Jump to Test

                // SUCCESS (28)
                .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::TRUE_0))

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
        .addData(XPathObject("div"))
        .addData(XPathObject("span"))
        
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

TEST_CASE("XPath Lexer basic test expression") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("(ancestor::*)[1]");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::OPENING_PAREN);
    requireToken(lexer, Lexer::TokenType::AXIS_NAME, "ancestor");
    requireToken(lexer, Lexer::TokenType::DOUBLE_COLON);
    requireToken(lexer, Lexer::TokenType::NAME_TEST, "*");
    requireToken(lexer, Lexer::TokenType::CLOSING_PAREN);
    requireToken(lexer, Lexer::TokenType::OPENING_BRACKET);
    requireToken(lexer, Lexer::TokenType::NUMBER, "1");
    requireToken(lexer, Lexer::TokenType::CLOSING_BRACKET);
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
    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    CHECK(step->axis == "child");
    CHECK(step->test == "para");
    CHECK(step->predicates.empty());
}

TEST_CASE("XPath Parser wildcard selection") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("*");

    // Should be child::* (NameTest)
    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    CHECK(step->axis == "child");
    CHECK(step->test == "*");
}

TEST_CASE("XPath Parser text node selection") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("text()");

    // Should be child::text() (NodeType)
    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    CHECK(step->axis == "child");
    CHECK(step->test == "text()"); 
}

TEST_CASE("XPath Parser attribute selection abbreviation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("@name");

    // Should be attribute::name
    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    CHECK(step->axis == "attribute");
    CHECK(step->test == "name");
}

TEST_CASE("XPath Parser attribute wildcard") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("@*");

    // Should be attribute::*
    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    CHECK(step->axis == "attribute");
    CHECK(step->test == "*");
}

TEST_CASE("XPath Parser numeric predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[1]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    CHECK(step->test == "para");
    REQUIRE(step->predicates.size() == 1);
    
    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::Number);
    Parser::Number* num = static_cast<Parser::Number*>(step->predicates[0].get());
    REQUIRE(num != nullptr);
    CHECK(num->num == "1");
}

TEST_CASE("XPath Parser function predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[last()]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step->predicates.size() == 1);

    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::FunctionCall);
    Parser::FunctionCall* func = static_cast<Parser::FunctionCall*>(step->predicates[0].get());
    CHECK(func->name == "last");
    CHECK(func->args.empty());
}

TEST_CASE("XPath Parser relative path with wildcard") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("*/para");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* binary = static_cast<Parser::BinaryOp*>(ast.get());
    CHECK(binary->op == "/");

    REQUIRE(binary->left->getType() == Parser::AstNode::Step);
    Parser::Step* left = static_cast<Parser::Step*>(binary->left.get());
    CHECK(left->test == "*");

    REQUIRE(binary->right->getType() == Parser::AstNode::Step);
    Parser::Step* right = static_cast<Parser::Step*>(binary->right.get());
    REQUIRE(right != nullptr);
    CHECK(right->test == "para");
}

TEST_CASE("XPath Parser absolute path with predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("/doc/chapter[5]/section[2]");
    
    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* op2 = static_cast<Parser::BinaryOp*>(ast.get()); 
    
    REQUIRE(op2->right->getType() == Parser::AstNode::Step);
    Parser::Step* section = static_cast<Parser::Step*>(op2->right.get());
    CHECK(section->test == "section");
    CHECK(section->predicates.size() == 1);

    REQUIRE(op2->left->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* op1 = static_cast<Parser::BinaryOp*>(op2->left.get());

    REQUIRE(op1->right->getType() == Parser::AstNode::Step);
    Parser::Step* chapter = static_cast<Parser::Step*>(op1->right.get());
    CHECK(chapter->test == "chapter");
    CHECK(chapter->predicates.size() == 1);

    REQUIRE(op1->left->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* op0 = static_cast<Parser::BinaryOp*>(op1->left.get());
    
    REQUIRE(op0->left->getType() == Parser::AstNode::RootNode);
    
    REQUIRE(op0->right->getType() == Parser::AstNode::Step);
    Parser::Step* doc = static_cast<Parser::Step*>(op0->right.get());
    CHECK(doc->test == "doc");
}

TEST_CASE("XPath Parser double slash expansion") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    // Should expand to chapter / descendant-or-self::node() / para
    std::unique_ptr<Parser::AstNode> ast = parse("chapter//para");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* finalSlash = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(finalSlash != nullptr);
    
    REQUIRE(finalSlash->right->getType() == Parser::AstNode::Step);
	Parser::Step* right = static_cast<Parser::Step*>(finalSlash->right.get());
    CHECK(right->test == "para");

    REQUIRE(finalSlash->left->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* firstSlash = static_cast<Parser::BinaryOp*>(finalSlash->left.get());
    REQUIRE(firstSlash != nullptr);

    REQUIRE(firstSlash->left->getType() == Parser::AstNode::Step);
	Parser::Step* chapter = static_cast<Parser::Step*>(firstSlash->left.get());
    CHECK(chapter->test == "chapter");

    REQUIRE(firstSlash->right->getType() == Parser::AstNode::Step);
	Parser::Step* hidden = static_cast<Parser::Step*>(firstSlash->right.get());
    REQUIRE(hidden != nullptr);
    CHECK(hidden->axis == "descendant-or-self");
    CHECK(hidden->test == "node()");
}

TEST_CASE("XPath Parser absolute double slash") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    // Root / descendant-or-self::node() / para
    std::unique_ptr<Parser::AstNode> ast = parse("//para");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* finalSlash = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(finalSlash != nullptr);

    REQUIRE(finalSlash->left->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* firstSlash = static_cast<Parser::BinaryOp*>(finalSlash->left.get());
    REQUIRE(firstSlash != nullptr);

    CHECK(static_cast<Parser::RootNode*>(firstSlash->left.get()) != nullptr);

    REQUIRE(firstSlash->right->getType() == Parser::AstNode::Step);
	Parser::Step* hidden = static_cast<Parser::Step*>(firstSlash->right.get());
    CHECK(hidden->axis == "descendant-or-self");
}

TEST_CASE("XPath Parser context node abbreviation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse(".");
    
    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "self");
    CHECK(step->test == "node()");
}

TEST_CASE("XPath Parser parent node abbreviation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("..");
    
    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "parent");
    CHECK(step->test == "node()");
}

TEST_CASE("XPath Parser parent attribute path") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("../@lang");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* binary = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(binary != nullptr);

    REQUIRE(binary->left->getType() == Parser::AstNode::Step);
	Parser::Step* left = static_cast<Parser::Step*>(binary->left.get());
    CHECK(left->axis == "parent");

    REQUIRE(binary->right->getType() == Parser::AstNode::Step);
	Parser::Step* right = static_cast<Parser::Step*>(binary->right.get());
    CHECK(right->axis == "attribute");
    CHECK(right->test == "lang");
}

TEST_CASE("XPath Parser predicate with attribute equality") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[@type=\"warning\"]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    REQUIRE(step->predicates.size() == 1);

    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* eqOp = static_cast<Parser::BinaryOp*>(step->predicates[0].get());
    REQUIRE(eqOp != nullptr);
    CHECK(eqOp->op == "=");

    REQUIRE(eqOp->left->getType() == Parser::AstNode::Step);
	Parser::Step* left = static_cast<Parser::Step*>(eqOp->left.get());
    CHECK(left->axis == "attribute");
    CHECK(left->test == "type");

    REQUIRE(eqOp->right->getType() == Parser::AstNode::Literal);
	Parser::Literal* right = static_cast<Parser::Literal*>(eqOp->right.get());
    CHECK(right->value == "warning");
}

TEST_CASE("XPath Parser multiple predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("para[5][@type=\"warning\"]");
    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    
    REQUIRE(step != nullptr);
    CHECK(step->predicates.size() == 2); 
    CHECK(step->predicates[0]->getType() == Parser::AstNode::Number);
    CHECK(step->predicates[1]->getType() == Parser::AstNode::BinaryOp);
}

TEST_CASE("XPath Parser predicate with child string value") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("chapter[title=\"Introduction\"]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* pred = static_cast<Parser::BinaryOp*>(step->predicates[0].get());
    
    REQUIRE(pred->left->getType() == Parser::AstNode::Step);
	Parser::Step* titleStep = static_cast<Parser::Step*>(pred->left.get());
    CHECK(titleStep->test == "title");
    CHECK(titleStep->axis == "child");
}

TEST_CASE("XPath Parser predicate existence test") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("chapter[title]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::Step);
	Parser::Step* predStep = static_cast<Parser::Step*>(step->predicates[0].get());
    CHECK(predStep->test == "title");
}

TEST_CASE("XPath Parser boolean and in predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("employee[@secretary and @assistant]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* andOp = static_cast<Parser::BinaryOp*>(step->predicates[0].get());
    
    REQUIRE(andOp != nullptr);
    CHECK(andOp->op == "and");

    REQUIRE(andOp->left->getType() == Parser::AstNode::Step);
	Parser::Step* left = static_cast<Parser::Step*>(andOp->left.get());
    CHECK(left->axis == "attribute");
    CHECK(left->test == "secretary");

    REQUIRE(andOp->right->getType() == Parser::AstNode::Step);
	Parser::Step* right = static_cast<Parser::Step*>(andOp->right.get());
    CHECK(right->axis == "attribute");
    CHECK(right->test == "assistant");
}

TEST_CASE("XPath Parser precedence step") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    std::unique_ptr<Parser::AstNode> ast = parse("ancestor::div[1]");
    REQUIRE(ast->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "ancestor");
    CHECK(step->test == "div");
    CHECK(step->predicates.size() == 1);
}

TEST_CASE("XPath Parser precedence grouping") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    std::unique_ptr<Parser::AstNode> ast = parse("(ancestor::div)[1]");
    REQUIRE(ast->getType() == Parser::AstNode::FilterExpr);
	Parser::FilterExpr* filter = static_cast<Parser::FilterExpr*>(ast.get());
    REQUIRE(filter != nullptr);
    
    REQUIRE(filter->subject->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(filter->subject.get());
    REQUIRE(step != nullptr);
    CHECK(step->axis == "ancestor");
    CHECK(step->predicates.empty());

    REQUIRE(filter->predicates.size() == 1);
    REQUIRE(filter->predicates[0]->getType() == Parser::AstNode::Number);
    CHECK(static_cast<Parser::Number*>(filter->predicates[0].get())->num == "1");
}

TEST_CASE("XPath Parser arithmetic precedence") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("5 + 3 * 2");
    
    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* plusOp = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(plusOp != nullptr);
    CHECK(plusOp->op == "+");

    REQUIRE(plusOp->right->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* multOp = static_cast<Parser::BinaryOp*>(plusOp->right.get());
    REQUIRE(multOp != nullptr);
    CHECK(multOp->op == "*");
}

TEST_CASE("XPath Parser unary minus") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("-child::para");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* subOp = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(subOp != nullptr);
    CHECK(subOp->op == "-");

    REQUIRE(subOp->left->getType() == Parser::AstNode::Number);
	Parser::Number* zero = static_cast<Parser::Number*>(subOp->left.get());
    CHECK(zero->num == "0");

    REQUIRE(subOp->right->getType() == Parser::AstNode::Step);
	Parser::Step* step = static_cast<Parser::Step*>(subOp->right.get());
    CHECK(step->test == "para");
}

TEST_CASE("XPath Parser complex union") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("book | cd");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* pipeOp = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(pipeOp != nullptr);
    CHECK(pipeOp->op == "|");
}

TEST_CASE("XPath Parser nested functions") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("substring-before(@src, '?')");

    REQUIRE(ast->getType() == Parser::AstNode::FunctionCall);
	Parser::FunctionCall* func = static_cast<Parser::FunctionCall*>(ast.get());
    REQUIRE(func != nullptr);
    CHECK(func->name == "substring-before");
    REQUIRE(func->args.size() == 2);

    REQUIRE(func->args[0]->getType() == Parser::AstNode::Step);
	Parser::Step* arg1 = static_cast<Parser::Step*>(func->args[0].get()); // @src
    CHECK(arg1->axis == "attribute");

    REQUIRE(func->args[1]->getType() == Parser::AstNode::Literal);
	Parser::Literal* arg2 = static_cast<Parser::Literal*>(func->args[1].get()); // '?'
    CHECK(arg2->value == "?");
}

TEST_CASE("XPath Parser complex filter on path") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("book/author[name='Bob'][1]");
    
    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* path = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(path->right->getType() == Parser::AstNode::Step);
	Parser::Step* authorStep = static_cast<Parser::Step*>(path->right.get());
    
    REQUIRE(authorStep != nullptr);
    CHECK(authorStep->test == "author");
    CHECK(authorStep->predicates.size() == 2);
}

TEST_CASE("XPath Parser union precedence") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("book | cd | dvd");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* topPipe = static_cast<Parser::BinaryOp*>(ast.get());
    CHECK(static_cast<Parser::Step*>(topPipe->right.get())->test == "dvd");

    REQUIRE(topPipe->left->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* leftPipe = static_cast<Parser::BinaryOp*>(topPipe->left.get());
    CHECK(static_cast<Parser::Step*>(leftPipe->right.get())->test == "cd");
}

TEST_CASE("XPath Parser ambiguous operators and names") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("div div mod * - 5");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* multOp = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(multOp != nullptr);
    CHECK(multOp->op == "*");

    REQUIRE(multOp->right->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* unaryNode = static_cast<Parser::BinaryOp*>(multOp->right.get());
    REQUIRE(unaryNode != nullptr);
    CHECK(unaryNode->op == "-"); 
    CHECK(static_cast<Parser::Number*>(unaryNode->left.get())->num == "0");
    CHECK(static_cast<Parser::Number*>(unaryNode->right.get())->num == "5");

    REQUIRE(multOp->left->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* divOp = static_cast<Parser::BinaryOp*>(multOp->left.get());
    REQUIRE(divOp != nullptr);
    CHECK(divOp->op == "div");

    REQUIRE(divOp->left->getType() == Parser::AstNode::Step);
	Parser::Step* divName = static_cast<Parser::Step*>(divOp->left.get());
    REQUIRE(divName != nullptr);
    CHECK(divName->axis == "child");
    CHECK(divName->test == "div");

    REQUIRE(divOp->right->getType() == Parser::AstNode::Step);
	Parser::Step* modName = static_cast<Parser::Step*>(divOp->right.get());
    REQUIRE(modName != nullptr);
    CHECK(modName->axis == "child");
    CHECK(modName->test == "mod");
}

TEST_CASE("XPath Parser ambiguous wildcard and multiply") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast = parse("* * *");

    REQUIRE(ast->getType() == Parser::AstNode::BinaryOp);
	Parser::BinaryOp* multOp = static_cast<Parser::BinaryOp*>(ast.get());
    REQUIRE(multOp != nullptr);
    CHECK(multOp->op == "*");

    REQUIRE(multOp->left->getType() == Parser::AstNode::Step);
	Parser::Step* left = static_cast<Parser::Step*>(multOp->left.get());
    REQUIRE(left != nullptr);
    CHECK(left->test == "*");
    
    REQUIRE(multOp->right->getType() == Parser::AstNode::Step);
	Parser::Step* right = static_cast<Parser::Step*>(multOp->right.get());
    REQUIRE(right != nullptr);
    CHECK(right->test == "*");
}

#include <iostream>

TEST_CASE("XPath Compiler does not create duplicate data") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    parser::StringCursor cursor("div[@class='2']/div[2*2]/@id | div[@class='2']/div[2*2]/@id");
    Lexer lexer(cursor);
    Parser parser(lexer);
    Compiler compiler(std::move(parser.buildAST()));
    std::unique_ptr<Program> pr = compiler.compile();

    // "div", "class", "2", 2, "id"
    REQUIRE(pr->getData().size() == 5);
}

TEST_CASE("XPath execute /store") {
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

    XPathQuery::Result res1 = std::move(XPathQuery("/store").execute(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    REQUIRE(res.asNodeset()[0] == &doc);
}

TEST_CASE("XPath execute /store/book") {
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

    XPathQuery::Result res1 = XPathQuery("/store/book").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
    REQUIRE(nodeset[1]->getTagName() == "book");
    REQUIRE(nodeset[1]->getAttributeValue("id") == "2");
}

TEST_CASE("XPath execute predicate /store/book[price > 15]") {
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

    XPathQuery::Result res1 = XPathQuery("/store/book[price > 15]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
}

TEST_CASE("XPath execute attribute Test /store/book[@id='1']") {
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

    XPathQuery::Result res1 = XPathQuery("/store/book[@id='1']").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute empty /store/book/author") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book1"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Book2")))
    );

    XPathQuery::Result res1 = XPathQuery("/store/book/author").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().empty());
}

TEST_CASE("XPath execute math /store/book[price div 2 < 15]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("price", false, Text("20"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("price", false, Text("40")))
    );

    XPathQuery::Result res1 = XPathQuery("/store/book[price div 2 < 15]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute function composition book[not(starts-with(title, 'Second'))]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", false,
        GenericNode("book", false, Attribute("id", "1"),
            GenericNode("title", false, Text("Book"))),
        GenericNode("book", false, Attribute("id", "2"),
            GenericNode("title", false, Text("Second Book")))
    );

    XPathQuery::Result res1 = XPathQuery("book[not(starts-with(title, 'Second'))]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute sum function /root/store[sum(book/price) > 50]") {
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

    XPathQuery::Result res1 = XPathQuery("/root/store[sum(book/price) > 50]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "store");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
}

TEST_CASE("XPath execute attributes /root/item/@id") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("item", false, Attribute("id", "a")),
        GenericNode("item", false, Attribute("id", "b"), Attribute("other", "x"))
    );

    XPathQuery::Result res1 = XPathQuery("/root/item/@id").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getXPathType() == Node::XPathType::ATTRIBUTE);
    REQUIRE(nodeset[0]->getStringValue() == "a"); 
    REQUIRE(nodeset[1]->getXPathType() == Node::XPathType::ATTRIBUTE);
    REQUIRE(nodeset[1]->getStringValue() == "b"); 
}

TEST_CASE("XPath execute booleans item[@x='1' and (@y='2' or @z='3')]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("item", false, Attribute("id", "1"), Attribute("x", "1"), Attribute("y", "2")),
        GenericNode("item", false, Attribute("id", "2"), Attribute("x", "1"), Attribute("z", "3")),
        GenericNode("item", false, Attribute("id", "3"), Attribute("x", "1"), Attribute("y", "9")),
        GenericNode("item", false, Attribute("id", "4"), Attribute("x", "0"), Attribute("y", "2"))
    );

    XPathQuery::Result res1 = XPathQuery("item[@x='1' and (@y='2' or @z='3')]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getAttributes()[0].getValue()== "1");
    REQUIRE(nodeset[1]->getAttributes()[0].getValue() == "2");
}

TEST_CASE("XPath execute booleans item[@x=$var1 and (@y=$var2 or @z=$var3)] with variables") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("item", false, Attribute("id", "1"), Attribute("x", "1"), Attribute("y", "2")),
        GenericNode("item", false, Attribute("id", "2"), Attribute("x", "1"), Attribute("z", "3")),
        GenericNode("item", false, Attribute("id", "3"), Attribute("x", "1"), Attribute("y", "9")),
        GenericNode("item", false, Attribute("id", "4"), Attribute("x", "0"), Attribute("y", "2"))
    );

    XPathQuery::Result res1 = XPathQuery("item[@x=$var1 and (@y=$var2 or @z=$var3)]").execute(&doc, 
                                                        [](std::string_view name) -> XPathObject {
                                                            if (name == "var1") return XPathObject("1");
                                                            if (name == "var2") return XPathObject("2");
                                                            if (name == "var3") return XPathObject("3");
                                                            throw std::runtime_error("Unknown variable");
                                                        });
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getAttributes()[0].getValue()== "1");
    REQUIRE(nodeset[1]->getAttributes()[0].getValue() == "2");
}

TEST_CASE("XPath execute union operator //div | //span") {
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

    XPathQuery::Result res1 = XPathQuery("//div | //span").execute(&doc);
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

TEST_CASE("XPath execute (1 < 5 - 6)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false);

    XPathQuery::Result res1 = XPathQuery("(1 < 5 - 6)").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isBool());
    REQUIRE_FALSE(res.asBool());
}

TEST_CASE("XPath execute concat() with many arguments") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false);

    XPathQuery::Result res1 = XPathQuery("concat('a', 'b', 'c', 'd', 'e')").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isString());
    REQUIRE(res.asString() == "abcde");
}

TEST_CASE("XPath execute arithmetic (1 + 2 * 3 * (6 div 2) - 3)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false);

    XPathQuery::Result res1 = XPathQuery("(1 + 2 * 3 * (6 div 2) - 5)").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNumber());
    REQUIRE(res.asNumber() == Catch::Approx(14));
}

TEST_CASE("XPath execute axis precedence (ancestor vs paren-ancestor)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic::tags;
    using namespace onyx::dynamic;

    GenericNode doc("root", false,
        GenericNode("grandparent", false,
            GenericNode("parent", false,
                GenericNode("child", false)
            )
        )
    );

    Node* childNode = doc.getChildren()[0]->getChildren()[0]->getChildren()[0];

    // ancestor::*[1] should be 'parent'
    XPathQuery::Result res1 = XPathQuery("ancestor::*[1]").execute(childNode);
    REQUIRE(res1.object.asNodeset()[0]->getTagName() == "parent");

    // (ancestor::*)[1] should be 'root' (first in document order)
    XPathQuery::Result res2 = XPathQuery("(ancestor::*)[1]").execute(childNode);
    REQUIRE(res2.object.asNodeset()[0]->getTagName() == "root");
}

TEST_CASE("XPath execute implicit numeric predicates and last()") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("list", false,
        GenericNode("item", false, Text("A")),
        GenericNode("item", false, Text("B")),
        GenericNode("item", false, Text("C")),
        GenericNode("item", false, Text("D"))
    );

    XPathQuery::Result res1 = XPathQuery("item[2]").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->serialize() == "B");

    XPathQuery::Result res2 = XPathQuery("item[last()]").execute(&doc);
    REQUIRE(res2.object.asNodeset().size() == 1);
    REQUIRE(res2.object.asNodeset()[0]->getFirstChild()->serialize() == "D");

    XPathQuery::Result res3 = XPathQuery("item[last()-1]").execute(&doc);
    REQUIRE(res3.object.asNodeset().size() == 1);
    REQUIRE(res3.object.asNodeset()[0]->getFirstChild()->serialize() == "C");
}

TEST_CASE("XPath execute following-sibling and preceding-sibling") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("data", false,
        GenericNode("a", false, Attribute("id", "1")),
        GenericNode("b", false, Attribute("id", "2")),
        GenericNode("c", false, Attribute("id", "3")),
        GenericNode("d", false, Attribute("id", "4"))
    );

    Node* nodeB = doc.getChildren()[1];

    XPathQuery::Result res1 = XPathQuery("following-sibling::node()[1]").execute(nodeB);
    REQUIRE(res1.object.asNodeset()[0]->getAttributeValue("id") == "3");

    XPathQuery::Result res2 = XPathQuery("preceding-sibling::node()[1]").execute(nodeB);
    REQUIRE(res2.object.asNodeset()[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute deep string concatenation (Element String Value)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("doc", false,
        GenericNode("p", false, 
            Text("Hello "),
            GenericNode("b", false, Text("World")),
            Text("!")
        )
    );

    XPathQuery::Result res1 = XPathQuery("string(/doc/p)").execute(&doc);
    REQUIRE(res1.object.asString() == "Hello World!");
}

TEST_CASE("XPath execute complex boolean and string functions") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("inventory", false,
        GenericNode("item", false, 
            Attribute("code", "A-123"), 
            GenericNode("name", false, Text("Widget"))),
        GenericNode("item", false, 
            Attribute("code", "B-456"), 
            GenericNode("name", false, Text("Gadget"))),
        GenericNode("item", false, 
            Attribute("code", "A-789"), 
            GenericNode("name", false, Text("Wodget")))
    );

    // Find items where code starts with 'A' AND name contains 'get' and name doesn't contain 'Gad'
    std::string query = "/inventory/item[starts-with(@code, 'A') and contains(name, 'get') and not(contains(name, 'Gad'))]";
    
    XPathQuery::Result res1 = XPathQuery(query).execute(&doc);
    
    REQUIRE(res1.object.asNodeset().size() == 2);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getTagName() == "name"); 
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getFirstChild()->serialize() == "Widget");
    REQUIRE(res1.object.asNodeset()[1]->getFirstChild()->getTagName() == "name"); 
    REQUIRE(res1.object.asNodeset()[1]->getFirstChild()->getFirstChild()->serialize() == "Wodget");
}

TEST_CASE("XPath execute substring and translate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false);

    XPathQuery::Result res1 = XPathQuery("substring('12345', 2, 3)").execute(&doc);
    REQUIRE(res1.object.asString() == "234");

    XPathQuery::Result res2 = XPathQuery("translate('bar', 'abc', 'ABC')").execute(&doc);
    REQUIRE(res2.object.asString() == "BAr");
    
    XPathQuery::Result res3 = XPathQuery("translate('data', 'd', '')").execute(&doc);
    REQUIRE(res3.object.asString() == "ata");
}

TEST_CASE("XPath execute parent abbreviation (..) and self (.)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("a", false,
        GenericNode("b", false,
            GenericNode("c", false, Attribute("attr", "val"))
        )
    );

    XPathQuery::Result res1 = XPathQuery("/a/b/c/../c/@attr").execute(&doc);
    
    REQUIRE(res1.object.asNodeset().size() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getStringValue() == "val");

    XPathQuery::Result res2 = XPathQuery("/a/b/./c").execute(&doc);
    REQUIRE(res2.object.asNodeset().size() == 1);
    REQUIRE(res2.object.asNodeset()[0]->getTagName() == "c");
}

TEST_CASE("XPath execute nodeset equality (Exists semantics)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("val", false, Text("foo")),
        GenericNode("val", false, Text("bar")),
        GenericNode("val", false, Text("baz"))
    );

    XPathQuery::Result res1 = XPathQuery("/root/val = 'bar'").execute(&doc);
    REQUIRE(res1.object.asBool() == true);

    XPathQuery::Result res2 = XPathQuery("/root/val = 'qux'").execute(&doc);
    REQUIRE(res2.object.asBool() == false);
    
    XPathQuery::Result res3 = XPathQuery("/root/val != 'bar'").execute(&doc);
    REQUIRE(res3.object.asBool() == true);
}

TEST_CASE("XPath execute number formatting and operations") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false);

    REQUIRE(XPathQuery("floor(1.9)").execute(&doc).object.asNumber() == 1.0);
    REQUIRE(XPathQuery("ceiling(1.1)").execute(&doc).object.asNumber() == 2.0);
    REQUIRE(XPathQuery("round(1.5)").execute(&doc).object.asNumber() == 2.0); // 1.5 rounds up
    REQUIRE(XPathQuery("round(1.4)").execute(&doc).object.asNumber() == 1.0);

    REQUIRE(XPathQuery("number('-10.5')").execute(&doc).object.asNumber() == -10.5);
}

TEST_CASE("XPath execute attribute node edge cases") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("item", false, Attribute("id", "1"), Text("Hello"))
    );

    XPathQuery::Result res1 = XPathQuery("/root/item/@id/child::node()").execute(&doc);
    REQUIRE(res1.object.asNodeset().empty());

    XPathQuery::Result res2 = XPathQuery("/root/item/text()/child::node()").execute(&doc);
    REQUIRE(res2.object.asNodeset().empty());
    
    XPathQuery::Result res3 = XPathQuery("/root/item/@id/..").execute(&doc);
    REQUIRE(res3.object.asNodeset().size() == 1);
    REQUIRE(res3.object.asNodeset()[0]->getTagName() == "item");
}

TEST_CASE("XPath execute axis test") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("A", false, Attribute("id", "1"),
            GenericNode("B", false, Attribute("id", "2")),
            GenericNode("C", false, Attribute("id", "3"),
                GenericNode("D", false, Attribute("id", "4"))
            )
        ),
        GenericNode("E", false, Attribute("id", "5"))
    );

    Node* nodeC = doc.getChildren()[0]->getChildren()[1];

    XPathQuery::Result resDesc = XPathQuery("/root/A/descendant::*").execute(&doc);
    REQUIRE(resDesc.object.asNodeset().size() == 3);

    Node* nodeD = nodeC->getChildren()[0];
    XPathQuery::Result resAnc = XPathQuery("ancestor::*").execute(nodeD);
    REQUIRE(resAnc.object.asNodeset().size() == 3);
    REQUIRE(resAnc.object.asNodeset()[0]->getTagName() == "root");

    Node* nodeB = doc.getChildren()[0]->getChildren()[0];
    XPathQuery::Result resFoll = XPathQuery("following::*").execute(nodeB);
    REQUIRE(resFoll.object.asNodeset().size() == 3);
    REQUIRE(resFoll.object.asNodeset()[0]->getAttributeValue("id") == "3");
    REQUIRE(resFoll.object.asNodeset()[2]->getAttributeValue("id") == "5");

    Node* nodeE = doc.getChildren()[1];
    XPathQuery::Result resPrec = XPathQuery("preceding::*").execute(nodeE);
    REQUIRE(resPrec.object.asNodeset().size() == 4);
    
    XPathQuery::Result resSelf = XPathQuery("self::*").execute(nodeC);
    REQUIRE(resSelf.object.asNodeset().size() == 1);
    REQUIRE(resSelf.object.asNodeset()[0] == nodeC);
}

TEST_CASE("XPath execute special node tests") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("elem", false, Text("text1")),
        GenericNode("elem", false, Text("text2")),
        GenericNode("other", false)
    );

    XPathQuery::Result res1 = XPathQuery("/root/*").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 3);

    XPathQuery::Result res2 = XPathQuery("/root/elem/text()").execute(&doc);
    REQUIRE(res2.object.asNodeset().size() == 2);
    REQUIRE(res2.object.asNodeset()[0]->serialize() == "text1");

    XPathQuery::Result res3 = XPathQuery("/root/node()").execute(&doc);
    REQUIRE(res3.object.asNodeset().size() == 3);
}

TEST_CASE("XPath execute finds processing instructions") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    GenericNode doc("root", false,
        ProcessingInstruction("php", "echo \"hello\";"),
        GenericNode("child", false),
        ProcessingInstruction("xml-stylesheet", "type=\"css\""),
        GenericNode("child", false));

    XPathQuery::Result resAllPI = XPathQuery("/root/processing-instruction()").execute(&doc);
    REQUIRE(resAllPI.object.asNodeset().size() == 2);
    
    Node* pi1 = resAllPI.object.asNodeset()[0];
    Node* pi2 = resAllPI.object.asNodeset()[1];
    
    REQUIRE(pi1->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi1)->getTarget() == "php");

    REQUIRE(pi2->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi2)->getTarget() == "xml-stylesheet");

    XPathQuery::Result resSpecificPI = XPathQuery("/root/processing-instruction('php')").execute(&doc);
    REQUIRE(resSpecificPI.object.asNodeset().size() == 1);
    Node* pi3 = resSpecificPI.object.asNodeset()[0];
    REQUIRE(pi3->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi3)->getTarget() == "php");
    REQUIRE(pi3->getStringValue() == "echo \"hello\";");

    XPathQuery::Result resSpecificPI2 = XPathQuery("/root/processing-instruction(\"xml-stylesheet\")").execute(&doc);
    REQUIRE(resSpecificPI2.object.asNodeset().size() == 1);
    Node* pi4 = resSpecificPI2.object.asNodeset()[0];
    REQUIRE(pi4->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi4)->getTarget() == "xml-stylesheet");
    REQUIRE(pi4->getStringValue() == "type=\"css\"");

    XPathQuery::Result resMissingPI = XPathQuery("/root/processing-instruction('unknown')").execute(&doc);
    REQUIRE(resMissingPI.object.asNodeset().empty());
}

TEST_CASE("XPath execute finds comments") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    
    GenericNode doc("root", false,
        GenericNode("child", false),
        Comment("This is a comment"),
        GenericNode("child", false),
        GenericNode("child", false));

    XPathQuery::Result resComments = XPathQuery("/root/comment()").execute(&doc);
    REQUIRE(resComments.object.asNodeset().size() == 1);
    REQUIRE(resComments.object.asNodeset()[0]->getXPathType() == Node::XPathType::COMMENT);
    REQUIRE(resComments.object.asNodeset()[0]->serialize() == "<!--This is a comment-->");
}

TEST_CASE("XPath execute complex path step predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("section", false, Attribute("id", "s1"),
            GenericNode("div", false, Attribute("class", "a")),
            GenericNode("div", false, Attribute("class", "b")),
            GenericNode("div", false, Attribute("class", "a"))
        ),
        GenericNode("section", false, Attribute("id", "s2"),
            GenericNode("div", false, Attribute("class", "a")),
            GenericNode("div", false, Attribute("class", "b"))
        )
    );

    XPathQuery::Result res1 = XPathQuery("/root/section/div[2]").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 2);
    REQUIRE(res1.object.asNodeset()[0]->getAttributeValue("class") == "b");
    REQUIRE(res1.object.asNodeset()[1]->getAttributeValue("class") == "b");

    XPathQuery::Result res2 = XPathQuery("(/root/section/div)[2]").execute(&doc);
    REQUIRE(res2.object.asNodeset().size() == 1);
    REQUIRE(res2.object.asNodeset()[0]->getAttributeValue("class") == "b");
}

TEST_CASE("XPath execute maintains document order on paths with reverse axis") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("section", false, Attribute("id", "s1"),
            GenericNode("div", false, Attribute("class", "a")),
            GenericNode("div", false, Attribute("class", "b")),
            GenericNode("div", false, Attribute("class", "a"))
        ),
        GenericNode("section", false, Attribute("id", "s2"),
            GenericNode("div", false, Attribute("class", "a")),
            GenericNode("div", false, Attribute("class", "b"))
        )
    );

    XPathQuery::Result res1 = XPathQuery("//section[@id='s1']/div[@class = 'a']/ancestor::*").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 2);
    REQUIRE(res1.object.asNodeset()[0]->getTagName() == "root");
    REQUIRE(res1.object.asNodeset()[1]->getTagName() == "section");
}

TEST_CASE("XPath execute correctly unions AttributeViewNode") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("section", false, Attribute("id", "s1"),
            GenericNode("div", false, Attribute("class", "a"), Attribute("id", "1")),
            GenericNode("div", false, Attribute("class", "b"), Attribute("id", "2")),
            GenericNode("div", false, Attribute("class", "a"), Attribute("id", "3"))
        ),
        GenericNode("section", false, Attribute("id", "s2"),
            GenericNode("div", false, Attribute("class", "a"), Attribute("id", "4")),
            GenericNode("div", false, Attribute("class", "b"), Attribute("id", "5"))
        )
    );

    XPathQuery::Result res1 = XPathQuery("//@class[. = 'a'] | //@class[. = 'a']").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 3);
    REQUIRE(res1.object.asNodeset()[0]->getParentNode()->getAttributeValue("id") == "1");
    REQUIRE(res1.object.asNodeset()[1]->getParentNode()->getAttributeValue("id") == "3");
    REQUIRE(res1.object.asNodeset()[2]->getParentNode()->getAttributeValue("id") == "4");
}

TEST_CASE("XPath execute correctly orders AttributeViewNode and RootViewNode") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("section", false, Attribute("id", "s1"),
            GenericNode("div", false, Attribute("class", "a"), Attribute("id", "1")),
            GenericNode("div", false, Attribute("class", "b"), Attribute("id", "2")),
            GenericNode("div", false, Attribute("class", "a"), Attribute("id", "3"))
        ),
        GenericNode("section", false, Attribute("id", "s2"),
            GenericNode("div", false, Attribute("class", "a"), Attribute("id", "4")),
            GenericNode("div", false, Attribute("class", "b"), Attribute("id", "5"))
        )
    );

    XPathQuery::Result res1 = XPathQuery("//div[@class='b'] | //@class[. = 'a'] | /ancestor::node()").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 6);
    REQUIRE(res1.object.asNodeset()[0]->getChildrenCount() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getTagName() == "root");
    REQUIRE(res1.object.asNodeset()[1]->getParentNode()->getAttributeValue("id") == "1");
    REQUIRE(res1.object.asNodeset()[2]->getAttributeValue("id") == "2");
    REQUIRE(res1.object.asNodeset()[3]->getParentNode()->getAttributeValue("id") == "3");
    REQUIRE(res1.object.asNodeset()[4]->getParentNode()->getAttributeValue("id") == "4");
    REQUIRE(res1.object.asNodeset()[5]->getAttributeValue("id") == "5");
}

TEST_CASE("XPath execute multiple predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("list", false,
        GenericNode("item", false, Attribute("id", "1"), Attribute("type", "A")),
        GenericNode("item", false, Attribute("id", "2"), Attribute("type", "B")),
        GenericNode("item", false, Attribute("id", "3"), Attribute("type", "A")),
        GenericNode("item", false, Attribute("id", "4"), Attribute("type", "B"))
    );

    XPathQuery::Result res1 = XPathQuery("item[@type='A'][2]").execute(&doc);
    
    REQUIRE(res1.object.asNodeset().size() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getAttributeValue("id") == "3");
}

TEST_CASE("XPath execute context position shifting in predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("list", false,
        GenericNode("item", false, Attribute("id", "1")),
        GenericNode("item", false, Attribute("id", "2")),
        GenericNode("item", false, Attribute("id", "3")),
        GenericNode("item", false, Attribute("id", "4"))
    );

    XPathQuery::Result res1 = XPathQuery("item[position() != 1][position() != 2]").execute(&doc);

    REQUIRE(res1.object.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res1.object.asNodeset();
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
    REQUIRE(nodeset[1]->getAttributeValue("id") == "4");
}

TEST_CASE("XPath execute complex predicate nesting") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", false,
        GenericNode("section", false, Attribute("status", "open"),
            GenericNode("div", false, Attribute("class", "content"))
        ),
        GenericNode("section", false, Attribute("status", "open"),
            GenericNode("span", false)
        ),
        GenericNode("section", false, Attribute("status", "closed"),
            GenericNode("div", false, Attribute("class", "content")) 
        )
    );

    XPathQuery::Result res1 = XPathQuery("section[@status='open'][div]").execute(&doc);

    REQUIRE(res1.object.asNodeset().size() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getTagName() == "div");
}

TEST_CASE("XPath execute empty and null expressions throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("").execute(&doc));
    REQUIRE_THROWS(XPathQuery("   ").execute(&doc));
}

TEST_CASE("XPath execute unclosed brackets and parenthesis throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("/root/item[@id='1'").execute(&doc));

    REQUIRE_THROWS(XPathQuery("count(/root/item").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("/root/item]").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("/root/item)").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("item[(@id='1']").execute(&doc));

    REQUIRE_THROWS(XPathQuery("count(/root").execute(&doc));

    REQUIRE_THROWS(XPathQuery("/root/item[").execute(&doc));
}

TEST_CASE("XPath execute invalid operator syntax throws") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("1 +").execute(&doc));
    REQUIRE_THROWS(XPathQuery("+ 1").execute(&doc));
    REQUIRE_THROWS(XPathQuery("1 or").execute(&doc));
    REQUIRE_THROWS(XPathQuery("div 5").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("1 ++ 1").execute(&doc));
    REQUIRE_THROWS(XPathQuery("1 div mod 2").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("item[@id = ]").execute(&doc));
}

TEST_CASE("XPath execute invalid path sequences throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("/root/").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("/root//").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("///root").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("child::").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("imaginary-axis::node()").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("/[1]").execute(&doc));
}

TEST_CASE("XPath execute invalid function call arguments throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("count()").execute(&doc));
    REQUIRE_THROWS(XPathQuery("count(1, 2)").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("concat('a')").execute(&doc)); 
    
    REQUIRE_THROWS(XPathQuery("not()").execute(&doc));
    REQUIRE_THROWS(XPathQuery("not(true(), false())").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("foo:bar()").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("unknown-function('arg')").execute(&doc));
}

TEST_CASE("XPath execute invalid predicates throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("item[]").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("[@id='1']").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("item[[1]]").execute(&doc));
}

TEST_CASE("XPath execute lexical errors throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", false);

    REQUIRE_THROWS(XPathQuery("'Unfinished string").execute(&doc));
    
    REQUIRE_THROWS(XPathQuery("1 & 1").execute(&doc));
}