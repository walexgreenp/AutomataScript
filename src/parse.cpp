#include "../include/parse.h"

/**
 * @brief Global parsing index.
 *
 * Points to the current token during parsing.
 */
int parsing_index;

/**
 * Parser::Parser
 *
 * @brief Constructor for the Parser class.
 *
 * Initializes the parser with a list of tokens and resets the parsing index.
 *
 * @param token_list The vector of tokens to parse.
 */
Parser::Parser(const std::vector<std::string> &token_list)
    : tokens(token_list) {

  this->parsing_index = 0;
  this->total_tokens = token_list.size();
}

/**
 * Parser::mainParser
 *
 * @brief Top-level parser that processes tokens into a vector of instructions.
 *
 * Scans through the token stream, dispatching to appropriate parsing functions
 * for Print, Test, and variable instructions.
 *
 * @return A vector of Instruction objects.
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

/**
 * Parser::parsePrint
 *
 * @brief Parses a Print instruction.
 *
 * Expected syntax:
 *   Print : Id()* ;
 *
 * @return An Instruction object for a Print operation.
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

/**
 * Parser::consumeToken
 *
 * @brief Advances the parsing index and validates its range.
 *
 * @return NO_ERR if successful; otherwise, an error value.
 */
int Parser::consumeToken() {
  parsing_index++;
  if (parsing_index > total_tokens) {
    return parsing_index;
  }

  return NO_ERR;
}

/**
 * Parser::generateErrorIV
 *
 * @brief Generates an instruction vector containing a single error instruction.
 *
 * @param err_val The error value or index where the error occurred.
 * @return A vector of Instruction objects containing the error.
 */
std::vector<Instruction> Parser::generateErrorIV(int err_val) {
  std::vector<Instruction> err_instruction;
  err_instruction.push_back(generateErrorInstruction(err_val));
  return err_instruction;
}

/**
 * Parser::generateErrorInstruction
 *
 * @brief Generates a single error instruction.
 *
 * @param err_val The error value or index.
 * @return An Instruction object representing an error.
 */
Instruction Parser::generateErrorInstruction(int err_val) {
  ErrorData *err_data = new ErrorData(ErrorData::Type::ParsingError, err_val);
  Instruction err_inst(Instruction::Type::Error, err_data);

  return err_inst;
}

/**
 * Parser::parseTest
 *
 * @brief Parses a Test instruction.
 *
 * Expected syntax:
 *   Test : Id("...") Runner Quotation Id("...") Quotation Semicolon
 *
 * @return An Instruction object for a Test operation.
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
  if (currToken.substr(0, 3) == "Id(") {
    test_value += currToken.substr(3, currToken.length() - 4);
    err_val = consumeToken();
    if (err_val != NO_ERR) {
      return generateErrorInstruction(err_val);
    }
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

/**
 * Parser::parseVar
 *
 * @brief Parses a variable assignment instruction.
 *
 * Expected syntax:
 *   Id() Assign <expression> ;
 *
 * @return An Instruction object for an assignment operation.
 */
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
/**
 * Parser::parseRhs
 *
 * @brief Parses the right-hand side (RHS) of an assignment.
 *
 * Begins by parsing an Exp_p2 expression and then checks for additional binary
 * operations.
 *
 * @return A pointer to an Exp representing the parsed expression.
 * @warn WARN: Incomplete!
 * @todo TODO: Loop to handle additional binary operations.
 */
Exp *Parser::parseRhs() {
  // Start by getting exp_p2
  std::string currToken;
  Exp_p2 *p2_exp = parseExp_p2();
  Exp *expression = new Exp(p2_exp);

  // TODO: Loop to see if there are any further expressions for binop_exp_p2s

  currToken = tokens[parsing_index];
  if (currToken != "Semicolon") {
    // TODO: Better return
    return nullptr;
  }
  int err_val = consumeToken();
  if (err_val != NO_ERR) {
    return nullptr;
  }
  return expression;
}

/**
 * Parser::parseExp_p2
 *
 * @brief Parses an Exp_p2 expression.
 *
 * First, parses an Exp_p1 expression and then checks for an optional unary
 * operator.
 *
 * @return A pointer to an Exp_p2 representing the parsed expression.
 * @warn WARN: Incomplete!
 * @todo TODO: Add binary operator checking.
 */
Exp_p2 *Parser::parseExp_p2() {
  int err;

  // Get p1 expression
  Exp_p1 *p1_exp = parseExp_p1();

  // Check if the unop exists (optional)
  std::string token = tokens[parsing_index];
  if (token == "Star") {
    // This exists!, so consume the token
    err = consumeToken();
    if (err != NO_ERR) {
      // TODO: Improve error handling
      return nullptr;
    }
    // KleeneStar
    Exp_p2 *p2_exp = new Exp_p2(p1_exp, Exp_p2::Type::KleeneStar);
    return p2_exp;
  } else if (token == "Plus") {
    // Positional Closure
    err = consumeToken();
    if (err != NO_ERR) {
      // TODO: Improve error handling
      return nullptr;
    }
    Exp_p2 *p2_exp = new Exp_p2(p1_exp, Exp_p2::Type::PosClos);
    return p2_exp;
  } else if (token == "Question") {
    // Optional
    err = consumeToken();
    if (err != NO_ERR) {
      // TODO: Improve error handling
      return nullptr;
    }
    Exp_p2 *p2_exp = new Exp_p2(p1_exp, Exp_p2::Type::Opt);
    return p2_exp;
  }

  // If it doesn't exist, return with type None
  Exp_p2 *p2_exp = new Exp_p2(p1_exp);
  return p2_exp;
}

/**
 * Parser::parseExp_p1
 *
 * @brief Parses an Exp_p1 expression.
 *
 * Currently supports only a subset:
 *   - Literal expressions enclosed in Quotation marks.
 *
 * @return A pointer to an Exp_p1 representing the parsed expression.
 * @warn WARN: Incomplete!
 * @todo TODO: Add support for ( exp ), lval, and other cases.
 */
Exp_p1 *Parser::parseExp_p1() {
  // Idea: Check next token. Match to any possible case.
  // If no case matches, there is an error here.
  // TODO: ( exp )
  Exp_p1 *p1_exp = new Exp_p1();
  std::string currToken = tokens[parsing_index];
  int err_val;

  // ( exp ) OR ( exp_ac - exp_ac )
  if(currToken == "OpenParen"){
    err_val = consumeToken();
    if (err_val != NO_ERR) {
      // TODO: Add better return types than just nullptr
      return nullptr;
    }

    // TODO: New stuff here
  }
  // exp_ac
  else if (currToken == "Quotation") {
    // Only path that can exist for Quotation
    err_val = consumeToken();
    if (err_val != NO_ERR) {
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
    if (currToken != "Quotation") {
      // TODO: Better return
      return nullptr;
    }
    err_val = consumeToken();
    if (err_val != NO_ERR) {
      // TODO: Better return
      return nullptr;
    }

    p1_exp->exp_p1_type = Exp_p1::Type::Exp_ac;
    p1_exp->identifier = literal;
  } else if(currToken.substr(0,3) == "Id("){
    err_val = consumeToken();
    if (err_val != NO_ERR) {
      // TODO: Add better return types than just nullptr
      return nullptr;
    }
    // TODO: New stuff here
  } else {
    // User error, should not be here.
    return nullptr;
  }

  return p1_exp;
}
