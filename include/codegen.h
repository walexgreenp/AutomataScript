#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../include/transpiler_types.h"
#include "../include/logger.h"

// Macro defn. for simpler syntax
#define GEN(line, out) (out) << (line) << std::endl;

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

  std::ofstream mainOutput;
  std::ofstream makeFile;
  std::ofstream complexCode;

  // Private class functions
  void generateTestCode();
  void generatePrintCode(PrintData* printData);
  void generateAssignCode(AssignData* assignData);

  void generateMakefile();
  void generateImplicitCode();

};


#endif /* CODEGEN_H */
