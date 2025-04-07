#ifndef CODEGEN_H
#define CODEGEN_H

#include "../include/logger.h"
#include "../include/transpiler_types.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Macro defn. for simpler syntax
#define GEN(line, out) (out) << (line) << std::endl;

// -----------------------------------------------------------------------------
// CodeGenerator Class
// -----------------------------------------------------------------------------
// The CodeGenerator class is responsible for generating the source code,
// Makefile, and additional support files based on a list of instructions.
class CodeGenerator {
public:
  // CodeGenerator constructor
  explicit CodeGenerator(std::string file_location,
                         const std::vector<Instruction> &instruction_list);
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
  void generateTestCode(TestData *testData);
  void generatePrintCode(PrintData *printData);
  void generateAssignCode(AssignData *assignData);

  // Assign class function
  std::string generateRhsCode(Exp *rhs);
  std::string generateExpP2Code(Exp_p2 *exp_p2);
  std::string generateExpP1Code(Exp_p1 *exp_p1);

  void generateMakefile();
  void generateImplicitCode();
};

#endif /* CODEGEN_H */
