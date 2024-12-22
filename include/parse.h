#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include <string>
#include "../include/transpiler_types.h"

std::vector<Instruction> mainParser(std::vector<std::string> tokens);

#endif /* PARSE_H */
