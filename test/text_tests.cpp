#include <chrono>
#include <optional>

#include "catch2/catch_all.hpp"
#include "onyx.h"
#include "text.h"

TEST_CASE("Escapes complex html", "[escape]") {
    using namespace onyx::text;

    std::string textToEscape =
        "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My "
        "Awesome Website</span></h1><p>Today's date is: "
        "<script>alert('Hacked!');</script></p><a "
        "href=\"https://example.com?param=<script>evil()</script>\">Click "
        "here</a><p>&copy; 2025 My Awesome Website</p></div>";

    std::string escaped =
        "&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to &lt;span "
        "style=&quot;color: red;&quot;&gt;My Awesome "
        "Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: "
        "&lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a "
        "href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/"
        "script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My "
        "Awesome Website&lt;/p&gt;&lt;/div&gt;";

    CHECK(escaped == escape(textToEscape));
}

TEST_CASE("Empty string remains unchanged", "[escape]") {
    using namespace onyx::text;
    std::string input = "";
    std::string expected = "";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Reserved HTML characters are properly escaped", "[escape]") {
    using namespace onyx::text;
    std::string input = "&<>\"'";
    std::string expected = "&amp;&lt;&gt;&quot;&#39;";
    REQUIRE(escape(input) == expected);
}

TEST_CASE("Non-escaping ASCII characters remain unchanged", "[escape]") {
    using namespace onyx::text;
    std::string input = "Hello, World!";
    std::string expected = "Hello, World!";
    REQUIRE(escape(input) == expected);
}

TEST_CASE(
    "Single non-ASCII character is converted to a numeric entity when "
    "multi-byte escaping is enabled",
    "[escape]") {
    using namespace onyx::text;
    std::string input = "café";
    std::string expected = "caf&#xe9;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE(
    "Single non-ASCII character is not converted to a numeric entity when "
    "multi-byte escaping is disabled",
    "[escape]") {
    using namespace onyx::text;
    std::string input = "café";
    std::string expected = "café";
    REQUIRE(escape(input, false) == expected);
}

TEST_CASE("Emoji (4-byte sequence) is converted to a numeric entity",
          "[escape]") {
    using namespace onyx::text;
    std::string input = "😊";
    std::string expected = "&#x1f60a;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE(
    "Mixed content with ASCII, reserved characters, and multi-byte sequences",
    "[escape]") {
    using namespace onyx::text;
    std::string input = "Hello <world> & café 😊";
    std::string expected = "Hello &lt;world&gt; &amp; caf&#xe9; &#x1f60a;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Escapes 1 million characters in under 150ms", "[escape]") {
    using namespace onyx::text;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
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
    using namespace onyx::text;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    std::string input(1'000'000, 'a');

    INFO(input.size());

    auto t1 = high_resolution_clock::now();
    std::string result = escape(input, true);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;

    REQUIRE(time.count() < 100);
}

TEST_CASE("Escapes random sequence correctly", "[escapeSequence]") {
    using namespace onyx::text;
    std::string input =
        "This is an ill--formatted html comment with two -- inside!";
    std::string expected =
        "This is an ill&#x2d;&#x2d;formatted html comment with two "
        "&#x2d;&#x2d; inside!";
    REQUIRE(escapeSequence(input, "--") == expected);
}

TEST_CASE("Escapes random sequence correctly", "[replaceSequence]") {
    using namespace onyx::text;
    std::string input =
        "This is an ill--formatted html comment with two -- inside!";
    std::string expected =
        "This is an ill- -formatted html comment with two - - inside!";
    REQUIRE(replaceSequence(input, "--", "- -") == expected);
}

TEST_CASE("Escapes empty sequence correctly", "[escapeSequence]") {
    using namespace onyx::text;
    std::string input = "This is a good sequence!";
    REQUIRE(escapeSequence(input, "") == input);
}

TEST_CASE("Does not escape sequence when not in string", "[replaceSequence]") {
    using namespace onyx::text;
    std::string input = "This is a good sequence!";
    REQUIRE(replaceSequence(input, "--", "- -") == input);
}

TEST_CASE("Escapes single sequence correctly", "[escapeSequence]") {
    using namespace onyx::text;
    std::string input = "-";
    std::string expected = "&#x2d;";
    REQUIRE(escapeSequence(input, "-") == expected);
}

