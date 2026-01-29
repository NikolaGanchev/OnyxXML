#pragma once

#include <vector>

#include "instruction.h"
#include "xpath_object.h"

namespace onyx::dynamic::xpath {
/**
 * @brief A class containing a bytecode program. It contains instructions and
 * data referenced by the instructions.
 *
 */
class Program {
   private:
    /**
     * @brief Represents the data section of a program
     *
     */
    std::vector<XPathObject> data;
    /**
     * @brief Represents the instruction section of a program
     *
     */
    std::vector<Instruction> instructions;

   public:
    /**
     * @brief A builder for Program
     *
     */
    class Builder {
       private:
        /**
         * @brief The internal program
         *
         */
        std::unique_ptr<Program> program;

       public:
        Builder();
        /**
         * @brief Add a new entry to the data section
         *
         * @param newData
         * @return Builder&
         */
        Builder& addData(const XPathObject& newData);

        /**
         * @brief Add a new instruction
         *
         * @param instruction
         * @return Builder&
         */
        Builder& addInstruction(const Instruction& instruction);

        /**
         * @brief Get the built program
         *
         * @return std::unique_ptr<Program>
         */
        std::unique_ptr<Program> build();
    };
    /**
     * @brief Construct a new Program object
     *
     */
    Program();

    /**
     * @brief Construct a new Program object
     *
     * @param data
     * @param instructions
     */
    Program(const std::vector<XPathObject>& data,
            const std::vector<Instruction>& instructions);

    /**
     * @brief Construct a new Program object
     *
     * @param data
     * @param instructions
     */
    Program(std::vector<XPathObject>&& data,
            std::vector<Instruction>&& instructions);

    /**
     * @brief Get a constant reference to the data vector
     *
     * @return const std::vector<XPathObject>&
     */
    const std::vector<XPathObject>& getData() const;

    /**
     * @brief Get a constant reference to the instructions vector
     *
     * @return const std::vector<XPathObject>&
     */
    const std::vector<Instruction>& getInstructions() const;
};
}  // namespace onyx::dynamic::xpath