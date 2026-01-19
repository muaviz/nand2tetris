#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_comments(FILE *src, FILE *dest);
void remove_whitespaces(FILE *src, FILE *dest);
void classifier(FILE *src);
