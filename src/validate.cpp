#include "../include/validate.h"

/**
 * Validator::Validator
 * @brief Constructor for the Validator class.
 *
 * Initializes the validator with a vector of instructions.
 *
 * @param instructions The vector of instructions to validate.
 */
Validator::Validator(const std::vector<Instruction> instructions) {
  this->instructions = instructions;
}

/**
 * Validator::validate
 * @brief Validates the instructions.
 *
 * Iterates through each instruction and checks for errors:
 * - Returns -1 if an error instruction is encountered.
 * - For Test instructions, verifies that the referenced variable exists.
 * - For Assign instructions, adds the variable to the set and checks for errors
 * in the RHS.
 *
 * @return 0 if all instructions are valid; -1 if any validation error is found.
 */
int Validator::validate() {

  for (int i = 0; i < instructions.size(); i++) {
    // Any error results in exit
    if (instructions[i].inst_type == Instruction::Type::Error) {
      // TODO: Add error details based on what is stored in instruction
      return -1;
    }

    // Test: Must check the variable exists in variables vector
    else if (instructions[i].inst_type == Instruction::Type::Test) {
      TestData *cur_inst = (TestData *)instructions[i].data;
      std::string nfa_name = cur_inst->NFA_name;

      // Find if this variable exists
      if (variables.find(nfa_name) == variables.end()) {
        // If not found, invalid variable access.
        // TODO: Return error message
        return -1;
      }
    }

    else if (instructions[i].inst_type == Instruction::Type::Assign) {
      AssignData *cur_inst = (AssignData *)instructions[i].data;
      std::string var_name = cur_inst->lhs;
      variables.insert(var_name);

      // TODO: Check RHS to make sure any var there exist already
    }
  }

  return 0;
}
