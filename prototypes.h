#define FILE_MAX_CHAR 10000
#define INPUT_MAX_CHAR 200
#define ATTR_MAX_CHAR 10

void inputAndCallCommand();
char *inputPath(char *lastChar);
char *fileName(char *address);
char *filePath(char *address);
int newFile(char *address);
int checkPath(char *address);
int insert(char *address, char *string, int line, int pos);
int cat(char *address, FILE *where);
int removeString(char *address, int line, int pos, int length, char direction);
int copy(char *address, int line, int pos, int length, char direction);
int cut(char *address, int line, int pos, int length, char direction);
int paste(char *address, int line, int pos);
int find(char *address, char *key, int options, int at, FILE *where);
void printFind(int startA[][2], int i, int by, FILE *where);
int replace(char *address, char *key, char *alternate, int options, int at);
int grep(char *addresses[], int n, char *key, int options, FILE *where);
void backupFile(char *address);
int undo(char *address);
int autoIndent(char *address);
int compare(char *address1, char *address2, FILE *where);
int tree(char *dir, int depth, int limit, int history[], FILE *where);
void indentTree(int depth, int history[], FILE *where);
char *readPipe();