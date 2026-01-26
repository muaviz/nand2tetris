#include "symb.h"
#include <stdio.h>
#include <string.h>

int length = 23;
int line_addr = 0;

stable symbtab[1024] = {
    {"R0", 0},   {"R1", 1},         {"R2", 2},      {"R3", 3},   {"R4", 4},
    {"R5", 5},   {"R6", 6},         {"R7", 7},      {"R8", 8},   {"R9", 9},
    {"R10", 10}, {"R11", 11},       {"R12", 12},    {"R13", 13}, {"R14", 14},
    {"R15", 15}, {"SP", 0},         {"LCL", 1},     {"ARG", 2},  {"THIS", 3},
    {"THAT", 4}, {"SCREEN", 16384}, {"KBD", 24576},
};

void symblookup(char *buff) {
  char label[128] = {0};
  if (buff[0] == '(') {
    buff += 1;
    char *tok = strtok(buff, ")");
    strcat(label, tok);
    addtotable(label, line_addr);
    line_addr--;
  }
  line_addr++;
}

void addtotable(char *lbl, int line_addr) {
  strcpy(symbtab[length].symbol, lbl);
  symbtab[length].addr = line_addr;
  length++;
}
