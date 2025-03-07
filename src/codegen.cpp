#include "../include/codegen.h"


/*
* CodeGenerator Object Constructor
* Opens file, stores handle in class object
*
*/
CodeGenerator::CodeGenerator(std::string file_location, const std::vector<Instruction>& instruction_list) : instructionList(instruction_list){

  this->fileLocation = file_location;

  // Open file, store handle in object
  outputFile.open(file_location);
  if (!outputFile.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + fileLocation);
  }

}


/*
* CodeGenerator object destructor
*
*/
CodeGenerator::~CodeGenerator(){
  if (outputFile.is_open()) {
    outputFile.close();
  }
}


/*
 * Main function, will call sub functions to help
 *
 */
void CodeGenerator::generateCode(){

  outputFile << "#include <iostream>" << std::endl;
  outputFile << "int main(){" << std::endl;
  outputFile << "  std::cout << \"------------------\" << std::endl;" << std::endl;
  outputFile << "  std::cout << \"* AutomataScript *\" << std::endl << std::endl;" << std::endl;


  // Call function for each type of instruction
  for(Instruction instruction : instructionList){
    switch(instruction.inst_type){
      case Instruction::Type::Test:
        generateTestCode();
        break;
      case Instruction::Type::Print:
        generatePrintCode((PrintData*)instruction.data);
        break;
      case Instruction::Type::Assign:
        generateAssignCode();
        break;
      case Instruction::Type::Error:
        std::cout << "ERROR, SHOULD NOT BE HERE!" << std::endl;
        break;
    }
  }

  outputFile << "  std::cout << \"------------------\" << std::endl;" << std::endl;
  outputFile << "  return 0;" << std::endl;
  outputFile << "}" << std::endl;
}


/*
* Function: GenerateTestCode
*
* Generate code for Test instructions
* WARN: Not implemented
*
*/
void CodeGenerator::generateTestCode(){
  // std::cout << "Generating Test Code " << std::endl;

  return;
}


/*
* Function: GeneratePrintCode
*
* Generate code for Print instructions
*
*/
void CodeGenerator::generatePrintCode(PrintData* printData){

  outputFile << "  std::cout << \"" << printData->output_string << "\" << std::endl;" << std::endl;

  return;
}


/*
* Function: GenerateAssignCode
*
* Generate code for Assign instructions
* WARN: Not implemented
*
*/
void CodeGenerator::generateAssignCode(){
  // std::cout << "Generating Assign Code " << std::endl;
  return;
}
