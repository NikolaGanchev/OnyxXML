#include "xpath/program.h"

namespace onyx::dynamic::xpath {
Program::Program() = default;

Program::Program(const std::vector<XPathObject>& data,
                 const std::vector<Instruction>& instructions)
    : data{data}, instructions{instructions} {}

Program::Program(std::vector<XPathObject>&& data,
                 std::vector<Instruction>&& Instruction)
    : data{data}, instructions{instructions} {}

Program::Builder& Program::Builder::addData(const XPathObject& newData) {
    if (!newData.isNumber() && !newData.isString()) {
        throw std::runtime_error(
            "Programs can only contain number or string data");
    }

    this->program->data.push_back(newData);

    return *this;
}

Program::Builder& Program::Builder::addInstruction(
    const Instruction& instruction) {
    this->program->instructions.push_back(instruction);

    return *this;
}

Program::Builder::Builder() { this->program = std::make_unique<Program>(); }

std::unique_ptr<Program> Program::Builder::build() {
    return std::move(this->program);
}

const std::vector<XPathObject>& Program::getData() const { return this->data; }

const std::vector<Instruction>& Program::getInstructions() const {
    return this->instructions;
}
}  // namespace onyx::dynamic::xpath