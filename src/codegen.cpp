#include "../include/codegen.h"


/*
* CodeGenerator Object Constructor
* Opens file, stores handle in class object
*
*/
CodeGenerator::CodeGenerator(std::string file_location, const std::vector<Instruction>& instruction_list) : instructionList(instruction_list){

  // Open files, store handle in object
  this->fileLocation = file_location;

  // Main function, should have all the basic stuff
  mainOutput.open(file_location+"cppcode.cpp");
  if (!mainOutput.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "cppcode.cpp");
  }

  // Complex code. For additional functions and other.
  // WARN: May just move this into mainOutput
  complexCode.open(file_location+"complexCode.cpp");
  if (!complexCode.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "complexCode.cpp");
  }

  // Makefile, easier for compilation vs calls in C++
  makeFile.open(file_location+"Makefile");
  if (!makeFile.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "Makefile");
  }

}


/*
* CodeGenerator object destructor
*
*/
CodeGenerator::~CodeGenerator(){
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


/*
 * Main function, will call sub functions to help
 *
 */
void CodeGenerator::generateCode(){

  // Generate main file
  GEN("#include <iostream>", mainOutput);
  GEN("#include \"regex.h\"", mainOutput);
  GEN("", mainOutput);
  GEN("int main(){", mainOutput);

  GEN("\t// Boilerplate.", mainOutput);
  GEN("\tstd::cout << \"------------------\" << std::endl;", mainOutput);
  GEN("\tstd::cout << \"* AutomataScript *\" << std::endl << std::endl;", mainOutput);

  GEN("", mainOutput);
  GEN("\t// Setting up important variables", mainOutput);
  GEN("\tNode* startNode;", mainOutput);
  GEN("\tint error_status;", mainOutput);

  GEN("", mainOutput);
  GEN("\t// Generating code", mainOutput);
  // Call function for each type of instruction
  for(Instruction instruction : instructionList){
    switch(instruction.inst_type){
      case Instruction::Type::Test:
        generateTestCode((TestData*)instruction.data);
        break;
      case Instruction::Type::Print:
        generatePrintCode((PrintData*)instruction.data);
        break;
      case Instruction::Type::Assign:
        generateAssignCode((AssignData*)instruction.data);
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


/*
* Function: GenerateTestCode
*
* Generate code for Test instructions
* WARN: Not implemented
*
*/
void CodeGenerator::generateTestCode(TestData* testData){
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





  /*
   - [x] startNode = [testData->NFA_name].first;
   - [x] error_status = RunNFA(startNode, 0, [testData->test_value]);
   if(error_status == -1){
    std::cout << "- " + [testData->NFA_name] + " <- " + [testData->test)value] + ": Rejected by NFA.";

   }
   else{
    std::cout << "- " + [testData->NFA_name] + " <- " + [testData->test)value] + ": Accepted by NFA.";
   }
   */





  return;
}


/*
* Function: GeneratePrintCode
*
* Generate code for Print instructions
*
*/
void CodeGenerator::generatePrintCode(PrintData* printData){
  std::string instruction = "";
  instruction += "\tstd::cout << \"";
  instruction += printData->output_string;
  instruction += "\" << std::endl;";

  GEN(instruction, mainOutput);

  return;
}


/*
* Function: GenerateAssignCode
*
* Generate code for Assign instructions
* WARN: Not implemented
*
*/
void CodeGenerator::generateAssignCode(AssignData* assignData){
  std::string instruction = "\t";

  // Grab the desired variable name
  std::string lhs = assignData->lhs;
  instruction += "std::pair<Node *, Node*> ";
  instruction += lhs;
  instruction += " = ";

  std::string rhs_code = generateRhsCode(assignData->rhs);
  Exp* rhs = assignData->rhs;

  instruction += rhs_code;

  instruction += ",\"";
  instruction += lhs;
  instruction += "\");";

  GEN(instruction, mainOutput);

  // TODO: Check if I should display anything to the user that the variable was created

  return;
}



/*
* Function: GenerateMakefile
*
* Generate the makefile to be used to compile the generated code
* TODO: Add parameters to specify which compiler to use
*/
void CodeGenerator::generateMakefile(){
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

/*
* Function: GenerateImplicitCode
*
* Generate the NFA code. This code exists in a file in src, so loop through and move to new file
*/
void CodeGenerator::generateImplicitCode(){
  // Implicit code.
 
  // Generating regex.cpp
  std::ofstream regexCpp;
  regexCpp.open(fileLocation+"regex.cpp");
  if (!regexCpp.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "regex.cpp");
  }
  std::ifstream cSourceFile("src/regex.cpp");
  if (!cSourceFile.is_open()) {
      std::cerr << "Error opg file!" << std::endl;
      return; // Exit on failure
  }
  std::string line;
  // Replace include path (firstline)
  std::getline(cSourceFile, line);
  line = "#include \"regex.h\"";
  GEN(line, regexCpp);
  while (std::getline(cSourceFile, line)) { // Read line by line
      GEN(line, regexCpp); // Use macro to write to output file
      LOG("Line read");
  }
  cSourceFile.close();

  // Generating regex.h
  std::ofstream regexH;
  regexH.open(fileLocation+"regex.h");
  if (!regexH.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "regex.h");
  }
  // h source file
  std::ifstream hSourceFile("include/regex.h");
  if (!hSourceFile.is_open()) {
      std::cerr << "Error opg file!" << std::endl;
      return; // Exit on failure
  }
  while (std::getline(hSourceFile, line)) { // Read line by line
      GEN(line, regexH); // Use macro to write to output file
      LOG("Line read");
  }
  hSourceFile.close();
 
  // Generating nfa.cpp
  std::ofstream nfaCpp;
  nfaCpp.open(fileLocation+"nfa.cpp");
  if (!nfaCpp.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "nfa.cpp");
  }
  std::ifstream nfaCSourceFile("src/nfa.cpp");
  if (!nfaCSourceFile.is_open()) {
      std::cerr << "Error opg file!" << std::endl;
      return; // Exit on failure
  }
  std::getline(nfaCSourceFile, line);
  line = "#include \"nfa.h\"";
  GEN(line, nfaCpp);
  while (std::getline(nfaCSourceFile, line)) { // Read line by line
      GEN(line, nfaCpp); // Use macro to write to output file
      LOG("Line read");
  }
  nfaCSourceFile.close();

  // Generating nfa.h
  std::ofstream nfaH;
  nfaH.open(fileLocation+"nfa.h");
  if (!nfaH.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation + "nfa.h");
  }
  // h source file
  std::ifstream nfaHSourceFile("include/nfa.h");
  if (!nfaHSourceFile.is_open()) {
      std::cerr << "Error opg file!" << std::endl;
      return; // Exit on failure
  }
  while (std::getline(nfaHSourceFile, line)) { // Read line by line
      GEN(line, nfaH); // Use macro to write to output file
      LOG("Line read");
  }
  nfaHSourceFile.close();
}


// WARN: Incomplete
std::string CodeGenerator::generateRhsCode(Exp* rhs){
  std::string rhs_out = "";

  Exp_p2* exp_p2 = rhs->exp_p2;
  std::string exp_p2_code = generateExpP2Code(exp_p2);
  rhs_out += exp_p2_code;



  // TODO: CodeGen for binop_exp_p2s
  
  return rhs_out;
}



// WARN: Incomplete
std::string CodeGenerator::generateExpP2Code(Exp_p2* exp_p2){
  std::string instruction = "";
  Exp_p1* exp_p1 = exp_p2->exp_p1;

  instruction += generateExpP1Code(exp_p1);
  
  // TODO: CodeGen for binop_p1

  return instruction;
}


// WARN: Incomplete
std::string CodeGenerator::generateExpP1Code(Exp_p1* exp_p1){
  std::string output = "";

  if(exp_p1->exp_p1_type == Exp_p1::Type::Exp_ac){
    output += "LiteralNFA(\"";
    output += exp_p1->identifier;
    output += "\"";
    return output;
  }


  // TODO: Cases for Exp, Lval

  return "";
}
