#include "parser.h"
#include "code.h"
#include "iohandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser(char *buff) {
  ld.dest = NULL;
  ld.comp = NULL;
  ld.jmp = NULL;
  if (buff[0] == '@') {
    ld.type = 'A';
    ld.value = atoi(buff + 1);
  } else {
    ld.type = 'C';
    if (buff[1] == '=' || buff[2] == '=' || buff[3] == '=') {
      ld.dest = strtok(buff, "=");
      ld.comp = strtok(NULL, ";");
      ld.jmp = strtok(NULL, ";");
    } else {
      ld.dest = NULL;
      ld.comp = strtok(buff, ";");
      ld.jmp = strtok(NULL, ";");
    }
  }
  if (ld.dest) {
    ld.dest[strcspn(ld.dest, "\r\n")] = '\0';
  }
  if (ld.comp)
    ld.comp[strcspn(ld.comp, "\r\n")] = '\0';
  if (ld.jmp)
    ld.jmp[strcspn(ld.jmp, "\r\n")] = '\0';

  if (ld.dest == NULL) {
    ld.dest = "null";
  }
  if (ld.jmp == NULL) {
    ld.jmp = "null";
  }
}
