
#pragma once

#include <streambuf>
#include <string_view>

namespace onyx::dynamic::parser::impl {
/**
 * @brief This is a minimal read only std::streambuf constructed via
 * std::string_view. Attempting to write to it will throw an exception.
 *
 */
template <typename char_type>
class StringViewReadBuffer
    : public std::basic_streambuf<char_type, std::char_traits<char_type>> {
   public:
    /**
     * @brief The Base std::basic_streambuf type
     *
     */
    using Base = std::basic_streambuf<char_type, std::char_traits<char_type>>;
    /**
     * @brief The integer type of the Base
     *
     */
    using int_type = typename Base::int_type;

    /**
     * @brief Construct a new String View Read Buffer object
     *
     * @param input
     */
    StringViewReadBuffer(std::basic_string_view<char_type> input) {
        char* inputBuffer = const_cast<char*>(input.data());
        Base::setg(inputBuffer, inputBuffer, inputBuffer + input.size());
    }

   protected:
    /**
     * @brief Overriden to forbid writing
     *
     * @param ch
     * @return int_type
     */
    virtual int_type overflow(int_type ch) override {
        throw std::logic_error("Trying to write to read only buffer.");
    }

    /**
     * @brief Overriden to forbid writing
     *
     * @param s
     * @param count
     * @return std::streamsize
     */
    virtual std::streamsize xsputn(const char_type* s,
                                   std::streamsize count) override {
        throw std::logic_error("Trying to write to read only buffer.");
    }
};
}  // namespace onyx::dynamic::parser::impl