TEST_CASE("Escapes unicode sequence correctly", "[escapeSequence]") {
    using namespace onyx::text;
    std::string input = "Hello! 😊😊";
    std::string expected = "Hello! &#x1f60a;&#x1f60a;";
    REQUIRE(escapeSequence(input, "😊😊") == input);
}

TEST_CASE("Basic replacements", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"cat", "dog"}, {"bat", "rat"}};

    std::string input = "The cat chased the bat.";
    std::string expected = "The dog chased the rat.";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("No replacements when no keys match", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"zebra", "lion"}, {"monkey", "ape"}};

    std::string input = "No matching keys here.";
    REQUIRE(replaceSequences(input, dict) == input);
}

TEST_CASE("Empty input string returns empty string", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"hello", "hi"}};

    std::string input = "";
    REQUIRE(replaceSequences(input, dict).empty());
}

TEST_CASE("Empty dictionary returns input unchanged", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict;

    std::string input = "Sample text.";
    REQUIRE(replaceSequences(input, dict) == input);
}

TEST_CASE("Single character replacements", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{{"a", "1"},
                                                                    {"b", "2"}};

    std::string input = "abba";
    std::string expected = "1221";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("Overlapping keys - order sensitive", "[replaceSequences]") {
    using namespace onyx::text;
    // Order matters: first "aa" then "a"
    std::vector<std::pair<std::string_view, std::string_view>> dict{{"aa", "X"},
                                                                    {"a", "Y"}};

    std::string input = "aaaa";
    // Replace "aa" pairs first, so "aaaa" -> "XX"
    REQUIRE(replaceSequences(input, dict) == "XX");
}

TEST_CASE("Overlapping keys - reversed order", "[replaceSequences]") {
    using namespace onyx::text;
    // Now first "a", then "aa"
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"a", "Y"}, {"aa", "X"}};

    std::string input = "aaaa";
    // Since "a" replaces first, it will replace all 'a's individually: "YYYY"
    REQUIRE(replaceSequences(input, dict) == "YYYY");
}

TEST_CASE("Full string replacement", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"entire", "whole"}};

    std::string input = "entire";
    std::string expected = "whole";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("Case sensitivity check", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"Hello", "Hi"}, {"world", "Earth"}};

    std::string input = "Hello world. hello World.";
    std::string expected = "Hi Earth. hello World.";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE(" Replacement strings containing keys - no recursion",
          "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"a", "ab"}, {"b", "bc"}};

    std::string input = "ab";
    std::string result = replaceSequences(input, dict);
    REQUIRE(result == "abbc");
}

TEST_CASE("Key is empty string", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{{"", "X"}};

    std::string input = "abc";
    REQUIRE(replaceSequences(input, dict) == input);
}

TEST_CASE("Replacement string empty (deletion)", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{{"a", ""}};

    std::string input = "abracadabra";
    std::string expected = "brcdbr";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("Multiple replacements in sequence", "[replaceSequences]") {
    using namespace onyx::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"foo", "bar"}, {"bar", "baz"}, {"baz", "qux"}};

    std::string input = "foo bar baz";
    std::string expected = "bar baz qux";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("No entities: returns original string",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    std::string input = "Hello, World!";
    REQUIRE(expandEntitiesAndNormalizeEol(input, {}, '\n') == input);
}

TEST_CASE("Named entities: basic XML escapes",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    REQUIRE(expandEntitiesAndNormalizeEol("&lt;&gt;&amp;&quot;&apos;", {},
                                          '\n') == std::string("<>&\"'"));
}

TEST_CASE("Mixed content with named entities",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    std::string input = "1 &lt; 2 &amp;&amp; 3 &gt; 2";
    std::string expected = "1 < 2 && 3 > 2";
    REQUIRE(expandEntitiesAndNormalizeEol(input, {}, '\n') == expected);
}

TEST_CASE("Decimal numeric entities", "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    std::string input = "&#65;&#66;&#67;";  // A B C
    std::string expected = "ABC";
    REQUIRE(expandEntitiesAndNormalizeEol(input, {}, '\n') == expected);
}

TEST_CASE("Hex numeric entities", "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    std::string input = "&#x41;&#x42;&#x43;";  // A B C
    std::string expected = "ABC";
    REQUIRE(expandEntitiesAndNormalizeEol(input, {}, '\n') == expected);
}

