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
 * Constructor for a parser object
 */
Parser::Parser(const std::vector<std::string> &token_list)
    : tokens(token_list) {

  this->parsing_index = 0;
  this->total_tokens = token_list.size();
}

/*
 * Top Level parser.
 *
 * Will look for the 3 types, and flow through the proper function
 *
 */
std::vector<Instruction> Parser::mainParser() {
  std::vector<Instruction> instructions;
  std::string token;
  int err_val;

  while (parsing_index < total_tokens) {
    token = tokens[parsing_index];

    // Handle Print
    if (token == "Print") {
      err_val = consumeToken();
      if (err_val != NO_ERR) {
        return generateErrorIV(err_val);
      }

      Instruction printInst = parsePrint();
      instructions.push_back(printInst);
    }

    // Handle Test
    else if (token == "Test") {
      err_val = consumeToken();
      if (err_val != NO_ERR) {
        return generateErrorIV(err_val);
      }

      Instruction testInst = parseTest();
      instructions.push_back(testInst);
    }

    // Handle Variable
    else if (token.substr(0, 3) == "Id(") {
      // No consume, need to use Id in recursive call

      Instruction varInst = parseVar();
      instructions.push_back(varInst);
      std::string currToken = tokens[parsing_index];
    }

    // Invalid, return.
    else {
      // Set up structure to pass back error info
      return generateErrorIV(parsing_index);
    }
  }

  return instructions;
};

/*
 * Function: parsePrint()
 *
 * Parse the print and store into Instruction
 *
 * print ::= `PRINT` `:`  Id()* `;`
 *
 */
Instruction Parser::parsePrint() {
  std::string currToken;

  // Parsing `:`
  currToken = tokens[parsing_index];
  if (currToken != "Colon") {
    return generateErrorInstruction(parsing_index);
  }
  int err_val = consumeToken();
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing Id()*
  std::string output_string = "- ";
  while (1) {
    currToken = tokens[parsing_index];
    err_val = consumeToken();
    if (err_val != NO_ERR) {
      return generateErrorInstruction(err_val);
    }

    if (currToken.substr(0, 3) != "Id(") {
      break;
    }

    output_string += currToken.substr(3, currToken.length() - 4);
    output_string += " ";
  }

  // Parsing `;`
  if (currToken != "Semicolon") {
    return generateErrorInstruction(parsing_index);
  }
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Setting data for the instruction for vector
  PrintData *print_data = new PrintData(output_string);
  Instruction inst_out(Instruction::Type::Print, print_data);

  return inst_out;
}

/*
 * Function: consumeToken
 *
 * Increment the parsing index, ensure it's valid
 *
 */
int Parser::consumeToken() {
  parsing_index++;
  if (parsing_index > total_tokens) {
    return parsing_index;
  }

  return NO_ERR;
}

/*
 * Function: generateErrorIV
 *
 * Generate the instruction vector formatted to read errors
 *
 */
std::vector<Instruction> Parser::generateErrorIV(int err_val) {
  std::vector<Instruction> err_instruction;
  err_instruction.push_back(generateErrorInstruction(err_val));
  return err_instruction;
}

/*
 * Function: generateErrorInstruction
 *
 * Return a single instruction that is an error
 *
 */
Instruction Parser::generateErrorInstruction(int err_val) {
  ErrorData *err_data = new ErrorData(ErrorData::Type::ParsingError, err_val);
  Instruction err_inst(Instruction::Type::Error, err_data);

  return err_inst;
}

/*
 * Function: parseTest()
 *
 * Parse the print and store into Instruction
 *
 * print ::= `PRINT` `:`  Id()* `;`
 *
 */
