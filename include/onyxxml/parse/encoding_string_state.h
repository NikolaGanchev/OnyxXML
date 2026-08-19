#pragma once

#include <optional>

#include "text.h"

namespace onyx::dynamic::parser {

/**
 * @brief Holds the state of an std::string_view that may potentially be
 * transcoded.
 *
 */
class EncodingStringState {
   private:
    /**
     * @brief Only has value if the original std::string_view has been
     * transcoded.
     *
     */
    std::optional<std::string> owned;
    /**
     * @brief The original std::string_view.
     *
     */
    std::string_view original;

   public:
    /**
     * @brief Construct a new Encoding String State object
     *
     * @param string
     */
    EncodingStringState(std::string_view string) : original(string) {}

    /**
     * @brief Get the input std::string_view. It may be the original or point to
     * an internally owned transcoded string.
     *
     * @return std::string_view
     */
    std::string_view getInput() {
        if (owned.has_value()) return owned->data();
        return original;
    }

    EncodingStringState(EncodingStringState&) = delete;
    EncodingStringState(const EncodingStringState&) = delete;

    /**
     * @brief Construct a new Encoding String State object via move
     *
     * @param other
     */
    EncodingStringState(EncodingStringState&& other) {
        this->owned = std::move(other.owned);
        this->original = other.original;
    }

    /**
     * @brief Transcode the string to the given encoding.
     *
     * @param encoding
     * @return true The string was transcoded
     * @return false The string was not transcoded
     */
    bool transcodeToUTF8(const std::string& encoding) {
        owned = text::transcodeToUtf8(original, encoding);
        return owned.has_value();
    }
};
}  // namespace onyx::dynamic::parser