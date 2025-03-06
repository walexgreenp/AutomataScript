#include <fstream>
#include <iostream>
#include <vector>
#include "../include/tokenize.h"
#include "../include/parse.h"
#include "../include/codegen.h"
#include "../include/validate.h"
#include "../include/transpiler_types.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <source_file>\n";
    return 1;
  }

  std::ifstream source(argv[1]);
  if (!source) {
    std::cerr << "Failed to open the file.\n";
    return 1;
  }

  // Read the entire file into a string
  std::string input((std::istreambuf_iterator<char>(source)),
                    std::istreambuf_iterator<char>());

  // Tokenize everything. This should return a vector with all the tokens
  std::vector<std::string> tokens = mainTokenizer(input);
  if(tokens[0].substr(0, 4) == "ERRO"){
    // TODO: Add better error prints
    std::cout << tokens[0] << std::endl;
    return 1;
  }
  log_tokens(tokens);

  // Take the tokens, turn into AST.
  Parser parser(tokens);
  std::vector<Instruction> all_instructions = parser.mainParser();
  if(all_instructions[0].inst_type == Instruction::Type::Error){
    std::cout << "Parsing error at index " << ((ErrorData*)all_instructions[0].data)->error_index << std::endl;
    return 1;
  }

  // Validation phase
  // If parsing is successful, there may be places that are invalid, e.g. attempting to access an unused variable, etc.
  Validator validator(all_instructions);
  int validate_status = validator.validate();
  if(validate_status == -1){
    std::cout << "Validation error" << std::endl;
    return 1;
  }


  // Generate the C++ code, store it in file location
  CodeGenerator codegen("bin/tmp/cppcode.cpp", all_instructions);
  codegen.generateCode();

  // Compile & run generated code
  std::string cppDirectory = "bin/tmp/";
  std::string binaryName = "./bin/compiled_output";
  // TODO: Add ways to use different compilers with setting flags
  std::string compileCommand = "g++ " + cppDirectory + "/cppcode.cpp -o " + binaryName;
  if (std::system(compileCommand.c_str()) != 0) {
    std::cerr << "Compilation failed!\n";
    return 2;
  }
  std::system(binaryName.c_str());

  return 0;
}
