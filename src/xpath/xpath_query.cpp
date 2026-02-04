#include "xpath/xpath_query.h"

#include "xpath/compiler.h"
#include "xpath/lexer.h"
#include "xpath/parser.h"
#include "xpath/program.h"

namespace onyx::dynamic::xpath {
XPathQuery::XPathQuery(std::string_view query) : vm{nullptr} {
    parser::StringCursor cursor(query.data());
    Lexer lexer(cursor);
    Parser parser(lexer);
    Compiler compiler(std::move(parser.buildAST()));
    std::unique_ptr<Program> pr = std::move(compiler.compile());
    this->vm = std::move(VirtualMachine(std::move(pr)));
};

XPathQuery::Result XPathQuery::execute(
    Node* node, std::function<XPathObject(std::string_view)> variableProvider) {
    return this->vm.executeOn(node, variableProvider);
}
};  // namespace onyx::dynamic::xpath