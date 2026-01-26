#include "iohandler.h"
#include "code.h"
#include "parser.h"
#include "preprocessor.h"
#include "symb.h"
#include <stdio.h>
#include <string.h>

// Declarations
char *ASM_NAME;
char *COM_ASM;
char *IM_ASM;
char *WSP_ASM;
char *HACK_NAME;
FILE *ASM_ptr;
FILE *COM_ptr;
FILE *IM_ptr;
FILE *WSP_ptr;
FILE *HACK_ptr;

// Initialisations
int first_line = 1;

void name_init(char *ASM) {
  ASM_NAME = ASM;
  COM_ASM = "nocomments.asm";
  IM_ASM = "intermediate.asm";
  WSP_ASM = "nowhitespaces.asm";

  char *base = strdup(ASM_NAME);
  char *token = strtok(base, ".");
  if (token != NULL) {
    HACK_NAME = strcat(token, ".hack");
  }
}

int no_comments() {
  ASM_ptr = fopen(ASM_NAME, "r");
  COM_ptr = fopen(COM_ASM, "w");
  if (ASM_ptr == NULL || COM_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 2;
  }
  remove_comments(ASM_ptr, COM_ptr);
  fclose(ASM_ptr);
  fclose(COM_ptr);
  return 0;
}

int no_whitespaces() {
  COM_ptr = fopen(COM_ASM, "r");
  IM_ptr = fopen(IM_ASM, "w+");
  WSP_ptr = fopen(WSP_ASM, "w");
  if (COM_ptr == NULL || WSP_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 3;
  }
  remove_whitespaces(COM_ptr, IM_ptr);
  rewind(IM_ptr);
  remove_whitespaces(IM_ptr, WSP_ptr);
  fclose(COM_ptr);
  fclose(WSP_ptr);
  return 0;
}

int first_pass() {
  char buff[1024] = {0};
  WSP_ptr = fopen(WSP_ASM, "r");
  if (WSP_ptr == NULL) {
    perror("Couldn't open the file\n");
    return 9;
  }
  while (fgets(buff, sizeof(buff), WSP_ptr)) {
    symblookup(buff);
  }
  fclose(WSP_ptr);
  return 0;
}

int translate() {
  char inst[17];
  char buff[256];
  WSP_ptr = fopen(WSP_ASM, "r");
  HACK_ptr = fopen(HACK_NAME, "a");
  if (WSP_ptr == NULL || HACK_ptr == NULL) {
    perror("Couldn't open the file\n");
    return 4;
  }
  while (fgets(buff, sizeof(buff), WSP_ptr)) {
    if (buff[0] != '(') {
      parser(buff);
      cnvrt(&ld, inst);       // fills inst
      writeb(HACK_ptr, inst); // sends inst to file
    }
  }
  fclose(WSP_ptr);
  fclose(HACK_ptr);
  return 0;
}

int writeb(FILE *dst, char ins[17]) {
  if (first_line) {
    fprintf(dst, "%s", ins);
    first_line = 0;
  } else {
    ins[16] = '\0';
    fprintf(dst, "\n%s", ins);
  }
  return 0;
}
