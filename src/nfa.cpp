#include "../include/nfa.h"

/* Declaring extern locally */
std::priority_queue<std::pair<int, std::string>,
                    std::vector<std::pair<int, std::string>>, ComparePair>
    lengthPQ;

/*
 * Function: TransitionState
 *
 *  Add any of the next states of the currentState to the nextStates queue
 *  For character transition make sure to increment index, for epsilon
 *  transition make sure not to increment index Return true if reaches terminal
 *  node and input is exhausted
 *
 *  @param currentState Current state of the NFA
 *  @param input Represents the symbol (or epsilon) being processed
 *  @param nextStates Represents a queue that holds all states that haven't been
 * processed
 *
 * @return TRUE if terminal node is found, otherwise FALSE
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

/*
 * Function: RunNFA
 *  Initialize nextStates queue with starting state
 *  Pop off queue and run TransitionState on dequeued state
 *  Continue until empty queue or TransitionState returns true
 *
 * @param startNode
 * @param input
 *
 * @return -1 if terminal not found.
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
