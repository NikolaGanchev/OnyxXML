#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace onyx::dynamic::parser {
/**
 * @brief A controller that orchestrates parser related methods for encoding
 * support
 *
 */
class EncodingController {
   public:
    /**
     * @brief Action commands that some of the EncodingController methods may
     * return.
     *
     */
    enum class ParserAction {
        /**
         * @brief Continue parsing
         *
         */
        CONTINUE,
        /**
         * @brief The parser needs to be restarted
         *
         */
        RESTART
    };
    /**
     * @brief Whether the parser needs to validate UTF-8. If transcoding has
     * been or is being done, the content is valid UTF-8.
     *
     */
    bool validate = true;
    /**
     * @brief Whether the current state is final for this run.
     *
     */
    bool isFinal = false;
    /**
     * @brief True if the encoding controller was interrupted by the need to
     * restart the parser, which cannot be done from inside.
     *
     */
    bool waitingParseDueToRestart = false;
    /**
     * @brief The number of times the parse function has been triggered via
     * triggerParse().
     *
     */
    size_t parseCallCount = 0;
    /**
     * @brief The current known input encoding.
     *
     */
    std::string encoding = "";
    /**
     * @brief Function for parsing the content.
     *
     */
    std::function<void(EncodingController&)> parse;
    /**
     * @brief Function for transcoding the content. Returns true if transcoding
     * was initiated or done. Otherwise returns false.
     *
     */
    std::function<bool(EncodingController&)> transcode;

    /**
     * @brief Construct a new Encoding Controller object
     *
     * @param parse
     * @param transcode
     */
    explicit EncodingController(
        std::function<void(EncodingController&)> parse,
        std::function<bool(EncodingController&)> transcode)
        : parse{parse}, transcode{transcode} {}

    /**
     * @brief Start the controller
     *
     * @param userEncoding
     */
    void begin(std::string_view userEncoding) {
        validate = true;
        isFinal = false;

        if (!userEncoding.empty()) {
            encoding = userEncoding;
            if (encoding == "UTF-8" || encoding == "ASCII") {
                isFinal = true;
                triggerParse();
                return;
            } else {
                validate = !transcode(*this);
                isFinal = true;
                triggerParse();
                return;
            }
        } else {
            encoding = "UTF-8";
            triggerParse();
        }
    }

    /**
     * @brief To be called when the parser finds an encoding in the text.
     *
     * @param found
     * @param mustRestartOnTranscode
     * @return ParserAction
     */
    ParserAction foundEncoding(std::string_view found,
                               bool mustRestartOnTranscode) {
        if (isFinal) {
            if (found != encoding) {
                throw std::invalid_argument(
                    "Declared encoding does not match given encoding");
            }

            return ParserAction::CONTINUE;
        } else {
            if (found == encoding) {
                isFinal = true;
                return ParserAction::CONTINUE;
            } else {
                encoding = found;
                isFinal = true;

                bool canContinue = !transcode(*this);
                validate = canContinue;
                waitingParseDueToRestart = !canContinue;

                return (canContinue || !mustRestartOnTranscode)
                           ? ParserAction::CONTINUE
                           : ParserAction::RESTART;
            }
        }
    }

    /**
     * @brief Triggers a parse.
     *
     */
    void triggerParse() {
        parseCallCount++;
        parse(*this);
    }

    /**
     * @brief Triggers a parse only if the controlling is waiting for a parse
     * due to a restart.
     *
     */
    void triggerParseIfWaiting() {
        if (waitingParseDueToRestart) {
            waitingParseDueToRestart = false;
            triggerParse();
        }
    }
};

}  // namespace onyx::dynamic::parser