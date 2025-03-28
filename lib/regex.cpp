#include "regex.h"

// nfas.push_back(
//     ConcatNFA(BracketNFA(alphabet, "Id"),
//               KleeneStarNFA(BracketNFA(alphanumeric, "Id")))); // Id
std::pair<Node *, Node *> LiteralNFA(std::string acceptable_chars,
                                     std::string identifyingSymbol) {

  struct Node *start_node = new Node();
  struct Node *prev_node = start_node;
  struct Node *end_node;
  struct Node *current_node;

  start_node->tokenIdentifier = identifyingSymbol;
  // Create a new node.
  // Make a transition from prev_node to new_node with regex_expression[i]
  for (char c : acceptable_chars) {
    current_node = new Node();
    current_node->isTerminalNode = false;
    current_node->tokenIdentifier = identifyingSymbol;
    Label label = Label();
    label.type = Label::CHAR;
    label.value = c;
    prev_node->transitions.push_back({label, current_node});
    prev_node = current_node;
  }
  current_node->isTerminalNode = true;
  end_node = current_node;

  return {start_node, end_node};
}

std::pair<Node *, Node *> KleeneStarNFA(std::pair<Node *, Node *> nfa) {
  Label label = Label();
  label.type = Label::EPSILON;
  nfa.second->transitions.push_back({label, nfa.first});
  return nfa;
}

std::pair<Node *, Node *> BracketNFA(std::vector<char> accepting_chars,
                                     std::string identifyingSymbol) {
  struct Node *start_node = new Node();
  struct Node *end_node = new Node();

  for (char chr : accepting_chars) {
    Label label = Label();
    label.type = Label::CHAR;
    label.value = chr;
    start_node->transitions.push_back({label, end_node});
    start_node->tokenIdentifier = identifyingSymbol;
  }

  end_node->tokenIdentifier = identifyingSymbol;
  end_node->isTerminalNode = true;
  return {start_node, end_node};
}

std::pair<Node *, Node *> ConcatNFA(std::pair<Node *, Node *> nfa1,
                                    std::pair<Node *, Node *> nfa2) {
  Label label = Label();
  label.type = Label::EPSILON;
  nfa1.second->isTerminalNode = false;
  nfa1.second->transitions.push_back({label, nfa2.second});
  return {nfa1.first, nfa2.second};
}

// Definition of PlusNFA for chars (assuming not passed a literal)
std::pair<Node *, Node *> PlusNFA(std::vector<char> accepting_chars,
                                  std::string identifyingSymbol) {
  std::pair<Node *, Node *> first_bracket_nfa, second_bracket_nfa, final_nfa;
  first_bracket_nfa = BracketNFA(accepting_chars, identifyingSymbol);
  second_bracket_nfa = BracketNFA(accepting_chars, identifyingSymbol);

  final_nfa = KleeneStarNFA(second_bracket_nfa);
  final_nfa = ConcatNFA(first_bracket_nfa, final_nfa);
  return final_nfa;
}

// PlusNFA when a predefined NFA is passed, such as LiteralNFA
std::pair<Node *, Node *> PlusNFA(std::pair<Node *, Node *> nfa) {
  // For A+, the NFA must match one occurrence of A (which is ensured by the
  // given NFA) and then allow additional repetitions of A. This is achieved by
  // adding an epsilon transition from the terminal node back to the start node.
  Label label;
  label.type = Label::EPSILON;
  nfa.second->transitions.push_back({label, nfa.first});
  return nfa;
}

Node *ThompsonNFA(std::vector<std::pair<Node *, Node *>> nfas) {
  struct Node *start_node = new Node();
  Label label = Label();
  label.type = Label::EPSILON;

  for (auto nfa : nfas) {
    start_node->transitions.push_back({label, nfa.first});
  }
  return start_node;
}

std::pair<Node *, Node *> OptionalNFA(std::pair<Node *, Node *> nfa) {
  // Add an epsilon transition from the start node to the terminal node.
  // This allows the NFA to skip the contained expression.
  Label label;
  label.type = Label::EPSILON;
  nfa.first->transitions.push_back({label, nfa.second});
  return nfa;
}
