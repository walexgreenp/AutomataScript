#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include <string>
#include "../include/transpiler_types.h"
#include "../include/logger.h"

class Parser{
public:
  // Parser constructor
  explicit Parser(const std::vector<std::string>& token_list);

  // Main Function
  std::vector<Instruction> mainParser();

private:
  const std::vector<std::string> tokens;
  size_t parsing_index;
  size_t total_tokens;

  // Parsing functions
  Instruction parsePrint();
  Instruction parseTest();
  Instruction parseVar();

  // Expression parsing functions
  Exp* parseRhs();
  Exp_p2* parseExp_p2();
  Exp_p1* parseExp_p1();

  // Helper functions
  int consumeToken();
  std::vector<Instruction> generateErrorIV(int err_val);
  Instruction generateErrorInstruction(int err_val);
};

#endif /* PARSE_H */
