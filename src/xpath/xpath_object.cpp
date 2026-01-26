#include "xpath/xpath_object.h"
#include "xpath/functions.h"

namespace onyx::dynamic::xpath {
XPathObject::XPathObject(double d) : value(d) {}

XPathObject::XPathObject(bool b) : value(b) {}

XPathObject::XPathObject(std::string s) : value(std::move(s)) {}

XPathObject::XPathObject(std::vector<Node*> ns) : value(std::move(ns)) {}

bool XPathObject::isNodeset() const {
    return std::holds_alternative<std::vector<Node*>>(value);
}

bool XPathObject::isNumber() const { 
    return std::holds_alternative<double>(value); 
}

bool XPathObject::isString() const {
    return std::holds_alternative<std::string>(value);
}

bool XPathObject::isBool() const { 
    return std::holds_alternative<bool>(value);
}

bool XPathObject::asBool() const {
    return functions::boolean(this);
}

double XPathObject::asNumber() const {
    return functions::number(this);
}

std::string XPathObject::asString() const {
    return functions::string(this);
}

const std::vector<Node*>& XPathObject::asNodeset() const {
    if (std::holds_alternative<std::vector<Node*>>(value)) {
        return std::get<std::vector<Node*>>(value);
    }
    throw std::runtime_error("Tried to cast non-nodeset to nodeset");
}
}  // namespace onyx::dynamic::xpath
