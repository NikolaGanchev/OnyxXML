#pragma once

#include <concepts>
#include <utility>
#include <vector>

namespace onyx::dynamic::parser {
template <typename T>
concept isParserPolicy = requires(
    T t, typename T::StringType&& text, typename T::StringType&& tagName,
    typename T::StringType&& version, typename T::StringType&& encoding,
    bool hasEntities, bool hasEncoding, bool isStandalone, bool hasStandalone,
    bool isSelfClosing, std::vector<typename T::StringType>& attributeNames,
    std::vector<std::pair<typename T::StringType, bool>>& attributeValues,
    std::vector<typename T::StackType>& stack, T::StackType& stackElement,
    T::StringType& tag, typename T::CursorType& cursor) {
    typename T::StringType;
    typename T::CursorType;
    typename T::StackType;
    t.textAction(std::move(text), hasEntities, stack, cursor);
    t.commentAction(std::move(text), stack, cursor);
    t.cdataAction(std::move(text), stack, cursor);
    t.instructionAction(std::move(tagName), std::move(text), stack, cursor);
    t.xmlDeclarationAction(std::move(version), std::move(encoding), hasEncoding,
                           isStandalone, hasStandalone, stack, cursor);
    t.doctypeAction(std::move(text), stack, cursor);
    t.openAction(std::move(tagName), isSelfClosing, attributeNames,
                 attributeValues, stack, cursor);
    t.closeAction(std::move(tagName), stack, cursor);
    t.initStack(stack);
    { t.equalStackElementToTag(stackElement, tag) } -> std::same_as<bool>;
    { t.isStackRoot(stackElement) } -> std::same_as<bool>;
};
}  // namespace onyx::dynamic::parser