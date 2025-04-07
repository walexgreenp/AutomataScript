#include "../include/codegen.h"

/**
 * CodeGenerator::CodeGenerator
 * @brief Constructor for the CodeGenerator class.
 *
 * Opens files for output and stores the file handles in the class.
 *
 * @param file_location The base directory where generated files will be
 * written.
 * @param instruction_list A vector of instructions to drive code generation.
 */
CodeGenerator::CodeGenerator(std::string file_location,
                             const std::vector<Instruction> &instruction_list)
    : instructionList(instruction_list) {

  // Open files, store handle in object
  this->fileLocation = file_location;

  // Main function, should have all the basic stuff
  mainOutput.open(file_location + "cppcode.cpp");
  if (!mainOutput.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "cppcode.cpp");
  }

  // Complex code. For additional functions and other.
  // WARN: May just move this into mainOutput
  complexCode.open(file_location + "complexCode.cpp");
  if (!complexCode.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "complexCode.cpp");
  }

  // Makefile, easier for compilation vs calls in C++
  makeFile.open(file_location + "Makefile");
  if (!makeFile.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "Makefile");
  }
}

/**
 * CodeGenerator::~CodeGenerator
 * @brief Destructor for the CodeGenerator class.
 *
 * Closes all open file handles.
 */
CodeGenerator::~CodeGenerator() {
  if (mainOutput.is_open()) {
    mainOutput.close();
  }
  if (makeFile.is_open()) {
    makeFile.close();
  }
  if (complexCode.is_open()) {
    complexCode.close();
  }
}

/**
 * CodeGenerator::generateCode
 * @brief Main function to generate code.
 *
 * Calls sub-functions to generate the main C++ file, Makefile, and implicit
 * code.
 */
void CodeGenerator::generateCode() {

  // Generate main file
  GEN("#include <iostream>", mainOutput);
  GEN("#include \"regex.h\"", mainOutput);
  GEN("", mainOutput);
  GEN("int main(){", mainOutput);

  GEN("\t// Boilerplate.", mainOutput);
  GEN("\tstd::cout << \"------------------\" << std::endl;", mainOutput);
  GEN("\tstd::cout << \"* AutomataScript *\" << std::endl << std::endl;",
      mainOutput);

  GEN("", mainOutput);
  GEN("\t// Setting up important variables", mainOutput);
  GEN("\tNode* startNode;", mainOutput);
  GEN("\tint error_status;", mainOutput);

  GEN("", mainOutput);
  GEN("\t// Generating code", mainOutput);
  // Call function for each type of instruction
  for (Instruction instruction : instructionList) {
    switch (instruction.inst_type) {
    case Instruction::Type::Test:
      generateTestCode((TestData *)instruction.data);
      break;
    case Instruction::Type::Print:
      generatePrintCode((PrintData *)instruction.data);
      break;
    case Instruction::Type::Assign:
      generateAssignCode((AssignData *)instruction.data);
      break;
    case Instruction::Type::Error:
      std::cout << "ERROR, SHOULD NOT BE HERE!" << std::endl;
      break;
    }
  }

  GEN("", mainOutput);
  GEN("\t// End.", mainOutput);
  GEN("\tstd::cout << \"------------------\" << std::endl;", mainOutput);
  GEN("\treturn 0;", mainOutput);
  GEN("}", mainOutput);

  // Generate Makefile
  generateMakefile();

  // Generate implicit code
  generateImplicitCode();
}

/**
 * CodeGenerator::generateTestCode
 * @brief Generates code for Test instructions.
 *
 * Constructs code to run a test NFA and prints whether the test was accepted or
 * rejected.
 *
 * @param testData Pointer to TestData containing the NFA name and test value.
 * @warn WARN: Not implemented
 */
void CodeGenerator::generateTestCode(TestData *testData) {
  std::string instruction = "";

  // Get the appropriate node value.
  instruction += "\tstartNode = ";
  instruction += testData->NFA_name;
  instruction += ".first;";
  GEN(instruction, mainOutput);

  // Run the NFA, store the status
  instruction = "\terror_status = RunNFA(startNode, 0, \"";
  instruction += testData->test_value;
  instruction += "\");";
  GEN(instruction, mainOutput);

  // If statement
  instruction = "\tif(error_status == -1){";
  GEN(instruction, mainOutput);

  // Print NFA rejected
  instruction = "\t\tstd::cout << \"- ";
  instruction += testData->NFA_name;
  instruction += " <- ";
  instruction += testData->test_value;
  instruction += ": Rejected by NFA.\" << std::endl;";
  GEN(instruction, mainOutput);

  // If statement close
  instruction = "\t}";
  GEN(instruction, mainOutput);

  // Else...
  instruction = "\telse{";
  GEN(instruction, mainOutput);

  // Print NFA rejected
  instruction = "\t\tstd::cout << \"- ";
  instruction += testData->NFA_name;
  instruction += " <- ";
  instruction += testData->test_value;
  instruction += ": Accepted by NFA.\" << std::endl;";
  GEN(instruction, mainOutput);

  // Else statement close
  instruction = "\t}";
  GEN(instruction, mainOutput);

  return;
}

