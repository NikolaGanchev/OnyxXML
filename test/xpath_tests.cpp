#include <limits>
#include <string_view>

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

    REQUIRE(functions::string(XPathObject(
                std::numeric_limits<double>::quiet_NaN())) == "NaN");
    REQUIRE(functions::string(XPathObject(+0.0)) == "0");
    REQUIRE(functions::string(XPathObject(-0.0)) == "0");
    REQUIRE(functions::string(XPathObject(
                std::numeric_limits<double>::infinity())) == "Infinity");
    REQUIRE(functions::string(XPathObject(
                -std::numeric_limits<double>::infinity())) == "-Infinity");
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
    std::vector<Node*> nodes1 = {&node1};
    XPathObject obj1(nodes1);

    cdiv node2{span(Text("Deep")), Text(" Text")};
    std::vector<Node*> nodes2 = {&node2};
    XPathObject obj2(nodes2);

    GenericNode node31("div", NonVoid, Attribute("href", "test"));
    AttributeViewNode node32(&node31, 0);
    std::vector<Node*> nodes3 = {&node32, &node31};
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
    REQUIRE(!functions::boolean(
        XPathObject(std::numeric_limits<double>::quiet_NaN())));
    REQUIRE(functions::boolean(
        XPathObject(std::numeric_limits<double>::infinity())));
    REQUIRE(functions::boolean(
        XPathObject(-std::numeric_limits<double>::infinity())));
    REQUIRE(functions::boolean(XPathObject(-1.0)));
    REQUIRE(functions::boolean(XPathObject(0.00000001)));
    REQUIRE(functions::boolean(XPathObject(5.67890)));
}

TEST_CASE("xpath boolean function correctly converts nodeset to boolean") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;

    std::vector<Node*> empty;
    std::vector<Node*> nonEmpty;

    tags::GenericNode node("node", tags::GenericNode::Type::NonVoid);

    nonEmpty.push_back(&node);

    REQUIRE(!functions::boolean(XPathObject(empty)));
    REQUIRE(functions::boolean(XPathObject(nonEmpty)));
}

TEST_CASE("xpath number function correctly converts number to number") {
    using namespace onyx::dynamic::xpath;

    REQUIRE(functions::number(XPathObject(1.0)) == 1.0);
    REQUIRE(functions::number(XPathObject(0.0)) == 0.0);
    REQUIRE(functions::number(
        XPathObject(std::isnan(std::numeric_limits<double>::quiet_NaN()))));
    REQUIRE(functions::number(
                XPathObject(std::numeric_limits<double>::infinity())) ==
            std::numeric_limits<double>::infinity());
    REQUIRE(functions::number(
                XPathObject(-std::numeric_limits<double>::infinity())) ==
            -std::numeric_limits<double>::infinity());
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
    std::vector<Node*> nodes1 = {&node1};
    XPathObject obj1(nodes1);

    cdiv node2{span(Text("1")), Text(".2")};
    std::vector<Node*> nodes2 = {&node2};
    XPathObject obj2(nodes2);

    GenericNode node31("div", NonVoid, Attribute("href", "2.3"));
    AttributeViewNode node32(&node31, 0);
    std::vector<Node*> nodes3 = {&node32, &node31};
    XPathObject obj3(nodes3);

    cdiv node4{span(Text("1")), Text(".2 test")};
    std::vector<Node*> nodes4 = {&node4};
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

    REQUIRE_THROWS_WITH(XPathObject(1.0).asNodeset(),
                        "Tried to cast non-nodeset to nodeset");
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
    std::vector<Node*> nodes = {&node10, &node20};
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
    std::vector<Node*> nodes2 = {&node2};
    XPathObject set2(nodes2);

    REQUIRE(set2 != XPathObject(1.0));
    REQUIRE_FALSE(set2 == XPathObject(1.0));
}

TEST_CASE("XPathObject nodeset vs string comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text nodeA{"A"};
    Text nodeB{"B"};
    std::vector<Node*> nodes = {&nodeA, &nodeB};
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
    std::vector<Node*> nodes = {&node1, &node2};
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
    std::vector<Node*> nodes1 = {&node11, &node12};

    Text node21{"2"};
    Text node22{"3"};
    std::vector<Node*> nodes2 = {&node21, &node22};

    XPathObject set1(nodes1);
    XPathObject set2(nodes2);

    REQUIRE(set1 == set2);
    REQUIRE(set1 != set2);
}

TEST_CASE("XPathObject nodeset vs nodeset subset comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text node1{"1"};
    std::vector<Node*> nodes1 = {&node1};

    Text node21{"1"};
    Text node22{"2"};
    std::vector<Node*> nodes2 = {&node21, &node22};

    XPathObject set1(nodes1);
    XPathObject set2(nodes2);

    REQUIRE(set1 == set2);
    REQUIRE(set1 != set2);
}

TEST_CASE("XPathObject nodeset vs nodeset single match comparison") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    Text node1{"1"};
    std::vector<Node*> nodes1 = {&node1};

    Text node2{"1"};
    std::vector<Node*> nodes2 = {&node2};

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

    std::vector<Node*> nodesA = {&node10, &node20};
    XPathObject setA(nodesA);

    std::vector<Node*> nodesB = {&node5};
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
    std::vector<Node*> nodes = {&nodeA, &nodeB};
    XPathObject set(nodes);

    REQUIRE(set == XPathObject("foo"));
    REQUIRE(set != XPathObject("foo"));
}

TEST_CASE("Virtual machine runs") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

    // 0="store", 1="book"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            // Select /store
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 10))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

    // 0="store", 1="book", 2="price", 3=15.0
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            .addData(XPathObject("price"))
            .addData(XPathObject(15.0))
            // Select /store
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 19))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(
                Instruction(OPCODE::LOOP_ENTER, 17))  // Jump to UNION
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))
            .addInstruction(
                Instruction(OPCODE::COMPARE, COMPARE_MODE::GREATER_THAN))
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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

    // 0="store", 1="book", 2="id", 3="1"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            .addData(XPathObject("id"))
            .addData(XPathObject("1"))
            // Select /store
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 19))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 17))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))  // "id"
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))  // "1"
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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2"))));

    // 0="store", 1="book", 2="author"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            .addData(XPathObject("author"))
            // 1. Select /store
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 16))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 14))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))  // "author"
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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("price", NonVoid, Text("20"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("price", NonVoid, Text("40"))));

    // 0="store", 1="book", 2="price", 3=2.0, 4=15.0
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            .addData(XPathObject("price"))
            .addData(XPathObject(2.0))
            .addData(XPathObject(15.0))
            // Select /store/book
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 23))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 21))
            // Select 'price'
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            // Load 2
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))
            .addInstruction(
                Instruction(OPCODE::CALCULATE, CALCULATE_MODE::DIVIDE))
            // Load 15
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 5))
            .addInstruction(
                Instruction(OPCODE::COMPARE, COMPARE_MODE::LESS_THAN))
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

TEST_CASE(
    "Virtual machine function composition book[not(starts-with(title, "
    "'Second'))]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "store", NonVoid,
        GenericNode("book", NonVoid, Attribute("id", "1"),
                    GenericNode("title", NonVoid, Text("Book"))),
        GenericNode("book", NonVoid, Attribute("id", "2"),
                    GenericNode("title", NonVoid, Text("Second Book"))));

    // 0="store", 1="book", 2="title", 3="Second"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            .addData(XPathObject("title"))
            .addData(XPathObject("Second"))
            // Select /store/book
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 23))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 21))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))
            .addInstruction(
                Instruction(OPCODE::CALL, FUNCTION_CODE::STARTS_WITH_2))
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

    GenericNode doc(
        "root", NonVoid,
        GenericNode("store", NonVoid, Attribute("id", "1"),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("20")))),
        GenericNode("store", NonVoid, Attribute("id", "2"),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("40"))),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("30")))));

    // 0="store", 1="book", 2="price", 3=50.0, 4="root"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("store"))
            .addData(XPathObject("book"))
            .addData(XPathObject("price"))
            .addData(XPathObject(50.0))
            .addData(XPathObject("root"))
            // Select /root/store
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 5))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 28))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))  // store
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 26))

            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))  // book
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 20))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))  // price
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))

            // Call sum()
            .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::SUM_1))

            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))  // 50.0
            .addInstruction(
                Instruction(OPCODE::COMPARE, COMPARE_MODE::GREATER_THAN))
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

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "a")),
                    GenericNode("item", NonVoid, Attribute("id", "b"),
                                Attribute("other", "x")));

    // 0="root", 1="item", 2="id"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("root"))
            .addData(XPathObject("item"))
            .addData(XPathObject("id"))

            // Select /root
            .addInstruction(Instruction(OPCODE::LOAD_ROOT))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 16))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))  // "item"
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 14))
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))  // "id"
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

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "1"),
                                Attribute("x", "1"), Attribute("y", "2")),
                    GenericNode("item", NonVoid, Attribute("id", "2"),
                                Attribute("x", "1"), Attribute("z", "3")),
                    GenericNode("item", NonVoid, Attribute("id", "3"),
                                Attribute("x", "1"), Attribute("y", "9")),
                    GenericNode("item", NonVoid, Attribute("id", "4"),
                                Attribute("x", "0"), Attribute("y", "2")));

    // 0="root", 1="item", 2="x", 3="1", 4="y", 5="2", 6="z", 7="3"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
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
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))
            .addInstruction(
                Instruction(OPCODE::LOOP_ENTER, 33))  // Skip to HALT
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::CHILD))

            .addInstruction(Instruction(OPCODE::LOOP_ENTER, 32))

            // Check @x == '1'
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 3))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 4))
            .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
            // If False, Jump to FAIL
            .addInstruction(Instruction(OPCODE::JUMP_F, 31))

            // Check (@y='2' OR @z='3')

            // Check @y == '2'
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 5))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 6))
            .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
            // If True, Jump to SUCCESS
            .addInstruction(Instruction(OPCODE::JUMP_T, 33))

            // Check @z == '3'
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 7))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::ATTRIBUTE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 8))
            .addInstruction(Instruction(OPCODE::COMPARE, COMPARE_MODE::EQUAL))
            // If True, Jump to SUCCESS
            .addInstruction(Instruction(OPCODE::JUMP_T, 33))

            // FAIL (31)
            .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::FALSE_0))
            .addInstruction(Instruction(OPCODE::JUMP, 34))  // Jump to Test

            // SUCCESS (33)
            .addInstruction(Instruction(OPCODE::CALL, FUNCTION_CODE::TRUE_0))

            // TEST (34)
            .addInstruction(Instruction(OPCODE::CONTEXT_NODE_TEST))
            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::LOOP_UNION))

            .addInstruction(Instruction(OPCODE::LOOP_NEXT))
            .addInstruction(Instruction(OPCODE::HALT))  // 34
            .build();

    VirtualMachine vm(std::move(pr));
    VirtualMachine::ExecutionResult res1 = std::move(vm.executeOn(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getAttributes()[0].getValue() == "1");
    REQUIRE(nodeset[1]->getAttributes()[0].getValue() == "2");
}

