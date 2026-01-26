#include <stdio.h>

void name_init(char *ASM);
int no_comments();
int no_whitespaces();
int first_pass();
int translate();
int writeb(FILE *dst, char ins[17]);
