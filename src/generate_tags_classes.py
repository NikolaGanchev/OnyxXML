import io
import os
from collections import namedtuple

Tag = namedtuple('Tag', ['tagName', 'isVoid', 'dynamicName', 'compileName'])

def read_tags(tags_file_path):
    tags = []
    
    try:
        with open(tags_file_path, 'r') as input_file:
            input_file.readline()
            
            while True:
                line = input_file.readline()
                if not line:
                    break
                
                comma_pos = line.find(',')
                tag_name = line[:comma_pos]
                line = line[comma_pos+1:]
                
                is_void_char = line[0]
                is_void = (is_void_char != '0')
                line = line[1:]
                
                dynamic_name = tag_name
                compile_name = tag_name
                
                if line.startswith(','):
                    line = line[1:]
                    next_comma = line.find(',')
                    dynamic_name = line[:next_comma]
                    line = line[next_comma+1:]
                    compile_name = line.strip()
                
                tags.append(Tag(tag_name, is_void, dynamic_name, compile_name))
                
    except FileNotFoundError:
        print(f"File '{tags_file_path}' not found.")
        return []
        
    return tags

def generate_dynamic(tags, output_path):
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    
    header_content = io.StringIO()
    cpp_content = io.StringIO()
    
    header_content.write('#pragma once\n')
    header_content.write('#include "node.h"\n')
    header_content.write('#include "void_node.h"\n\n')
    header_content.write('namespace Templater::dynamic::tags {\n')
    
    cpp_content.write('#include "tags.h"\n\n')
    cpp_content.write('namespace Templater::dynamic::tags {\n')
    
    for tag in tags:
        if not tag.isVoid:
            header_content.write(f'class {tag.dynamicName} : public Node {{\n')
            header_content.write('   public:\n')
            header_content.write('    using Node::Node;\n')
            header_content.write('    bool isVoid() const override;\n')
            header_content.write('    const std::string& getTagName() const override;\n')
            header_content.write('    std::unique_ptr<Node> shallowCopy() const override;\n')
            header_content.write('};\n')
            
            cpp_content.write(f'const std::string& {tag.dynamicName}::getTagName() const {{\n')
            cpp_content.write(f'    static const std::string name = "{tag.tagName}";\n')
            cpp_content.write('    return name;\n')
            cpp_content.write('}\n')
            
            cpp_content.write(f'std::unique_ptr<Node> {tag.dynamicName}::shallowCopy() const {{\n')
            cpp_content.write(f'    return std::make_unique<{tag.dynamicName}>(this->getAttributes(), std::vector<NodeHandle>{{}});\n')
            cpp_content.write('}\n')
            
            cpp_content.write(f'bool {tag.dynamicName}::isVoid() const {{\n')
            cpp_content.write(f'    return {int(tag.isVoid)};\n')
            cpp_content.write('}\n')
        else:
            header_content.write(f'class {tag.dynamicName} : public VoidNode {{\n')
            header_content.write('    public:\n')
            header_content.write('    using VoidNode::VoidNode;\n')
            header_content.write('    const std::string& getTagName() const override;\n')
            header_content.write('    std::unique_ptr<Node> shallowCopy() const override;\n')
            header_content.write('};\n')
            
            cpp_content.write(f'const std::string& {tag.dynamicName}::getTagName() const {{\n')
            cpp_content.write(f'    static const std::string name = "{tag.tagName}";\n')
            cpp_content.write('    return name;\n')
            cpp_content.write('}\n')
            
            cpp_content.write(f'std::unique_ptr<Node> {tag.dynamicName}::shallowCopy() const {{\n')
            cpp_content.write(f'    return std::make_unique<{tag.dynamicName}>(this->getAttributes());\n')
            cpp_content.write('}\n')
    
    header_content.write('}\n\n')
    cpp_content.write('}\n\n')
    

    header_path = os.path.join(output_path, 'tags.h')
    cpp_path = os.path.join(output_path, 'tags.cpp')
    
    with open(header_path, 'w') as f:
        f.write(header_content.getvalue())
        
    with open(cpp_path, 'w') as f:
        f.write(cpp_content.getvalue())

def generate_compile(tags, output_path):
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    
    header_content = io.StringIO()
    header_content.write('#pragma once\n')
    header_content.write('#include "compile/document_utils.h"\n')
    header_content.write('#include "dynamic/tags.h"\n\n')
    header_content.write('namespace Templater::compile::ctags {\n\n')
    
    for tag in tags:
        serialization_size = (len(tag.tagName) + 4) if tag.isVoid else (2 * len(tag.tagName) + 5)
        
        header_content.write(f'template <typename... Children>\n')
        header_content.write(f'struct {tag.compileName} {{\n')
        header_content.write('    static consteval size_t size() {\n')
        header_content.write(f'        size_t size = {serialization_size};\n')
        header_content.write('        ((size += Children::size()), ...);\n')
        header_content.write('        return size;\n')
        header_content.write('    }\n')
        
        serialize_method = 'serializeVoidNode' if tag.isVoid else 'serializeNode'
        header_content.write(f'    static consteval std::array<char, size() + 1> serialize() {{\n')
        header_content.write(f'        return DocumentUtils::{serialize_method}<size(), Children...>("{tag.tagName}");\n')
        header_content.write('    }\n')
        
        header_content.write('    static std::unique_ptr<Templater::dynamic::Node> dynamicTree() {\n')
        header_content.write(f'        std::unique_ptr<Templater::dynamic::tags::{tag.dynamicName}> node = std::make_unique<Templater::dynamic::tags::{tag.dynamicName}>();\n')
        header_content.write('        (DocumentUtils::parseChildren<Children>(node.get()), ...);\n')
        header_content.write('        return node;\n')
        header_content.write('    }\n    };\n')
    
    header_content.write('}\n\n')
    
    header_path = os.path.join(output_path, 'tags.h')
    with open(header_path, 'w') as f:
        f.write(header_content.getvalue())

def main():
    import sys
    if len(sys.argv) != 4:
        print("Usage: python script.py tags_file dynamic_output_dir compile_output_dir")
        return
    
    tags_file = sys.argv[1]
    dynamic_dir = sys.argv[2]
    compile_dir = sys.argv[3]
    
    tags = read_tags(tags_file)
    if not tags:
        return
        
    generate_dynamic(tags, dynamic_dir)
    generate_compile(tags, compile_dir)

if __name__ == "__main__":
    main()