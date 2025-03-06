#include "../include/validate.h"


Validator::Validator(const std::vector<Instruction> instructions){
  this->instructions = instructions;
}

int Validator::validate(){

  for(int i = 0; i < instructions.size(); i++){
    // Any error results in exit
    if(instructions[i].inst_type == Instruction::Type::Error){
      // TODO: Add error details based on what is stored in instruction
      return -1;
    }

    // Test: Must check the variable exists in variables vector
    else if(instructions[i].inst_type == Instruction::Type::Test){
      TestData* cur_inst = (TestData*) instructions[i].data;
      std::string nfa_name = cur_inst->NFA_name;

      // Find if this variable exists
      if(variables.find(nfa_name) == variables.end()){
        // If not found, invalid variable access.
        // TODO: Return error message
        return -1;
      }
    }

    else if(instructions[i].inst_type == Instruction::Type::Assign){
      AssignData* cur_inst = (AssignData*) instructions[i].data;
      std::string var_name = cur_inst->lhs;
      variables.insert(var_name);

      // TODO: Check RHS to make sure any var there exist already
    }
  }

  return 0;
}


