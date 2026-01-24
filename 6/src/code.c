#include "code.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

line_data ld;
char bufferC[17] = {0};
char bufferA[17] = {0};

typedef struct {
  char *mnemonic;
  char *bits;
} table;

table dst[8] = {
    {"null", "000"}, {"M", "001"},  {"D", "010"},  {"MD", "011"},
    {"A", "100"},    {"AM", "101"}, {"AD", "110"}, {"AMD", "111"},
};
table jmp[8] = {
    {"null", "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
    {"JLT", "100"},  {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"},
};
table comp[28] = {
    {"0", "0101010"},   {"1", "0111111"},   {"-1", "0111010"},
    {"D", "0001100"},   {"A", "0110000"},   {"!D", "0001101"},
    {"!A", "0110001"},  {"-D", "0001111"},  {"-A", "0110011"},
    {"D+1", "0011111"}, {"A+1", "0110111"}, {"D-1", "0001110"},
    {"A-1", "0110010"}, {"D+A", "0000010"}, {"D-A", "0010011"},
    {"A-D", "0000111"}, {"D&A", "0000000"}, {"D|A", "0010101"},
    {"M", "1110000"},   {"!M", "1110001"},  {"-M", "1110011"},
    {"M+1", "1110111"}, {"M-1", "1110010"}, {"D+M", "1000010"},
    {"D-M", "1010011"}, {"M-D", "1000111"}, {"D&M", "1000000"},
    {"D|M", "1010101"},
};

void cnvrt(line_data *ld, char inst[17]) {
  if (ld->type == 'C') {
    strcpy(bufferC, "111");
    for (int i = 0; i < 28; i++) {
      if (strcmp(comp[i].mnemonic, ld->comp) == 0) {
        strcat(bufferC, comp[i].bits);
      }
    }
    for (int i = 0; i < 8; i++) {
      if (strcmp(dst[i].mnemonic, ld->dest) == 0) {
        strcat(bufferC, dst[i].bits);
        // printf("%s\n", bufferC);
      }
    }
    for (int i = 0; i < 8; i++) {
      if (strcmp(jmp[i].mnemonic, ld->jmp) == 0) {
        strcat(bufferC, jmp[i].bits);
      }
    }
    strcpy(inst, bufferC);
  } else if (ld->type == 'A') {
    strcpy(inst, "0");
    tobinary(ld->value);
    strcat(inst, bufferA);
  }
}

void tobinary(int dec) {
  int mask = 0b100000000000000;
  for (int i = 0; i < 15; i++) {
    bufferA[i] = (dec & mask) ? '1' : '0';
    mask >>= 1;
  }
}
