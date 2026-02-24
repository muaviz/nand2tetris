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
  Parser P(VM_NAME);

  while (P.hasMoreCommands()) {
    P.advance();
  }
  return 0;
}
