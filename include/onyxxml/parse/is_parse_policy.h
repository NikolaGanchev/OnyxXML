#pragma once

#include <concepts>

namespace onyx::dynamic::parser {
template <typename T>
concept isParserPolicy = requires(
    T t, typename T::StringType&& text, typename T::StringType&& tagName,
    typename T::StringType&& attributeName,
    typename T::StringType&& attributeValue, typename T::StringType&& version,
    typename T::StringType&& encoding, bool hasEntities, bool hasEncoding,
    bool isStandalone, bool hasStandalone, bool isSelfClosing,
    typename T::CursorType& cursor) {
    typename T::StringType;
    typename T::CursorType;
    t.textAction(std::move(text), hasEntities, cursor);
    t.commentAction(std::move(text), cursor);
    t.cdataAction(std::move(text), cursor);
    t.instructionAction(std::move(tagName), std::move(text), cursor);
    t.attributeAction(std::move(attributeName), std::move(attributeValue),
                      hasEntities, cursor);
    t.xmlDeclarationAction(std::move(version), std::move(encoding), hasEncoding,
                           isStandalone, hasStandalone, cursor);
    t.doctypeAction(std::move(text), cursor);
    t.openAction(std::move(tagName), isSelfClosing, cursor);
    t.closeAction(std::move(tagName), cursor);
};
}  // namespace onyx::dynamic::parser