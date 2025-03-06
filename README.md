# AutomataScript

AutomataScript is a custom programming language designed for defining and manipulating finite automata and regular expressions. It transpiles into C++ code, allowing seamless execution of automata-based logic. Look at examples/ directory to find sample code that is available. README.md in examples has notes on what is valid/invalid

## Features
- **Custom LL(1) Grammar** – Designed and implemented a formal grammar for parsing AutomataScript.
- **Lexer/Tokenizer** – Converts source code into tokens for syntactic analysis.
- **AST (Abstract Syntax Tree) Generation** – Structures the parsed code into an executable format.
- **Transpilation to C++** – Converts AutomataScript into equivalent C++ code for execution.
- **Automated Compilation & Execution** – A single command transpiles, compiles, and runs the generated code.
- **Error Handling** – Includes syntax and semantic error detection.

## Installation

To build the project, ensure you have a C++ compiler (e.g., `g++`) installed. Then, clone the repository and compile the transpiler:

```sh
git clone https://github.com/walexgreenp/AutomataScript
cd AutomataScript
make
```

## Usage

To run an AutomataScript file (`.as`):

```sh
./runAutomataScript path/to/script.as
```

This command:
1. Parses and transpiles `script.as` into C++ code.
2. Compiles the generated C++ code using the system’s compiler.
3. Executes the compiled program.

## Development Progress

### ✅ Completed
- [x] Implement LL(1) grammar
- [x] Create lexer and tokenizer
- [x] Build parser and AST generator for PRINT statements
- [x] Implement basic C++ transpilation
- [x] Automate compilation and execution
- [x] Basic error handling

### 🔧 In Progress / TODO
- [ ] Build parser and AST generator for variable creation
- [ ] Build parser and AST generator for Automata Run steps
- [ ] Complete validation phase of parser
- [ ] Add optimization for generated C++ code
- [ ] Improve error reporting with detailed messages
- [ ] Extend language features (e.g., loops, conditionals)
- [ ] Write comprehensive documentation and examples

### Current/next task
- [ ] Set up validation structures
- [ ] Basic validation of TEST calls (Calling test on non-existent NFA)

## Contributing
Contributions are welcome! To contribute:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature-branch`)
3. Commit your changes (`git commit -m "Add new feature"`)
4. Push to the branch (`git push origin feature-branch`)
5. Open a pull request
