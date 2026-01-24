typedef struct {
  int value;
  char *dest;
  char *comp;
  char *jmp;
  char type;
} line_data;

extern line_data ld;
extern char inst[17];

void cnvrt(line_data *ld, char inst[17]);
void tobinary(int decimal);
void rvrs(int d);
