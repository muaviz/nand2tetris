#include "preprocessor.h"
#include "string.h"

char line[256];

void remove_comments(FILE *src, FILE *dest) {
  while (fgets(line, sizeof(line), src)) {
    char *com_pos = strstr(line, "//");
    if (com_pos != NULL) {
      long len = com_pos - line;
      fprintf(dest, "%.*s\n", (int)len, line);
    } else {
      fprintf(dest, "%s\n", line);
    }
  }
}

void remove_whitespaces(FILE *src, FILE *dest) {
  while (fgets(line, sizeof(line), src)) {
    if (line[0] == '\n' || line[0] == '\r') {
      continue;
    }
    for (int i = 0; line[i] != '\0'; i++) {
      if (line[i] != ' ' && line[i] != '\t') {
        fputc(line[i], dest);
      }
    }
  }
}
