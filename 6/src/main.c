#include "iohandler.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Program Usage Documentation
  if (argc != 2) {
    printf("Usage: ./main AssemblyProgram.asm\n");
    return 1;
  }

  name_init(argv[1]);

  // removing comments and saving to nocomments.asm
  if (no_comments() != 0) {
    return 2;
  }

  // removing whitespaces and saving to nowhitespaces.asm
  if (no_whitespaces() != 0) {
    return 3;
  }

  if (translate() != 0) {
    return 4;
  }
}
