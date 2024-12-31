#include "../include/parse.h"

/*
* A program consists of a vector of instructions
* Instructions can have 3 types: PRINT, TEST, NFAvar
*
*
*/

// Global pointer to the current token
int parsing_index;


/*
* Top Level parser.
*
* Will look for the 3 types, and flow through the proper function
*
*/
std::vector<Instruction> mainParser(std::vector<std::string> tokens){
  std::vector<Instruction> instructions;
  std::cout << "Test" << std::endl;

  std::string token;


  while(parsing_index < tokens.size()){
    token = tokens[parsing_index];

    // Handle Print
    if(token == "Print"){

    }

    // Handle Test
    else if(token == "Test"){

    }

    // Handle Variable
    else if(token == "Id("){

    }

    // Invalid, return.
    else{
      // Set up structure to pass back error info
      ErrorData *err_data = new ErrorData(ErrorData::Type::ParsingError, parsing_index);
      Instruction err_inst(Instruction::Type::Error, err_data);

      // Return it as the only instruction in the vector
      std::vector<Instruction> err_instruction;
      err_instruction.push_back(err_inst);
      return err_instruction;
    }
  }

  return {};
};