TEST_CASE("Virtual machine union Operator //div | //span") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("div", NonVoid, GenericNode("span", NonVoid, Text("A"))),
        GenericNode("section", NonVoid, GenericNode("span", NonVoid, Text("B")),
                    GenericNode("div", NonVoid, Text("C"))));

    // 0="div", 1="span"
    std::unique_ptr<Program> pr =
        Program::Builder()
            .addData(XPathObject(""))
            .addData(XPathObject("div"))
            .addData(XPathObject("span"))

            // Context Node
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))

            // Select //div
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 1))
            .addInstruction(Instruction(OPCODE::SELECT, AXIS::DESCENDANT))

            // Select //span
            .addInstruction(Instruction(OPCODE::LOAD_CONTEXT_NODE))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 0))
            .addInstruction(Instruction(OPCODE::LOAD_CONSTANT, 2))
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

TEST_CASE("XPath Lexer * disambiguation as multiplication") {
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

    requireToken(lexer, Lexer::TokenType::NODE_TYPE,
                 "processing-instruction('xml-stylesheet')");
}

TEST_CASE("XPath Lexer empty processing instruction") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    parser::StringCursor cursor("processing-instruction()");
    Lexer lexer(cursor);

    requireToken(lexer, Lexer::TokenType::NODE_TYPE,
                 "processing-instruction()");
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
    parser::StringCursor cursor(
        "div div mod * .5 | child::* * 5 and node() or "
        "processing-instruction(' ) ') / .. // .");
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
    requireToken(lexer, Lexer::TokenType::NODE_TYPE,
                 "processing-instruction(' ) ')");
    requireToken(lexer, Lexer::TokenType::SLASH);
    requireToken(lexer, Lexer::TokenType::TWO_DOTS);
    requireToken(lexer, Lexer::TokenType::DOUBLE_SLASH);
    requireToken(lexer, Lexer::TokenType::DOT);
    requireToken(lexer, Lexer::TokenType::END);
}

std::unique_ptr<onyx::xpath::Parser::AstNode> parse(
    const std::string& expression) {
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
    Parser::Number* num =
        static_cast<Parser::Number*>(step->predicates[0].get());
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
    Parser::FunctionCall* func =
        static_cast<Parser::FunctionCall*>(step->predicates[0].get());
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
    Parser::BinaryOp* firstSlash =
        static_cast<Parser::BinaryOp*>(finalSlash->left.get());
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
    Parser::BinaryOp* firstSlash =
        static_cast<Parser::BinaryOp*>(finalSlash->left.get());
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
    Parser::BinaryOp* eqOp =
        static_cast<Parser::BinaryOp*>(step->predicates[0].get());
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
    std::unique_ptr<Parser::AstNode> ast =
        parse("chapter[title=\"Introduction\"]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* pred =
        static_cast<Parser::BinaryOp*>(step->predicates[0].get());

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
    Parser::Step* predStep =
        static_cast<Parser::Step*>(step->predicates[0].get());
    CHECK(predStep->test == "title");
}

TEST_CASE("XPath Parser boolean and in predicate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic;
    std::unique_ptr<Parser::AstNode> ast =
        parse("employee[@secretary and @assistant]");

    REQUIRE(ast->getType() == Parser::AstNode::Step);
    Parser::Step* step = static_cast<Parser::Step*>(ast.get());
    REQUIRE(step->predicates[0]->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* andOp =
        static_cast<Parser::BinaryOp*>(step->predicates[0].get());

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
    CHECK(static_cast<Parser::Number*>(filter->predicates[0].get())->num ==
          "1");
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
    Parser::BinaryOp* multOp =
        static_cast<Parser::BinaryOp*>(plusOp->right.get());
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
    Parser::Step* arg1 =
        static_cast<Parser::Step*>(func->args[0].get());  // @src
    CHECK(arg1->axis == "attribute");

    REQUIRE(func->args[1]->getType() == Parser::AstNode::Literal);
    Parser::Literal* arg2 =
        static_cast<Parser::Literal*>(func->args[1].get());  // '?'
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
    Parser::BinaryOp* leftPipe =
        static_cast<Parser::BinaryOp*>(topPipe->left.get());
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
    Parser::BinaryOp* unaryNode =
        static_cast<Parser::BinaryOp*>(multOp->right.get());
    REQUIRE(unaryNode != nullptr);
    CHECK(unaryNode->op == "-");
    CHECK(static_cast<Parser::Number*>(unaryNode->left.get())->num == "0");
    CHECK(static_cast<Parser::Number*>(unaryNode->right.get())->num == "5");

    REQUIRE(multOp->left->getType() == Parser::AstNode::BinaryOp);
    Parser::BinaryOp* divOp =
        static_cast<Parser::BinaryOp*>(multOp->left.get());
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

    parser::StringCursor cursor(
        "div[@class='2']/div[2*2]/@id | div[@class='2']/div[2*2]/@id");
    Lexer lexer(cursor);
    Parser parser(lexer);
    Compiler compiler(std::move(parser.buildAST()));
    std::unique_ptr<Program> pr = compiler.compile();

    // "div", "class", "2", 2, "id", "" (empty namespace URI)
    REQUIRE(pr->getData().size() == 6);
}

TEST_CASE("XPath execute /store") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

    XPathQuery::Result res1 = std::move(XPathQuery("/store").execute(&doc));
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    REQUIRE(res.asNodeset()[0] == &doc);
}

TEST_CASE("XPath execute /store/book") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

    XPathQuery::Result res1 =
        XPathQuery("/store/book[price > 15]").execute(&doc);
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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1")),
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2")),
                                GenericNode("price", NonVoid, Text("20"))));

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

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("title", NonVoid, Text("Book1"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("title", NonVoid, Text("Book2"))));

    XPathQuery::Result res1 = XPathQuery("/store/book/author").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().empty());
}

TEST_CASE("XPath execute math /store/book[price div 2 < 15]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("store", NonVoid,
                    GenericNode("book", NonVoid, Attribute("id", "1"),
                                GenericNode("price", NonVoid, Text("20"))),
                    GenericNode("book", NonVoid, Attribute("id", "2"),
                                GenericNode("price", NonVoid, Text("40"))));

    XPathQuery::Result res1 =
        XPathQuery("/store/book[price div 2 < 15]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNodeset());
    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE(
    "XPath execute function composition book[not(starts-with(title, "
    "'Second'))]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "store", NonVoid,
        GenericNode("book", NonVoid, Attribute("id", "1"),
                    GenericNode("title", NonVoid, Text("Book"))),
        GenericNode("book", NonVoid, Attribute("id", "2"),
                    GenericNode("title", NonVoid, Text("Second Book"))));

    XPathQuery::Result res1 =
        XPathQuery("book[not(starts-with(title, 'Second'))]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "book");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute sum function /root/store[sum(book/price) > 50]") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("store", NonVoid, Attribute("id", "1"),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("10"))),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("20")))),
        GenericNode("store", NonVoid, Attribute("id", "2"),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("40"))),
                    GenericNode("book", NonVoid,
                                GenericNode("price", NonVoid, Text("30")))));

    XPathQuery::Result res1 =
        XPathQuery("/root/store[sum(book/price) > 50]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 1);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getTagName() == "store");
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
}

TEST_CASE("XPath execute attributes /root/item/@id") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "a")),
                    GenericNode("item", NonVoid, Attribute("id", "b"),
                                Attribute("other", "x")));

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

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "1"),
                                Attribute("x", "1"), Attribute("y", "2")),
                    GenericNode("item", NonVoid, Attribute("id", "2"),
                                Attribute("x", "1"), Attribute("z", "3")),
                    GenericNode("item", NonVoid, Attribute("id", "3"),
                                Attribute("x", "1"), Attribute("y", "9")),
                    GenericNode("item", NonVoid, Attribute("id", "4"),
                                Attribute("x", "0"), Attribute("y", "2")));

    XPathQuery::Result res1 =
        XPathQuery("item[@x='1' and (@y='2' or @z='3')]").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getAttributes()[0].getValue() == "1");
    REQUIRE(nodeset[1]->getAttributes()[0].getValue() == "2");
}

TEST_CASE(
    "XPath execute booleans item[@x=$var1 and (@y=$var2 or @z=$var3)] with "
    "variables") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "1"),
                                Attribute("x", "1"), Attribute("y", "2")),
                    GenericNode("item", NonVoid, Attribute("id", "2"),
                                Attribute("x", "1"), Attribute("z", "3")),
                    GenericNode("item", NonVoid, Attribute("id", "3"),
                                Attribute("x", "1"), Attribute("y", "9")),
                    GenericNode("item", NonVoid, Attribute("id", "4"),
                                Attribute("x", "0"), Attribute("y", "2")));

    XPathQuery::Result res1 =
        XPathQuery("item[@x=$var1 and (@y=$var2 or @z=$var3)]",
                   [](std::string_view name) -> std::string { return ""; })
            .execute(&doc,
                     [](std::string_view uri,
                        std::string_view localName) -> XPathObject {
                         if (localName == "var1") return XPathObject("1");
                         if (localName == "var2") return XPathObject("2");
                         if (localName == "var3") return XPathObject("3");
                         throw std::runtime_error("Unknown variable");
                     });
    XPathObject& res = res1.object;

    REQUIRE(res.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res.asNodeset();
    REQUIRE(nodeset[0]->getAttributes()[0].getValue() == "1");
    REQUIRE(nodeset[1]->getAttributes()[0].getValue() == "2");
}

TEST_CASE("XPath execute union operator //div | //span") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("div", NonVoid, GenericNode("span", NonVoid, Text("A"))),
        GenericNode("section", NonVoid, GenericNode("span", NonVoid, Text("B")),
                    GenericNode("div", NonVoid, Text("C"))));

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

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 = XPathQuery("(1 < 5 - 6)").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isBool());
    REQUIRE_FALSE(res.asBool());
}

