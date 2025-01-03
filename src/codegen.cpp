#include "../include/codegen.h"
#include <fstream>


void generate_code(std::string file_location){
  std::ofstream outputFile(file_location);

  if(outputFile.is_open()){
    outputFile << "#include <iostream>" << std::endl;
    outputFile << "int main(){" << std::endl;
    outputFile << "  std::cout << \"This is a test\" << std::endl;" << std::endl;
    outputFile << "  return 0;" << std::endl;
    outputFile << "}" << std::endl;
  }
  else{
    std::cerr << "Error opening file" << std::endl;
  }

}
