#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>

struct Tag {
    std::string tagName;
    bool isVoid;
    std::string dynamicName;
    std::string compileName;
};

void readTags(std::vector<Tag>& tags, const char* tagsFilePath) {
    std::ifstream input_file(tagsFilePath);

    std::string line;

    // Skip first line
    std::getline(input_file, line);

    while (std::getline(input_file, line))
    {
        std::string tagName = line.substr(0, line.find(','));
        line.erase(0, line.find(',') + 1);
        char isVoidChar = line[0];
        line.erase(0, 1);

        std::string dynamicName = tagName;
        std::string compileName = tagName;

        if (line[0] == ',') {
            line.erase(0, 1);
            dynamicName = line.substr(0, line.find(','));
            line.erase(0, line.find(',') + 1);
            compileName = line;
        }

        tags.emplace_back(Tag{tagName, isVoidChar != '0', dynamicName, compileName});
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
    headerDynamic << "#include \"node.h\" \n";
    headerDynamic << "#include \"void_node.h\" \n\n";
    headerDynamic << "namespace Templater::dynamic::tags {\n";

    cppDynamic << "#include \"tags.h\"\n\n";
    cppDynamic << "namespace Templater::dynamic::tags {\n";

    for (auto& tag: tags) {
        if (!tag.isVoid) {
            headerDynamic << "    class " << tag.dynamicName << ": public Node {\n"
            "        public:\n"
            "            using Node::Node;\n"
            "            bool isVoid() const override;\n"
            "            const std::string& getTagName() const override;\n"
            "            std::unique_ptr<Node> shallowCopy() const override;\n"
            "    };\n";

            cppDynamic << "    const std::string& " << tag.dynamicName << "::getTagName() const {\n"
            "        static const std::string name = \"" << tag.tagName << "\";\n"
            "        return name;\n"
            "    }\n"
            "    std::unique_ptr<Node> " << tag.dynamicName << "::shallowCopy() const {\n"
            "        return std::make_unique<" << tag.dynamicName << ">(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});\n"
            "    }\n"
            "    bool " << tag.dynamicName << "::isVoid() const {\n"
            "        return " << (int) (tag.isVoid) << ";\n"
            "    }\n";
        } else {
            headerDynamic << "    class " << tag.dynamicName << ": public VoidNode {\n"
            "        public:\n"
            "            using VoidNode::VoidNode;\n"
            "            const std::string& getTagName() const override;\n"
            "            std::unique_ptr<Node> shallowCopy() const override;\n"
            "    };\n";

            cppDynamic << "    const std::string& " << tag.dynamicName << "::getTagName() const {\n"
            "        static const std::string name = \"" << tag.tagName << "\";\n"
            "        return name;\n"
            "    }\n"
            "    std::unique_ptr<Node> " << tag.dynamicName << "::shallowCopy() const {\n"
            "        return std::make_unique<" << tag.dynamicName << ">(this->getAttributes());\n"
            "    }\n";
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
    headerCompile << "#include \"compile/document_utils.h\" \n";
    headerCompile << "#include \"dynamic/tags.h\" \n\n";
    headerCompile << "namespace Templater::compile::ctags {\n";

    for (auto& tag: tags) {
        int serializationSize = tag.isVoid ? (tag.tagName.size() + 4): (tag.tagName.size() * 2 + 5); // if void, expect <tagName />. Else, expect <tagName></tagName>
        headerCompile << "    template <typename... Children>\n"
                            "    struct " << tag.compileName << " {\n"
                                    "        static consteval size_t size() {\n"
                                    "            size_t size = " << serializationSize << ";\n"
                                    "            ((size += Children::size()), ...);\n"
                                    "            return size;\n"
                                    "        }\n"
                                    "        static consteval std::array<char, size() + 1> serialize() {\n"
                                    "            return DocumentUtils::" << ((tag.isVoid) ? "serializeVoidNode": "serializeNode") << "<size(), Children...>(\"" << tag.tagName << "\");\n"
                                    "        }\n"
                                    "        static std::unique_ptr<Templater::dynamic::Node> dynamicTree() {\n"
                                    "            std::unique_ptr<Templater::dynamic::tags::" << tag.dynamicName << "> node = std::make_unique<Templater::dynamic::tags::" << tag.dynamicName << ">();\n"
                                    "            (DocumentUtils::parseChildren<Children>(node.get()), ...);\n"
                                    "            return node;\n"
                                    "        }\n"
                                    "    };\n";
    }

    headerCompile << "}\n\n";

    headerCompile.close();
}

int main(int argc, const char *argv[]) {
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