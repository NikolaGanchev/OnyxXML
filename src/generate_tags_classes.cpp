#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>

struct Tag {
    std::string name;
    bool isVoid;
    std::string htmlTag;
};

void readTags(std::vector<Tag>& tags) {
    std::ifstream input_file("tags.txt");

    std::string line;

    // Skip first line
    std::getline(input_file, line);

    while (std::getline(input_file, line))
    {
        int delimeterPos = line.find(',');
        std::string tag = line.substr(0, delimeterPos);
        char isVoidChar = line[delimeterPos+1];
        bool isVoid = isVoidChar != '0';
        std::string htmlTag = tag;
        if (line.size() > delimeterPos+3) {
            htmlTag = line.substr(delimeterPos+3);
        }
        tags.emplace_back(tag, isVoid, htmlTag);
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
    headerDynamic << "#include \"html_object.h\" \n\n";
    headerDynamic << "namespace Templater::html::dtags {\n";
    headerDynamic << "    using namespace Templater::html; \n";

    cppDynamic << "#include \"tags.h\"\n\n";

    for (auto& tag: tags) {
        headerDynamic << "    class " << tag.name << ": public Object {\n"
                                    "        public:\n"
                                    "            template <typename... Args>\n"
                                    "            explicit " << tag.name << "(Args&&... args);\n"
                                    "            explicit " << tag.name << "(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);\n"
                                    "            bool isVoid() const override;\n"
                                    "            std::shared_ptr<Object> clone() const override;\n"
                                    "            const std::string& getTagName() const override;\n"
                                    "    };\n";
        
        cppDynamic << "const std::string& Templater::html::dtags::" << tag.name << "::getTagName() const {\n"
                                    "    static const std::string name = \"" << tag.htmlTag << "\";\n"
                                    "    return name;\n"
                                    "}\n\n"
                                    "Templater::html::dtags::" << tag.name << "::" << tag.name << "(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)\n"
                                    "   : Templater::html::Object(std::move(attributes), std::move(children)) {}\n\n"
                                    "bool Templater::html::dtags::" << tag.name << "::isVoid() const {\n"
                                    "    return " << (int) (tag.isVoid) << ";\n"
                                    "}\n\n"
                                    "std::shared_ptr<Templater::html::Object> Templater::html::dtags::" << tag.name << "::clone() const {\n"
                                    "    return std::make_shared<" << tag.name << ">(*this);\n"
                                    "}\n";
    }

    headerDynamic << "}\n\n";

    for (auto& tag: tags) {
        headerDynamic << "template <typename... Args>\n"
                                "Templater::html::dtags::" << tag.name << "::" << tag.name << "(Args&&... args)\n"
                                "    : Templater::html::Object(std::forward<Args>(args)...) {}\n\n";
    }

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
    headerCompile << "namespace Templater::html::ctags {\n";

    for (auto& tag: tags) {
        headerCompile << "    template <typename... Children>\n"
                            "    struct " << tag.name << " {\n"
                                    "        static constexpr std::shared_ptr<Object> value() {\n"
                                    "            Templater::html::dtags::" << tag.name << " node = Templater::html::dtags::" << tag.name << "();\n"
                                    "            (parseChildren<Children>(node), ...);\n"
                                    "            return node.clone();\n"
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