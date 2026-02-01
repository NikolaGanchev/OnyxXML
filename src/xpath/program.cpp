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

std::string Program::toString() const {
    std::stringstream result;

    for (size_t i = 0; i < this->instructions.size(); i++) {
        result << i << " ";
        const Instruction& instr = this->instructions[i];
        OPCODE opcode = instr.getOpcode();
        std::string_view opcodeStr = Instruction::opcodeToString(opcode);
        
        result << opcodeStr;

        if (opcode == OPCODE::LOAD_CONSTANT) {
            if (instr.getOperandImm() >= this->data.size()) {
                throw std::runtime_error("Program requires data address that does not exist.");
            }
            result << " \"" << this->data[instr.getOperandImm()].asString() << "\"";
        }

        if (opcode == OPCODE::JUMP 
            || opcode == OPCODE::JUMP_T
            || opcode == OPCODE::JUMP_F
            || opcode == OPCODE::LOOP_ENTER) {
            result << " " << instr.getOperandImm();
        }

        if (opcode == OPCODE::COMPARE) {
            result << " " << Instruction::compareModeToString(static_cast<COMPARE_MODE>(instr.getOperandImm()));
        }
        
        if (opcode == OPCODE::CALCULATE) {
            result << " " << Instruction::calcModeToString(static_cast<CALCULATE_MODE>(instr.getOperandImm()));
        }
        
        if (opcode == OPCODE::CALL) {
            result << " " << Instruction::functionCodeToString(static_cast<FUNCTION_CODE>(instr.getOperandImm()));
        }
        
        if (opcode == OPCODE::SELECT) {
            result << " " << Instruction::axisToString(static_cast<AXIS>(instr.getOperandImm()));
        }

        result << "\n";
    }

    return result.str();
}
}  // namespace onyx::dynamic::xpath