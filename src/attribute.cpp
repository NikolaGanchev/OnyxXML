#include "attribute.h"

#include <limits>
#include <optional>
#include <string_view>

namespace onyx::dynamic {
Attribute::Attribute(std::string name, std::string value, bool shouldEscape,
                     bool shouldEscapeMultiByte)
    : name(std::move(name)),
      value(std::move(value)),
      _shouldEscape(shouldEscape),
      _shouldEscapeMultiByte(shouldEscapeMultiByte) {}

Attribute::Attribute(std::string name)
    : name(std::move(name)),
      value(""),
      _shouldEscape(true),
      _shouldEscapeMultiByte(false) {}

void Attribute::setValue(const std::string& value) { this->value = value; }

const std::string& Attribute::getName() const { return this->name; }

const std::string& Attribute::getValue() const { return this->value; }

std::string& Attribute::getValueMutable() { return this->value; }

bool Attribute::shouldEscape() const { return this->_shouldEscape; }

bool Attribute::shouldEscapeMultiByte() const {
    return this->_shouldEscapeMultiByte;
}

std::optional<std::string_view> Attribute::getNamespacePrefix() const {
    if (this->namespaceSeparatorIndex == 0) {
        this->namespaceSeparatorIndex = this->name.find(':');
    }

    if (this->namespaceSeparatorIndex == std::string::npos) return std::nullopt;

    return std::string_view(this->name.data(), this->namespaceSeparatorIndex);
}

std::string_view Attribute::getNCNameWithoutNamespace() const {
    if (this->namespaceSeparatorIndex == 0) {
        this->namespaceSeparatorIndex = this->name.find(':');
    }

    if (this->namespaceSeparatorIndex == std::string::npos) {
        return this->getName();
    }

    return std::string_view(this->name.data())
        .substr(this->namespaceSeparatorIndex + 1);
}

bool Attribute::operator==(const Attribute& other) const {
    if (this == &other) return true;

    return (this->name == other.name) && (this->value == other.value);
}

bool Attribute::operator!=(const Attribute& other) const {
    return !(*this == other);
}
}  // namespace onyx::dynamic