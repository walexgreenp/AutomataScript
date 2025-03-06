#include "../include/debug.h"

/*
* Log tokens with their index/identifier
* Outputted at logs/token_log.log
*/
void log_tokens(std::vector<std::string> tokens){
  std::ofstream outputFile;
  outputFile.open("logs/token_log.log");

  if (!outputFile.is_open()) {
    throw std::ios_base::failure("Failed to open file for token logging. ");
  }

  for(int i = 0; i < tokens.size(); i++){
    outputFile << i << ": " << tokens[i] << std::endl;
  }

  outputFile.close();
}
