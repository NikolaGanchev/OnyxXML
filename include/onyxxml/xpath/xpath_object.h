#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "../node.h"

namespace onyx::dynamic::xpath {
/**
 * @brief A type representing an XPath object. Can encode a double, std::string,
 * bool or nodeset (std::vector of node pointers)
 *
 */
struct XPathObject {
    using ValueType =
        std::variant<double, std::string, bool, std::vector<Node*>>;
    ValueType value;

    /**
     * @brief Construct a new XPathObject object from a double
     *
     * @param d
     */
    explicit XPathObject(double d);

    /**
     * @brief Construct a new XPathObject object from a bool
     *
     * @param b
     */
    explicit XPathObject(bool b);

    /**
     * @brief Construct a new XPathObject object from a string
     *
     * @param s
     */
    explicit XPathObject(std::string s);

    /**
     * @brief Construct a new XPathObject object from a const char*. Delegates
     * to std::string constructor.
     *
     * @param s
     */
    explicit XPathObject(const char* s);

    /**
     * @brief Construct a new XPathObject object from a nodeset
     *
     * @param ns
     */
    explicit XPathObject(std::vector<Node*> ns);

    /**
     * @brief Whether the object is a nodeset
     *
     * @return true
     * @return false
     */
    bool isNodeset() const;

    /**
     * @brief Whether the object is a number
     *
     * @return true
     * @return false
     */
    bool isNumber() const;

    /**
     * @brief Whether the object is a string
     *
     * @return true
     * @return false
     */
    bool isString() const;

    /**
     * @brief Whether the object is a boolean
     *
     * @return true
     * @return false
     */
    bool isBool() const;

    /**
     * @brief Cast to bool
     *
     * @return true
     * @return false
     */
    bool asBool() const;

    /**
     * @brief Cast to number
     *
     * @return double
     */
    double asNumber() const;

    /**
     * @brief Cast to string
     *
     * @return std::string
     */
    std::string asString() const;

    /**
     * @brief Cast to Nodeset or throw
     *
     * @return const std::vector<Node*>&
     */
    const std::vector<Node*>& asNodeset() const;

    /**
     * @brief Equality according to
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#booleans
     *
     * @param r
     * @return true
     * @return false
     */
    bool operator==(const XPathObject& r) const;

    /**
     * @brief Inequality according to
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#booleans
     *
     * @param r
     * @return true
     * @return false
     */
    bool operator!=(const XPathObject& r) const;

    /**
     * @brief Less-than according to
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#booleans
     *
     * @param r
     * @return true
     * @return false
     */
    bool operator<(const XPathObject& r) const;

    /**
     * @brief Greater-than according to
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#booleans
     *
     * @param r
     * @return true
     * @return false
     */
    bool operator>(const XPathObject& r) const;

    /**
     * @brief Less-or-equal-to according to
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#booleans
     *
     * @param r
     * @return true
     * @return false
     */
    bool operator<=(const XPathObject& r) const;

    /**
     * @brief Greater-or-equal-to according to
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#booleans
     *
     * @param r
     * @return true
     * @return false
     */
    bool operator>=(const XPathObject& r) const;
};
}  // namespace onyx::dynamic::xpath