TEST_CASE("Mixed decimal, hex, and named entities",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    // &x26 is an invalid entity
    REQUIRE_THROWS_WITH(
        expandEntitiesAndNormalizeEol("X &lt; &#60; &amp; # &x26;", {}, '\n'),
        "& outside of entities not allowed.");
}

TEST_CASE("Invalid or unterminated entities throw",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol(
                            "&unknown; &incomplete &amp something;", {}, '\n'),
                        "& outside of entities not allowed.");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol(
                            "&unknown; &amp &amp; something;", {}, '\n'),
                        "& outside of entities not allowed.");
}

TEST_CASE("expandEntities expands \\r to \\n",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    REQUIRE(
        expandEntitiesAndNormalizeEol("Some text \r other text.", {}, '\n') ==
        std::string("Some text \n other text."));
    REQUIRE(
        expandEntitiesAndNormalizeEol("Some text other text.\r", {}, '\n') ==
        std::string("Some text other text.\n"));
}

TEST_CASE("expandEntities expands \\r\\n to \\n",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;
    REQUIRE(
        expandEntitiesAndNormalizeEol("Some text \r\n other text.", {}, '\n') ==
        std::string("Some text \n other text."));
    REQUIRE(
        expandEntitiesAndNormalizeEol("Some text other text.\r\n", {}, '\n') ==
        std::string("Some text other text.\n"));
}

TEST_CASE(
    "Numeric entities resolving to forbidden ASCII control characters throw",
    "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;

    // 0x00 (Null)
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#0;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#x00;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");

    // 0x0B (Vertical Tab) - inside the forbidden 0x00-0x1F block
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#11;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#x0B;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");

    // 0x1F (Information Separator One) - upper bound of forbidden ASCII
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#31;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#x1F;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
}

TEST_CASE("Numeric entities resolving to UTF-16 Surrogate Blocks throw",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;

    // 0xD800 (Start of High Surrogates)
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#55296;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xD800;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");

    // 0xDBFF (End of High Surrogates)
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xDBFF;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");

    // 0xDC00 (Start of Low Surrogates)
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xDC00;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");

    // 0xDFFF (End of Low Surrogates)
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#57343;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xDFFF;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
}

TEST_CASE("Numeric entities resolving to restricted U+FFFE or U+FFFF throw",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;

    // 0xFFFE
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#65534;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xFFFE;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");

    // 0xFFFF
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#65535;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xFFFF;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
}

TEST_CASE("Numeric entities resolving to out-of-bounds Unicode throw",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;

    // 0x110000 (Outside the maximum valid Unicode scalar U+10FFFF)
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#1114112;", {}, '\n'),
                        "Numeric entity resolves to an invalid XML character");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#x110000;", {}, '\n'),
                        "& outside of entities not allowed.");
    REQUIRE_THROWS_WITH(expandEntitiesAndNormalizeEol("&#xFFFFFFFF;", {}, '\n'),
                        "& outside of entities not allowed.");
}

TEST_CASE("Numeric entities resolving to boundary valid XML characters succeed",
          "[expandEntitiesAndNormalizeEol]") {
    using namespace onyx::text;

    // Allowed Control Characters (Tab, LF, CR)
    REQUIRE(expandEntitiesAndNormalizeEol("&#x09;", {}, '\n') == "\t");
    REQUIRE(expandEntitiesAndNormalizeEol("&#x0A;", {}, '\n') == "\n");
    REQUIRE(expandEntitiesAndNormalizeEol("&#x0D;", {}, '\n') == "\r");

    // 0x20 (Space) - Lower bound of standard printable characters
    REQUIRE(expandEntitiesAndNormalizeEol("&#x20;", {}, '\n') == " ");

    // 0xD7FF - Character immediately preceding the Surrogate Block
    // UTF-8 encoding for U+D7FF is ED 9F BF
    REQUIRE(expandEntitiesAndNormalizeEol("&#xD7FF;", {}, '\n') ==
            "\xED\x9F\xBF");

    // 0xE000 - Character immediately following the Surrogate Block
    // UTF-8 encoding for U+E000 is EE 80 80
    REQUIRE(expandEntitiesAndNormalizeEol("&#xE000;", {}, '\n') ==
            "\xEE\x80\x80");

    // 0xFFFD (Replacement Character) - Character immediately preceding
    // FFFE/FFFF restriction
    // UTF-8 encoding for U+FFFD is EF BF BD
    REQUIRE(expandEntitiesAndNormalizeEol("&#xFFFD;", {}, '\n') ==
            "\xEF\xBF\xBD");

    // 0x10000 - Character immediately following FFFE/FFFF restriction
    // UTF-8 encoding for U+10000 is F0 90 80 80
    REQUIRE(expandEntitiesAndNormalizeEol("&#x10000;", {}, '\n') ==
            "\xF0\x90\x80\x80");

    // 0x10FFFF - The absolute maximum valid Unicode character
    // UTF-8 encoding for U+10FFFF is F4 8F BF BF
    REQUIRE(expandEntitiesAndNormalizeEol("&#x10FFFF;", {}, '\n') ==
            "\xF4\x8F\xBF\xBF");
}

