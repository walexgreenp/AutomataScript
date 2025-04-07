#include "../include/tokenize.h"
#include "../include/nfa.h"
#include "../include/regex.h"
#include <iostream>
#include <vector>

typedef std::vector<std::pair<Node *, Node *>> nfa_list;

/**
 * removeComments
 * @brief Removes both line and block comments from the given input string.
 *
 * Scans the input string character by character, detecting and skipping
 * comments.
 *
 * @param input The input string from which comments should be removed.
 * @return A string with comments removed.
 */
std::string removeComments(std::string input) {
  std::string result;
  bool inLineComment = false;
  bool inBlockComment = false;
  for (size_t i = 0; i < input.size(); ++i) {
    if (!inLineComment && !inBlockComment && input[i] == '/' &&
        i + 1 < input.size()) {
      if (input[i + 1] == '/') {
        inLineComment = true;
        ++i;
        continue;
      } else if (input[i + 1] == '*') {
        inBlockComment = true;
        ++i;
        continue;
      }
    }
    if (inLineComment && input[i] == '\n') {
      inLineComment = false;
      result += " ";
      continue;
    }
    if (inBlockComment && input[i] == '*' && i + 1 < input.size() &&
        input[i + 1] == '/') {
      inBlockComment = false;
      result += " ";
      ++i;
      continue;
    }
    if (!inLineComment && !inBlockComment) {
      result += input[i];
    }
  }
  return result;
}

/**
 * mainTokenizer
 * @brief Tokenizes the given input string by first removing comments and then
 * applying NFAs.
 *
 * Initializes NFAs for tokenizing, iterates through the input, and collects
 * tokens.
 *
 * @param input The raw input string to be tokenized.
 * @return A vector of token strings.
 */
std::vector<std::string> mainTokenizer(std::string input) {
  std::vector<std::string> tokens;

  input = removeComments(input);
  input += "\n";

  // Initialize the NFAs for tokenizing
  Node *startNode = initNFA();

  // Start position for the RunNFA
  int startIndex = 0;
  while (startIndex < input.length()) {
    int tokenLength = RunNFA(startNode, startIndex, input);

    if (tokenLength > 0) {
      std::string token = input.substr(startIndex, tokenLength);
      std::string out = outputToken(token);
      if (out != "IGNORE") {
        // Don't add IGNORE tokens
        tokens.push_back(out);
      }
      startIndex += tokenLength; // Move the start index forward by the length
                                 // of the token
    } else {
      std::cout << "Error" << std::endl;
      // TODO: Add better error return
      return {"ERROR: Tokenizing"};
      startIndex++; // Move to the next character to recover from the error
    }
  }
  // startINDEX == input.length

  return tokens;
}

/**
 * outputToken
 * @brief Determines the token type from the lexeme and returns the formatted
 * token string.
 *
 * Uses the top value from a priority queue (lengthPQ) to decide the token type,
 * then formats and returns the token accordingly.
 *
 * @param lexeme The lexeme string to be processed.
 * @return The formatted token string.
 */
std::string outputToken(const std::string &lexeme) {
  // This function should determine the type of token based on the start node
  // and lexeme Here you would map the node to a token type and print it
  std::string outputToken;
  std::pair<int, std::string> firstVal = lengthPQ.top();
  // TODO: Change code below to use the pair rather than just int
  lengthPQ.pop();
  if (!lengthPQ.empty() && firstVal.first == lengthPQ.top().first) {
    // Find the keyword
    // THIS IS A KEYWORD, NOT A VARIABLE
    if (firstVal.second == "Id" || firstVal.second == "Num") {
      firstVal = lengthPQ.top();
    }

    outputToken = firstVal.second;
  } else {
    if (firstVal.second == "Id" || lengthPQ.top().second == "Id") {
      outputToken += "Id(";
      outputToken += lexeme;
      outputToken += ")";
    } else if (firstVal.second == "Num" || lengthPQ.top().second == "Num") {
      outputToken += "Num(";
      outputToken += lexeme;
      outputToken += ")";
    } else if (firstVal.second == "IGNORE") {
      return "IGNORE";
    } else if (firstVal.second == "String" ||
               lengthPQ.top().second == "String") {
      outputToken += "String(";
      outputToken += lexeme;
      outputToken += ")";
    } else {
      outputToken += firstVal.second;
    }
  }

  return outputToken;
}

