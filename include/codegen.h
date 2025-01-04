#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../include/transpiler_types.h"


class CodeGenerator{
public: 
  // CodeGenerator constructor
  explicit CodeGenerator(std::string file_location, const std::vector<Instruction>& instruction_list);
  ~CodeGenerator();

  // Main Function
  void generateCode();

private:
  // Private member variables
  const std::vector<Instruction> instructionList;
  std::string fileLocation;
  std::ofstream outputFile;

  // Private class functions
  void generateTestCode();
  void generatePrintCode(PrintData* printData);
  void generateAssignCode();

};


#endif /* CODEGEN_H */
