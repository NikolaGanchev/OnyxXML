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

int main() {
    std::vector<Tag> tags{};
    readTags(tags);
    
    if (!std::filesystem::exists("./dynamic")) {
        std::filesystem::create_directory("./dynamic");
    }

    std::ofstream header_file("./dynamic/tags.h");
    std::ofstream cpp_file("./dynamic/tags.cpp");

    header_file << "#pragma once\n";
    header_file << "#include \"html_object.h\" \n\n";
    header_file << "namespace Templater::html::dtags {\n";
    header_file << "    using namespace Templater::html; \n";

    cpp_file << "#include \"tags.h\"\n\n";

    for (auto& tag: tags) {
        header_file << "    class " << tag.name << ": public Object {\n"
                                    "        public:\n"
                                    "            template <typename... Args>\n"
                                    "            explicit " << tag.name << "(Args&&... args);\n"
                                    "            explicit " << tag.name << "(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);\n"
                                    "            bool isVoid() const override;\n"
                                    "            std::shared_ptr<Object> clone() const override;\n"
                                    "            const std::string& getTagName() const override;\n"
                                    "    };\n";
        
        cpp_file << "const std::string& Templater::html::dtags::" << tag.name << "::getTagName() const {\n"
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

    header_file << "}\n\n";

    for (auto& tag: tags) {
        header_file << "template <typename... Args>\n"
                                "Templater::html::dtags::" << tag.name << "::" << tag.name << "(Args&&... args)\n"
                                "    : Templater::html::Object(std::forward<Args>(args)...) {}\n\n";
    }

    header_file.close();
    cpp_file.close();

    return 0;
}