/**
 * initNFA
 * @brief Initializes the composite NFA used for tokenization.
 *
 * Constructs NFAs for numbers, identifiers, strings, literals, and whitespaces,
 * then combines them using Thompson's construction.
 *
 * @return A pointer to the start node of the combined NFA.
 */
Node *initNFA() {
  std::vector<std::pair<Node *, Node *>> nfas;
  std::vector<char> alphabet;
  for (char letter = 'a'; letter <= 'z'; letter++) {
    alphabet.push_back(letter);
  }
  for (char letter = 'A'; letter <= 'Z'; letter++) {
    alphabet.push_back(letter);
  }

  std::vector<char> alphanumeric = alphabet;
  std::vector<char> numbers;
  for (char digit = '0'; digit <= '9'; digit++) {
    alphanumeric.push_back(digit);
    numbers.push_back(digit);
  }
  // std::vector<char> total_alphabet = alphanumeric;
  std::vector<char> whitespaces = {' ', '\t', '\n', '\r'};
  std::vector<char> literals = {'&', ':', ';', ',', '_', '-', '+', '-',
                                '*', '/', '<', '<', '>', '>', '.', '=',
                                '(', ')', '[', ']', '{', '}', '!', '^'};
  nfas.push_back(PlusNFA(numbers, "Num")); // Num

  nfas.push_back(
      ConcatNFA(BracketNFA(alphabet, "Id"),
                KleeneStarNFA(BracketNFA(alphanumeric, "Id")))); // Id

  // Make NFA for strings
  // '"' (whitespace* alphanumeric*)* '"'
  nfas.push_back(ConcatNFA(
      LiteralNFA("\"", "String"),
      ConcatNFA(ConcatNFA(BracketNFA(alphabet, "String"),
                          KleeneStarNFA(BracketNFA(alphanumeric, "String"))),
                LiteralNFA("\"", "String"))));

  // Construct literals with their identifiers (Including whitespaces)
  makeLiteralNFAs(nfas);

  // All Whitespaces
  nfas.push_back(BracketNFA(whitespaces, "IGNORE"));
  std::vector<char> all_alphabet_no_backslash = {};

  for (int i = 0; i < 256; i++) {
    if (i == 10) {
      continue;
    }
    all_alphabet_no_backslash.push_back(static_cast<char>(i));
  }
  std::vector<char> all_alphabet = all_alphabet_no_backslash;
  all_alphabet.push_back('\n');

  return ThompsonNFA(nfas);
}

/**
 * makeLiteralNFAs
 * @brief Adds literal NFAs to the provided list of NFAs.
 *
 * Constructs NFAs for various literal symbols such as quotation marks,
 * semicolons, assignment operators, and others, then appends them to the given
 * vector.
 *
 * @param nfas A reference to a vector of NFAs (each represented as a pair of
 * nodes).
 */
void makeLiteralNFAs(std::vector<std::pair<Node *, Node *>> &nfas) {
  nfas.push_back(LiteralNFA("\"", "Quotation"));
  nfas.push_back(LiteralNFA(";", "Semicolon"));
  nfas.push_back(LiteralNFA("|", "Or"));
  nfas.push_back(LiteralNFA("=", "Assign"));
  nfas.push_back(LiteralNFA("*", "Star"));
  nfas.push_back(LiteralNFA("+", "Plus"));
  nfas.push_back(LiteralNFA("^", "Karet"));
  nfas.push_back(LiteralNFA("?", "Question"));
  nfas.push_back(LiteralNFA("-", "Dash"));
  nfas.push_back(LiteralNFA("(", "OpenParen"));
  nfas.push_back(LiteralNFA(")", "CloseParen"));
  nfas.push_back(LiteralNFA("TEST", "Test"));
  nfas.push_back(LiteralNFA("PRINT", "Print"));
  nfas.push_back(LiteralNFA(":", "Colon"));
  nfas.push_back(LiteralNFA("<<", "Runner"));
}