Instruction Parser::parseTest() {
  std::string currToken;

  // Parsing `:`
  currToken = tokens[parsing_index];
  if (currToken != "Colon") {
    return generateErrorInstruction(parsing_index);
  }
  int err_val = consumeToken();
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing lval
  std::string nfa_name = "";
  currToken = tokens[parsing_index];
  if (currToken.substr(0, 3) != "Id(") {
    return generateErrorInstruction(parsing_index);
  }
  nfa_name += currToken.substr(3, currToken.length() - 4);
  err_val = consumeToken();
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing "<<"
  currToken = tokens[parsing_index];
  if (currToken != "Runner") {
    return generateErrorInstruction(parsing_index);
  }
  err_val = consumeToken(); // Consume "<<"
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing "
  currToken = tokens[parsing_index];
  if (currToken != "Quotation") {
    return generateErrorInstruction(parsing_index);
  }
  err_val = consumeToken(); // Consume "
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing Id()
  std::string test_value = "";
  currToken = tokens[parsing_index];
  if (currToken.substr(0, 3) != "Id(") {
    return generateErrorInstruction(parsing_index);
  }
  test_value += currToken.substr(3, currToken.length() - 4);
  err_val = consumeToken();
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing "
  currToken = tokens[parsing_index];
  if (currToken != "Quotation") {
    return generateErrorInstruction(parsing_index);
  }
  err_val = consumeToken(); // Consume "
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing `;`
  currToken = tokens[parsing_index];
  if (currToken != "Semicolon") {
    return generateErrorInstruction(parsing_index);
  }
  err_val = consumeToken(); // Consume semicolon
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Setting data for the instruction for vector
  TestData *test_data = new TestData(nfa_name, test_value);
  Instruction inst_out(Instruction::Type::Test, test_data);

  return inst_out;
}

Instruction Parser::parseVar() {
  int err_val;
  std::string currToken;

  // Parsing Id()
  std::string lhs = "";
  currToken = tokens[parsing_index];
  if (currToken.substr(0, 3) != "Id(") {
    return generateErrorInstruction(parsing_index);
  }
  lhs += currToken.substr(3, currToken.length() - 4);
  err_val = consumeToken();
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing `=`
  currToken = tokens[parsing_index];
  if (currToken != "Assign") {
    return generateErrorInstruction(parsing_index);
  }
  err_val = consumeToken();
  if (err_val != NO_ERR) {
    return generateErrorInstruction(err_val);
  }

  // Parsing rhs
  Exp *expression = parseRhs(); // TODO: parseRhs
  if (expression->exp_p2 == nullptr) {
    return generateErrorInstruction(parsing_index);
  }

  // Setting data for the instruction for vector
  AssignData *assign_data = new AssignData(lhs, expression);
  Instruction inst_out(Instruction::Type::Assign, assign_data);

  return inst_out;
}

// WARN: Incomplete!
// Currently supports small subset of instructions
// Supported:
//  - Basic -> A = "abc";
Exp* Parser::parseRhs() {
  // Start by getting exp_p2
  std::string currToken;
  Exp_p2* p2_exp = parseExp_p2();
  Exp *expression = new Exp(p2_exp);

  // TODO: Loop to see if there are any further expressions for binop_exp_p2s

  currToken = tokens[parsing_index];
  if(currToken != "Semicolon"){
    // TODO: Better return
    return nullptr;
  }
  int err_val = consumeToken();
  if(err_val != NO_ERR){
    return nullptr;
  }
  return expression;
}

// WARN: Incomplete!
// Need to add binop check
Exp_p2* Parser::parseExp_p2(){

  Exp_p1* p1_exp = parseExp_p1();
  Exp_p2* p2_exp = new Exp_p2(p1_exp);

  // TODO: Check if binop exists

  return p2_exp;
}

// WARN: Incomplete!
// Small subset supported
// Supported:
//  - exp_ac
Exp_p1* Parser::parseExp_p1(){
  // Idea: Check next token. Match to any possible case.
  // If no case matches, there is an error here.
  // TODO: ( exp )
  Exp_p1* p1_exp = new Exp_p1();
  std::string currToken = tokens[parsing_index];
  int err_val;

  // exp_ac (Id())
  if(currToken == "Quotation"){
    // Only path that can exist for Quotation
    err_val = consumeToken();
    if(err_val != NO_ERR){
      // TODO: Add better return types than just nullptr
      return nullptr;
    }
    
    // Parsing Id
    std::string literal = "";
    currToken = tokens[parsing_index];
    if (currToken.substr(0, 3) != "Id(") {
      // TODO: Better return
      return nullptr;
    }
    literal += currToken.substr(3, currToken.length() - 4);
    err_val = consumeToken();
    if (err_val != NO_ERR) {
      // TODO: Better return
      return nullptr;
    }

    // Parsing closing quote
    currToken = tokens[parsing_index];
    if(currToken != "Quotation"){
      // TODO: Better return
      return nullptr;
    }
    err_val = consumeToken();
    if (err_val != NO_ERR){
      // TODO: Better return
      return nullptr;
    }

    p1_exp->exp_p1_type = Exp_p1::Type::Exp_ac;
    p1_exp->identifier = literal;
  }
  else{
    // TODO: add other cases here (e.g. ( exp ), lval, etc)
    return nullptr;
  }

  return p1_exp;
}




