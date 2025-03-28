#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

/*
 * struct ComparePair
 *
 * Updates comparison operator to help with priority queue operations.
 * Set the < to > if want to check for smallest value (I can't think of a case
 * where this would be useful)
 *
 */
struct ComparePair {
  bool operator()(const std::pair<int, std::string> &lhs,
                  const std::pair<int, std::string> &rhs) const {
    // Compare based on the first element of the pair (the int)
    return lhs.first < rhs.first; // Change > to < if you want ascending order
  }
};

/*
 * Priority queue used to store nodes. This holds all states that have accepted.
 */
extern std::priority_queue<std::pair<int, std::string>,
                           std::vector<std::pair<int, std::string>>,
                           ComparePair>
    lengthPQ;

/*
 * struct: Label
 *
 * Holds the value of the transition. In the case of an epsilon transition, type
 * should be set to EPSILON, and the value of char value field is undefined.
 *
 */
struct Label {
  enum { EPSILON, CHAR } type;
  // char is undefined when type is epsilon
  char value;
};

/*
 * struct: Node
 *
 * Struct holding variables used for each node in an NFA
 *
 * @field transitions: vector holding all the transitions from this node to any
 * other. The pair holds the label (epsilon or character), followed by the
 * address of the node that this transition would take you to.
 * @field isTerminalNode: Set to true if this is an accepting node.
 * @field tokenIdentifier: Holds the value of which token this node belongs to.
 * Only used when lexing source code
 *
 */
struct Node {
  // Remember to handle epsilon transitions
  std::vector<std::pair<Label, Node *>> transitions;
  bool isTerminalNode;
  std::string tokenIdentifier;
};

/*
 * struct: State
 *
 * Struct holding value for this current run. Holds the index that is currently
 * being viewed.
 *
 * @field currentIndex: current index of the input string
 * @field currentNode: pointer to the node attached to this state
 *
 */
struct State {
  // The current index represents where you are in the input string for this run
  int currentIndex;
  // The current node is
  // a pointer to where you are in the graph
  Node *currentNode;
};

/* Function Prototypes */
bool TransitionState(State &currentState, std::string input,
                     std::queue<State> &nextStates);
int RunNFA(Node *startNode, int startIndex, std::string input);

#endif /* NFA_H */
