#pragma once

#include <variant>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include "../node.h"

namespace onyx::dynamic::xpath {
/**
 * @brief A type representing an XPath object. Can encode a double, std::string, bool or nodeset (std::vector of node pointers)
 * 
 */
struct XPathObject {
    using ValueType = std::variant<double, std::string, bool, std::vector<Node*>>;
    ValueType value;

    /**
     * @brief Construct a new XPathObject object from a double
     * 
     * @param d 
     */
    XPathObject(double d);

    /**
     * @brief Construct a new XPathObject object from a bool
     * 
     * @param b 
     */
    XPathObject(bool b);

    /**
     * @brief Construct a new XPathObject object from a string
     * 
     * @param s 
     */
    XPathObject(std::string s);

    /**
     * @brief Construct a new XPathObject object from a nodeset
     * 
     * @param ns 
     */
    XPathObject(std::vector<Node*> ns);

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
};
}
