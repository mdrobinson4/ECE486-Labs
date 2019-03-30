typedef struct memory MEMORY;
MEMORY *newMEMORY(int count);
void setCommand(MEMORY *m, char command, int index);
MEMORY *readFile(FILE *fp);

char getAssociativity();
char getReplacementPolicy();
FILE *getFile();
