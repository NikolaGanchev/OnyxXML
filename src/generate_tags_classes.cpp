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

void readTags(std::vector<Tag>& tags) {
    std::ifstream input_file("tags.txt");

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

        tags.emplace_back(tagName, isVoidChar != '0', dynamicName, compileName);
    }

    input_file.close();
}

void generateDynamic(const std::vector<Tag>& tags) {

    if (!std::filesystem::exists("./dynamic")) {
        std::filesystem::create_directory("./dynamic");
    }

    std::ofstream headerDynamic("./dynamic/tags.h");
    std::ofstream cppDynamic("./dynamic/tags.cpp");

    headerDynamic << "#pragma once\n";
    headerDynamic << "#include \"object.h\" \n\n";
    headerDynamic << "namespace Templater::dynamic::dtags {\n";
    headerDynamic << "    using namespace Templater::dynamic; \n";

    cppDynamic << "#include \"tags.h\"\n\n";

    for (auto& tag: tags) {
        if (!tag.isVoid) {
            headerDynamic << "    class " << tag.dynamicName << ": public Object {\n"
            "        public:\n"
            "            using Object::Object;\n"
            "            bool isVoid() const override;\n"
            "            const std::string& getTagName() const override;\n"
            "    };\n";

            cppDynamic << "const std::string& Templater::dynamic::dtags::" << tag.dynamicName << "::getTagName() const {\n"
            "    static const std::string name = \"" << tag.tagName << "\";\n"
            "    return name;\n"
            "}\n\n"
            "bool Templater::dynamic::dtags::" << tag.dynamicName << "::isVoid() const {\n"
            "    return " << (int) (tag.isVoid) << ";\n"
            "}\n\n";
        } else {
            headerDynamic << "    class " << tag.dynamicName << ": public VoidObject {\n"
            "        public:\n"
            "            using VoidObject::VoidObject;\n"
            "            const std::string& getTagName() const override;\n"
            "    };\n";

            cppDynamic << "const std::string& Templater::dynamic::dtags::" << tag.dynamicName << "::getTagName() const {\n"
            "    static const std::string name = \"" << tag.tagName << "\";\n"
            "    return name;\n"
            "}\n\n";
        }
    }

    headerDynamic << "}\n\n";

    headerDynamic.close();
    cppDynamic.close();
}

void generateCompile(const std::vector<Tag>& tags) {

    if (!std::filesystem::exists("./compile")) {
        std::filesystem::create_directory("./compile");
    }

    std::ofstream headerCompile("./compile/tags.h");

    headerCompile << "#pragma once\n";
    headerCompile << "#include \"document.h\" \n";
    headerCompile << "#include \"dynamic/tags.h\" \n\n";
    headerCompile << "namespace Templater::compile::ctags {\n";

    for (auto& tag: tags) {
        headerCompile << "    template <typename... Children>\n"
                            "    struct " << tag.compileName << " {\n"
                                    "        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {\n"
                                    "            std::shared_ptr<Templater::dynamic::dtags::" << tag.dynamicName << "> node = std::make_shared<Templater::dynamic::dtags::" << tag.dynamicName << ">();\n"
                                    "            (parseChildren<Children>(node), ...);\n"
                                    "            return node;\n"
                                    "        }\n"
                                    "    };\n";
    }

    headerCompile << "}\n\n";

    headerCompile.close();
}

int main() {
    std::vector<Tag> tags{};
    readTags(tags);
    
    generateDynamic(tags);
    generateCompile(tags);

    return 0;
}