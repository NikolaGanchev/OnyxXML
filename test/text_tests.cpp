#include "catch2/catch_all.hpp"
#include "templater.h"

#include <chrono>

TEST_CASE("Escapes complex html", "[escape]" ) {
    using namespace Templater::text;

    std::string textToEscape = "<div class=\"content\"><h1>Welcome to <span style=\"color: red;\">My Awesome Website</span></h1><p>Today's date is: <script>alert('Hacked!');</script></p><a href=\"https://example.com?param=<script>evil()</script>\">Click here</a><p>&copy; 2025 My Awesome Website</p></div>";

    std::string escaped = "&lt;div class=&quot;content&quot;&gt;&lt;h1&gt;Welcome to &lt;span style=&quot;color: red;&quot;&gt;My Awesome Website&lt;/span&gt;&lt;/h1&gt;&lt;p&gt;Today&#39;s date is: &lt;script&gt;alert(&#39;Hacked!&#39;);&lt;/script&gt;&lt;/p&gt;&lt;a href=&quot;https://example.com?param=&lt;script&gt;evil()&lt;/script&gt;&quot;&gt;Click here&lt;/a&gt;&lt;p&gt;&amp;copy; 2025 My Awesome Website&lt;/p&gt;&lt;/div&gt;";

    CHECK(escaped == escape(textToEscape));
}

TEST_CASE("Empty string remains unchanged", "[escape]") {
    using namespace Templater::text;
    std::string input = "";
    std::string expected = "";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Reserved HTML characters are properly escaped", "[escape]") {
    using namespace Templater::text;
    std::string input = "&<>\"'";
    std::string expected = "&amp;&lt;&gt;&quot;&#39;";
    REQUIRE(escape(input) == expected);
}

TEST_CASE("Non-escaping ASCII characters remain unchanged", "[escape]") {
    using namespace Templater::text;
    std::string input = "Hello, World!";
    std::string expected = "Hello, World!";
    REQUIRE(escape(input) == expected);
}

TEST_CASE("Single non-ASCII character is converted to a numeric entity when multi-byte escaping is enabled", "[escape]") {
    using namespace Templater::text;
    std::string input = "café";
    std::string expected = "caf&#xe9;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Single non-ASCII character is not converted to a numeric entity when multi-byte escaping is disabled", "[escape]") {
    using namespace Templater::text;
    std::string input = "café";
    std::string expected = "café";
    REQUIRE(escape(input, false) == expected);
}

TEST_CASE("Emoji (4-byte sequence) is converted to a numeric entity", "[escape]") {
    using namespace Templater::text;
    std::string input = "😊";
    std::string expected = "&#x1f60a;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Mixed content with ASCII, reserved characters, and multi-byte sequences", "[escape]") {
    using namespace Templater::text;
    std::string input = "Hello <world> & café 😊";
    std::string expected = "Hello &lt;world&gt; &amp; caf&#xe9; &#x1f60a;";
    REQUIRE(escape(input, true) == expected);
}

TEST_CASE("Escapes 1 million characters in under 150ms", "[escape]") {
    using namespace Templater::text;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
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
    using namespace Templater::text;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
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
    using namespace Templater::text;
    std::string input = "This is an ill--formatted html comment with two -- inside!";
    std::string expected = "This is an ill&#x2d;&#x2d;formatted html comment with two &#x2d;&#x2d; inside!";
    REQUIRE(escapeSequence(input, "--") == expected);
}

TEST_CASE("Escapes random sequence correctly", "[replaceSequence]") {
    using namespace Templater::text;
    std::string input = "This is an ill--formatted html comment with two -- inside!";
    std::string expected = "This is an ill- -formatted html comment with two - - inside!";
    REQUIRE(replaceSequence(input, "--", "- -") == expected);
}

TEST_CASE("Escapes empty sequence correctly", "[escapeSequence]") {
    using namespace Templater::text;
    std::string input = "This is a good sequence!";
    REQUIRE(escapeSequence(input, "") == input);
}

TEST_CASE("Does not escape sequence when not in string", "[replaceSequence]") {
    using namespace Templater::text;
    std::string input = "This is a good sequence!";
    REQUIRE(replaceSequence(input, "--", "- -") == input);
}

TEST_CASE("Escapes single sequence correctly", "[escapeSequence]") {
    using namespace Templater::text;
    std::string input = "-";
    std::string expected = "&#x2d;";
    REQUIRE(escapeSequence(input, "-") == expected);
}

TEST_CASE("Escapes unicode sequence correctly", "[escapeSequence]") {
    using namespace Templater::text;
    std::string input = "Hello! 😊😊";
    std::string expected = "Hello! &#x1f60a;&#x1f60a;";
    REQUIRE(escapeSequence(input, "😊😊") == input);
}

