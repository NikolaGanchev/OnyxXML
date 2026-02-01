#include <memory>
#include <stack>

#include "xpath/functions.h"
#include "xpath/virtual_machine.h"

namespace onyx::dynamic::xpath {

#define GET_THREE_STACK_STRINGS(functionName)                           \
    if (stack.size() < 3) {                                             \
        throw std::runtime_error(#functionName " needs two arguments"); \
    }                                                                   \
                                                                        \
    std::string str3 = stack.top().asString();                          \
                                                                        \
    stack.pop();                                                        \
                                                                        \
    std::string str2 = stack.top().asString();                          \
                                                                        \
    stack.pop();                                                        \
                                                                        \
    std::string str1 = stack.top().asString();                          \
                                                                        \
    stack.pop();

#define GET_TWO_STACK_STRINGS(functionName)                             \
    if (stack.size() < 2) {                                             \
        throw std::runtime_error(#functionName " needs two arguments"); \
    }                                                                   \
                                                                        \
    std::string str2 = stack.top().asString();                          \
                                                                        \
    stack.pop();                                                        \
                                                                        \
    std::string str1 = stack.top().asString();                          \
                                                                        \
    stack.pop();

VirtualMachine::FunctionRegistry VirtualMachine::registerFunctions() {
    FunctionRegistry registry;
    using Context = const FrameContext&;
    using Stack = DataStack&;

    registry.emplace(
        FUNCTION_CODE::LAST_0, [](Context context, Stack stack) -> XPathObject {
            return XPathObject(static_cast<double>(context.getContextSize()));
        });

    registry.emplace(
        FUNCTION_CODE::POSITION_0,
        [](Context context, Stack stack) -> XPathObject {
            return XPathObject(static_cast<double>(context.currentIndex + 1));
        });

    registry.emplace(
        FUNCTION_CODE::COUNT_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("count needs one argument");
            }

            if (!stack.top().isNodeset()) {
                throw std::runtime_error("count needs a nodeset argument");
            }

            XPathObject res(
                static_cast<double>(stack.top().asNodeset().size()));

            stack.pop();

            return res;
        });

    registry.emplace(FUNCTION_CODE::ID_1,
                     [](Context context, Stack stack) -> XPathObject {
                         // TODO
                         return XPathObject(std::string(""));
                     });

    registry.emplace(FUNCTION_CODE::LOCAL_NAME_1,
                     [](Context context, Stack stack) -> XPathObject {
                         // TODO
                         return XPathObject(std::string(""));
                     });

    registry.emplace(FUNCTION_CODE::NAME_1,
                     [](Context context, Stack stack) -> XPathObject {
                         // TODO
                         return XPathObject(std::string(""));
                     });

    registry.emplace(
        FUNCTION_CODE::STRING_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("string needs one argument");
            }

            XPathObject res(functions::string(stack.top()));

            stack.pop();

            return res;
        });

    registry.emplace(FUNCTION_CODE::CONCAT_2,
                     [](Context context, Stack stack) -> XPathObject {
                         GET_TWO_STACK_STRINGS(concat);

                         return XPathObject(str1 + str2);
                     });

    registry.emplace(FUNCTION_CODE::STARTS_WITH_2,
                     [](Context context, Stack stack) -> XPathObject {
                         GET_TWO_STACK_STRINGS(starts - with);

                         return XPathObject(str1.starts_with(str2));
                     });

    registry.emplace(
        FUNCTION_CODE::CONTAINS_2,
        [](Context context, Stack stack) -> XPathObject {
            GET_TWO_STACK_STRINGS(contains);

            return XPathObject(str1.find(str2) != std::string::npos);
        });

    registry.emplace(
        FUNCTION_CODE::SUBSTRING_BEFORE_2,
        [](Context context, Stack stack) -> XPathObject {
            GET_TWO_STACK_STRINGS(substring - before);

            return XPathObject(functions::string_before(str1, str2));
        });

    registry.emplace(
        FUNCTION_CODE::SUBSTRING_AFTER_2,
        [](Context context, Stack stack) -> XPathObject {
            GET_TWO_STACK_STRINGS(substring - after);

            return XPathObject(functions::string_after(str1, str2));
        });

    registry.emplace(
        FUNCTION_CODE::SUBSTRING_2,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 2) {
                throw std::runtime_error("substring needs two arguments");
            }

            double num = stack.top().asNumber();

            stack.pop();

            std::string str = stack.top().asString();

            stack.pop();

            return XPathObject(functions::substring(str, num));
        });

    registry.emplace(
        FUNCTION_CODE::SUBSTRING_3,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 2) {
                throw std::runtime_error("substring needs two arguments");
            }

            double num2 = stack.top().asNumber();

            stack.pop();

            double num1 = stack.top().asNumber();

            stack.pop();

            std::string str = stack.top().asString();

            stack.pop();

            return XPathObject(functions::substring(str, num1, num2));
        });

    registry.emplace(
        FUNCTION_CODE::STRING_LENGTH_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("string-length needs one arguments");
            }

            std::string str = stack.top().asString();

            stack.pop();

            return XPathObject(static_cast<double>(str.length()));
        });

    registry.emplace(FUNCTION_CODE::NORMALIZE_SPACE_1,
                     [](Context context, Stack stack) -> XPathObject {
                         // TODO
                         return XPathObject(std::string(""));
                     });

    registry.emplace(FUNCTION_CODE::TRANSLATE_3,
                     [](Context context, Stack stack) -> XPathObject {
                         // TODO
                         return XPathObject(std::string(""));
                     });

    registry.emplace(
        FUNCTION_CODE::BOOLEAN_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("boolean needs one argument");
            }

            XPathObject res(functions::boolean(stack.top()));

            stack.pop();

            return res;
        });

    registry.emplace(FUNCTION_CODE::NOT_1,
                     [](Context context, Stack stack) -> XPathObject {
                         if (stack.size() < 1) {
                             throw std::runtime_error("not needs one argument");
                         }

                         XPathObject res(!stack.top().asBool());

                         stack.pop();

                         return res;
                     });

    registry.emplace(FUNCTION_CODE::TRUE_0,
                     [](Context context, Stack stack) -> XPathObject {
                         return XPathObject(true);
                     });

    registry.emplace(FUNCTION_CODE::FALSE_0,
                     [](Context context, Stack stack) -> XPathObject {
                         return XPathObject(false);
                     });

    registry.emplace(FUNCTION_CODE::LANG_1,
                     [](Context context, Stack stack) -> XPathObject {
                         // TODO
                         return XPathObject(false);
                     });

    registry.emplace(
        FUNCTION_CODE::NUMBER_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("number needs one argument");
            }

            XPathObject res(functions::number(stack.top()));

            stack.pop();

            return res;
        });

    registry.emplace(
        FUNCTION_CODE::SUM_1, [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("sum needs one argument");
            }

            double res = 0;

            const std::vector<Node*>& nodeset = stack.top().asNodeset();
            for (Node* node : nodeset) {
                res += functions::number(XPathObject(node->getStringValue()));
            }

            stack.pop();

            return XPathObject(res);
        });

    registry.emplace(
        FUNCTION_CODE::FLOOR_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("floor needs one argument");
            }

            double res = stack.top().asNumber();

            stack.pop();

            return XPathObject(std::floor(res));
        });

    registry.emplace(
        FUNCTION_CODE::CEILING_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("ceiling needs one argument");
            }

            double res = stack.top().asNumber();

            stack.pop();

            return XPathObject(std::ceil(res));
        });

    registry.emplace(
        FUNCTION_CODE::ROUND_1,
        [](Context context, Stack stack) -> XPathObject {
            if (stack.size() < 1) {
                throw std::runtime_error("round needs one argument");
            }

            double res = stack.top().asNumber();

            stack.pop();

            return XPathObject(functions::round(res));
        });

    return registry;
}

#undef GET_TWO_STACK_STRINGS
#undef GET_THREE_STACK_STRINGS
}  // namespace onyx::dynamic::xpath