/**
 * CodeGenerator::generatePrintCode
 * @brief Generates code for Print instructions.
 *
 * Outputs a print instruction to the main file.
 *
 * @param printData Pointer to PrintData containing the string to print.
 */
void CodeGenerator::generatePrintCode(PrintData *printData) {
  std::string instruction = "";
  instruction += "\tstd::cout << \"";
  instruction += printData->output_string;
  instruction += "\" << std::endl;";

  GEN(instruction, mainOutput);

  return;
}

/**
 * CodeGenerator::generateAssignCode
 * @brief Generates code for Assign instructions.
 *
 * Produces code to assign an expression result to a variable.
 *
 * @param assignData Pointer to AssignData containing the left-hand side and the
 * expression.
 * @warn WARN: Not implemented
 * @todo TODO: Check if I should display anything to the user that the variable
 * was created
 */
void CodeGenerator::generateAssignCode(AssignData *assignData) {
  std::string instruction = "\t";

  // Grab the desired variable name
  std::string lhs = assignData->lhs;
  instruction += "std::pair<Node *, Node*> ";
  instruction += lhs;
  instruction += " = ";

  Exp *rhs = assignData->rhs;
  std::string rhs_code = generateRhsCode(rhs); // Generate exp

  instruction += rhs_code;

  GEN(instruction, mainOutput);

  // TODO: Check if I should display anything to the user that the variable was
  // created

  return;
}

/**
 * CodeGenerator::generateMakefile
 * @brief Generates the Makefile used to compile the generated code.
 *
 * @todo TODO: Add parameters to specify which compiler to use.
 */
void CodeGenerator::generateMakefile() {
  GEN("# Makefile", makeFile);

  GEN("# Compiler + Flags", makeFile);
  GEN("CXX = g++", makeFile);
  GEN("CXXFLAGS = -Iinclude -std=c++17 -w", makeFile);
  GEN("", makeFile)

  GEN("SRC = $(wildcard *.cpp)", makeFile);
  GEN("OBJ = $(SRC:.cpp=.o)", makeFile);
  GEN("", makeFile)

  GEN("TARGET = ../compiled_output", makeFile);
  GEN("", makeFile)

  GEN("all = $(TARGET)", makeFile);
  GEN("", makeFile)

  GEN("$(TARGET): $(OBJ)", makeFile);
  GEN("\t@$(CXX) $(OBJ) -o $(TARGET)", makeFile);
  GEN("", makeFile)

  GEN("%.o: %.cpp", makeFile);
  GEN("\t@$(CXX) $(CXXFLAGS) -c $< -o $@", makeFile);
  GEN("", makeFile)

  GEN("clean:", makeFile);
  GEN("\t@rm -f $(OBJ) $(TARGET)", makeFile);
}

/**
 * CodeGenerator::generateImplicitCode
 * @brief Generates the implicit NFA code by copying existing source files.
 *
 * Reads library files (regex.cpp, regex.h, nfa.cpp, nfa.h) and writes them to
 * the output directory.
 */
