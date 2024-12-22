#include "../include/nfa.h"
#include "../include/tokenize.h"
#include "../include/regex.h"
#include <fstream>
#include <iostream>
#include <vector>

typedef std::vector<std::pair<Node *, Node *>> nfa_list;

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

std::vector<std::string> mainTokenizer(std::string input) {
  std::vector<std::string> tokens;
  int counter = 0; // Counter for unmatched "/*"

  size_t pos = 0; // Position in the string
  input = removeComments(input);
  input += "\n";

  // Initialize the NFAs for tokenizing
  Node *startNode = initNFA();

  // Start position for the RunNFA
  int startIndex = 0;
  // fileLength = input.length();
  while (startIndex < input.length()) {
    int tokenLength = RunNFA(startNode, startIndex, input);

    if (tokenLength > 0) {
      std::string token = input.substr(startIndex, tokenLength);
      std::string out = outputToken(token);
      tokens.push_back(out);
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
    } else if(firstVal.second == "String" || lengthPQ.top().second == "String"){
      outputToken += "String(";
      outputToken += lexeme;
      outputToken += ")";
    }
    else {
      outputToken += firstVal.second;
    }
  }

  std::cout << outputToken << std::endl;
  return outputToken;
}

Node *initNFA() {
  Node *new_node = new Node();
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
  std::pair<Node *, Node *> current_nfa;
  nfas.push_back(PlusNFA(numbers, "Num")); // Num

  nfas.push_back(
      ConcatNFA(BracketNFA(alphabet, "Id"),
                KleeneStarNFA(BracketNFA(alphanumeric, "Id")))); // Id

  // Make NFA for strings
  // '"' (whitespace* alphanumeric*)* '"'
  nfas.push_back(
    ConcatNFA(
      LiteralNFA("\"", "String"),
      ConcatNFA(
        ConcatNFA(BracketNFA(alphabet, "String"),
                KleeneStarNFA(BracketNFA(alphanumeric, "String"))),
        LiteralNFA("\"", "String")
      )
    )
  ); // String
  // nfas.push_back(
  //   ConcatNFA(
  //     LiteralNFA("\"", "Id"),
  //     ConcatNFA(
  //       KleeneStarNFA(
  //         ConcatNFA(
  //           KleeneStarNFA(
  //             LiteralNFA({' '}, "Id")
  //           ),
  //           KleeneStarNFA(
  //             BracketNFA(alphanumeric, "Id")
  //           )
  //         )
  //       ),
  //       LiteralNFA("\"", "Id")
  //     ) // Concat2
  //   ) // /ConcatNFA
  // );

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
  std:: vector<char> all_alphabet = all_alphabet_no_backslash;
  all_alphabet.push_back('\n');

  return ThompsonNFA(nfas);
}

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