TEST_CASE("XPath execute concat() with many arguments") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("concat('a', 'b', 'c', 'd', 'e')").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isString());
    REQUIRE(res.asString() == "abcde");
}

TEST_CASE(
    "XPath execute substring-after() with second argument longer than 1 "
    "character") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("substring-after('1999/04/01','/04')").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isString());
    REQUIRE(res.asString() == "/01");
}

TEST_CASE("XPath execute arithmetic (1 + 2 * 3 * (6 div 2) - 3)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("(1 + 2 * 3 * (6 div 2) - 5)").execute(&doc);
    XPathObject& res = res1.object;

    REQUIRE(res.isNumber());
    REQUIRE(res.asNumber() == Catch::Approx(14));
}

TEST_CASE("XPath execute axis precedence (ancestor vs parent-ancestor)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::dynamic::tags;
    using namespace onyx::dynamic;

    GenericNode doc("root", NonVoid,
                    GenericNode("grandparent", NonVoid,
                                GenericNode("parent", NonVoid,
                                            GenericNode("child", NonVoid))));

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

    GenericNode doc("list", NonVoid, GenericNode("item", NonVoid, Text("A")),
                    GenericNode("item", NonVoid, Text("B")),
                    GenericNode("item", NonVoid, Text("C")),
                    GenericNode("item", NonVoid, Text("D")));

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

    GenericNode doc("data", NonVoid,
                    GenericNode("a", NonVoid, Attribute("id", "1")),
                    GenericNode("b", NonVoid, Attribute("id", "2")),
                    GenericNode("c", NonVoid, Attribute("id", "3")),
                    GenericNode("d", NonVoid, Attribute("id", "4")));

    Node* nodeB = doc.getChildren()[1];

    XPathQuery::Result res1 =
        XPathQuery("following-sibling::node()[1]").execute(nodeB);
    REQUIRE(res1.object.asNodeset()[0]->getAttributeValue("id") == "3");

    XPathQuery::Result res2 =
        XPathQuery("preceding-sibling::node()[1]").execute(nodeB);
    REQUIRE(res2.object.asNodeset()[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute deep string concatenation (Element String Value)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "doc", NonVoid,
        GenericNode("p", NonVoid, Text("Hello "),
                    GenericNode("b", NonVoid, Text("World")), Text("!")));

    XPathQuery::Result res1 = XPathQuery("string(/doc/p)").execute(&doc);
    REQUIRE(res1.object.asString() == "Hello World!");
}

TEST_CASE("XPath execute complex boolean and string functions") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("inventory", NonVoid,
                    GenericNode("item", NonVoid, Attribute("code", "A-123"),
                                GenericNode("name", NonVoid, Text("Widget"))),
                    GenericNode("item", NonVoid, Attribute("code", "B-456"),
                                GenericNode("name", NonVoid, Text("Gadget"))),
                    GenericNode("item", NonVoid, Attribute("code", "A-789"),
                                GenericNode("name", NonVoid, Text("Wodget"))));

    // Find items where code starts with 'A' AND name contains 'get' and name
    // doesn't contain 'Gad'
    std::string query =
        "/inventory/item[starts-with(@code, 'A') and contains(name, 'get') and "
        "not(contains(name, 'Gad'))]";

    XPathQuery::Result res1 = XPathQuery(query).execute(&doc);

    REQUIRE(res1.object.asNodeset().size() == 2);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getTagName() ==
            "name");
    REQUIRE(res1.object.asNodeset()[0]
                ->getFirstChild()
                ->getFirstChild()
                ->serialize() == "Widget");
    REQUIRE(res1.object.asNodeset()[1]->getFirstChild()->getTagName() ==
            "name");
    REQUIRE(res1.object.asNodeset()[1]
                ->getFirstChild()
                ->getFirstChild()
                ->serialize() == "Wodget");
}

TEST_CASE("XPath execute substring") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("substring('12345', 2, 3)").execute(&doc);
    REQUIRE(res1.object.asString() == "234");

    XPathQuery::Result res2 =
        XPathQuery("substring('12345', 1.5, 2.6)").execute(&doc);
    REQUIRE(res2.object.asString() == "234");

    XPathQuery::Result res3 =
        XPathQuery("substring('12345', 0, 3)").execute(&doc);
    REQUIRE(res3.object.asString() == "12");

    XPathQuery::Result res4 =
        XPathQuery("substring('12345', 0 div 0, 3)").execute(&doc);
    REQUIRE(res4.object.asString() == "");

    XPathQuery::Result res5 =
        XPathQuery("substring('12345', 1, 0 div 0)").execute(&doc);
    REQUIRE(res5.object.asString() == "");

    XPathQuery::Result res6 =
        XPathQuery("substring('12345', -42, 1 div 0)").execute(&doc);
    REQUIRE(res6.object.asString() == "12345");

    XPathQuery::Result res7 =
        XPathQuery("substring('12345', -1 div 0, 1 div 0)").execute(&doc);
    REQUIRE(res7.object.asString() == "");
}

TEST_CASE("XPath substring handles unicode") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("substring('Здравей', 2, 3)").execute(&doc);
    REQUIRE(res1.object.asString() == "дра");

    XPathQuery::Result res2 =
        XPathQuery("substring('你好世界', 2, 2)").execute(&doc);
    REQUIRE(res2.object.asString() == "好世");

    XPathQuery::Result res3 =
        XPathQuery("substring('🍎🍌🍇🍓', 2, 2)").execute(&doc);
    REQUIRE(res3.object.asString() == "🍌🍇");

    XPathQuery::Result res4 =
        XPathQuery("substring('Aλ文🍎B', 2, 3)").execute(&doc);
    REQUIRE(res4.object.asString() == "λ文🍎");

    XPathQuery::Result res5 =
        XPathQuery("substring('こんにちは', 0, 3)").execute(&doc);
    REQUIRE(res5.object.asString() == "こん");

    XPathQuery::Result res6 =
        XPathQuery("substring('🍎💧🌱', 2, 100)").execute(&doc);
    REQUIRE(res6.object.asString() == "💧🌱");

    XPathQuery::Result res7 =
        XPathQuery("substring('αβγδε', -2, 1 div 0)").execute(&doc);
    REQUIRE(res7.object.asString() == "αβγδε");
}

TEST_CASE("XPath execute translate") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("translate('bar', 'abc', 'ABC')").execute(&doc);
    REQUIRE(res1.object.asString() == "BAr");

    XPathQuery::Result res2 =
        XPathQuery("translate('data', 'd', '')").execute(&doc);
    REQUIRE(res2.object.asString() == "ata");
}

TEST_CASE("XPath translate handles unicode") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 =
        XPathQuery("translate('αβγδε', 'αγ', 'ΑΓ')").execute(&doc);
    REQUIRE(res1.object.asString() == "ΑβΓδε");

    XPathQuery::Result res2 =
        XPathQuery("translate('你好世界', '你好世', '您安')").execute(&doc);
    REQUIRE(res2.object.asString() == "您安界");

    XPathQuery::Result res3 =
        XPathQuery("translate('🍎🍌🍇🍉', '🍌🍉', '🍓🍍')").execute(&doc);
    REQUIRE(res3.object.asString() == "🍎🍓🍇🍍");

    XPathQuery::Result res4 =
        XPathQuery("translate('hello свят', 'eя', '⭐i')").execute(&doc);
    REQUIRE(res4.object.asString() == "h⭐llo свiт");

    XPathQuery::Result res5 =
        XPathQuery("translate('Кола', 'КолКа', 'стоВл')").execute(&doc);
    REQUIRE(res5.object.asString() == "стол");

    XPathQuery::Result res6 =
        XPathQuery("translate('⭐Hello⭐World⭐', '⭐', '')").execute(&doc);
    REQUIRE(res6.object.asString() == "HelloWorld");

    XPathQuery::Result res7 =
        XPathQuery("translate('café', 'é', 'eXYZ')").execute(&doc);
    REQUIRE(res7.object.asString() == "cafe");
}

TEST_CASE("XPath execute string-length") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    XPathQuery::Result res1 = XPathQuery("string-length('')").execute(&doc);
    REQUIRE(res1.object.asNumber() == 0.0);

    XPathQuery::Result res2 =
        XPathQuery("string-length('hello')").execute(&doc);
    REQUIRE(res2.object.asNumber() == 5.0);

    XPathQuery::Result res3 =
        XPathQuery("string-length('Здравей')").execute(&doc);
    REQUIRE(res3.object.asNumber() == 7.0);

    XPathQuery::Result res4 =
        XPathQuery("string-length('你好世界')").execute(&doc);
    REQUIRE(res4.object.asNumber() == 4.0);

    XPathQuery::Result res5 =
        XPathQuery("string-length('🍎🍌🍇🍓')").execute(&doc);
    REQUIRE(res5.object.asNumber() == 4.0);

    XPathQuery::Result res6 =
        XPathQuery("string-length('Aλ文🍎B')").execute(&doc);
    REQUIRE(res6.object.asNumber() == 5.0);
}

TEST_CASE("XPath execute parent abbreviation (..) and self (.)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "a", NonVoid,
        GenericNode("b", NonVoid,
                    GenericNode("c", NonVoid, Attribute("attr", "val"))));

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

    GenericNode doc("root", NonVoid, GenericNode("val", NonVoid, Text("foo")),
                    GenericNode("val", NonVoid, Text("bar")),
                    GenericNode("val", NonVoid, Text("baz")));

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

    GenericNode doc("root", NonVoid);

    REQUIRE(XPathQuery("floor(1.9)").execute(&doc).object.asNumber() == 1.0);
    REQUIRE(XPathQuery("ceiling(1.1)").execute(&doc).object.asNumber() == 2.0);
    REQUIRE(XPathQuery("round(1.5)").execute(&doc).object.asNumber() ==
            2.0);  // 1.5 rounds up
    REQUIRE(XPathQuery("round(1.4)").execute(&doc).object.asNumber() == 1.0);

    REQUIRE(XPathQuery("number('-10.5')").execute(&doc).object.asNumber() ==
            -10.5);
}

