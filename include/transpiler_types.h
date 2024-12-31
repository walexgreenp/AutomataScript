#ifndef TRANSPILER_TYPES_H
#define TRANSPILER_TYPES_H

#include <memory>
#include <string>
#include <vector>

/* Forward declarations of types */
struct Exp;
struct Exp_p2;
struct Exp_p1;
struct PrintData;
struct AssignData;
struct TestData;
struct Instruction;


/* Structures */
struct Exp{
  Exp_p2* exp_p2;
  std::vector<std::pair<std::string, Exp_p2*>> binop_exp_p2s;
};

struct Exp_p2{
  Exp_p1* exp_p1;
  std::string binop_p1; // OPTIONAL
};

struct Exp_p1{
  // `(` exp `)`
  Exp* exp; 

  // exp_ac*
  // (a-z)
  std::string identifier;
};

struct PrintData{
  std::string output_string;
};

struct TestData{
  std::string NFA_name;
  std::string test_value;
};

struct AssignData{
  std::string lhs;
  Exp* rhs;
};

struct ErrorData{
  enum class Type {ParsingError} error_type;
  int error_index; // Only set if Type == parsing_error
  
  // Constructors
  ErrorData(Type type, int index) : error_type(type), error_index(index) {};
};


struct Instruction{
  enum class Type {Print, Test, Assign, Error} inst_type;
  void* data;

  // Constructors
  Instruction(Type type, void* ptr) : inst_type(type), data(ptr) {};
};

#endif /* TRANSPILER_TYPES_H */
