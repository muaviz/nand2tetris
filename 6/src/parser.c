#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser(char *buff) {
  char *dest;
  char *comp;
  char *jmp;
  if (buff[0] == '@') {
    int value = atoi(buff + 1);
    printf("%d\n", value);
  } else {
    if (buff[1] == '=') {
      dest = strtok(buff, "=");
      comp = strtok(NULL, ";");
      jmp = strtok(NULL, ";");
    } else {
      dest = NULL;
      comp = strtok(buff, ";");
      jmp = strtok(NULL, ";");
    }
  }
  if (jmp == NULL) {
    comp[strcspn(comp, "\n")] = '\0';
  } else {
    jmp[strcspn(jmp, "\n")] = '\0';
  }
  printf("%s", dest);
  printf("%s", comp);
  printf("%s", jmp);
}
