#pragma once

#include <limits>

namespace onyx::dynamic::parser {

struct ValidatingConfig {
    constexpr static bool validate = true;
    constexpr static bool validateDuplicateAttributes = true;
    constexpr static bool validateNamespacePrefixesResolve = true;
    constexpr static bool requireEncoding = false;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};

struct NonValidatingConfig {
    constexpr static bool validate = false;
    constexpr static bool validateDuplicateAttributes = false;
    constexpr static bool validateNamespacePrefixesResolve = false;
    constexpr static bool requireEncoding = false;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};

struct EncodingAutodetectionConfig {
    constexpr static bool validate = true;
    constexpr static bool validateDuplicateAttributes = true;
    constexpr static bool validateNamespacePrefixesResolve = true;
    constexpr static bool requireEncoding = true;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};
}  // namespace onyx::dynamic::parser