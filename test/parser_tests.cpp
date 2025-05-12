#include "catch2/catch_all.hpp"
#include "templater.h"

#include <chrono>

TEST_CASE("dom parser works") {
    using namespace Templater::tags;
    using namespace Templater::parser;

    std::string input = "<html><head></head></html>";

    GenericNode output{
        "html", false,
        GenericNode("head", false)
    };

    ParseResult pr = DomParser::parse(input);

    REQUIRE(output.deepEquals(*pr.root));
}

TEST_CASE("dom parser works with text") {
    using namespace Templater::tags;
    using namespace Templater::parser;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::string input = "<html><body><div> Hello<span></span>World! </div></body></html>";
    
    GenericNode output{
        "html", false,
        GenericNode("body", false,
            GenericNode("div", false,
                Text(" Hello"), GenericNode("span", false), Text("World! ")))
    };

    auto t1 = high_resolution_clock::now();
        ParseResult pr = DomParser::parse(input);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;

    INFO(time.count());

    INFO(pr.root->serialize());
    REQUIRE(output.deepEquals(*pr.root));
}