#include <iostream>

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    std::cout << "usage: ./build/VMTranslator <file.vm>\n";
    return 1;
  }
  return 0;
}
