#include "../include/nfa.h"
#include "../include/regex.h"
#include <fstream>
#include <iostream>
#include <vector>

Node *initNFA();
std::string outputToken(const std::string &lexeme);
void makeLiteralNFAs(std::vector<std::pair<Node *, Node *>> &nfas);
std::string removeComments(std::string input);
std::vector<std::string> mainTokenizer(std::string input);
