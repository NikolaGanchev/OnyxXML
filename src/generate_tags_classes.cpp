#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Tag {
    std::string tagName;
    bool isVoid;
    bool supportsNamespacePrefix;
    std::string dynamicName;
    std::string compileName;
};

void readTags(std::vector<Tag>& tags, const char* tagsFilePath) {
    std::ifstream input_file(tagsFilePath);

    std::string line;

    // Skip first line
    std::getline(input_file, line);

    while (std::getline(input_file, line)) {
        std::string tagName = line.substr(0, line.find(','));
        line.erase(0, line.find(',') + 1);
        char isVoidChar = line[0];
        line.erase(0, 2);
        char supportsNamespacePrefix = line[0];
        line.erase(0, 1);

        std::string dynamicName = tagName;
        std::string compileName = tagName;

        if (line[0] == ',') {
            line.erase(0, 1);
            dynamicName = line.substr(0, line.find(','));
            line.erase(0, line.find(',') + 1);
            compileName = line;
        }

        tags.emplace_back(Tag{tagName, isVoidChar != '0',
                              supportsNamespacePrefix != '0', dynamicName,
                              compileName});
    }

    input_file.close();
}

void generateDynamic(const std::vector<Tag>& tags, const char* path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }

    std::filesystem::path fullPath = path;

    std::ofstream headerDynamic(fullPath / "tags.h");
    std::ofstream cppDynamic(fullPath / "tags.cpp");

    headerDynamic << "#pragma once\n";
    headerDynamic << "#include \"onyxxml/node.h\" \n";
    headerDynamic << "#include \"onyxxml/namespace_node.h\" \n";
    headerDynamic << "#include \"onyxxml/void_node.h\" \n";
    headerDynamic << "#include \"onyxxml/void_namespace_node.h\" \n\n";
    headerDynamic << "namespace onyx::dynamic::tags {\n";

    cppDynamic << "#include \"tags.h\"\n\n";
    cppDynamic << "namespace onyx::dynamic::tags {\n";

    for (auto& tag : tags) {
        if (!tag.isVoid) {
            std::string parentNode =
                tag.supportsNamespacePrefix ? "NamespaceNode" : "Node";
            headerDynamic << "class " << tag.dynamicName << ": public "
                          << parentNode
                          << " {\n"
                             "   public:\n"
                             "    using "
                          << parentNode << "::" << parentNode
                          << ";\n"
                             "    bool isVoid() const override;\n"
                             "    const std::string& getTagName() "
                             "const override;\n"
                             "    std::unique_ptr<Node> shallowCopy() "
                             "const override;\n"
                             "};\n";

            cppDynamic << "const std::string& " << tag.dynamicName
                       << "::getTagName() const {\n"
                          "    static const std::string name = \""
                       << tag.tagName
                       << "\";\n"
                          "    return name;\n"
                          "}\n"
                          "std::unique_ptr<Node> "
                       << tag.dynamicName
                       << "::shallowCopy() const {\n"
                          "    return std::make_unique<"
                       << tag.dynamicName << ">("
                       << (tag.supportsNamespacePrefix
                               ? "this->getNamespacePrefix().has_value() ? "
                                 "std::string(this->getNamespacePrefix().value("
                                 ")) : std::string(\"\"), "
                               : "")
                       << "this->getAttributes(), std::vector<NodeHandle>{});\n"
                          "}\n"
                          "bool "
                       << tag.dynamicName
                       << "::isVoid() const {\n"
                          "    return "
                       << (int)(tag.isVoid)
                       << ";\n"
                          "}\n";
        } else {
            std::string parentNode =
                tag.supportsNamespacePrefix ? "VoidNamespaceNode" : "VoidNode";
            headerDynamic << "class " << tag.dynamicName << ": public "
                          << parentNode
                          << " {\n"
                             "    public:\n"
                             "    using "
                          << parentNode << "::" << parentNode
                          << ";\n"
                             "    const std::string& getTagName() "
                             "const override;\n"
                             "    std::unique_ptr<Node> shallowCopy() "
                             "const override;\n"
                             "};\n";

            cppDynamic << "const std::string& " << tag.dynamicName
                       << "::getTagName() const {\n"
                          "    static const std::string name = \""
                       << tag.tagName
                       << "\";\n"
                          "    return name;\n"
                          "}\n"
                          "std::unique_ptr<Node> "
                       << tag.dynamicName
                       << "::shallowCopy() const {\n"
                          "    return std::make_unique<"
                       << tag.dynamicName << ">("
                       << (tag.supportsNamespacePrefix
                               ? "this->getNamespacePrefix().has_value() ? "
                                 "std::string(this->getNamespacePrefix().value("
                                 ")) : std::string(\"\"), "
                               : "")
                       << "this->getAttributes());\n"
                          "}\n";
        }
    }

    headerDynamic << "}\n\n";
    cppDynamic << "}\n\n";

    headerDynamic.close();
    cppDynamic.close();
}

void generateCompile(const std::vector<Tag>& tags, const char* path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }

    std::filesystem::path fullPath = path;

    std::ofstream headerCompile(fullPath / "tags.h");

    headerCompile << "#pragma once\n";
    headerCompile
        << "#include \"onyxxml/compile/compile_base_serializable_node.h\" \n";
    headerCompile << "#include \"dynamic/tags.h\" \n\n";
    headerCompile << "namespace onyx::compile::ctags {\n";

    for (auto& tag : tags) {
        headerCompile
            << "template <typename... Children>\n"
               "struct "
            << tag.compileName << ": public BaseSerializableNode<\""
            << tag.tagName << "\", " << ((tag.isVoid) ? "true" : "false")
            << ", Children...>"
            << " {\n"
               "    static std::unique_ptr<onyx::dynamic::Node> "
               "dynamicTree() {\n"
               "        std::unique_ptr<onyx::dynamic::tags::"
            << tag.dynamicName
            << "> node = std::make_unique<onyx::dynamic::tags::"
            << tag.dynamicName << ">("
            << (tag.supportsNamespacePrefix ? "\"\"" : "")
            << ");\n"
               "        (DocumentUtils::parseChildren<Children>(node.get()), "
               "...);\n"
               "        return node;\n"
               "    }\n"
               "};\n";
    }

    headerCompile << "}\n\n";

    headerCompile.close();
}

int main(int argc, const char* argv[]) {
    if (argc != 4) return 1;
    const char* tagsFilePath = argv[1];
    const char* dynamicGeneratePath = argv[2];
    const char* compileGeneratePath = argv[3];

    std::vector<Tag> tags{};
    readTags(tags, tagsFilePath);

    generateDynamic(tags, dynamicGeneratePath);
    generateCompile(tags, compileGeneratePath);

    return 0;
}