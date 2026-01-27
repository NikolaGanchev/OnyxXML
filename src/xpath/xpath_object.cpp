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
    return functions::boolean(*this);
}

double XPathObject::asNumber() const {
    return functions::number(*this);
}

std::string XPathObject::asString() const {
    return functions::string(*this);
}

const std::vector<Node*>& XPathObject::asNodeset() const {
    if (std::holds_alternative<std::vector<Node*>>(value)) {
        return std::get<std::vector<Node*>>(value);
    }
    throw std::runtime_error("Tried to cast non-nodeset to nodeset");
}

enum class COMP_OP : uint8_t {
    EQ,
    NE,
    GT,
    LT,
    LE,
    GE
};

bool performScalarComparison(double n1, double n2, COMP_OP op) {
    switch (op) {
        case COMP_OP::EQ: return n1 == n2;
        case COMP_OP::NE: return n1 != n2;
        case COMP_OP::LT: return n1 < n2;
        case COMP_OP::GT: return n1 > n2;
        case COMP_OP::LE: return n1 <= n2;
        case COMP_OP::GE: return n1 >= n2;
    }
    return false;
}

bool performStringComparison(const std::string& n1, const std::string& n2, COMP_OP op) {
    if (op != COMP_OP::EQ && op != COMP_OP::NE) {
        return performScalarComparison(
            functions::number(XPathObject(n1)), 
            functions::number(XPathObject(n2)), 
            op
        );
    }

    if (op == COMP_OP::EQ) {
        return n1 == n2;
    } else {
        return n1 != n2;
    }
}

bool compare(const XPathObject& l, const XPathObject& r, COMP_OP op) {
    if (l.isNodeset() && r.isNodeset()) {
        std::vector<Node*> nodeset1 = l.asNodeset();
        std::vector<Node*> nodeset2 = r.asNodeset();

        std::vector<std::string> nodesetStrings2;

        for (size_t i = 0; i < nodeset1.size(); i++) {
            std::string value = nodeset1[i]->getStringValue();

            for (size_t j = 0; j < nodeset2.size(); j++) {
                if (nodesetStrings2.size() == j) {
                    nodesetStrings2.push_back(nodeset2[j]->getStringValue());
                }

                if (performStringComparison(value, nodesetStrings2[j], op)) {
                    return true;
                }
            }
        }

        return false;
    }

    if (l.isNodeset() || r.isNodeset()) {
        const XPathObject& nodesetObj = l.isNodeset() ? l : r;
        const XPathObject& other  = l.isNodeset() ? r : l;

        std::vector<Node*> nodeset = l.isNodeset() ? l.asNodeset() : r.asNodeset();

        if (l.isNumber()) {
            double num = l.asNumber();
            for (size_t i = 0; i < nodeset.size(); i++) {
                if (performScalarComparison(
                        num,
                        functions::number(XPathObject(nodeset[i]->getStringValue())), 
                        op)){
                    return true;
                }
            }

            return false;
        } 

        if (r.isNumber()) {
            double num = r.asNumber();
            for (size_t i = 0; i < nodeset.size(); i++) {
                if (performScalarComparison(
                        functions::number(XPathObject(nodeset[i]->getStringValue())), 
                        num, 
                        op)){
                    return true;
                }
            }

            return false;
        }

        if (l.isString()) {
            std::string str = l.asString();
            for (size_t i = 0; i < nodeset.size(); i++) {
                if (performStringComparison(str, nodeset[i]->getStringValue(), op)) {
                    return true;
                }
            }

            return false;
        }

        if (r.isString()) {
            std::string str = r.asString();
            for (size_t i = 0; i < nodeset.size(); i++) {
                if (performStringComparison(nodeset[i]->getStringValue(), str, op)) {
                    return true;
                }
            }

            return false;
        }

        if (other.isBool()) {
            bool setAsBool = functions::boolean(nodesetObj);
            bool otherBool = other.asBool();

            if (op == COMP_OP::EQ) {
                return setAsBool == otherBool;
            }

            if (op == COMP_OP::NE) {
                return setAsBool != otherBool;
            }

            if (l.isBool()) {
                return performScalarComparison(otherBool, setAsBool, op);
            } else {
                return performScalarComparison(setAsBool, otherBool, op);
            }   
        }
    }

    if (op != COMP_OP::EQ && op != COMP_OP::NE) {
        return performScalarComparison(l.asNumber(), r.asNumber(), op);
    }

    if (l.isBool() || r.isBool()) {
        if (op == COMP_OP::EQ) {
            return l.asBool() == r.asBool();
        } else {
            return l.asBool() != r.asBool();
        }
    }

    if (l.isNumber() || r.isNumber()) {
        if (op == COMP_OP::EQ) {
            return l.asNumber() == r.asNumber();
        } else {
            return l.asNumber() != r.asNumber();
        }
    }

    return op == COMP_OP::EQ ? (l.asString() == r.asString()) : (l.asString() != r.asString());
}

bool XPathObject::operator==(const XPathObject& r) const {
    return compare(*this, r, COMP_OP::EQ);
}

bool XPathObject::operator!=(const XPathObject& r) const {
    return compare(*this, r, COMP_OP::NE);
}

bool XPathObject::operator<(const XPathObject& r) const {
    return compare(*this, r, COMP_OP::LT);
}

bool XPathObject::operator>(const XPathObject& r) const {
    return compare(*this, r, COMP_OP::GT);
}

bool XPathObject::operator<=(const XPathObject& r) const {
    return compare(*this, r, COMP_OP::LE);
}

bool XPathObject::operator>=(const XPathObject& r) const {
    return compare(*this, r, COMP_OP::GE);
}
}  // namespace onyx::dynamic::xpath
