#pragma once

#include <array>
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "helpers.h"
#include "is_cursor.h"
#include "is_parse_policy.h"
#include "is_parser_config.h"
#include "parse/helpers.h"
#include "text_transformation_mode.h"

namespace onyx::dynamic::parser {

template <bool validate, typename CursorType>
ONYX_INLINE void readOrThrow(CursorType& pos, std::string_view expected,
                             const char* exceptionString)
    requires(isCursor<CursorType>)
{
    if (validate) {
        if (!pos.consumeIfMatches(expected)) {
            throw std::invalid_argument(exceptionString);
        }
    } else {
        pos.advance(expected.size());
    }
}

constexpr auto isValidByte = [] {
    std::array<bool, 128> arr{};
    arr[0x00] = true;  // '\t'
    arr[0x09] = true;  // '\t'
    arr[0x0A] = true;  // '\n'
    arr[0x0D] = true;  // '\r'

    for (int i = 0x20; i < 128; ++i) {
        arr[i] = true;
    }

    return arr;
}();

template <typename CursorType>
ONYX_INLINE void validateXMLChar(unsigned char current, CursorType& pos,
                                 bool validateUTF8 = true) {
    // 1 byte sequence, i.e., ASCII
    // Range: 0x00 - 0x7F
    if (current <= 0x7F) {
        if (!isValidByte[current]) {
            throw std::invalid_argument(
                "Document contains forbidden ASCII character");
        }
        return;
    }

    // 2 byte sequence
    // Range: 0xC2 - 0xDF
    if (current >= 0xC2 && current <= 0xDF) {
        pos.captureAdvance();
        if (validateUTF8) {
            unsigned char b2 = pos.captureCurrent();
            if (b2 < 0x80 || b2 > 0xBF) {
                throw std::invalid_argument(
                    "Document contains truncated or malformed utf-8");
            }
        }
        return;
    }

    // 3 byte sequence
    // Range: 0xE0 - 0xEF
    if (current >= 0xE0 && current <= 0xEF) {
        pos.captureAdvance();
        unsigned char b2 = pos.captureCurrent();
        pos.captureAdvance();
        unsigned char b3 = pos.captureCurrent();

        if (!validateUTF8) {
            // U+FFFE and U+FFFF are forbidden in XML
            if (current == 0xEF && b2 == 0xBF && (b3 == 0xBE || b3 == 0xBF)) {
                throw std::invalid_argument(
                    "Document contains restricted U+FFFE or U+FFFF");
            }
            return;
        }

        if (b3 < 0x80 || b3 > 0xBF) {
            throw std::invalid_argument(
                "Document contains truncated or malformed utf-8");
        }

        if (current == 0xE0) {
            // Overlong encoding of 2 byte characters
            if (b2 < 0xA0 || b2 > 0xBF) {
                throw std::invalid_argument(
                    "Document contains overlong utf-8 encoding");
            }
        } else if (current == 0xED) {
            // Surrogate blocks (U+D800 - U+DFFF) are forbidden in XML
            if (b2 < 0x80 || b2 > 0x9F) {
                throw std::invalid_argument(
                    "Document contains forbidden character from surrogate "
                    "block");
            }
        } else if (current == 0xEF) {
            if (b2 < 0x80 || b2 > 0xBF) {
                throw std::invalid_argument(
                    "Document contains truncated or malformed utf-8");
            }
            // U+FFFE and U+FFFF are forbidden in XML
            if (b2 == 0xBF && (b3 == 0xBE || b3 == 0xBF)) {
                throw std::invalid_argument(
                    "Document contains restricted U+FFFE or U+FFFF");
            }
        } else {
            // Standard 3 byte sequences (0xE1-0xEC, 0xEE)
            if (b2 < 0x80 || b2 > 0xBF) {
                throw std::invalid_argument(
                    "Document contains truncated or malformed utf-8");
            }
        }
        return;
    }

    // 4 byte sequence
    // Range: 0xF0 - 0xF4
    if (current >= 0xF0 && current <= 0xF4) {
        pos.captureAdvance();
        unsigned char b2 = pos.captureCurrent();
        pos.captureAdvance();
        unsigned char b3 = pos.captureCurrent();
        pos.captureAdvance();
        unsigned char b4 = pos.captureCurrent();

        if (validateUTF8) {
            if (b3 < 0x80 || b3 > 0xBF || b4 < 0x80 || b4 > 0xBF) {
                throw std::invalid_argument(
                    "Document contains truncated or malformed utf-8");
            }

            if (current == 0xF0) {
                // Overlong encoding of 3 byte character
                if (b2 < 0x90 || b2 > 0xBF) {
                    throw std::invalid_argument(
                        "Document contains overlong utf-8 encoding");
                }
            } else if (current == 0xF4) {
                // Character above U+10FFFF
                if (b2 < 0x80 || b2 > 0x8F) {
                    throw std::invalid_argument(
                        "Document contains utf-8 character out of bounds");
                }
            } else {
                // Standard 4 byte sequence (0xF1-0xF3)
                if (b2 < 0x80 || b2 > 0xBF) {
                    throw std::invalid_argument(
                        "Document contains truncated or malformed utf-8");
                }
            }
        }
        return;
    }

    // invalid starting byte (0x80-0xBF, 0xC0-0xC1, 0xF5-0xFF)
    throw std::invalid_argument(
        "Document contains invalid utf-8 starting byte");
}

template <typename CursorType>
ONYX_INLINE void validateCDataEnding(CursorType& pos) {
    if (pos.captureCurrent() == ']') {
        if (pos.capturePeek(1) != '\0' && pos.capturePeek(1) == ']' &&
            pos.capturePeek(2) != '\0' && pos.capturePeek(2) == '>') {
            throw std::invalid_argument(
                "']]>' is forbidden outside of CDATA ending "
                "sequences");
        }
    }
}

template <bool validate, bool shouldValidateCDataEndings = true,
          typename CursorType, typename... Chars>
ONYX_INLINE void readUntilAnyOf(CursorType& pos, bool validateUTF8,
                                Chars... targets) {
    unsigned char current = pos.captureCurrent();
    if constexpr (validate) {
        if constexpr (shouldValidateCDataEndings) {
            validateCDataEnding(pos);
        }
        validateXMLChar(current, pos, validateUTF8);
        while (current != '\0' && (... && (current != targets))) {
            pos.captureAdvance();
            current = pos.captureCurrent();
            if constexpr (shouldValidateCDataEndings) {
                validateCDataEnding(pos);
            }
            validateXMLChar(current, pos, validateUTF8);
        }
    } else {
        while ((... && (current != targets))) {
            pos.captureAdvance();
            current = pos.captureCurrent();
        }
    }
}

template <typename Config>
ONYX_INLINE void validateRequireDeclaration(bool foundXmlDeclaration) {
    if constexpr (Config::validate && Config::requireEncoding) {
        if (!foundXmlDeclaration) {
            throw std::invalid_argument("Required XML declaration not found");
        }
    }
}

template <typename Config, typename Policy>
struct ParseState {
    using StringType = typename Policy::StringType;
    using CursorStringType = typename Policy::CursorType::StringType;
    using StackType = typename Policy::StackType;