TEST_CASE("XPath execute attribute node edge cases") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "1"), Text("Hello")));

    XPathQuery::Result res1 =
        XPathQuery("/root/item/@id/child::node()").execute(&doc);
    REQUIRE(res1.object.asNodeset().empty());

    XPathQuery::Result res2 =
        XPathQuery("/root/item/text()/child::node()").execute(&doc);
    REQUIRE(res2.object.asNodeset().empty());

    XPathQuery::Result res3 = XPathQuery("/root/item/@id/..").execute(&doc);
    REQUIRE(res3.object.asNodeset().size() == 1);
    REQUIRE(res3.object.asNodeset()[0]->getTagName() == "item");
}

TEST_CASE("XPath execute axis test") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("A", NonVoid, Attribute("id", "1"),
                                GenericNode("B", NonVoid, Attribute("id", "2")),
                                GenericNode("C", NonVoid, Attribute("id", "3"),
                                            GenericNode("D", NonVoid,
                                                        Attribute("id", "4")))),
                    GenericNode("E", NonVoid, Attribute("id", "5")));

    Node* nodeC = doc.getChildren()[0]->getChildren()[1];

    XPathQuery::Result resDesc =
        XPathQuery("/root/A/descendant::*").execute(&doc);
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

    GenericNode doc("root", NonVoid,
                    GenericNode("elem", NonVoid, Text("text1")),
                    GenericNode("elem", NonVoid, Text("text2")),
                    GenericNode("other", NonVoid));

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

    GenericNode doc("root", NonVoid,
                    ProcessingInstruction("php", "echo \"hello\";"),
                    GenericNode("child", NonVoid),
                    ProcessingInstruction("xml-stylesheet", "type=\"css\""),
                    GenericNode("child", NonVoid));

    XPathQuery::Result resAllPI =
        XPathQuery("/root/processing-instruction()").execute(&doc);
    REQUIRE(resAllPI.object.asNodeset().size() == 2);

    Node* pi1 = resAllPI.object.asNodeset()[0];
    Node* pi2 = resAllPI.object.asNodeset()[1];

    REQUIRE(pi1->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi1)->getTarget() == "php");

    REQUIRE(pi2->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi2)->getTarget() ==
            "xml-stylesheet");

    XPathQuery::Result resSpecificPI =
        XPathQuery("/root/processing-instruction('php')").execute(&doc);
    REQUIRE(resSpecificPI.object.asNodeset().size() == 1);
    Node* pi3 = resSpecificPI.object.asNodeset()[0];
    REQUIRE(pi3->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi3)->getTarget() == "php");
    REQUIRE(pi3->getStringValue() == "echo \"hello\";");

    XPathQuery::Result resSpecificPI2 =
        XPathQuery("/root/processing-instruction(\"xml-stylesheet\")")
            .execute(&doc);
    REQUIRE(resSpecificPI2.object.asNodeset().size() == 1);
    Node* pi4 = resSpecificPI2.object.asNodeset()[0];
    REQUIRE(pi4->getXPathType() == Node::XPathType::PROCESSING_INSTRUCTION);
    REQUIRE(static_cast<ProcessingInstruction*>(pi4)->getTarget() ==
            "xml-stylesheet");
    REQUIRE(pi4->getStringValue() == "type=\"css\"");

    XPathQuery::Result resMissingPI =
        XPathQuery("/root/processing-instruction('unknown')").execute(&doc);
    REQUIRE(resMissingPI.object.asNodeset().empty());
}

TEST_CASE("XPath execute finds comments") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid, GenericNode("child", NonVoid),
                    Comment("This is a comment"), GenericNode("child", NonVoid),
                    GenericNode("child", NonVoid));

    XPathQuery::Result resComments =
        XPathQuery("/root/comment()").execute(&doc);
    REQUIRE(resComments.object.asNodeset().size() == 1);
    REQUIRE(resComments.object.asNodeset()[0]->getXPathType() ==
            Node::XPathType::COMMENT);
    REQUIRE(resComments.object.asNodeset()[0]->serialize() ==
            "<!--This is a comment-->");
}

TEST_CASE("XPath execute complex path step predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("section", NonVoid, Attribute("id", "s1"),
                    GenericNode("div", NonVoid, Attribute("class", "a")),
                    GenericNode("div", NonVoid, Attribute("class", "b")),
                    GenericNode("div", NonVoid, Attribute("class", "a"))),
        GenericNode("section", NonVoid, Attribute("id", "s2"),
                    GenericNode("div", NonVoid, Attribute("class", "a")),
                    GenericNode("div", NonVoid, Attribute("class", "b"))));

    XPathQuery::Result res1 = XPathQuery("/root/section/div[2]").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 2);
    REQUIRE(res1.object.asNodeset()[0]->getAttributeValue("class") == "b");
    REQUIRE(res1.object.asNodeset()[1]->getAttributeValue("class") == "b");

    XPathQuery::Result res2 =
        XPathQuery("(/root/section/div)[2]").execute(&doc);
    REQUIRE(res2.object.asNodeset().size() == 1);
    REQUIRE(res2.object.asNodeset()[0]->getAttributeValue("class") == "b");
}

TEST_CASE("XPath execute maintains document order on paths with reverse axis") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("section", NonVoid, Attribute("id", "s1"),
                    GenericNode("div", NonVoid, Attribute("class", "a")),
                    GenericNode("div", NonVoid, Attribute("class", "b")),
                    GenericNode("div", NonVoid, Attribute("class", "a"))),
        GenericNode("section", NonVoid, Attribute("id", "s2"),
                    GenericNode("div", NonVoid, Attribute("class", "a")),
                    GenericNode("div", NonVoid, Attribute("class", "b"))));

    XPathQuery::Result res1 =
        XPathQuery("//section[@id='s1']/div[@class = 'a']/ancestor::*")
            .execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 2);
    REQUIRE(res1.object.asNodeset()[0]->getTagName() == "root");
    REQUIRE(res1.object.asNodeset()[1]->getTagName() == "section");
}

TEST_CASE("XPath execute correctly unions AttributeViewNode") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("section", NonVoid, Attribute("id", "s1"),
                    GenericNode("div", NonVoid, Attribute("class", "a"),
                                Attribute("id", "1")),
                    GenericNode("div", NonVoid, Attribute("class", "b"),
                                Attribute("id", "2")),
                    GenericNode("div", NonVoid, Attribute("class", "a"),
                                Attribute("id", "3"))),
        GenericNode("section", NonVoid, Attribute("id", "s2"),
                    GenericNode("div", NonVoid, Attribute("class", "a"),
                                Attribute("id", "4")),
                    GenericNode("div", NonVoid, Attribute("class", "b"),
                                Attribute("id", "5"))));

    XPathQuery::Result res1 =
        XPathQuery("//@class[. = 'a'] | //@class[. = 'a']").execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 3);
    REQUIRE(res1.object.asNodeset()[0]->getParentNode()->getAttributeValue(
                "id") == "1");
    REQUIRE(res1.object.asNodeset()[1]->getParentNode()->getAttributeValue(
                "id") == "3");
    REQUIRE(res1.object.asNodeset()[2]->getParentNode()->getAttributeValue(
                "id") == "4");
}

TEST_CASE("XPath execute correctly orders AttributeViewNode and RootViewNode") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("section", NonVoid, Attribute("id", "s1"),
                    GenericNode("div", NonVoid, Attribute("class", "a"),
                                Attribute("id", "1")),
                    GenericNode("div", NonVoid, Attribute("class", "b"),
                                Attribute("id", "2")),
                    GenericNode("div", NonVoid, Attribute("class", "a"),
                                Attribute("id", "3"))),
        GenericNode("section", NonVoid, Attribute("id", "s2"),
                    GenericNode("div", NonVoid, Attribute("class", "a"),
                                Attribute("id", "4")),
                    GenericNode("div", NonVoid, Attribute("class", "b"),
                                Attribute("id", "5"))));

    XPathQuery::Result res1 =
        XPathQuery("//div[@class='b'] | //@class[. = 'a'] | /ancestor::node()")
            .execute(&doc);
    REQUIRE(res1.object.asNodeset().size() == 6);
    REQUIRE(res1.object.asNodeset()[0]->getChildrenCount() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getTagName() ==
            "root");
    REQUIRE(res1.object.asNodeset()[1]->getParentNode()->getAttributeValue(
                "id") == "1");
    REQUIRE(res1.object.asNodeset()[2]->getAttributeValue("id") == "2");
    REQUIRE(res1.object.asNodeset()[3]->getParentNode()->getAttributeValue(
                "id") == "3");
    REQUIRE(res1.object.asNodeset()[4]->getParentNode()->getAttributeValue(
                "id") == "4");
    REQUIRE(res1.object.asNodeset()[5]->getAttributeValue("id") == "5");
}

TEST_CASE("XPath execute multiple predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("list", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "1"),
                                Attribute("type", "A")),
                    GenericNode("item", NonVoid, Attribute("id", "2"),
                                Attribute("type", "B")),
                    GenericNode("item", NonVoid, Attribute("id", "3"),
                                Attribute("type", "A")),
                    GenericNode("item", NonVoid, Attribute("id", "4"),
                                Attribute("type", "B")));

    XPathQuery::Result res1 = XPathQuery("item[@type='A'][2]").execute(&doc);

    REQUIRE(res1.object.asNodeset().size() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getAttributeValue("id") == "3");
}

TEST_CASE("XPath execute context position shifting in predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("list", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "1")),
                    GenericNode("item", NonVoid, Attribute("id", "2")),
                    GenericNode("item", NonVoid, Attribute("id", "3")),
                    GenericNode("item", NonVoid, Attribute("id", "4")));

    XPathQuery::Result res1 =
        XPathQuery("item[position() != 1][position() != 2]").execute(&doc);

    REQUIRE(res1.object.asNodeset().size() == 2);
    const std::vector<Node*>& nodeset = res1.object.asNodeset();
    REQUIRE(nodeset[0]->getAttributeValue("id") == "2");
    REQUIRE(nodeset[1]->getAttributeValue("id") == "4");
}

TEST_CASE("XPath execute complex predicate nesting") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("section", NonVoid, Attribute("status", "open"),
                    GenericNode("div", NonVoid, Attribute("class", "content"))),
        GenericNode("section", NonVoid, Attribute("status", "open"),
                    GenericNode("span", NonVoid)),
        GenericNode(
            "section", NonVoid, Attribute("status", "closed"),
            GenericNode("div", NonVoid, Attribute("class", "content"))));

    XPathQuery::Result res1 =
        XPathQuery("section[@status='open'][div]").execute(&doc);

    REQUIRE(res1.object.asNodeset().size() == 1);
    REQUIRE(res1.object.asNodeset()[0]->getFirstChild()->getTagName() == "div");
}

