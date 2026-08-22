#pragma once

#include "parse/helpers.h"
namespace onyx::dynamic::parser {

template <typename _CursorType, typename _StringType, typename _StackType,
          typename _Stack>
struct BaseParserPolicy {
    using CursorType = _CursorType;
    using StringType = _StringType;
    using StackType = _StackType;
    using Stack = _Stack;

    ONYX_INLINE void textAction(StringType&&, Stack&, CursorType&) {}
    ONYX_INLINE void commentAction(StringType&&, Stack&, CursorType&) {}
    ONYX_INLINE void cdataAction(StringType&&, Stack&, CursorType&) {}
    ONYX_INLINE void instructionAction(StringType&&, StringType&&, Stack&,
                                       CursorType&) {}
    ONYX_INLINE void xmlDeclarationAction(StringType&&, StringType&&, bool,
                                          bool, bool, Stack&, CursorType&) {}
    ONYX_INLINE void doctypeAction(StringType&&, Stack&, CursorType&) {}
    ONYX_INLINE void openAction(StringType&&, bool, std::vector<StringType>&,
                                std::vector<StringType>&, Stack&, CursorType&) {
    }
    ONYX_INLINE void closeAction(StringType&&, Stack&, CursorType&) {}

    ONYX_INLINE void initStack(std::vector<StackType>&) {}
    ONYX_INLINE bool equalStackElementToTag(StackType&,
                                            typename CursorType::StringType&) {
        return false;
    }
    ONYX_INLINE bool isStackRoot(StackType&) { return false; }
};

}  // namespace onyx::dynamic::parser