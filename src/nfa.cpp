#include "../include/nfa.h"

/**
 * @brief Declares a locally scoped priority queue to store pairs of integers
 * and strings.
 *
 * The priority queue uses a custom comparator (ComparePair) to order the pairs.
 */
std::priority_queue<std::pair<int, std::string>,
                    std::vector<std::pair<int, std::string>>, ComparePair>
    lengthPQ;

/**
 * TransitionState
 *
 * @brief Adds any of the next states of the current state to the nextStates
 * queue. For a character transition, the index is incremented; for an epsilon
 * transition, the index remains unchanged. Returns true if a terminal node is
 * reached while the input is exhausted.
 *
 * @param currentState The current state of the NFA.
 * @param input The input string representing the symbol (or epsilon) being
 * processed.
 * @param nextStates A queue that holds all states that haven't been processed.
 * @return true if a terminal node is found; otherwise, false.
 */
bool TransitionState(State &currentState, std::string input,
                     std::queue<State> &nextStates) {
  bool reachedTerminal = false;
  // transition is one value from pair<Label, Node*>
  for (auto transition : currentState.currentNode->transitions) {
    // Handle epsilon transition
    if (transition.first.type == Label::EPSILON) {

      nextStates.push({currentState.currentIndex, transition.second});
      if (transition.second->isTerminalNode) {
        lengthPQ.push(std::make_pair(currentState.currentIndex,
                                     transition.second->tokenIdentifier));
        reachedTerminal = true;
      }
    }
    // Handle input char is one of the transitions
    else if (transition.first.value == input[currentState.currentIndex]) {
      nextStates.push({currentState.currentIndex + 1, transition.second});
      if (transition.second->isTerminalNode) {
        lengthPQ.push(std::make_pair(currentState.currentIndex + 1,
                                     transition.second->tokenIdentifier));
        reachedTerminal = true;
      }
    }
  }
  return reachedTerminal;
}

/**
 * RunNFA
 *
 * @brief Initializes the nextStates queue with the starting state, then
 * processes each state by calling TransitionState until the queue is empty or a
 * terminal state is found.
 *
 * @param startNode The starting node of the NFA.
 * @param startIndex The starting index in the input string.
 * @param input The input string to be processed by the NFA.
 * @return The number of characters processed if a terminal state is found;
 *         otherwise, returns -1.
 */
int RunNFA(Node *startNode, int startIndex, std::string input) {
  // empty the PQ
  while (!lengthPQ.empty()) {
    lengthPQ.pop();
  }

  std::queue<State> nextStates;

  State currState = {startIndex, startNode};
  nextStates.push(currState);

  while (!nextStates.empty()) {
    currState = nextStates.front();
    TransitionState(currState, input, nextStates);
    nextStates.pop();
  }

  if (lengthPQ.empty()) {
    return -1;
  }
  return lengthPQ.top().first - startIndex;
}