TEST_CASE("Basic replacements", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"cat", "dog"},
        {"bat", "rat"}
    };

    std::string input = "The cat chased the bat.";
    std::string expected = "The dog chased the rat.";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("No replacements when no keys match", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"zebra", "lion"},
        {"monkey", "ape"}
    };

    std::string input = "No matching keys here.";
    REQUIRE(replaceSequences(input, dict) == input);
}

TEST_CASE("Empty input string returns empty string", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"hello", "hi"}
    };

    std::string input = "";
    REQUIRE(replaceSequences(input, dict).empty());
}

TEST_CASE("Empty dictionary returns input unchanged", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict;

    std::string input = "Sample text.";
    REQUIRE(replaceSequences(input, dict) == input);
}

TEST_CASE("Single character replacements", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"a", "1"},
        {"b", "2"}
    };

    std::string input = "abba";
    std::string expected = "1221";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("Overlapping keys - order sensitive", "[replaceSequences]") {
    using namespace Templater::text;
    // Order matters: first "aa" then "a"
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"aa", "X"},
        {"a", "Y"}
    };

    std::string input = "aaaa";
    // Replace "aa" pairs first, so "aaaa" -> "XX"
    REQUIRE(replaceSequences(input, dict) == "XX");
}

TEST_CASE("Overlapping keys - reversed order", "[replaceSequences]") {
    using namespace Templater::text;
    // Now first "a", then "aa"
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"a", "Y"},
        {"aa", "X"}
    };

    std::string input = "aaaa";
    // Since "a" replaces first, it will replace all 'a's individually: "YYYY"
    REQUIRE(replaceSequences(input, dict) == "YYYY");
}

TEST_CASE("Full string replacement", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"entire", "whole"}
    };

    std::string input = "entire";
    std::string expected = "whole";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("Case sensitivity check", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"Hello", "Hi"},
        {"world", "Earth"}
    };

    std::string input = "Hello world. hello World.";
    std::string expected = "Hi Earth. hello World.";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE(" Replacement strings containing keys - no recursion", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"a", "ab"},
        {"b", "bc"}
    };

    std::string input = "ab";
    std::string result = replaceSequences(input, dict);
    REQUIRE(result == "abbc");
}

TEST_CASE("Key is empty string", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"", "X"}
    };

    std::string input = "abc";
    REQUIRE(replaceSequences(input, dict) == input);
}

TEST_CASE("Replacement string empty (deletion)", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"a", ""}
    };

    std::string input = "abracadabra";
    std::string expected = "brcdbr";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("Multiple replacements in sequence", "[replaceSequences]") {
    using namespace Templater::text;
    std::vector<std::pair<std::string_view, std::string_view>> dict{
        {"foo", "bar"},
        {"bar", "baz"},
        {"baz", "qux"}
    };

    std::string input = "foo bar baz";
    std::string expected = "bar baz qux";
    REQUIRE(replaceSequences(input, dict) == expected);
}

TEST_CASE("No entities: returns original string", "[expandEntities]") {
    using namespace Templater::text;
    std::string input = "Hello, World!";
    REQUIRE(expandEntities(input) == input);
}

TEST_CASE("Named entities: basic XML escapes", "[expandEntities]") {
    using namespace Templater::text;
    REQUIRE(expandEntities("&lt;&gt;&amp;&quot;&apos;") == std::string("<>&\"'"));
}

TEST_CASE("Mixed content with named entities", "[expandEntities]") {
    using namespace Templater::text;
    std::string input = "1 &lt; 2 &amp;&amp; 3 &gt; 2";
    std::string expected = "1 < 2 && 3 > 2";
    REQUIRE(expandEntities(input) == expected);
}

TEST_CASE("Decimal numeric entities", "[expandEntities]") {
    using namespace Templater::text;
    std::string input = "&#65;&#66;&#67;";  // A B C
    std::string expected = "ABC";
    REQUIRE(expandEntities(input) == expected);
}

TEST_CASE("Hex numeric entities", "[expandEntities]") {
    using namespace Templater::text;
    std::string input = "&#x41;&#x42;&#x43;";  // A B C
    std::string expected = "ABC";
    REQUIRE(expandEntities(input) == expected);
}

TEST_CASE("Mixed decimal, hex, and named entities", "[expandEntities]") {
    using namespace Templater::text;
    std::string input = "X &lt; &#60; &amp; # &x26;";
    // Here, &#60; == '<', &lt; == '<', &amp; == '&', # is literal, &x26; is invalid so left unchanged
    std::string expected = "X < < & # &x26;";
    REQUIRE(expandEntities(input) == expected);
}

TEST_CASE("Invalid or unterminated entities are left intact", "[expandEntities]") {
    using namespace Templater::text;
    REQUIRE(expandEntities("&unknown; &incomplete &amp something;") == std::string("&unknown; &incomplete &amp something;"));
}
