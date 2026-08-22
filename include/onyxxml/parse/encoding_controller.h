#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "text.h"

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
     * @brief True is the encoding controller requested a speculative parse to
     * find the real encoding using an autodetected encoding.
     *
     */
    bool waitingSpeculativeParse = false;
    /**
     * @brief Whether a speculative parse was used to finish the whole parse
     * process.
     *
     */
    bool continuedSpeculativeParse = false;
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
     * @brief Function for autodetecting an encoding the content.
     *
     */
    std::function<std::pair<std::string, text::XmlEncodingAutodetectionResult>(
        EncodingController&)>
        autodetectEncoding;
    /**
     * @brief Function used for finding the encoding in a document without
     * parsing all of it. If the source can continue reading without restarting
     * on transcode, the whole parse can instead be done via this function.
     *
     */
    std::function<void(EncodingController&)> transcodeAndSpeculativeParse;
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
        std::function<bool(EncodingController&)> transcode,
        std::function<
            std::pair<std::string, text::XmlEncodingAutodetectionResult>(
                EncodingController&)>
            autodetectEncoding,
        std::function<void(EncodingController&)> speculativeEncodingFindParse)
        : parse{parse},
          transcode{transcode},
          autodetectEncoding{autodetectEncoding},
          transcodeAndSpeculativeParse{speculativeEncodingFindParse} {}

    /**
     * @brief Start the controller
     *
     * @param userEncoding
     * @param shouldAutodetectEncoding
     */
    void begin(std::string_view userEncoding) {
        if (userEncoding == "autodetect") {
            auto [detectedEncoding, result] = autodetectEncoding(*this);
            encoding = detectedEncoding;
            switch (result) {
                case text::XmlEncodingAutodetectionResult::END_OF_FILE:
                    encoding = "UTF-8";
                    break;
                case text::XmlEncodingAutodetectionResult::FAMILY_DETECTED:
                    waitingSpeculativeParse = true;
                    transcodeAndSpeculativeParse(*this);
                    if (continuedSpeculativeParse) {
                        return;
                    }
                    break;
                case text::XmlEncodingAutodetectionResult::ENCODING_DETECTED:
                case text::XmlEncodingAutodetectionResult::UNKNOWN:
                    break;
            }
        } else if (!userEncoding.empty()) {
            encoding = userEncoding;
            isFinal = true;
        } else {
            encoding = "UTF-8";
        }

        if (encoding != "UTF-8" && encoding != "ASCII") {
            triggerTranscode();
        }

        triggerParse();
    }

    /**
     * @brief To be called when the parser finds an encoding in the text.
     *
     * @param found
     * @param mustRestartOnTranscode
     * @param mustRestartOnSpeculativeParse
     * @return ParserAction
     */
    ParserAction foundEncoding(std::string_view found,
                               bool mustRestartOnTranscode,
                               bool mustRestartOnSpeculativeParse) {
        if (isFinal) {
            if (found != encoding) {
                throw std::invalid_argument(
                    "Declared encoding does not match given encoding");
            }

            return ParserAction::CONTINUE;
        } else {
            if (found == encoding) {
                isFinal = true;
                if (waitingSpeculativeParse) {
                    waitingSpeculativeParse = false;

                    continuedSpeculativeParse = !mustRestartOnSpeculativeParse;

                    if (mustRestartOnSpeculativeParse)
                        return ParserAction::RESTART;
                }
                return ParserAction::CONTINUE;
            } else {
                encoding = found;
                isFinal = true;

                bool canContinue = !triggerTranscode();
                bool willContinue = canContinue || !mustRestartOnTranscode;
                waitingParseDueToRestart = !willContinue;

                if (waitingSpeculativeParse) {
                    waitingSpeculativeParse = false;

                    continuedSpeculativeParse =
                        willContinue && !mustRestartOnSpeculativeParse;

                    if (!continuedSpeculativeParse) {
                        return ParserAction::RESTART;
                    }
                }

                return willContinue ? ParserAction::CONTINUE
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

    bool triggerTranscode() {
        validate = !transcode(*this);
        return !validate;
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