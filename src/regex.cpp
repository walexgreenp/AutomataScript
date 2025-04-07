#include "../include/regex.h"

/**
 * LiteralNFA
 * @brief Constructs an NFA that recognizes a literal string of acceptable
 * characters.
 *
 * Creates a chain of nodes, each connected by transitions labeled with a
 * character from the given string. The final node is marked as terminal.
 *
 * @param acceptable_chars The string containing the characters to be
 * recognized.
 * @param identifyingSymbol The token identifier for the NFA.
 * @return A pair of pointers {start_node, end_node} representing the
 * constructed NFA.
 */
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

/**
 * KleeneStarNFA
 * @brief Applies the Kleene star operation to an NFA.
 *
 * Adds an epsilon transition from the terminal node back to the start node,
 * allowing for zero or more repetitions of the pattern.
 *
 * @param nfa The NFA (as a pair of start and end nodes) to be modified.
 * @return The modified NFA with Kleene star applied.
 */
std::pair<Node *, Node *> KleeneStarNFA(std::pair<Node *, Node *> nfa) {
  Label label = Label();
  label.type = Label::EPSILON;
  nfa.second->transitions.push_back({label, nfa.first});
  return nfa;
}

/**
 * BracketNFA
 * @brief Constructs an NFA that recognizes any single character from a set.
 *
 * Creates an NFA with a single transition from the start node to the end node
 * for each character in the provided vector.
 *
 * @param accepting_chars A vector of characters that are accepted.
 * @param identifyingSymbol The token identifier for the NFA.
 * @return A pair of pointers {start_node, end_node} representing the
 * constructed NFA.
 */
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

/**
 * ConcatNFA
 * @brief Concatenates two NFAs.
 *
 * Connects the terminal node of the first NFA to the start node of the second
 * NFA using an epsilon transition. The resulting NFA represents the sequence of
 * patterns.
 *
 * @param nfa1 The first NFA (as a pair of nodes).
 * @param nfa2 The second NFA (as a pair of nodes).
 * @return A pair of pointers representing the concatenated NFA.
 */
std::pair<Node *, Node *> ConcatNFA(std::pair<Node *, Node *> nfa1,
                                    std::pair<Node *, Node *> nfa2) {
  Label label = Label();
  label.type = Label::EPSILON;
  nfa1.second->isTerminalNode = false;
  nfa1.second->transitions.push_back({label, nfa2.second});
  return {nfa1.first, nfa2.second};
}

/**
 * PlusNFA
 * @brief Constructs an NFA that recognizes one or more occurrences of a
 * pattern.
 *
 * Combines two bracket NFAs with Kleene star and concatenation operations to
 * achieve the positive closure (plus) effect.
 *
 * @param accepting_chars A vector of characters accepted by the pattern.
 * @param identifyingSymbol The token identifier for the NFA.
 * @return A pair of pointers representing the constructed NFA.
 */
std::pair<Node *, Node *> PlusNFA(std::vector<char> accepting_chars,
                                  std::string identifyingSymbol) {
  std::pair<Node *, Node *> first_bracket_nfa, second_bracket_nfa, final_nfa;
  first_bracket_nfa = BracketNFA(accepting_chars, identifyingSymbol);
  second_bracket_nfa = BracketNFA(accepting_chars, identifyingSymbol);

  final_nfa = KleeneStarNFA(second_bracket_nfa);
  final_nfa = ConcatNFA(first_bracket_nfa, final_nfa);
  return final_nfa;
}

/**
 * ThompsonNFA
 * @brief Combines multiple NFAs using Thompson's construction.
 *
 * Creates a new start node with epsilon transitions to the start nodes of each
 * provided NFA, effectively creating a union of all NFAs.
 *
 * @param nfas A vector of NFAs (each as a pair of start and end nodes) to
 * combine.
 * @return A pointer to the new start node of the combined NFA.
 */
Node *ThompsonNFA(std::vector<std::pair<Node *, Node *>> nfas) {
  struct Node *start_node = new Node();
  Label label = Label();
  label.type = Label::EPSILON;

  for (auto nfa : nfas) {
    start_node->transitions.push_back({label, nfa.first});
  }
  return start_node;
}
