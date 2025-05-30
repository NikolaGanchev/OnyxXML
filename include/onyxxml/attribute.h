#pragma once

#include <string>

namespace Templater::dynamic {
class Node;

/**
 * @brief Represents an XML attribute with a name and value
 *
 */
class Attribute {
    friend Node;

   private:
    /**
     * @brief The name of the attribute.
     *
     */
    std::string name;
    /**
     * @brief The value of the attribute.
     *
     */
    std::string value;
    /**
     * @brief whether the value should be escaped.
     *
     */
    bool _shouldEscape;
    /**
     * @brief Set the Value string
     *
     */
    void setValue(const std::string&);
    /**
     * @brief Get a mutable reference to the inner value string
     *
     * @return std::string&
     */
    std::string& getValueMutable();

   public:
    /**
     * @brief Construct a new Attribute object.
     *
     * @param name The name of the attribute
     * @param value The value of the attribute
     * @param shouldEscape Signifies if the value is xml safe; if the value is
     * unsafe, this argument should be true, which is the default value.
     * shouldEscape is used by consumers of the class to decide whether the
     * value should be escaped and Has no effect on the constructed object's
     * behaviour.
     */
    explicit Attribute(std::string name, std::string value,
                       bool shouldEscape = true);

    /**
     * @brief Construct a new Attribute object with an empty value.
     * The constructed object is marked unsafe for its value, meaning
     * shouldEscape() will return true. This is due to the possibility that the
     * object is later modified. If an Attribute object is needed which has an
     * empty value and which is marked as safe,
     * @ref Attribute(std::string, std::string, bool shouldEscape)
     * "Attribute(std::string name, \"\", bool shouldEscape = true)" should be
     * used instead.
     *
     * @param name The name of the attribute
     */
    explicit Attribute(std::string name);

    /**
     * @brief Get the name string.
     *
     * @return const std::string&
     */
    const std::string& getName() const;

    /**
     * @brief Get the value string
     *
     * @return const std::string&
     */
    const std::string& getValue() const;

    /**
     * @brief Get whether the value should be escaped
     *
     * @return true The value is unsafe and should be escaped
     * @return false The value is safe and should not be escaped
     */
    bool shouldEscape() const;

    /**
     * @brief Compares two Attributes by their name and value
     *
     * @param other
     * @return true The Attributes are equal
     * @return false The Attributes are not equal
     */
    bool operator==(const Attribute& other) const;

    /**
     * @brief Compares two Attributes by their name and value
     *
     * @param other
     * @return true The Attributes are not equal
     * @return false The Attributes are equal
     */
    bool operator!=(const Attribute& other) const;
};
}  // namespace Templater::dynamic