    bool firstTag = true;
    bool foundXmlDeclaration = false;
    bool foundDoctype = false;
    std::vector<StringType> attributeNames;
    std::vector<StringType> attributeValues;
    std::vector<StackType> stack;
};

/**
 * @brief Returns true to continue or false to break
 *
 */
template <typename Config, typename Policy>
ONYX_INLINE bool parseText(ParseState<Config, Policy>& state,
                           typename Policy::CursorType& pos, Policy& policy,
                           bool validateUTF8) {
    using State = ParseState<Config, Policy>;
    validateRequireDeclaration<Config>(state.foundXmlDeclaration);
    if constexpr (Config::validate) {
        if (state.firstTag && !state.foundXmlDeclaration &&
            !state.foundDoctype) {
            throw std::invalid_argument(
                "Top level text forbidden in XML document");
        }
    }
    pos.beginCapture();
    TextTransformationMode transformationMode = TextTransformationMode::NONE;
    bool end = false;
    readUntilAnyOf<Config::validate>(pos, validateUTF8, '\0', '<', '&', '\r');
    while (pos.captureCurrent() != '<') {
        if (pos.captureCurrent() == '\0') {
            // If we find the end of the document, one of the following
            // must be true:
            // 1) We are in a tag contents and found the
            // end of the document. Then, the stack will catch this.
            //
            // 2) We are between sibling tags. But, if they have a
            // common parent, the document cannot end here and the stack
            // will catch this, since the parent is not closed. Them
            // being top level siblings is a contradiction since the
            // second sibling must be after the text, yet there is
            // nothing after the text.
            //
            // 3) The text is before the beginning of the document,
            // i.e., the first tag. This is malformed XML, due
            // to the rule of the root element.
            //
            // 4) The text is after the last element.
            // Then, whitespace is explicitly allowed. Since this
            // segment is not consumed and anything other than
            // whitespace is an exception, it does not need to be
            // specially validated or expanded. Thus, it can keep the
            // simple while loop.

            if constexpr (Config::validate) {
                while (pos.current() != '\0') {
                    if (!isWhitespace(pos.current())) {
                        throw std::invalid_argument(
                            "Invalid end after tag open");
                    }
                    pos.advance();
                }
            }
            end = true;
            break;
        }
        if (pos.captureCurrent() == '&' || pos.captureCurrent() == '\r')
            transformationMode = TextTransformationMode::TEXT;
        pos.captureAdvance();
        readUntilAnyOf<Config::validate>(pos, validateUTF8, '\0', '<', '&',
                                         '\r');
    }
    if (end) return false;
    typename State::CursorStringType text = pos.getCaptured();
    pos.bringToCapture();
    policy.textAction(
        std::move(policy.transformText(std::move(text), transformationMode)),
        state.stack, pos);

    return true;
}

/**
 * @brief Returns true to continue or false to stop parsing
 *
 */
template <typename Config, typename Policy>
ONYX_INLINE bool parseXmlDeclaration(ParseState<Config, Policy>& state,
                                     typename Policy::CursorType& pos,
                                     Policy& policy, bool validateUTF8) {
    using State = ParseState<Config, Policy>;
    if constexpr (Config::validate) {
        if (!state.firstTag) {
            throw std::invalid_argument(
                "XML declaration is only allowed at the first "
                "position in the prologue");
        }
        if (state.foundXmlDeclaration) {
            throw std::invalid_argument("Multiple XML declarations found");
        } else {
            state.foundXmlDeclaration = true;
        }
    }
    /* Invariant - right after xml tag */
    bool hasVersion = false;
    bool hasEncoding = false;
    bool hasStandalone = false;
    typename State::CursorStringType version, encoding, standalone;

    /* Walk through all pseudo-attributes in the xml declaration */
    while (pos.current() != '\0' && pos.current() != '?') {
        /* skip whitespace */
        skipWhitespace(pos);
        if (pos.current() == '?') {
            break;
        }
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        /* read attribute name */
        typename State::CursorStringType attrName = readName(pos);
        if constexpr (Config::validate) {
            if (attrName.empty()) {
                throw std::invalid_argument(
                    "Invalid XML declaration attribute name");
            }
        }
        pos.advance(attrName.size());

        /* expect '=' */
        skipWhitespace(pos);
        if constexpr (Config::validate) {
            if (pos.current() != '=') {
                throw std::invalid_argument(
                    "No '=' after XML declaration attribute "
                    "name");
            }
        }
        pos.advance();
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        skipWhitespace(pos);
        if constexpr (Config::validate) {
            if ((pos.current() != '"' && pos.current() != '\'')) {
                throw std::invalid_argument(
                    "XML declaration attribute value not "
                    "quoted");
            }
        }
        char quote = pos.current();
        pos.advance();

        pos.beginCapture();
        if constexpr (Config::validate) {
            readUntilAnyOf<Config::validate>(pos, validateUTF8, '?', quote);
        } else {
            readUntilAnyOf<Config::validate>(pos, validateUTF8, quote);
        }
        if constexpr (Config::validate) {
            if ((pos.captureCurrent() == '?' || pos.captureCurrent() == '\0' ||
                 pos.captureCurrent() != quote)) {
                throw std::invalid_argument(
                    "Unterminated XML declaration attribute "
                    "value");
            }
        }
        typename State::CursorStringType val = pos.getCaptured();
        pos.bringToCapture();
        pos.advance(); /* skip closing quote */
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        if (attrName == "version") {
            if constexpr (Config::validate) {
                if (hasVersion) {
                    throw std::invalid_argument(
                        "XML Declaration 'version' declared more "
                        "than once");
                }
            }
            version = val;
            hasVersion = true;
        } else if (attrName == "encoding") {
            if constexpr (Config::validate) {
                if (hasEncoding) {
                    throw std::invalid_argument(
                        "XML Declaration 'encoding' declared more "
                        "than once");
                }
            }
            encoding = val;
            hasEncoding = true;
            if constexpr (Config::validate) {
                if (!hasVersion) {
                    throw std::invalid_argument(
                        "XML Declaration cannot declare 'encoding' "
                        "before 'version'");
                }
                if (hasStandalone) {
                    throw std::invalid_argument(
                        "XML Declaration cannot declare "
                        "'standalone' before 'encoding' when "
                        "'encoding' is present");
                }
            }
        } else if (attrName == "standalone") {
            if constexpr (Config::validate) {
                if (hasStandalone) {
                    throw std::invalid_argument(
                        "XML Declaration 'standalone' declared "
                        "more than once");
                }
            }
            standalone = val;
            hasStandalone = true;
            if constexpr (Config::validate) {
                if (!hasVersion) {
                    throw std::invalid_argument(
                        "XML Declaration cannot declare "
                        "'standalone' "
                        "before 'version'");
                }
            }
        } else {
            if constexpr (Config::validate) {
                throw std::invalid_argument(
                    std::string("Invalid XML declaration attribute '") +
                    std::string(attrName) + "'");
            }
        }
    }
    /* Invariant - pos at ?*/
    if constexpr (Config::validate) {
        if (pos.peek(1) != '>') {
            throw std::invalid_argument("Unclosed XML declaration");
        }
    }
    pos.advance();
    if constexpr (Config::validate) {
        if (pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
        }
    }
    pos.advance();
    pos.beginCapture();  // Synchronize capture and current

    /* enforce presence and legality */
    if constexpr (Config::validate) {
        if (!hasVersion) {
            throw std::invalid_argument("XML declaration must include version");
        }
    }
    if constexpr (Config::validate) {
        if (version != "1.0" && version != "1.1") {
            throw std::invalid_argument(
                "Unsupported XML version, must be '1.0' or "
                "'1.1'");
        }
    }
    /* encoding is optional in 1.0, but if present must match
     * NMTOKEN */
    if constexpr (Config::validate) {
        if constexpr (Config::requireEncoding) {
            if (!hasEncoding) {
                throw std::invalid_argument(
                    "Required encoding not found in XML "
                    "declaration");
            }
        }
        if (hasEncoding) {
            /* simple check: no spaces, starts with letter */
            if (encoding.empty() || !isalpha(encoding[0]))
                throw std::invalid_argument(
                    "Invalid encoding in XML declaration");
            typename State::CursorStringType encodingCopy = encoding;
            if (!policy.foundEncoding(std::move(policy.transformText(
                                          std::move(encodingCopy),
                                          TextTransformationMode::UPPERCASE)),
                                      pos, validateUTF8)) {
                return false;
            }
        }
    }
    /* standalone defaults to "no" if not present */
    if (!hasStandalone) {
        standalone = "no";
    } else {
        if constexpr (Config::validate) {
            if (standalone != "yes" && standalone != "no") {
                throw std::invalid_argument(
                    "Invalid standalone value, must be 'yes' "
                    "or "
                    "'no'");
            }
        }
    }
    bool isStandalone = (standalone[0] == 'y');
    if (encoding.size() == 0) encoding = "UTF-8";

    policy.xmlDeclarationAction(
        std::move(policy.transformText(std::move(version),
                                       TextTransformationMode::NONE)),
        std::move(policy.transformText(std::move(encoding),
                                       TextTransformationMode::UPPERCASE)),
        hasEncoding, isStandalone, hasStandalone, state.stack, pos);
    return true;
}

template <typename Config, typename Policy>
ONYX_INLINE void parseProcessingInstruction(
    typename Policy::CursorType::StringType& tagName,
    ParseState<Config, Policy>& state, typename Policy::CursorType& pos,
    Policy& policy, bool validateUTF8) {
    using State = ParseState<Config, Policy>;
    if constexpr (Config::validate) {
        if ((pos.current() != ' ' || pos.peek(1) == '\0')) {
            throw std::invalid_argument(
                "No space between processing instruction target and "
                "processing instruction content");
        }
    }
    pos.advance();

    pos.beginCapture();

    TextTransformationMode transformationMode = TextTransformationMode::NONE;
    readUntilAnyOf<Config::validate>(pos, validateUTF8, '?', '\r');

    if constexpr (Config::validate) {
        if (pos.captureCurrent() == '\0')
            throw std::invalid_argument(
                "Invalid processing instruction without ending");
    }

    while (!(pos.captureCurrent() == '?' && pos.capturePeek(1) == '>')) {
        if (pos.captureCurrent() == '\r') {
            transformationMode = TextTransformationMode::EOL_ONLY;
        }
        pos.captureAdvance();
        readUntilAnyOf<Config::validate>(pos, validateUTF8, '?', '\r');
        if constexpr (Config::validate) {
            if (pos.captureCurrent() == '\0')
                throw std::invalid_argument(
                    "Invalid processing instruction without ending");
        }
    }

    typename State::CursorStringType processingInstruction = pos.getCaptured();

    pos.bringToCapture();
    pos.advance(2);

    /* Invariant - just after processing instruction end */
    policy.instructionAction(
        std::move(policy.transformText(std::move(tagName),
                                       TextTransformationMode::NONE)),
        std::move(policy.transformText(std::move(processingInstruction),
                                       TextTransformationMode::EOL_ONLY)),
        state.stack, pos);
    state.firstTag = false;
}

/**
 * @brief Returns true to continue or false to stop parsing
 *
 */
template <typename Config, typename Policy>
ONYX_INLINE bool dispatchProcessingInstructionLike(
    ParseState<Config, Policy>& state, typename Policy::CursorType& pos,
    Policy& policy, bool validateUTF8) {
    using State = ParseState<Config, Policy>;

    pos.advance();
    if constexpr (Config::validate) {
        if (pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
        }
    }
    typename State::CursorStringType tagName = readName(pos);
    if constexpr (Config::validate) {
        if (tagName.empty()) {
            throw std::invalid_argument("Invalid tag name");
        }
    }
    pos.advance(tagName.size());

    /* Invariant - after tag name */
    if (tagName.size() == 3 &&
        tolower(static_cast<unsigned char>(tagName[0])) == 'x' &&
        tolower(static_cast<unsigned char>(tagName[1])) == 'm' &&
        tolower(static_cast<unsigned char>(tagName[2])) == 'l') {
        return parseXmlDeclaration<Config>(state, pos, policy, validateUTF8);
    }

    validateRequireDeclaration<Config>(state.foundXmlDeclaration);
    parseProcessingInstruction<Config>(tagName, state, pos, policy,
                                       validateUTF8);
    return true;
}

template <typename Config, typename Policy>
ONYX_INLINE void parseComment(ParseState<Config, Policy>& state,
                              typename Policy::CursorType& pos, Policy& policy,
                              bool validateUTF8) {
    using State = ParseState<Config, Policy>;
    pos.advance();
    if constexpr (Config::validate) {
        if (pos.current() != '-') {
            throw std::invalid_argument("Premature end of comment");
        }
    }
    pos.advance();
    /* Invariant - right after <!-- of comment */
    if constexpr (Config::validate) {
        if (pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
        }
    }
    pos.beginCapture();

    TextTransformationMode transformationMode = TextTransformationMode::NONE;
    readUntilAnyOf<Config::validate>(pos, validateUTF8, '-', '\r');
    if constexpr (Config::validate) {
        if (pos.captureCurrent() == '\0') {
            throw std::invalid_argument("Invalid comment without ending");
        }
    }
    // Found singular '-'
    // Loop continues until finding '--'
    while (!(pos.captureCurrent() == '-' && pos.capturePeek(1) == '-')) {
        if (pos.captureCurrent() == '\r') {
            transformationMode = TextTransformationMode::EOL_ONLY;
        }
        pos.captureAdvance();
        readUntilAnyOf<Config::validate>(pos, validateUTF8, '-', '\r');
        if constexpr (Config::validate) {
            if (pos.captureCurrent() == '\0') {
                throw std::invalid_argument("Invalid comment without ending");
            }
        }
    }

    typename State::CursorStringType commentText = pos.getCaptured();

    pos.captureAdvance(2);

    /* Invariant - either at error state or at > */
    if constexpr (Config::validate) {
        if (pos.captureCurrent() != '>') {
            throw std::invalid_argument("-- inside of comment not allowed");
        }
    }
    pos.bringToCapture();
    pos.advance();
    policy.commentAction(std::move(policy.transformText(std::move(commentText),
                                                        transformationMode)),
                         state.stack, pos);
    state.firstTag = false;
}

template <typename Config, typename Policy>
ONYX_INLINE void parseCData(ParseState<Config, Policy>& state,
                            typename Policy::CursorType& pos, Policy& policy,
                            bool validateUTF8) {
    using State = ParseState<Config, Policy>;
    pos.advance();
    readOrThrow<Config::validate>(pos, "CDATA[",
                                  "Premature end of CDATA section");
    /* Invariant - right after <![CDATA[ */
    if constexpr (Config::validate) {
        if (pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
        }
    }
    pos.beginCapture();

    TextTransformationMode transformationMode = TextTransformationMode::NONE;
    readUntilAnyOf<Config::validate, false>(pos, validateUTF8, ']', '\r');
    if constexpr (Config::validate) {
        if (pos.captureCurrent() == '\0')
            throw std::invalid_argument("Invalid CDATA without ending");
    }
    // Found single ']'
    // Loop continues until finding ']]>'
    while (!(pos.captureCurrent() == ']' && pos.capturePeek(1) == ']' &&
             pos.capturePeek(2) != '\0' && pos.capturePeek(2) == '>')) {
        if (pos.captureCurrent() == '\r') {
            transformationMode = TextTransformationMode::EOL_ONLY;
        }
        pos.captureAdvance();
        readUntilAnyOf<Config::validate, false>(pos, validateUTF8, ']', '\r');
        if constexpr (Config::validate) {
            if (pos.captureCurrent() == '\0')
                throw std::invalid_argument("Invalid CDATA without ending");
        }
    }

    typename State::CursorStringType cdataText = pos.getCaptured();

    pos.bringToCapture();
    pos.advance(3);
    policy.cdataAction(std::move(policy.transformText(std::move(cdataText),
                                                      transformationMode)),
                       state.stack, pos);
    state.firstTag = false;
}

template <typename Config, typename Policy>
ONYX_INLINE void parseBody(typename Policy::CursorType& pos, Policy& policy,
                           bool validateUTF8 = true)
    /* In GCC 15.1 and older MSVC versions, having
        `isCursor<CursorType>, isParserPolicy<Policy>`
        with a comma instead of `&&` actually causes an internal compiler error.
        Discovered via trial and error.
    */
    requires(isCursor<typename Policy::CursorType> && isParserPolicy<Policy> &&
             isParserConfig<Config>)
{
    using State = ParseState<Config, Policy>;
    State state;
    policy.initStack(state.stack);
    while (pos.current() != '\0') {
        /* Invariant - always at the start of either a tag or a sequence of
         * text */
        if (pos.current() != '<') {
            // In text
            if (!parseText<Config>(state, pos, policy, validateUTF8)) {
                break;
            }
            continue;
        }

        /* Invariant - always in tag name */
        pos.advance();

        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        if (pos.current() == '?') {
            /* Invariant - at processing instruction */
            if (!dispatchProcessingInstructionLike<Config>(state, pos, policy,
                                                           validateUTF8)) {
                return;
            }
            continue;
        } else if (pos.current() == '!') {
            validateRequireDeclaration<Config>(state.foundXmlDeclaration);
            pos.advance();
            if constexpr (Config::validate) {
                if (pos.current() == '\0') {
                    throw std::invalid_argument("Premature end of <! tag");
                }
            }

            if (pos.current() == '-') {
                parseComment<Config>(state, pos, policy, validateUTF8);
                continue;
            } else if (pos.current() == '[') {
                parseCData<Config>(state, pos, policy, validateUTF8);
                continue;
            } else if (pos.current() == 'D') {
                pos.advance();

                readOrThrow<Config::validate>(
                    pos, "OCTYPE ", "Premature end of DOCTYPE section");

                if constexpr (Config::validate) {
                    if (state.foundDoctype) {
                        throw std::invalid_argument(
                            "Multiple Document Type Declarations found");
                    } else {
                        state.foundDoctype = true;
                    }
                    if (!state.firstTag) {
                        throw std::invalid_argument(
                            "Document Type Declaration is only allowed before "
                            "all "
                            "XML elements except the XML declaration");
                    }
                }

                pos.beginCapture();

                // Warning - some valid DOCTYPE declarations may contain '>'
                // inside of quotes. Currently, this implementation would fail
                // on them.
                readUntilAnyOf<Config::validate>(pos, validateUTF8, '>', '\r');
                if constexpr (Config::validate) {
                    if (pos.captureCurrent() == '\0') {
                        throw std::invalid_argument(
                            "Invalid DOCTYPE without ending");
                    }
                }

                TextTransformationMode transformationMode =
                    TextTransformationMode::NONE;
                while (pos.captureCurrent() != '>') {
                    // Always at '\r' here if not at '>'
                    transformationMode = TextTransformationMode::EOL_ONLY;
                    pos.captureAdvance();
                    readUntilAnyOf<Config::validate>(pos, validateUTF8, '>',
                                                     '\r');
                    if constexpr (Config::validate) {
                        if (pos.captureCurrent() == '\0') {
                            throw std::invalid_argument(
                                "Invalid DOCTYPE without ending");
                        }
                    }
                }

                typename State::CursorStringType doctypeText =
                    pos.getCaptured();

                pos.bringToCapture();
                pos.advance();
                policy.doctypeAction(
                    std::move(policy.transformText(std::move(doctypeText),
                                                   transformationMode)),
                    state.stack, pos);
                state.firstTag = false;
                continue;
            } else {
                if constexpr (Config::validate) {
                    throw std::invalid_argument("Tag name cannot contain '!'");
                }
            }
        }
        validateRequireDeclaration<Config>(state.foundXmlDeclaration);

        bool isClosing = (pos.current() == '/');

        if (isClosing) pos.advance();
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        /* Invariant - pos always at tag name start */
        typename State::CursorStringType tagName = readName(pos);
        if constexpr (Config::validate) {
            if (tagName.empty()) {
                throw std::invalid_argument("Invalid tag name");
            }
        }
        pos.advance(tagName.size());

        bool couldHaveAttributes = isWhitespace(pos.current()) && !isClosing;

        /* Invariant - pos always after tag name end */
        skipWhitespace(pos);
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        if (couldHaveAttributes) {
            /* Invariant - either at start of attribute or at > */
            while (pos.current() != '>' && pos.current() != '/') {
                typename State::CursorStringType attributeName = readName(pos);
                if constexpr (Config::validate) {
                    if (attributeName.empty()) {
                        throw std::invalid_argument("Invalid non-closing tag");
                    }
                }
                pos.advance(attributeName.size());

                /* Invariant - after attribute name */
                skipWhitespace(pos);

                /* Invariant - at = or error state */
                if constexpr (Config::validate) {
                    if ((pos.current() == '\0' || pos.current() != '=')) {
                        throw std::invalid_argument("No = after attribute");
                    }
                }
                pos.advance();
                /* Invariant - after attribute = */
                skipWhitespace(pos);
                char attributeQuote = '\0';
                if constexpr (Config::validate) {
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end at attribute");
                    }
                }

                if (pos.current() == '\'' || pos.current() == '\"') {
                    attributeQuote = pos.current();
                } else {
                    if constexpr (Config::validate) {
                        throw std::invalid_argument(
                            "No quote (\" or \') after attribute =");
                    }
                }
                pos.advance();

                /* Invariant - after attribute value opening quote */
                pos.beginCapture();
                TextTransformationMode transformationMode =
                    TextTransformationMode::NONE;
                if constexpr (Config::validate) {
                    readUntilAnyOf<Config::validate>(pos, validateUTF8,
                                                     attributeQuote, '<', '&',
                                                     '\r', '\n', '\t');
                } else {
                    readUntilAnyOf<Config::validate>(pos, validateUTF8,
                                                     attributeQuote, '&', '\r',
                                                     '\n', '\t');
                }

                while (pos.captureCurrent() != attributeQuote) {
                    if constexpr (Config::validate) {
                        if (pos.captureCurrent() == '\0') {
                            throw std::invalid_argument(
                                "Improperly closed attribute value");
                        }
                        if (pos.captureCurrent() == '<') {
                            throw std::invalid_argument(
                                "Cannot have '<' inside of attribute value");
                        }
                    }
                    if (pos.captureCurrent() == '&' ||
                        pos.captureCurrent() == '\r' ||
                        pos.captureCurrent() == '\n' ||
                        pos.captureCurrent() == '\t')
                        transformationMode = TextTransformationMode::ATTRIBUTE;
                    pos.captureAdvance();
                    if constexpr (Config::validate) {
                        readUntilAnyOf<Config::validate>(pos, validateUTF8,
                                                         attributeQuote, '<',
                                                         '&', '\r', '\n', '\t');
                    } else {
                        readUntilAnyOf<Config::validate>(pos, validateUTF8,
                                                         attributeQuote, '&',
                                                         '\r', '\n', '\t');
                    }
                }
                typename State::CursorStringType attributeValue =
                    pos.getCaptured();
                pos.bringToCapture();

                /* Invariant - pos is at closing quote */
                pos.advance();

                /* Invariant - pos is just after closing quote */
                /* Allows non-standard <tagName name="value"/> where the
                 * backslash is exactly after the quote */
                if constexpr (Config::validate) {
                    if (!isWhitespace(pos.current()) && pos.current() != '>' &&
                        pos.current() != '/') {
                        throw std::invalid_argument(
                            "No whitespace after attribute closing quote");
                    }
                }

                if constexpr (Config::validate &&
                              Config::validateDuplicateAttributes) {
                    for (size_t i = 0; i < state.attributeNames.size(); i++) {
                        if (state.attributeNames[i] == attributeName) {
                            throw std::invalid_argument(
                                "Duplicate attribute name");
                        }
                    }
                }

                if constexpr (Config::validate) {
                    if (state.attributeNames.size() >=
                        Config::maxAttributeCount) {
                        throw std::invalid_argument(
                            "Tag has too many attributes");
                    }
                }
                state.attributeNames.push_back(std::move(policy.transformText(
                    std::move(attributeName), TextTransformationMode::NONE)));
                state.attributeValues.push_back(std::move(policy.transformText(
                    std::move(attributeValue), transformationMode)));

                /* Continues to either >, /> or another attribute */
                skipWhitespace(pos);
                if constexpr (Config::validate) {
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end after attribute");
                    }
                }
            }
        }