TEST_CASE("expandText expands \\r to \\n", "[expandText]") {
    using namespace onyx::text;
    REQUIRE(expandText("Some text \r other text.") ==
            std::string("Some text \n other text."));
    REQUIRE(expandText("Some text other text.\r") ==
            std::string("Some text other text.\n"));
}

TEST_CASE("expandText expands \\r\\n to \\n", "[expandText]") {
    using namespace onyx::text;
    REQUIRE(expandText("Some text \r\n other text.") ==
            std::string("Some text \n other text."));
    REQUIRE(expandText("Some text other text.\r\n") ==
            std::string("Some text other text.\n"));
}

TEST_CASE("expandEOLOnly expands \\r to \\n", "[expandEOLOnly]") {
    using namespace onyx::text;
    REQUIRE(expandEOLOnly("Some text \r other text.") ==
            std::string("Some text \n other text."));
    REQUIRE(expandEOLOnly("Some text other text.\r") ==
            std::string("Some text other text.\n"));
}

TEST_CASE("expandEOLOnly expands \\r\\n to \\n", "[expandEOLOnly]") {
    using namespace onyx::text;
    REQUIRE(expandEOLOnly("Some text \r\n other text.") ==
            std::string("Some text \n other text."));
    REQUIRE(expandEOLOnly("Some text other text.\r\n") ==
            std::string("Some text other text.\n"));
}

TEST_CASE("expandAttributeValue expands \\r to ' '", "[expandAttributeValue]") {
    using namespace onyx::text;
    REQUIRE(expandAttributeValue("Some text \r other text.") ==
            std::string("Some text   other text."));
    REQUIRE(expandAttributeValue("Some text other text.\r") ==
            std::string("Some text other text. "));
}

TEST_CASE("expandAttributeValue expands \\r\\n to ' '",
          "[expandAttributeValue]") {
    using namespace onyx::text;
    REQUIRE(expandAttributeValue("Some text \r\n other text.") ==
            std::string("Some text   other text."));
    REQUIRE(expandAttributeValue("Some text other text.\r\n") ==
            std::string("Some text other text. "));
}

TEST_CASE("expandAttributeValue expands \\t to ' '", "[expandAttributeValue]") {
    using namespace onyx::text;
    REQUIRE(expandAttributeValue("Some text \t other text.") ==
            std::string("Some text   other text."));
    REQUIRE(expandAttributeValue("Some text other text.\t") ==
            std::string("Some text other text. "));
}

TEST_CASE("expandAttributeValue expands \\n to ' '", "[expandAttributeValue]") {
    using namespace onyx::text;
    REQUIRE(expandAttributeValue("Some text \n other text.") ==
            std::string("Some text   other text."));
    REQUIRE(expandAttributeValue("Some text other text.\n") ==
            std::string("Some text other text. "));
}

TEST_CASE("expandAttributeValue expands complex sequence properly",
          "[expandAttributeValue]") {
    using namespace onyx::text;
    REQUIRE(expandAttributeValue("Some text \r\r\n\t\n other text.") ==
            std::string("Some text      other text."));
    REQUIRE(expandAttributeValue("Some text other text.\r\r\n\t\n") ==
            std::string("Some text other text.    "));
}

TEST_CASE("Empty string returns nullopt without allocation",
          "[transcodeToUtf8]") {
    using namespace onyx::text;
    REQUIRE(transcodeToUtf8("", "ISO-8859-1") == std::nullopt);
    REQUIRE(transcodeToUtf8("", "UTF-8") == std::nullopt);
}

