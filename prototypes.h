#define FILE_MAX_CHAR 10000
#define INPUT_MAX_CHAR 200
#define ATTR_MAX_CHAR 15
#define BG_COLOR 1
#define BAR_COLOR 2
#define MODE_COLOR 3
#define FILE_NAME_COLOR 4
#define LINE_NO_COLOR 5

int inputAndCallCommand();
char *inputPath(char *lastChar, char *input, int *loc);
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
void initWindow(WINDOW *bar);
char scan(char *text, char *input, int *loc);
int scanNumber(char *last, char *input, int *loc);
void clearBar();
void clearLine(int line);
void clearScreen();
void initLine(int no);