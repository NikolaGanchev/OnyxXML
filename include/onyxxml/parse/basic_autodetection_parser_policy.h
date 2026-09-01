#pragma once

#include "base_parser_policy.h"
#include "parse/helpers.h"

namespace onyx::dynamic::parser {
template <typename _CursorType, typename _StringType, typename _StackType,
          typename _Stack>
struct BasicAutodetectionParserPolicy
    : BaseParserPolicy<_CursorType, _StringType, _StackType, _Stack> {
    using CursorType = _CursorType;
    using StringType = _StringType;
    using StackType = _StackType;
    using Stack = _Stack;

    void throwRequiresDeclarationEncoding() {
        throw std::logic_error("Received non-declaration encoding event");
    }

    ONYX_INLINE void textAction(StringType, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void commentAction(StringType, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void cdataAction(StringType, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void instructionAction(StringType, StringType, Stack&,
                                       CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void xmlDeclarationAction(StringType, StringType, bool, bool,
                                          bool, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void doctypeAction(StringType, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void openAction(StringType, StringType, bool,
                                std::vector<StringType>&,
                                std::vector<StringType>&, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }
    ONYX_INLINE void closeAction(StringType, StringType, Stack&, CursorType&) {
        throwRequiresDeclarationEncoding();
    }

    ONYX_INLINE void initStack(std::vector<StackType>&) {}
    ONYX_INLINE bool equalStackElementToTag(StackType&,
                                            typename CursorType::StringType&,
                                            typename CursorType::StringType&) {
        return false;
    }
    ONYX_INLINE bool isStackRoot(StackType&) { return false; }
};
}  // namespace onyx::dynamic::parser