#pragma once

#include <memory>
#include "parser.h"
#include "program.h"
#include "xpath/axis.h"
#include "xpath/function_code.h"

namespace onyx::dynamic::xpath {
/**
 * @brief Translates an XPath AST into a Virtual Machine Program
 * 
 */
class Compiler {
private:
    /**
     * @brief The AST to compile.
     * 
     */
    std::unique_ptr<Parser::AstNode> ast;

    /**
     * @brief Holds a mapping between axis names and the AXIS enum.
     * 
     */
    std::unordered_map<std::string_view, AXIS> axisCache;

    /**
     * @brief Fills the axisCache.
     * 
     */
    void initAxisCache();

    /**
     * @brief Resolves an axis name to an AXIS enum entry.
     * 
     * @param op 
     * @return AXIS 
     */
    AXIS resolveAxis(std::string_view op);

    /**
     * @brief Mapping between function names and their FUNCTION_CODE and number of arguments.
     * For functions with optional arguments, returns the greatest possible amount of arguments.
     * For 'concat', which has a variadic number of arguments, returns 2.
     * 
     */
    std::unordered_map<std::string_view, std::pair<FUNCTION_CODE, size_t>> functionCache;

    /**
     * @brief Fills the functionCache.
     * 
     */
    void initFunctionCache();
public:
    /**
     * @brief Create a Compiler object
     * 
     * @param std::unique_ptr<Parser::AstNode> Root of the AST
     */
    Compiler(std::unique_ptr<Parser::AstNode> ast);

    /**
     * @brief Compile the AST
     * 
     * @return std::unique_ptr<Program> 
     */
    std::unique_ptr<Program> compile();
};
};