void CodeGenerator::generateImplicitCode() {
  // Implicit code.

  // Generating regex.cpp
  std::ofstream regexCpp;
  regexCpp.open(fileLocation + "regex.cpp");
  if (!regexCpp.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "regex.cpp");
  }
  std::ifstream cSourceFile("lib/regex.cpp");
  if (!cSourceFile.is_open()) {
    std::cerr << "Error opg file!" << std::endl;
    return; // Exit on failure
  }
  std::string line;
  // Replace include path (firstline)
  while (std::getline(cSourceFile, line)) { // Read line by line
    GEN(line, regexCpp);                    // Use macro to write to output file
  }
  cSourceFile.close();

  // Generating regex.h
  std::ofstream regexH;
  regexH.open(fileLocation + "regex.h");
  if (!regexH.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "regex.h");
  }
  // h source file
  std::ifstream hSourceFile("lib/regex.h");
  if (!hSourceFile.is_open()) {
    std::cerr << "Error opg file!" << std::endl;
    return; // Exit on failure
  }
  while (std::getline(hSourceFile, line)) { // Read line by line
    GEN(line, regexH);                      // Use macro to write to output file
  }
  hSourceFile.close();

  // Generating nfa.cpp
  std::ofstream nfaCpp;
  nfaCpp.open(fileLocation + "nfa.cpp");
  if (!nfaCpp.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "nfa.cpp");
  }
  std::ifstream nfaCSourceFile("lib/nfa.cpp");
  if (!nfaCSourceFile.is_open()) {
    std::cerr << "Error opg file!" << std::endl;
    return; // Exit on failure
  }
  while (std::getline(nfaCSourceFile, line)) { // Read line by line
    GEN(line, nfaCpp); // Use macro to write to output file
  }
  nfaCSourceFile.close();

  // Generating nfa.h
  std::ofstream nfaH;
  nfaH.open(fileLocation + "nfa.h");
  if (!nfaH.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation +
                                 "nfa.h");
  }
  // h source file
  std::ifstream nfaHSourceFile("lib/nfa.h");
  if (!nfaHSourceFile.is_open()) {
    std::cerr << "Error opg file!" << std::endl;
    return; // Exit on failure
  }
  while (std::getline(nfaHSourceFile, line)) { // Read line by line
    GEN(line, nfaH); // Use macro to write to output file
  }
  nfaHSourceFile.close();
}

/**
 * CodeGenerator::generateRhsCode
 * @brief Generates code for the right-hand side (RHS) expression.
 *
 * @param rhs Pointer to the expression object.
 * @return A string containing the generated code for the RHS.
 * @warn WARN: Incomplete
 * @todo TODO: CodeGen for (binop exp_p2)*
 */
std::string CodeGenerator::generateRhsCode(Exp *rhs) {
  std::string rhs_out = "";

  Exp_p2 *exp_p2 = rhs->exp_p2;
  std::string exp_p2_code = generateExpP2Code(exp_p2);
  rhs_out += exp_p2_code;

  // TODO: CodeGen for (binop exp_p2)*

  return rhs_out;
}

/**
 * CodeGenerator::generateExpP2Code
 * @brief Generates code for an Exp_p2 expression.
 *
 * Depending on whether a unary operator is present, either generates code for
 * the underlying Exp_p1 or wraps it with the appropriate NFA function.
 *
 * @param exp_p2 Pointer to the Exp_p2 object.
 * @return A string containing the generated code for the Exp_p2 expression.
 * @warn WARN: Incomplete
 */
std::string CodeGenerator::generateExpP2Code(Exp_p2 *exp_p2) {
  std::string instruction = "";

  // If there is no unop, this is simply the p1 expression
  if (exp_p2->unop_type == Exp_p2::Type::None) {
    Exp_p1 *exp_p1 = exp_p2->exp_p1;
    instruction += generateExpP1Code(exp_p1);
    instruction += ",\"";
    instruction += "tmp";
    instruction += "\");";
  }

  // Unop, wrap p1 expression
  else {
    switch (exp_p2->unop_type) {
    case Exp_p2::Type::KleeneStar:
      instruction += "KleeneStarNFA(";
      break;
    case Exp_p2::Type::PosClos:
      instruction += "PlusNFA(";
      break;
    case Exp_p2::Type::Opt:
      instruction += "OptionalNFA(";
      break;
    default:
      return "ERROR";
    }

    Exp_p1 *exp_p1 = exp_p2->exp_p1;
    instruction += generateExpP1Code(exp_p1);
    instruction += ",\"";
    instruction += "tmp";
    instruction += "\"";
    instruction += ")";
    instruction += ");";
  }

  return instruction;
}

/**
 * CodeGenerator::generateExpP1Code
 * @brief Generates code for an Exp_p1 expression.
 *
 * Handles literal expressions; additional cases for Exp and Lval are to be
 * added.
 *
 * @param exp_p1 Pointer to the Exp_p1 object.
 * @return A string containing the generated code for the Exp_p1 expression.
 * @warn WARN: Incomplete
 * @todo TODO: Cases for Exp, Lval
 */
std::string CodeGenerator::generateExpP1Code(Exp_p1 *exp_p1) {
  std::string output = "";

  if (exp_p1->exp_p1_type == Exp_p1::Type::Exp_ac) {
    output += "LiteralNFA(\"";
    output += exp_p1->identifier;
    output += "\"";
    return output;
  }

  // TODO: Cases for Exp, Lval

  return "";
}