TEST_CASE("XPath execute empty and null expressions throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", NonVoid);

    REQUIRE_THROWS(XPathQuery("").execute(&doc));
    REQUIRE_THROWS(XPathQuery("   ").execute(&doc));
}

TEST_CASE("XPath execute unclosed brackets and parenthesis throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", NonVoid);

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
    GenericNode doc("root", NonVoid);

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
    GenericNode doc("root", NonVoid);

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
    GenericNode doc("root", NonVoid);

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
    GenericNode doc("root", NonVoid);

    REQUIRE_THROWS(XPathQuery("item[]").execute(&doc));

    REQUIRE_THROWS(XPathQuery("[@id='1']").execute(&doc));

    REQUIRE_THROWS(XPathQuery("item[[1]]").execute(&doc));
}

TEST_CASE("XPath execute lexical errors throw") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    GenericNode doc("root", NonVoid);

    REQUIRE_THROWS(XPathQuery("'Unfinished string").execute(&doc));

    REQUIRE_THROWS(XPathQuery("1 & 1").execute(&doc));
}

TEST_CASE("XPath execute namespace element matching") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid, GenericNode("item", NonVoid, Attribute("id", "1")),
        GenericNode("a:item", NonVoid, Attribute("id", "2"),
                    Attribute("xmlns:a", "http://example.com/ns1")),
        GenericNode("b:item", NonVoid, Attribute("id", "3"),
                    Attribute("xmlns:b", "http://example.com/ns2")));

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "x") return "http://example.com/ns1";
        if (prefix == "y") return "http://example.com/ns2";
        return "";
    };

    XPathQuery::Result resNoNs =
        XPathQuery("/root/item", resolver).execute(&doc);
    REQUIRE(resNoNs.object.asNodeset().size() == 1);
    REQUIRE(resNoNs.object.asNodeset()[0]->getAttributeValue("id") == "1");

    XPathQuery::Result resNs1 =
        XPathQuery("/root/x:item", resolver).execute(&doc);
    REQUIRE(resNs1.object.asNodeset().size() == 1);
    REQUIRE(resNs1.object.asNodeset()[0]->getAttributeValue("id") == "2");

    XPathQuery::Result resNs2 =
        XPathQuery("/root/y:item", resolver).execute(&doc);
    REQUIRE(resNs2.object.asNodeset().size() == 1);
    REQUIRE(resNs2.object.asNodeset()[0]->getAttributeValue("id") == "3");

    XPathQuery::Result resWild = XPathQuery("/root/*", resolver).execute(&doc);
    REQUIRE(resWild.object.asNodeset().size() == 3);
}

TEST_CASE("XPath execute built-in 'xml' namespace matching") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("para", NonVoid, Attribute("id", "1"),
                    Attribute("xml:lang", "en"), Text("English text")),
        GenericNode("para", NonVoid, Attribute("id", "2"),
                    Attribute("lang", "en"), Text("No-namespace lang")),
        GenericNode("para", NonVoid, Attribute("id", "3"),
                    Attribute("xml:lang", "bg"), Text("Български текст")));

    XPathQuery::Result resXmlLang =
        XPathQuery("/root/para[@xml:lang='en']").execute(&doc);
    REQUIRE(resXmlLang.object.asNodeset().size() == 1);
    REQUIRE(resXmlLang.object.asNodeset()[0]->getAttributeValue("id") == "1");

    XPathQuery::Result resAllXmlLang =
        XPathQuery("/root/para/@xml:lang").execute(&doc);
    REQUIRE(resAllXmlLang.object.asNodeset().size() == 2);

    XPathQuery::Result resNoNsLang =
        XPathQuery("/root/para/@lang").execute(&doc);
    REQUIRE(resNoNsLang.object.asNodeset().size() == 1);
    REQUIRE(
        resNoNsLang.object.asNodeset()[0]->getParentNode()->getAttributeValue(
            "id") == "2");
}

TEST_CASE("XPath execute namespace-aware attribute matching") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "1"),
                    Attribute("custom:flag", "true"),
                    Attribute("xmlns:custom", "http://example.com/custom")),
        GenericNode("item", NonVoid, Attribute("id", "2"),
                    Attribute("flag", "true")));

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "c") return "http://example.com/custom";
        return "";
    };

    XPathQuery::Result resCustom =
        XPathQuery("/root/item[@c:flag='true']", resolver).execute(&doc);
    REQUIRE(resCustom.object.asNodeset().size() == 1);
    REQUIRE(resCustom.object.asNodeset()[0]->getAttributeValue("id") == "1");

    XPathQuery::Result resNoNs =
        XPathQuery("/root/item[@flag='true']", resolver).execute(&doc);
    REQUIRE(resNoNs.object.asNodeset().size() == 1);
    REQUIRE(resNoNs.object.asNodeset()[0]->getAttributeValue("id") == "2");
}

TEST_CASE("XPath execute undeclared namespace prefix throws on compile time") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;
    using namespace onyx::dynamic;

    auto emptyResolver = [](std::string_view) -> std::string { return ""; };

    REQUIRE_THROWS_WITH(XPathQuery("/root/unknown:child", emptyResolver),
                        "Could not resolve namespace prefix in query");

    REQUIRE_THROWS_WITH(
        XPathQuery("/root/child[@unknown:attr='val']", emptyResolver),
        "Could not resolve namespace prefix in query");
}

TEST_CASE("XPath execute prefix wildcard element matching (prefix:*)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("a:title", NonVoid, Attribute("id", "1"),
                                Attribute("xmlns:a", "http://example.com/ns1"),
                                Text("Title 1")),
                    GenericNode("a:summary", NonVoid, Attribute("id", "2"),
                                Attribute("xmlns:a", "http://example.com/ns1"),
                                Text("Summary 1")),
                    GenericNode("b:title", NonVoid, Attribute("id", "3"),
                                Attribute("xmlns:b", "http://example.com/ns2"),
                                Text("Title 2")),
                    GenericNode("title", NonVoid, Attribute("id", "4"),
                                Text("No Namespace Title")));

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "x") return "http://example.com/ns1";
        if (prefix == "y") return "http://example.com/ns2";
        return "";
    };

    XPathQuery::Result resNs1 = XPathQuery("/root/x:*", resolver).execute(&doc);
    REQUIRE(resNs1.object.asNodeset().size() == 2);
    REQUIRE(resNs1.object.asNodeset()[0]->getAttributeValue("id") == "1");
    REQUIRE(resNs1.object.asNodeset()[1]->getAttributeValue("id") == "2");

    XPathQuery::Result resNs2 = XPathQuery("/root/y:*", resolver).execute(&doc);
    REQUIRE(resNs2.object.asNodeset().size() == 1);
    REQUIRE(resNs2.object.asNodeset()[0]->getAttributeValue("id") == "3");

    XPathQuery::Result resAll = XPathQuery("/root/*", resolver).execute(&doc);
    REQUIRE(resAll.object.asNodeset().size() == 4);
}

TEST_CASE("XPath execute prefix wildcard attribute matching (@prefix:*)") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "1"),
                    Attribute("meta:author", "Alice"),
                    Attribute("meta:date", "2026"),
                    Attribute("xmlns:meta", "http://example.com/meta"),
                    Attribute("xml:lang", "en"), Attribute("class", "card")));

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "m") return "http://example.com/meta";
        return "";
    };

    XPathQuery::Result resMetaAttrs =
        XPathQuery("/root/item/@m:*", resolver).execute(&doc);
    REQUIRE(resMetaAttrs.object.asNodeset().size() == 2);

    XPathQuery::Result resXmlAttrs =
        XPathQuery("/root/item/@xml:*", resolver).execute(&doc);
    REQUIRE(resXmlAttrs.object.asNodeset().size() == 1);
    REQUIRE(resXmlAttrs.object.asNodeset()[0]->getStringValue() == "en");

    XPathQuery::Result resAllAttrs =
        XPathQuery("/root/item/@*", resolver).execute(&doc);
    REQUIRE(resAllAttrs.object.asNodeset().size() == 5);
}

TEST_CASE("XPath execute prefix wildcard in predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "catalog", NonVoid,
        GenericNode(
            "book", NonVoid, Attribute("id", "1"),
            GenericNode("meta:data", NonVoid,
                        Attribute("xmlns:meta", "http://example.com/meta"),
                        Text("Available"))),
        GenericNode(
            "book", NonVoid, Attribute("id", "2"),
            GenericNode("description", NonVoid, Text("No custom metadata"))));

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "m") return "http://example.com/meta";
        return "";
    };

    XPathQuery::Result res =
        XPathQuery("/catalog/book[m:*]", resolver).execute(&doc);
    REQUIRE(res.object.asNodeset().size() == 1);
    REQUIRE(res.object.asNodeset()[0]->getAttributeValue("id") == "1");
}

TEST_CASE("XPath execute undeclared prefix wildcard throws on compile time") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    auto emptyResolver = [](std::string_view) -> std::string { return ""; };

    REQUIRE_THROWS(XPathQuery("/root/unknown:*", emptyResolver));
    REQUIRE_THROWS(XPathQuery("/root/item/@unknown:*", emptyResolver));
}

TEST_CASE("XPath attribute axis ignores namespace declarations") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid,
                    Attribute("xmlns", "http://example.com/default"),
                    Attribute("xmlns:custom", "http://example.com/custom"),
                    Attribute("xmlns:extra", "http://example.com/extra"),
                    Attribute("id", "1"), Attribute("class", "main"),
                    Attribute("custom:attr", "val")));

    auto resolver = [](std::string_view prefix) -> std::string { return ""; };

    XPathQuery::Result resWildcard =
        XPathQuery("/root/*/@*", resolver).execute(&doc);
    REQUIRE(resWildcard.object.asNodeset().size() == 3);

    XPathQuery::Result resXmlnsDefault =
        XPathQuery("/root/*/@xmlns", resolver).execute(&doc);
    REQUIRE(resXmlnsDefault.object.asNodeset().empty());

    XPathQuery::Result resXmlnsPrefixed =
        XPathQuery("/root/*/@xmlns:custom", resolver).execute(&doc);
    REQUIRE(resXmlnsPrefixed.object.asNodeset().empty());

    XPathQuery::Result resCountPred =
        XPathQuery("/root/*[count(@*) = 3]", resolver).execute(&doc);
    REQUIRE(resCountPred.object.asNodeset().size() == 1);
    REQUIRE(resCountPred.object.asNodeset()[0]->getAttributeValue("id") == "1");

    XPathQuery::Result resNotXmlns =
        XPathQuery("/root/*[not(@xmlns)]", resolver).execute(&doc);
    REQUIRE(resNotXmlns.object.asNodeset().size() == 1);
}

