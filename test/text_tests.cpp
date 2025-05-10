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
