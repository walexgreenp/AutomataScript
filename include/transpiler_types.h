#ifndef TRANSPILER_TYPES_H
#define TRANSPILER_TYPES_H

#include <memory>
#include <string>
#include <vector>

/* Macro Definitions */
#define NO_ERR -1

/* Forward declarations of types */
struct Exp;
struct Exp_p2;
struct Exp_p1;
struct PrintData;
struct AssignData;
struct TestData;
struct Instruction;

/* Structures */
struct Exp {
  Exp_p2 *exp_p2;
  std::vector<std::pair<std::string, Exp_p2 *>> binop_exp_p2s;

  Exp() : exp_p2(nullptr) {};
  Exp(Exp_p2 *p2_exp) : exp_p2(p2_exp) {};
};

struct Exp_p2 {
  Exp_p1 *exp_p1;
  enum class Type { None, KleeneStar, PosClos, Opt } unop_type;

  // "Default" constructor
  Exp_p2(Exp_p1 *p1_exp) : exp_p1(p1_exp), unop_type(Type::None) {};

  // Optional unop type is defined
  Exp_p2(Exp_p1 *p1_exp, Type type_unop)
      : exp_p1(p1_exp), unop_type(type_unop) {};
};

struct Exp_p1 {
  // Exp: New expression
  // Exp_ac: Literal (string)
  // Lval: NFA (pre-existing)
  enum class Type { Exp, Exp_ac, Lval } exp_p1_type;

  // `(` exp `)`
  Exp *exp;
  // exp_ac*
  // (a-z)
  std::string identifier;

  Exp_p1() : exp(nullptr) {};
};

struct PrintData {
  std::string output_string;

  PrintData(std::string str) : output_string(str) {};
};

struct TestData {
  std::string NFA_name;
  std::string test_value;

  TestData(std::string name, std::string value)
      : NFA_name(name), test_value(value) {};
};

struct AssignData {
  std::string lhs;
  Exp *rhs;

  AssignData(std::string left, Exp *right) : lhs(left), rhs(right) {};
};

struct ErrorData {
  enum class Type { ParsingError } error_type;
  int error_index; // Only set if Type == parsing_error

  // Constructors
  ErrorData(Type type, int index) : error_type(type), error_index(index) {};
};

struct Instruction {
  enum class Type { Print, Test, Assign, Error } inst_type;
  void *data;

  // Constructors
  Instruction(Type type, void *ptr) : inst_type(type), data(ptr) {};
};

#endif /* TRANSPILER_TYPES_H */