TEST_CASE("UTF-8 encoding triggers fast path", "[transcodeToUtf8]") {
    using namespace onyx::text;
    std::string input = "Hello, World!";

    REQUIRE(transcodeToUtf8(input, "UTF-8") == std::nullopt);
}

TEST_CASE("ASCII encoding triggers fast path", "[transcodeToUtf8]") {
    using namespace onyx::text;
    std::string input = "Standard ASCII text";
    REQUIRE(transcodeToUtf8(input, "ASCII") == std::nullopt);
}

TEST_CASE("Transcodes ISO-8859-1 (Latin-1) to UTF-8", "[transcodeToUtf8]") {
    using namespace onyx::text;
    // 'café' in ISO-8859-1: c a f \xE9
    std::string input = "caf\xE9";
    std::string expected = "caf\xC3\xA9";

    std::optional<std::string> result = transcodeToUtf8(input, "ISO-8859-1");
    REQUIRE(result.has_value());
    REQUIRE(result.value() == expected);
}

TEST_CASE("Transcodes UTF-16LE to UTF-8", "[transcodeToUtf8]") {
    using namespace onyx::text;
    // "ABC" in UTF-16LE is \x41\x00 \x42\x00 \x43\x00
    std::string input = {'\x41', '\x00', '\x42', '\x00', '\x43', '\x00'};
    std::string expected = "ABC";

    std::optional<std::string> result =
        transcodeToUtf8(std::string_view(input.data(), 6), "UTF-16LE");
    REQUIRE(result.has_value());
    REQUIRE(result.value() == expected);
}

TEST_CASE("Dynamically expands buffer when E2BIG is triggered",
          "[transcodeToUtf8]") {
    using namespace onyx::text;

    // Create a string of 1000 ISO-8859-1 'é' characters
    // Initial capacity guess is 1500 bytes
    // The actual UTF-8 output will be 2000 bytes
    // This guarantees the E2BIG continuation loop will execute and resize
    // safely
    std::string input(1000, '\xE9');

    std::string expected;
    expected.reserve(2000);
    for (int i = 0; i < 1000; i++) {
        expected += "\xC3\xA9";
    }

    std::optional<std::string> result = transcodeToUtf8(input, "ISO-8859-1");
    REQUIRE(result.has_value());
    REQUIRE(result.value().size() == 2000);
    REQUIRE(result.value() == expected);
}

TEST_CASE("Throws runtime_error on unknown or invalid encoding",
          "[transcodeToUtf8]") {
    using namespace onyx::text;
    std::string input = "Some text";

    REQUIRE_THROWS_WITH(
        transcodeToUtf8(input, "EXAMPLE-ENCODING"),
        Catch::Matchers::ContainsSubstring("Failed to initialize iconv_open"));
}

TEST_CASE("Throws runtime_error on incomplete multibyte sequence",
          "[transcodeToUtf8]") {
    using namespace onyx::text;
    // Missing the trailing \x00 for the final UTF-16 character
    std::string input = "\x41\x00\x42";

    REQUIRE_THROWS_WITH(
        transcodeToUtf8(std::string_view(input.data(), 3), "UTF-16LE"),
        "Incomplete multibyte sequence in input");
}

TEST_CASE("Throws runtime_error on invalid multibyte sequence",
          "[transcodeToUtf8]") {
    using namespace onyx::text;
    // \xFF is not a valid starting byte in UTF-8.
    std::string input = "\xFF\xFF\xFF";

    REQUIRE_THROWS_WITH(
        transcodeToUtf8(input, "EUC-JP"),
        Catch::Matchers::ContainsSubstring("Invalid multibyte sequence"));
}

TEST_CASE("Transcodes 1 million characters in under 150ms",
          "[transcodeToUtf8]") {
    using namespace onyx::text;
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

    // 1 million ISO-8859-1 'é' characters
    std::string input(1'000'000, '\xE9');

    INFO("String size: " << input.size());

    auto t1 = high_resolution_clock::now();
    std::optional<std::string> result = transcodeToUtf8(input, "ISO-8859-1");
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;

    REQUIRE(result.has_value());
    REQUIRE(result.value().size() == 2'000'000);
    REQUIRE(time.count() < 150.0);
}