TEST_CASE("XPath namespace axis implicit xml namespace node") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resAll =
        XPathQuery("/root/namespace::*", resolver).execute(&doc);
    REQUIRE(resAll.object.asNodeset().size() == 1);

    Node* nsNode = resAll.object.asNodeset()[0];
    REQUIRE(nsNode->getXPathType() == Node::XPathType::NAMESPACE);
    REQUIRE(static_cast<NamespaceViewNode*>(nsNode)->getPrefix() == "xml");
    REQUIRE(nsNode->getStringValue() == "http://www.w3.org/XML/1998/namespace");

    XPathQuery::Result resDirectXml =
        XPathQuery("/root/namespace::xml", resolver).execute(&doc);
    REQUIRE(resDirectXml.object.asNodeset().size() == 1);
    REQUIRE(resDirectXml.object.asNodeset()[0]->getStringValue() ==
            "http://www.w3.org/XML/1998/namespace");
}

TEST_CASE("XPath namespace axis local namespace declarations") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid,
                                Attribute("xmlns:a", "http://example.com/nsA"),
                                Attribute("xmlns:b", "http://example.com/nsB"),
                                Attribute("id", "1")));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resA =
        XPathQuery("/root/item/namespace::a", resolver).execute(&doc);
    REQUIRE(resA.object.asNodeset().size() == 1);
    REQUIRE(resA.object.asNodeset()[0]->getStringValue() ==
            "http://example.com/nsA");

    XPathQuery::Result resB =
        XPathQuery("/root/item/namespace::b", resolver).execute(&doc);
    REQUIRE(resB.object.asNodeset().size() == 1);
    REQUIRE(resB.object.asNodeset()[0]->getStringValue() ==
            "http://example.com/nsB");

    XPathQuery::Result resWildcard =
        XPathQuery("/root/item/namespace::*", resolver).execute(&doc);
    REQUIRE(resWildcard.object.asNodeset().size() == 3);

    XPathQuery::Result resMissing =
        XPathQuery("/root/item/namespace::nonexistent", resolver).execute(&doc);
    REQUIRE(resMissing.object.asNodeset().empty());
}

TEST_CASE("XPath namespace axis ancestor inheritance and shadowing") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    // root declares a and b
    // parent shadows a and declares c
    // child inherits parent's a, parent's c, and root's b and implicit xml
    GenericNode doc(
        "root", NonVoid, Attribute("xmlns:a", "http://example.com/a_root"),
        Attribute("xmlns:b", "http://example.com/b_root"),
        GenericNode("parent", NonVoid,
                    Attribute("xmlns:a", "http://example.com/a_parent"),
                    Attribute("xmlns:c", "http://example.com/c_parent"),
                    GenericNode("child", NonVoid, Attribute("id", "target"))));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    // child inherits 'b' from root
    XPathQuery::Result resInheritedB =
        XPathQuery("//child/namespace::b", resolver).execute(&doc);
    REQUIRE(resInheritedB.object.asNodeset().size() == 1);
    REQUIRE(resInheritedB.object.asNodeset()[0]->getStringValue() ==
            "http://example.com/b_root");

    // child sees shadowed 'a' from parent, not root
    XPathQuery::Result resShadowedA =
        XPathQuery("//child/namespace::a", resolver).execute(&doc);
    REQUIRE(resShadowedA.object.asNodeset().size() == 1);
    REQUIRE(resShadowedA.object.asNodeset()[0]->getStringValue() ==
            "http://example.com/a_parent");

    // child sees 'c' from parent
    XPathQuery::Result resInheritedC =
        XPathQuery("//child/namespace::c", resolver).execute(&doc);
    REQUIRE(resInheritedC.object.asNodeset().size() == 1);
    REQUIRE(resInheritedC.object.asNodeset()[0]->getStringValue() ==
            "http://example.com/c_parent");

    // child has 4 namespace nodes - 'xml', 'a', 'b' and 'c'
    XPathQuery::Result resAllChild =
        XPathQuery("//child/namespace::*", resolver).execute(&doc);
    REQUIRE(resAllChild.object.asNodeset().size() == 4);
}

TEST_CASE(
    "XPath namespace axis default namespace declaration and undeclaration") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    Attribute("xmlns", "http://example.com/default"),
                    GenericNode("item1", NonVoid),
                    GenericNode("item2", NonVoid, Attribute("xmlns", "")));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resAllItem1 =
        XPathQuery(
            "/*[local-name()='root']/*[local-name()='item1']/namespace::node()",
            resolver)
            .execute(&doc);
    // Should have 2 nodes, 'xml' and default ''
    REQUIRE(resAllItem1.object.asNodeset().size() == 2);

    XPathQuery::Result resAllItem2 =
        XPathQuery(
            "/*[local-name()='root']/*[local-name()='item2']/namespace::node()",
            resolver)
            .execute(&doc);
    // Only 'xml' should remain
    REQUIRE(resAllItem2.object.asNodeset().size() == 1);
    REQUIRE(static_cast<NamespaceViewNode*>(resAllItem2.object.asNodeset()[0])
                ->getPrefix() == "xml");
}

TEST_CASE("XPath namespace axis document order") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "1"),
                    Attribute("class", "card"),
                    Attribute("xmlns:foo", "http://example.com/foo")));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resUnion =
        XPathQuery("/root/item | /root/item/@* | /root/item/namespace::*",
                   resolver)
            .execute(&doc);

    const std::vector<Node*>& nodes = resUnion.object.asNodeset();
    // 1 element ('item') + 2 namespaces ('xml', 'foo') + 2 attributes ('id',
    // 'class') = 5 nodes
    REQUIRE(nodes.size() == 5);

    REQUIRE(nodes[0]->getXPathType() == Node::XPathType::ELEMENT);
    REQUIRE(nodes[0]->getTagName() == "item");

    REQUIRE(nodes[1]->getXPathType() == Node::XPathType::NAMESPACE);
    REQUIRE(nodes[2]->getXPathType() == Node::XPathType::NAMESPACE);

    REQUIRE(nodes[3]->getXPathType() == Node::XPathType::ATTRIBUTE);
    REQUIRE(nodes[4]->getXPathType() == Node::XPathType::ATTRIBUTE);

    REQUIRE(nodes[3]->getStringValue() == "1");
    REQUIRE(nodes[4]->getStringValue() == "card");
}

TEST_CASE("XPath namespace axis node test filtering") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid,
                    Attribute("xmlns:a", "http://example.com/a"),
                    Text("child text"), GenericNode("nested", NonVoid)));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resNodeTest =
        XPathQuery("/root/item/namespace::node()", resolver).execute(&doc);
    REQUIRE(resNodeTest.object.asNodeset().size() == 2);

    XPathQuery::Result resTextTest =
        XPathQuery("/root/item/namespace::text()", resolver).execute(&doc);
    REQUIRE(resTextTest.object.asNodeset().empty());

    XPathQuery::Result resCommentTest =
        XPathQuery("/root/item/namespace::comment()", resolver).execute(&doc);
    REQUIRE(resCommentTest.object.asNodeset().empty());
}

TEST_CASE("XPath namespace axis within predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("elem", NonVoid, Attribute("id", "1"),
                    Attribute("xmlns:sec", "http://example.com/security")),
        GenericNode("elem", NonVoid, Attribute("id", "2"),
                    Attribute("xmlns:pub", "http://example.com/public")));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resPrefixPred =
        XPathQuery("/root/elem[namespace::sec]", resolver).execute(&doc);
    REQUIRE(resPrefixPred.object.asNodeset().size() == 1);
    REQUIRE(resPrefixPred.object.asNodeset()[0]->getAttributeValue("id") ==
            "1");

    XPathQuery::Result resUriPred =
        XPathQuery("/root/elem[namespace::* = 'http://example.com/public']",
                   resolver)
            .execute(&doc);
    REQUIRE(resUriPred.object.asNodeset().size() == 1);
    REQUIRE(resUriPred.object.asNodeset()[0]->getAttributeValue("id") == "2");

    XPathQuery::Result resCount =
        XPathQuery("/root/elem[count(namespace::*) = 2]", resolver)
            .execute(&doc);
    REQUIRE(resCount.object.asNodeset().size() == 2);
}

TEST_CASE("XPath namespace axis parent axis navigation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "target"),
                    Attribute("xmlns:custom", "http://example.com/custom")));

    auto resolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resParent =
        XPathQuery("/root/item/namespace::custom/..", resolver).execute(&doc);
    REQUIRE(resParent.object.asNodeset().size() == 1);
    REQUIRE(resParent.object.asNodeset()[0]->getAttributeValue("id") ==
            "target");
}

TEST_CASE("XPath local-name() strips prefix on elements and attributes") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "101"),
                    Attribute("pref:flag", "active"),
                    Attribute("xmlns:pref", "http://example.com/pref")),
        GenericNode("pref:gadget", NonVoid,
                    Attribute("xmlns:pref", "http://example.com/pref"),
                    Attribute("pref:serial", "XYZ-99")));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "p") return "http://example.com/pref";
        return "";
    };

    XPathQuery::Result resElem =
        XPathQuery("local-name(/root/item)", resolver).execute(&doc);
    REQUIRE(resElem.object.asString() == "item");

    XPathQuery::Result resPrefElem =
        XPathQuery("local-name(/root/p:gadget)", resolver).execute(&doc);
    REQUIRE(resPrefElem.object.asString() == "gadget");

    XPathQuery::Result resAttr =
        XPathQuery("local-name(/root/item/@id)", resolver).execute(&doc);
    REQUIRE(resAttr.object.asString() == "id");

    XPathQuery::Result resPrefAttr =
        XPathQuery("local-name(/root/item/@p:flag)", resolver).execute(&doc);
    REQUIRE(resPrefAttr.object.asString() == "flag");

    XPathQuery::Result resPrefAttr2 =
        XPathQuery("local-name(/root/p:gadget/@p:serial)", resolver)
            .execute(&doc);
    REQUIRE(resPrefAttr2.object.asString() == "serial");
}

