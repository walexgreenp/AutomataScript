#ifndef REGEX_H
#define REGEX_H

#include "nfa.h"

std::pair<Node *, Node *> LiteralNFA(std::string acceptable_chars,
                                     std::string identifyingSymbol);
std::pair<Node *, Node *> KleeneStarNFA(std::pair<Node *, Node *> nfa);
std::pair<Node *, Node *> BracketNFA(std::vector<char> accepting_strings,
                                     std::string identifyingSymbol);
std::pair<Node *, Node *> ConcatNFA(std::pair<Node *, Node *> nfa1,
                                    std::pair<Node *, Node *> nfa2);
std::pair<Node *, Node *> PlusNFA(std::vector<char> accepting_chars,
                                  std::string identifyingSymbol);
std::pair<Node *, Node *> PlusNFA(std::pair<Node *, Node *> nfa);
std::vector<char> CaretGenerate(char char_ignore);
std::pair<Node *, Node *> CommentNFA();
Node *ThompsonNFA(std::vector<std::pair<Node *, Node *>> nfas);
std::pair<Node *, Node *> OptionalNFA(std::pair<Node *, Node *> nfa);
#endif /* REGEX_H */
