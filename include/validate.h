#ifndef VALIDATE_H
#define VALIDATE_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "../include/transpiler_types.h"

class Validator{
public:
  explicit Validator(const std::vector<Instruction> instructions);
  
  // Main function
  int validate();

private:
  std::unordered_set<std::string> variables;
  std::vector<Instruction> instructions;


};



#endif /* VALIDATE_H */
