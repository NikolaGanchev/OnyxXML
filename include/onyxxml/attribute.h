#pragma once

#include <limits>
#include <optional>
#include <string>
#include <string_view>

namespace onyx::dynamic {
class Node;

/**
 * @brief Represents an XML attribute with a name and value. Can query
 * namespaces, but comparisons only work based on the literal qualified name
 * string. No method for querying the resolved namespace URI is provided.
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
     * @brief Whether unicode sequences should be escaped. Set at construction.
     *
     */
    bool _shouldEscapeMultiByte;

    /**
     * @brief If the Attribute name has a namespace prefix, it will be held
     * fully inside the name string. This index denotes the index of ':' of the
     * QName.
     *
     * If it does not, this will be std::string::npos, as no valid QName ends
     * with ':'.
     *
     * If the namespace has never been queried, this value will be 0, as no
     * QName can start with ':' either. This is mainly a performance
     * optimization.
     *
     */
    mutable std::size_t namespaceSeparatorIndex = 0;

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
     * @param shouldEscapeMultiByte Signifies if multi-byte sequences should be
     * escaped. For use with legacy systems. False by default. Even if true, no
     * escaping is to be done unless shouldEscape is also true.
     */
    explicit Attribute(std::string name, std::string value,
                       bool shouldEscape = true,
                       bool shouldEscapeMultiByte = false);

    /**
     * @brief Construct a new Attribute object.
     *
     * @param name The name of the attribute
     * @param value The value of the attribute
     * @param namespaceSeparatorIndex The index of the namespace ':' separator
     * in the name. std::string::npos if not available. 0 to use dynamic
     * finding.
     * @param shouldEscape Signifies if the value is xml safe; if the value is
     * unsafe, this argument should be true, which is the default value.
     * shouldEscape is used by consumers of the class to decide whether the
     * value should be escaped and Has no effect on the constructed object's
     * behaviour.
     * @param shouldEscapeMultiByte Signifies if multi-byte sequences should be
     * escaped. For use with legacy systems. False by default. Even if true, no
     * escaping is to be done unless shouldEscape is also true.
     */
    explicit Attribute(std::string name, std::string value,
                       std::size_t namespaceSeparatorIndex,
                       bool shouldEscape = true,
                       bool shouldEscapeMultiByte = false);

    /**
     * @brief Construct a new Attribute object with an empty value.
     * The constructed object is marked unsafe for its value, meaning
     * shouldEscape() will return true. This is due to the possibility that the
     * object is later modified. If an Attribute object is needed which has an
     * empty value and which is marked as safe,
     * @ref Attribute(std::string, std::string, bool shouldEscape, bool
     * shouldEscapeMultiByte) "Attribute(std::string name, \"\", bool
     * shouldEscape = false, bool shouldEscapeMultiByte = false)" should be used
     * instead.
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
     * @brief Get the name without the namespace. Returns the name if there is
     * no namespace.
     *
     * @return const std::string&
     */
    std::string_view getNCNameWithoutNamespace() const;

    /**
     * @brief Get the value string
     *
     * @return const std::string&
     */
    const std::string& getValue() const;

    /**
     * @brief Returns a *non-null terminated string_view* of the namespace
     * prefix if it is available, or std::nullopt if it is not.
     *
     * @return std::optional<std::string_view>
     */
    std::optional<std::string_view> getNamespacePrefix() const;

    /**
     * @brief Get whether the value should be escaped
     *
     * @return true The value is unsafe and should be escaped
     * @return false The value is safe and should not be escaped
     */
    bool shouldEscape() const;

    /**
     * @brief Get whether multi-byte sequences should be escaped
     *
     * @return true
     * @return false
     */
    bool shouldEscapeMultiByte() const;

    /**
     * @brief Compares two Attributes by their name and value. Namespaces are
     * treated literally.
     *
     * @param other
     * @return true The Attributes are equal
     * @return false The Attributes are not equal
     */
    bool operator==(const Attribute& other) const;

    /**
     * @brief Compares two Attributes by their name and value. Namespaces are
     * treated literally.
     *
     * @param other
     * @return true The Attributes are not equal
     * @return false The Attributes are equal
     */
    bool operator!=(const Attribute& other) const;
};
}  // namespace onyx::dynamic