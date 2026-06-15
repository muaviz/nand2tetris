#include "CodeWriter.h"
#include "Parser.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    std::cout << "usage: ./build/VMTranslator <file.vm>\n";
    return 1;
  }
  std::string VM_NAME(argv[1]);
  std::string ASM_NAME;

  std::string newExt = ".asm";
  size_t dotPos = VM_NAME.find_last_of('.');
  if (dotPos != std::string::npos) {
    ASM_NAME = VM_NAME.substr(0, dotPos) + newExt;
  } else {
    std::cerr << "Error creating ASM file" << std::endl;
    throw std::runtime_error("Error creating asm file");
  }

  Parser P(VM_NAME);
  CodeWriter C(ASM_NAME);

  while (P.hasMoreCommands()) {
    P.advance();
    CMD_TYPE type = P.commandType();
    if (type == CMD_TYPE::C_ARITHMETIC) {
      C.WriteArithmetic(P.arg1());
    } else if (type == CMD_TYPE::C_PUSH) {
      C.WritePushPop(CMD_TYPE::C_PUSH, P.arg1(), P.arg2());
    } else if (type == CMD_TYPE::C_POP) {
      C.WritePushPop(CMD_TYPE::C_POP, P.arg1(), P.arg2());
    }
  }

  return 0;
}
