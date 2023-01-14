typedef struct character Char;
void inputAndCallCommand();
int newFile(char *address);
int insert(char *address, char *string, int line, int pos);
Char *startString(char c);
Char *addChar(Char *node, char c);
Char *removeChar(Char *first, Char *node);
Char *endString(Char *node);
void structToString(Char *first, char *string);