TEST_CASE("XPath local-name() test on all node types") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    Attribute("xmlns:ns", "http://example.com/ns"),
                    Attribute("xmlns", "http://example.com/default"),
                    Comment("Sample comment"),
                    ProcessingInstruction("render-target", "format=\"pdf\""),
                    GenericNode("leaf", NonVoid, Text("Plain text child")));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "d") return "http://example.com/default";
        if (p == "n") return "http://example.com/ns";
        return "";
    };

    XPathQuery::Result resDoc =
        XPathQuery("local-name(/*[local-name()='root']/..)", resolver)
            .execute(&doc);
    REQUIRE(resDoc.object.asString() == "");

    XPathQuery::Result resText =
        XPathQuery("local-name(//text())", resolver).execute(&doc);
    REQUIRE(resText.object.asString() == "");

    XPathQuery::Result resComment =
        XPathQuery("local-name(//comment())", resolver).execute(&doc);
    REQUIRE(resComment.object.asString() == "");

    XPathQuery::Result resPI =
        XPathQuery("local-name(//processing-instruction())", resolver)
            .execute(&doc);
    REQUIRE(resPI.object.asString() == "render-target");

    XPathQuery::Result resNsPref =
        XPathQuery("local-name(/*[local-name()='root']/namespace::ns)",
                   resolver)
            .execute(&doc);
    REQUIRE(resNsPref.object.asString() == "ns");

    XPathQuery::Result resNsXml =
        XPathQuery("local-name(/*[local-name()='root']/namespace::xml)",
                   resolver)
            .execute(&doc);
    REQUIRE(resNsXml.object.asString() == "xml");

    XPathQuery::Result resNsDef =
        XPathQuery(
            "local-name(/*[local-name()='root']/namespace::*[. = "
            "'http://example.com/default'])",
            resolver)
            .execute(&doc);
    REQUIRE(resNsDef.object.asString() == "");
}

TEST_CASE(
    "XPath local-name() default context node evaluation inside predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "catalog", NonVoid,
        GenericNode("ns1:entry", NonVoid,
                    Attribute("xmlns:ns1", "http://example.com/ns1"),
                    Attribute("code", "E1")),
        GenericNode("ns2:entry", NonVoid,
                    Attribute("xmlns:ns2", "http://example.com/ns2"),
                    Attribute("code", "E2")),
        GenericNode("entry", NonVoid, Attribute("code", "E3")),
        GenericNode("summary", NonVoid, Attribute("code", "S1")));

    XPathQuery::Result resEntries =
        XPathQuery("/catalog/*[local-name() = 'entry']").execute(&doc);
    REQUIRE(resEntries.object.asNodeset().size() == 3);
    REQUIRE(resEntries.object.asNodeset()[0]->getAttributeValue("code") ==
            "E1");
    REQUIRE(resEntries.object.asNodeset()[1]->getAttributeValue("code") ==
            "E2");
    REQUIRE(resEntries.object.asNodeset()[2]->getAttributeValue("code") ==
            "E3");

    GenericNode attrDoc(
        "root", NonVoid,
        GenericNode("data", NonVoid, Attribute("a:key", "val1"),
                    Attribute("b:key", "val2"), Attribute("other", "val3"),
                    Attribute("xmlns:a", "http://example.com/a"),
                    Attribute("xmlns:b", "http://example.com/b")));

    XPathQuery::Result resAttrs =
        XPathQuery("/root/data/@*[local-name() = 'key']").execute(&attrDoc);
    REQUIRE(resAttrs.object.asNodeset().size() == 2);
}

TEST_CASE(
    "XPath local-name() nodeset document order and empty set edge cases") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("container", NonVoid,
                    GenericNode("first", NonVoid, Text("1")),
                    GenericNode("second", NonVoid, Text("2")),
                    GenericNode("third", NonVoid, Text("3")));

    XPathQuery::Result resEmpty =
        XPathQuery("local-name(/container/missing)").execute(&doc);
    REQUIRE(resEmpty.object.asString() == "");

    XPathQuery::Result resMultiOrder =
        XPathQuery(
            "local-name(/container/third | /container/first | "
            "/container/second)")
            .execute(&doc);
    REQUIRE(resMultiOrder.object.asString() == "first");

    XPathQuery::Result resReverse =
        XPathQuery("local-name(/container/third/preceding-sibling::*)")
            .execute(&doc);
    REQUIRE(resReverse.object.asString() == "first");
}

TEST_CASE("XPath name() function on elements and attributes") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "item-1"),
                    Attribute("pref:flag", "true"),
                    Attribute("xmlns:pref", "http://example.com/pref")),
        GenericNode("pref:box", NonVoid,
                    Attribute("xmlns:pref", "http://example.com/pref")));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "p") return "http://example.com/pref";
        return "";
    };

    XPathQuery::Result resElem =
        XPathQuery("name(/root/item)", resolver).execute(&doc);
    REQUIRE(resElem.object.asString() == "item");

    XPathQuery::Result resPrefElem =
        XPathQuery("name(/root/p:box)", resolver).execute(&doc);
    REQUIRE(resPrefElem.object.asString() == "pref:box");

    XPathQuery::Result resAttr =
        XPathQuery("name(/root/item/@id)", resolver).execute(&doc);
    REQUIRE(resAttr.object.asString() == "id");

    XPathQuery::Result resPrefAttr =
        XPathQuery("name(/root/item/@p:flag)", resolver).execute(&doc);
    REQUIRE(resPrefAttr.object.asString() == "pref:flag");
}

TEST_CASE("XPath name() zero-argument context sensitivity") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "catalog", NonVoid,
        GenericNode("book", NonVoid, Attribute("category", "fiction")),
        GenericNode("magazine", NonVoid, Attribute("category", "periodical")),
        GenericNode("newspaper", NonVoid));

    XPathQuery::Result resFilter =
        XPathQuery("/catalog/*[name() = 'book']").execute(&doc);
    REQUIRE(resFilter.object.asNodeset().size() == 1);
    REQUIRE(resFilter.object.asNodeset()[0]->getTagName() == "book");

    XPathQuery::Result resAttrFilter =
        XPathQuery("/catalog/*/@*[name() = 'category']").execute(&doc);
    REQUIRE(resAttrFilter.object.asNodeset().size() == 2);
}

TEST_CASE("XPath name() complex test on all node types") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    Attribute("xmlns:m", "http://example.com/meta"),
                    Comment("Sample comment"),
                    ProcessingInstruction("custom-engine", "mode=\"strict\""),
                    GenericNode("leaf", NonVoid, Text("Some raw text node")));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "meta") return "http://example.com/meta";
        return "";
    };

    XPathQuery::Result resRoot =
        XPathQuery("name(/root/..)", resolver).execute(&doc);
    REQUIRE(resRoot.object.asString() == "");

    XPathQuery::Result resText =
        XPathQuery("name(/root/leaf/text())", resolver).execute(&doc);
    REQUIRE(resText.object.asString() == "");

    XPathQuery::Result resComment =
        XPathQuery("name(/root/comment())", resolver).execute(&doc);
    REQUIRE(resComment.object.asString() == "");

    XPathQuery::Result resPI =
        XPathQuery("name(/root/processing-instruction())", resolver)
            .execute(&doc);
    REQUIRE(resPI.object.asString() == "custom-engine");

    XPathQuery::Result resNs =
        XPathQuery("name(/root/namespace::m)", resolver).execute(&doc);
    REQUIRE(resNs.object.asString() == "m");

    XPathQuery::Result resNsXml =
        XPathQuery("name(/root/namespace::xml)", resolver).execute(&doc);
    REQUIRE(resNsXml.object.asString() == "xml");
}

TEST_CASE("XPath name() empty nodeset and initial context evaluation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid, GenericNode("child", NonVoid));

    XPathQuery::Result resEmpty =
        XPathQuery("name(/root/nonexistent)").execute(&doc);
    REQUIRE(resEmpty.object.asString() == "");

    XPathQuery::Result resDocOrder =
        XPathQuery("name(/root/* | /root)").execute(&doc);
    REQUIRE(resDocOrder.object.asString() == "root");
}

TEST_CASE("XPath namespace-uri() on elements and attributes") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("item", NonVoid, Attribute("id", "101"),
                    Attribute("pref:flag", "active"),
                    Attribute("xmlns:pref", "http://example.com/pref")),
        GenericNode("pref:gadget", NonVoid,
                    Attribute("xmlns:pref", "http://example.com/pref"),
                    Attribute("rawAttr", "val")),
        GenericNode("plain", NonVoid));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "p") return "http://example.com/pref";
        return "";
    };

    XPathQuery::Result resPrefElem =
        XPathQuery("namespace-uri(/root/p:gadget)", resolver).execute(&doc);
    REQUIRE(resPrefElem.object.asString() == "http://example.com/pref");

    XPathQuery::Result resNoNsElem =
        XPathQuery("namespace-uri(/root/plain)", resolver).execute(&doc);
    REQUIRE(resNoNsElem.object.asString() == "");

    XPathQuery::Result resPrefAttr =
        XPathQuery("namespace-uri(/root/item/@p:flag)", resolver).execute(&doc);
    REQUIRE(resPrefAttr.object.asString() == "http://example.com/pref");

    XPathQuery::Result resPlainAttr =
        XPathQuery("namespace-uri(/root/item/@id)", resolver).execute(&doc);
    REQUIRE(resPlainAttr.object.asString() == "");
}

TEST_CASE("XPath namespace-uri() default namespace behavior") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("container", NonVoid,
                    Attribute("xmlns", "http://example.com/default"),
                    GenericNode("box", NonVoid, Attribute("weight", "10kg")));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "d") return "http://example.com/default";
        return "";
    };

    XPathQuery::Result resElem =
        XPathQuery("namespace-uri(/d:container/d:box)", resolver).execute(&doc);
    REQUIRE(resElem.object.asString() == "http://example.com/default");

    XPathQuery::Result resAttr =
        XPathQuery("namespace-uri(/d:container/d:box/@weight)", resolver)
            .execute(&doc);
    REQUIRE(resAttr.object.asString() == "");
}

