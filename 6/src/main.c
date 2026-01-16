#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Program Usage Documentation
  if (argc != 2) {
    printf("Usage: ./main AssemblyProgram.asm\n");
    return 1;
  }

  // Declarations
  char *ASM_NAME;
  char *COM_ASM;
  char *WSP_ASM;
  char *HACK_NAME;
  FILE *ASM_ptr;
  FILE *COM_ptr;
  FILE *WSP_ptr;
  FILE *HACK_ptr;

  // Initialisations
  ASM_NAME = argv[1];
  COM_ASM = "nocomments.asm";
  WSP_ASM = "nowhitespaces.asm";
  ASM_ptr = fopen(ASM_NAME, "r");
  COM_ptr = fopen(COM_ASM, "w");

  // opening files
  if (ASM_ptr == NULL || COM_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 2;
  }

  // removing comments and saving to nocomments.asm
  char line[256];
  while (fgets(line, sizeof(line), ASM_ptr)) {
    char *com_pos = strstr(line, "//");
    if (com_pos != NULL) {
      long len = com_pos - line;
      fprintf(COM_ptr, "%.*s\n", (int)len, line);
    } else {
      fprintf(COM_ptr, "%s\n", line);
    }
  }
  fclose(COM_ptr);

  // removing whitespaces and saving to nowhitespaces.asm
  COM_ptr = fopen(COM_ASM, "r");
  WSP_ptr = fopen(WSP_ASM, "w");
  if (COM_ptr == NULL || WSP_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 3;
  }
  while (fgets(line, sizeof(line), COM_ptr)) {
    if (line[0] == '\n' || line[0] == '\r') {
      continue;
    }
    printf("%s", line);
    for (int i = 0; line[i] != '\0'; i++) {
      if (line[i] != ' ' && line[i] != '\t') {
        fputc(line[i], WSP_ptr);
      }
    }
  }

  char *base = strdup(ASM_NAME);
  char *token = strtok(base, ".");
  if (token != NULL) {
    HACK_NAME = strcat(token, ".hack");
  }
  HACK_ptr = fopen(HACK_NAME, "w");

  if (HACK_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 4;
  }
  fputs("Hello\n", HACK_ptr);

  fclose(ASM_ptr);
  fclose(COM_ptr);
  fclose(WSP_ptr);
  fclose(HACK_ptr);
}