        bool isSelfClosing = false;
        /* Invariant - pos is always at / or > */
        if (pos.current() == '>') {
            pos.advance();
        } else if (pos.current() == '/') {
            if constexpr (Config::validate) {
                if (isClosing) {
                    throw std::invalid_argument(
                        "Trying to double-close closing tag");
                }
            }
            pos.advance();
            if constexpr (Config::validate) {
                if (pos.current() != '>') {
                    throw std::invalid_argument(
                        "Invalid tag close - must have > after /");
                }
            }
            isSelfClosing = true;
            pos.advance();
        } else {
            if constexpr (Config::validate) {
                throw std::invalid_argument("No tag close for tag " +
                                            std::string(tagName));
            }
        }

        /* Invariant - pos always after valid tag close */
        if (!isClosing) {
            /* Invariant - top stack node is always current parent */
            state.firstTag = false;
            policy.openAction(
                std::move(policy.transformText(std::move(tagName),
                                               TextTransformationMode::NONE)),
                isSelfClosing, state.attributeNames, state.attributeValues,
                state.stack, pos);
            state.attributeNames.resize(0);
            state.attributeValues.resize(0);
        } else {
            /* Invariant - when closing node, the current parent (stack top)
             * must be of the node type */
            if constexpr (Config::validate) {
                if (!policy.equalStackElementToTag(state.stack.back(),
                                                   tagName)) {
                    throw std::invalid_argument("Closing unopened tag");
                }
                if (state.stack.size() == 1) {
                    throw std::invalid_argument("Closing non-existent tags");
                }
            }
            policy.closeAction(
                std::move(policy.transformText(std::move(tagName),
                                               TextTransformationMode::NONE)),
                state.stack, pos);
        }
    }
    if constexpr (Config::validate) {
        if (state.stack.size() != 1 || !policy.isStackRoot(state.stack[0])) {
            throw std::invalid_argument("Unclosed tags left");
        }
    }
}
}  // namespace onyx::dynamic::parser