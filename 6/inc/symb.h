typedef struct {
  char symbol[128];
  int addr;
} stable;

extern stable symbtab[1024];
extern int length;

void symblookup(char *buff);
void addtotable(char *lbl, int line);