TEST_CASE("XPath namespace-uri() test on all node types") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    Attribute("xmlns:m", "http://example.com/meta"),
                    Attribute("xml:lang", "en"), Comment("Sample comment"),
                    ProcessingInstruction("render-target", "format=\"pdf\""),
                    GenericNode("leaf", NonVoid, Text("Plain text child")));

    auto resolver = [](std::string_view p) -> std::string {
        if (p == "meta") return "http://example.com/meta";
        return "";
    };

    XPathQuery::Result resDoc =
        XPathQuery("namespace-uri(/root/..)", resolver).execute(&doc);
    REQUIRE(resDoc.object.asString() == "");

    XPathQuery::Result resText =
        XPathQuery("namespace-uri(//text())", resolver).execute(&doc);
    REQUIRE(resText.object.asString() == "");

    XPathQuery::Result resComment =
        XPathQuery("namespace-uri(//comment())", resolver).execute(&doc);
    REQUIRE(resComment.object.asString() == "");

    XPathQuery::Result resPI =
        XPathQuery("namespace-uri(//processing-instruction())", resolver)
            .execute(&doc);
    REQUIRE(resPI.object.asString() == "");

    XPathQuery::Result resXmlAttr =
        XPathQuery("namespace-uri(/root/@xml:lang)", resolver).execute(&doc);
    REQUIRE(resXmlAttr.object.asString() ==
            "http://www.w3.org/XML/1998/namespace");

    XPathQuery::Result resNs =
        XPathQuery("namespace-uri(/root/namespace::m)", resolver).execute(&doc);
    REQUIRE(resNs.object.asString() == "");

    XPathQuery::Result resNsXml =
        XPathQuery("namespace-uri(/root/namespace::xml)", resolver)
            .execute(&doc);
    REQUIRE(resNsXml.object.asString() == "");
}

TEST_CASE(
    "XPath namespace-uri() default context node evaluation in predicates") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "feed", NonVoid,
        GenericNode("a:entry", NonVoid,
                    Attribute("xmlns:a", "http://example.com/alpha"),
                    Attribute("id", "1")),
        GenericNode("b:entry", NonVoid,
                    Attribute("xmlns:b", "http://example.com/beta"),
                    Attribute("id", "2")),
        GenericNode("entry", NonVoid, Attribute("id", "3")));

    XPathQuery::Result resAlpha =
        XPathQuery("/feed/*[namespace-uri() = 'http://example.com/alpha']")
            .execute(&doc);
    REQUIRE(resAlpha.object.asNodeset().size() == 1);
    REQUIRE(resAlpha.object.asNodeset()[0]->getAttributeValue("id") == "1");

    XPathQuery::Result resNone =
        XPathQuery("/feed/*[namespace-uri() = '']").execute(&doc);
    REQUIRE(resNone.object.asNodeset().size() == 1);
    REQUIRE(resNone.object.asNodeset()[0]->getAttributeValue("id") == "3");

    GenericNode attrDoc(
        "root", NonVoid,
        GenericNode("node", NonVoid, Attribute("custom:flag", "yes"),
                    Attribute("xmlns:custom", "http://example.com/custom"),
                    Attribute("plain", "no")));

    XPathQuery::Result resAttrs =
        XPathQuery(
            "/root/node/@*[namespace-uri() = 'http://example.com/custom']")
            .execute(&attrDoc);
    REQUIRE(resAttrs.object.asNodeset().size() == 1);
    REQUIRE(resAttrs.object.asNodeset()[0]->getStringValue() == "yes");
}

TEST_CASE("XPath namespace-uri() document order and empty set edge cases") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("a:first", NonVoid,
                    Attribute("xmlns:a", "http://example.com/first")),
        GenericNode("b:second", NonVoid,
                    Attribute("xmlns:b", "http://example.com/second")),
        GenericNode("c:third", NonVoid,
                    Attribute("xmlns:c", "http://example.com/third")));

    XPathQuery::Result resEmpty =
        XPathQuery("namespace-uri(/root/missing)").execute(&doc);
    REQUIRE(resEmpty.object.asString() == "");

    XPathQuery::Result resMultiOrder =
        XPathQuery("namespace-uri(/root/*[3] | /root/*[1] | /root/*[2])")
            .execute(&doc);
    REQUIRE(resMultiOrder.object.asString() == "http://example.com/first");

    XPathQuery::Result resReverse =
        XPathQuery("namespace-uri(/root/*[3]/preceding-sibling::*)")
            .execute(&doc);
    REQUIRE(resReverse.object.asString() == "http://example.com/first");
}

TEST_CASE("XPath variable resolution of basic types with unprefixed names") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("item", NonVoid, Attribute("id", "first")),
                    GenericNode("item", NonVoid, Attribute("id", "second")),
                    GenericNode("item", NonVoid, Attribute("id", "third")));

    std::map<std::pair<std::string, std::string>, XPathObject> variables{
        {{"", "targetId"}, XPathObject("second")},
        {{"", "targetIndex"}, XPathObject(3.0)},
        {{"", "isEnabled"}, XPathObject(true)}};

    auto varProvider = [&](std::string_view uri,
                           std::string_view local) -> XPathObject {
        auto it = variables.find({std::string(uri), std::string(local)});
        if (it != variables.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable not found");
    };

    auto nsResolver = [](std::string_view) -> std::string { return ""; };

    XPathQuery::Result resString =
        XPathQuery("/root/item[@id = $targetId]", nsResolver)
            .execute(&doc, varProvider);
    REQUIRE(resString.object.asNodeset().size() == 1);
    REQUIRE(resString.object.asNodeset()[0]->getAttributeValue("id") ==
            "second");

    XPathQuery::Result resPos =
        XPathQuery("/root/item[$targetIndex]", nsResolver)
            .execute(&doc, varProvider);
    REQUIRE(resPos.object.asNodeset().size() == 1);
    REQUIRE(resPos.object.asNodeset()[0]->getAttributeValue("id") == "third");

    XPathQuery::Result resBool =
        XPathQuery("/root/item[$isEnabled]", nsResolver)
            .execute(&doc, varProvider);
    REQUIRE(resBool.object.asNodeset().size() == 3);
}

TEST_CASE("XPath variable resolution of prefixed QNames with URI decoupling") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid,
                    GenericNode("box", NonVoid, Attribute("code", "A100")),
                    GenericNode("box", NonVoid, Attribute("code", "B200")));

    const std::string paramNsUri = "http://example.com/params";

    std::map<std::pair<std::string, std::string>, XPathObject> variables{
        {{paramNsUri, "filterCode"}, XPathObject("B200")},
        {{"", "filterCode"}, XPathObject("A100")}};

    auto varProvider = [&](std::string_view uri,
                           std::string_view local) -> XPathObject {
        auto it = variables.find({std::string(uri), std::string(local)});
        if (it != variables.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable not bound: " + std::string(uri) +
                                 ":" + std::string(local));
    };

    auto nsResolver = [&](std::string_view prefix) -> std::string {
        if (prefix == "p") return paramNsUri;
        return "";
    };

    XPathQuery::Result resPrefixed =
        XPathQuery("/root/box[@code = $p:filterCode]", nsResolver)
            .execute(&doc, varProvider);
    REQUIRE(resPrefixed.object.asNodeset().size() == 1);
    REQUIRE(resPrefixed.object.asNodeset()[0]->getAttributeValue("code") ==
            "B200");

    XPathQuery::Result resUnprefixed =
        XPathQuery("/root/box[@code = $filterCode]", nsResolver)
            .execute(&doc, varProvider);
    REQUIRE(resUnprefixed.object.asNodeset().size() == 1);
    REQUIRE(resUnprefixed.object.asNodeset()[0]->getAttributeValue("code") ==
            "A100");

    auto altNsResolver = [&](std::string_view prefix) -> std::string {
        if (prefix == "custom") return paramNsUri;
        return "";
    };
    XPathQuery::Result resAltPrefix =
        XPathQuery("/root/box[@code = $custom:filterCode]", altNsResolver)
            .execute(&doc, varProvider);
    REQUIRE(resAltPrefix.object.asNodeset().size() == 1);
    REQUIRE(resAltPrefix.object.asNodeset()[0]->getAttributeValue("code") ==
            "B200");
}

TEST_CASE(
    "XPath variable resolution of nodeset variables and path continuation") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc(
        "root", NonVoid,
        GenericNode("section", NonVoid, Attribute("id", "sec1"),
                    GenericNode("title", NonVoid, Text("Alpha")),
                    GenericNode("para", NonVoid, Text("First paragraph"))),
        GenericNode("section", NonVoid, Attribute("id", "sec2"),
                    GenericNode("title", NonVoid, Text("Beta")),
                    GenericNode("para", NonVoid, Text("Second paragraph"))));

    XPathQuery::Result initialQuery =
        XPathQuery("/root/section[@id = 'sec2']").execute(&doc);

    std::map<std::pair<std::string, std::string>, XPathObject> variables{
        {{"", "selectedSection"}, initialQuery.object}};

    auto varProvider = [&](std::string_view uri,
                           std::string_view local) -> XPathObject {
        return variables.at({std::string(uri), std::string(local)});
    };

    XPathQuery::Result resPath =
        XPathQuery("$selectedSection/title").execute(&doc, varProvider);
    REQUIRE(resPath.object.asNodeset().size() == 1);
    REQUIRE(resPath.object.asNodeset()[0]->getStringValue() == "Beta");

    XPathQuery::Result resPred =
        XPathQuery("$selectedSection[para = 'Second paragraph']")
            .execute(&doc, varProvider);
    REQUIRE(resPred.object.asNodeset().size() == 1);
    REQUIRE(resPred.object.asNodeset()[0]->getAttributeValue("id") == "sec2");
}

TEST_CASE(
    "XPath variable resolution with undeclared namespace prefix throws on "
    "compile time") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "known") return "http://example.com/known";
        return "";
    };

    REQUIRE_THROWS_WITH(
        XPathQuery("/root[$unknownPrefix:var = 'value']", resolver),
        Catch::Matchers::ContainsSubstring(
            "Could not resolve namespace prefix in query"));
}

TEST_CASE("XPath variable resolving exception propagates to caller") {
    using namespace onyx::dynamic::xpath;
    using namespace onyx::tags;

    GenericNode doc("root", NonVoid);

    auto resolver = [](std::string_view prefix) -> std::string {
        if (prefix == "known") return "http://example.com/known";
        return "";
    };

    auto varProvider = [](std::string_view, std::string_view) -> XPathObject {
        throw std::runtime_error("Variable lookup failure");
    };

    REQUIRE_THROWS_WITH(
        XPathQuery("/root[$known:missingVar = 'value']", resolver)
            .execute(&doc, varProvider),
        Catch::Matchers::ContainsSubstring("Variable lookup failure"));
}