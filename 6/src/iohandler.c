#include "iohandler.h"

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
void name_init(char *ASM) {
  ASM_NAME = ASM;
  COM_ASM = "nocomments.asm";
  WSP_ASM = "nowhitespaces.asm";

  // strcpy(ASM_NAME, ASM);
  // strcpy(COM_ASM, "nocomments.asm");
  // strcpy(WSP_ASM, "nowhitespaces.asm");

  char *base = strdup(ASM_NAME);
  char *token = strtok(base, ".");
  if (token != NULL) {
    // strcpy(HACK_NAME, strcat(token, ".hack"));
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
  WSP_ptr = fopen(WSP_ASM, "w");
  if (COM_ptr == NULL || WSP_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 3;
  }
  remove_whitespaces(COM_ptr, WSP_ptr);
  fclose(COM_ptr);
  fclose(WSP_ptr);
  return 0;
}

int convert_binary() {
  HACK_ptr = fopen(HACK_NAME, "w");
  if (HACK_ptr == NULL) {
    perror("Couldn't open the file!\n");
    return 4;
  }
  fputs("INCOMPLETE!!!\n", HACK_ptr);
  fclose(HACK_ptr);
  return 0;
}
