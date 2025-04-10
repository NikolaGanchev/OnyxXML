#include "attribute.h"

namespace Templater::dynamic {
    Attribute::Attribute(std::string name, std::string value, bool shouldEscape) 
        : name(std::move(name)), value(std::move(value)), _shouldEscape(shouldEscape) {}

    Attribute::Attribute(std::string name): name(std::move(name)), value(""), _shouldEscape(true) {}

    void Attribute::setValue(const std::string& value) {
        this->value = value;
    }

    const std::string& Attribute::getName() const {
        return this->name;
    }

    const std::string& Attribute::getValue() const {
        return this->value;
    }

    std::string& Attribute::getValueMutable() {
        return this->value;
    }

    bool Attribute::shouldEscape() const {
        return this->_shouldEscape;
    }

    
    bool Attribute::operator==(const Attribute& other) const {
        if (this == &other) return true;

        return this->name == other.name && this->value == other.value;
    }
}