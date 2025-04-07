#ifndef VALIDATE_H
#define VALIDATE_H

#include "../include/logger.h"
#include "../include/transpiler_types.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

// -----------------------------------------------------------------------------
// Validator Class
// -----------------------------------------------------------------------------
// The Validator class is responsible for validating an AST. This must be
// used to ensure proper programs are being fed by the user.
class Validator {
public:
  explicit Validator(const std::vector<Instruction> instructions);

  // Main function
  int validate();

private:
  std::unordered_set<std::string> variables;
  std::vector<Instruction> instructions;
};

#endif /* VALIDATE_H */
