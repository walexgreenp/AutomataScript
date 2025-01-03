#include <fstream>
#include <iostream>
#include <vector>
#include "../include/tokenize.h"
#include "../include/parse.h"
#include "../include/codegen.h"
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

  // Take the tokens, turn into AST.
  Parser parser(tokens);
  std::vector<Instruction> all_instructions = parser.mainParser();
  if(all_instructions[0].inst_type == Instruction::Type::Error){
    std::cout << "Parsing error at index " << ((ErrorData*)all_instructions[0].data)->error_index << std::endl;
  }

  generate_code("bin/tmp/cppcode.cpp");



}
