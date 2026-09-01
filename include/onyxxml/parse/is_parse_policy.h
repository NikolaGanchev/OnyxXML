#pragma once

#include <concepts>
#include <utility>
#include <vector>

#include "text_transformation_mode.h"

namespace onyx::dynamic::parser {
template <typename T>
concept isParserPolicy = requires(
    T t, typename T::StringType&& text, typename T::StringType&& tagName,
    typename T::StringType&& namespaceName, typename T::StringType&& version,
    typename T::StringType&& encoding, bool hasEncoding, bool isStandalone,
    bool hasStandalone, bool isSelfClosing,
    std::vector<typename T::StringType>& attributeNames,
    std::vector<typename T::StringType>& attributeValues,
    std::vector<typename T::StackType>& stack, T::StackType& stackElement,
    typename T::CursorType::StringType& tag,
    typename T::CursorType::StringType& namespaceNameRef,
    typename T::CursorType& cursor,
    typename T::CursorType::StringType&& cursorString,
    TextTransformationMode textTransformationMode, bool& validateUTF8) {
    typename T::StringType;
    typename T::CursorType;
    typename T::StackType;
    t.textAction(std::move(text), stack, cursor);
    t.commentAction(std::move(text), stack, cursor);
    t.cdataAction(std::move(text), stack, cursor);
    t.instructionAction(std::move(tagName), std::move(text), stack, cursor);
    t.xmlDeclarationAction(std::move(version), std::move(encoding), hasEncoding,
                           isStandalone, hasStandalone, stack, cursor);
    t.doctypeAction(std::move(text), stack, cursor);
    // The return value of foundEncoding decides whether to continue parsing
    // Calling this contractually requires that there is no loaded lookahead and
    // that the capture and current are synchronized
    {
        t.foundEncoding(std::move(text), cursor, validateUTF8)
    } -> std::same_as<bool>;
    t.openAction(std::move(namespaceName), std::move(tagName), isSelfClosing,
                 attributeNames, attributeValues, stack, cursor);
    t.closeAction(std::move(namespaceName), std::move(tagName), stack, cursor);
    t.initStack(stack);
    {
        t.equalStackElementToTag(stackElement, namespaceNameRef, tag)
    } -> std::same_as<bool>;
    { t.isStackRoot(stackElement) } -> std::same_as<bool>;
    {
        t.transformText(std::move(text), textTransformationMode)
    } -> std::same_as<typename T::StringType>;
};
}  // namespace onyx::dynamic::parser