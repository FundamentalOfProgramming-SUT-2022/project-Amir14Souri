#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "prototypes.h"

int main()
{
    inputAndCallCommand();

    return 0;
}

void inputAndCallCommand()
{
    while (1)
    {
        char *command = (char *)calloc(10, sizeof(char));
        scanf("%s", command);

        if (strcmp(command, "newfile") == 0)
        {
            if (newFile(inputPath()) == 1)
            {
                printf("This file already exists\n");
            }
        }
        else if (strcmp(command, "insert") == 0)
        {
            int line, pos;
            char *attribute = (char *)calloc(10, sizeof(char));
            char *file = (char *)calloc(100, sizeof(char));
            char *data = (char *)calloc(1000, sizeof(char));

            // -file attribute
            file = inputPath();

            // -str attribute
            scanf("%s ", attribute);
            char c = getchar();
            char end = (c == '"') ? '"' : ' ';
            for (int i = 0;; i++)
            {
                if (i != 0 || end == '"')
                {
                    c = getchar();
                }
                if (c == '\\')
                {
                    c = getchar();
                    switch (c)
                    {
                    case 'n':
                        c = '\n';
                        break;
                    case '\\':
                        c = '\\';
                        break;
                    case '"':
                        c = '\"';
                        break;
                    }
                }
                else if (c == end)
                {
                    *(data + i) = '\0';
                    break;
                }

                *(data + i) = c;
            }
            if (end == '"')
            {
                getchar();
            }

            // -pos attribute
            scanf("%s %d:%d", attribute, &line, &pos);
            getchar();

            int result = insert(file, data, line, pos);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
        }
        else if (strcmp(command, "cat") == 0)
        {
            int result = cat(inputPath());
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
        }
        else if (strcmp(command, "remove") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos, length;
            char direction;
            file = inputPath();

            // -pos attribute
            scanf("%s %d:%d", attribute, &line, &pos);
            getchar();

            // -size attribute
            scanf("%s %d", attribute, &length);
            getchar();

            // direction flag
            scanf("-%c", &direction);
            getchar();

            int result = removeString(file, line, pos, length, direction);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
        }
        else if (strcmp(command, "copy") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos, length;
            char direction;
            file = inputPath();

            // -pos attribute
            scanf("%s %d:%d", attribute, &line, &pos);
            getchar();

            // -size attribute
            scanf("%s %d", attribute, &length);
            getchar();

            // direction flag
            scanf("-%c", &direction);
            getchar();

            int result = copy(file, line, pos, length, direction);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
        }
        else if (strcmp(command, "cut") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos, length;
            char direction;
            file = inputPath();

            // -pos attribute
            scanf("%s %d:%d", attribute, &line, &pos);
            getchar();

            // -size attribute
            scanf("%s %d", attribute, &length);
            getchar();

            // direction flag
            scanf("-%c", &direction);
            getchar();

            int result = cut(file, line, pos, length, direction);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
        }
        else if (strcmp(command, "paste") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos;
            file = inputPath();

            // -pos attribute
            scanf("%s %d:%d", attribute, &line, &pos);

            int result = paste(file, line, pos);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else
        {
            while (1)
            {
                if (getchar() == '\n')
                {
                    break;
                }
            }
            printf("Invalid command\n");
            continue;
        }
    }
}

char *inputPath()
{
    char *file = (char *)calloc(100, sizeof(char));
    char *attribute = (char *)calloc(10, sizeof(char));
    scanf("%s ", attribute);
    char c = getchar();
    char end = (c == '"') ? '"' : ' ';
    if (end == '"')
    {
        getchar();
    }
    for (int i = 0;; i++)
    {
        c = getchar();
        if (c == end || c == '\n')
        {
            *(file + i) = '\0';
            break;
        }
        *(file + i) = c;
    }
    return file;
}

int newFile(char *address)
{
    char *path = (char *)calloc(100, sizeof(char));
    for (int i = 0;; i++)
    {
        if (address[i] == '/')
        {
            mkdir(path, 0777);
        }
        else if (address[i] == '\0')
        {
            path[i] = '\0';
            break;
        }
        path[i] = address[i];
    }

    FILE *exist = fopen(path, "r");
    if (exist != NULL)
    {
        fclose(exist);
        return 1;
    }
    else
    {
        FILE *file = fopen(path, "w");
        fclose(file);
        return 0;
    }
}

int checkPath(char *address)
{
    char *path = (char *)calloc(strlen(address), sizeof(char));
    for (int i = 0;; i++)
    {
        if (address[i] == '/')
        {
            if (mkdir(path, 0777) == 0)
            {
                rmdir(path);
                return 1;
            }
        }
        else if (address[i] == '\0')
        {
            return 0;
        }

        path[i] = address[i];
    }
}

int insert(char *address, char *string, int line, int pos)
{
    if (checkPath(address) == 1)
    {
        return 1;
    }
    FILE *file = fopen(address, "r");
    if (file == NULL)
    {
        return 2;
    }

    char *content = (char *)calloc(1000, sizeof(char));
    int curLine = 0;
    for (int i = -1;; i++)
    {
        char c;
        if (i == -1)
        {
            c = '\n';
        }
        else
        {
            c = fgetc(file);
        }

        if (c == '\n' && ++curLine == line)
        {
            *(content + i) = c;
            i++;
            for (int j = 0; j < pos; j++)
            {
                c = fgetc(file);
                if (c == '\n' || c == EOF)
                {
                    int nSpace = pos - j;
                    for (int k = 0; k < nSpace; k++)
                    {
                        *(content + i) = ' ';
                        i++;
                        j++;
                    }
                }
                else
                {
                    *(content + i) = c;
                    i++;
                }
            }
            *(content + i) = '\0';
            strcat(content, string);
            i += strlen(string) - 1;
        }
        else if (c == EOF && line > curLine)
        {
            for (int j = 0; j < line - curLine; j++)
            {
                *(content + i) = '\n';
                i++;
            }
            for (int j = 0; j < pos; j++)
            {
                *(content + i) = ' ';
                i++;
            }
            *(content + i) = '\0';
            strcat(content, string);
            i += strlen(string);
            *(content + i) = '\n';
            break;
        }
        else if (c == EOF)
        {
            if (*(content + i - 1) != '\n')
            {
                *(content + i) = '\n';
            }
            break;
        }
        else
        {
            *(content + i) = c;
        }
    }
    fclose(file);
    FILE *new = fopen(address, "w");
    fprintf(file, "%s", content);
    fclose(new);
    return 0;
}

int cat(char *address)
{
    if (checkPath(address) == 1)
    {
        return 1;
    }
    FILE *file = fopen(address, "r");
    if (file == NULL)
    {
        return 2;
    }

    char *content = (char *)calloc(1000, sizeof(char));
    for (int i = 0;; i++)
    {
        char c = fgetc(file);
        if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }

        *(content + i) = c;
    }
    fclose(file);
    printf("%s", content);
    return 0;
}

int removeString(char *address, int line, int pos, int length, char direction)
{
    if (checkPath(address) == 1)
    {
        return 1;
    }
    FILE *file = fopen(address, "r");
    if (file == NULL)
    {
        return 2;
    }

    char *content = (char *)calloc(1000, sizeof(char));
    char *newContent = (char *)calloc(1000, sizeof(char));
    int curLine = 1, curPos = 0, start = 0, indent = 0;
    for (int i = 0;; i++)
    {
        if (curLine == line && curPos == pos)
        {
            start = i;
        }

        char c = fgetc(file);
        if (c == '\n')
        {
            curLine++;
            curPos = -1;
        }
        else if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }

        curPos++;
        *(content + i) = c;
    }
    fclose(file);

    if (direction == 'b')
    {
        start -= length;
    }

    for (int i = 0;; i++)
    {
        if (i == start)
        {
            indent = length;
        }

        if (*(content + i + indent) == '\0')
        {
            *(newContent + i) = '\0';
            break;
        }

        *(newContent + i) = *(content + i + indent);
    }
    FILE *new = fopen(address, "w");
    fprintf(new, "%s", newContent);
    fclose(new);
    return 0;
}

int copy(char *address, int line, int pos, int length, char direction)
{
    if (checkPath(address) == 1)
    {
        return 1;
    }
    FILE *file = fopen(address, "r");
    if (file == NULL)
    {
        return 2;
    }

    char *content = (char *)calloc(1000, sizeof(char));
    char *target = (char *)calloc(length, sizeof(char));
    int curLine = 1, curPos = 0, start = 0;
    for (int i = 0;; i++)
    {
        if (curLine == line && curPos == pos)
        {
            start = i;
        }

        char c = fgetc(file);
        if (c == '\n')
        {
            curLine++;
            curPos = -1;
        }
        else if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }

        curPos++;
        *(content + i) = c;
    }
    fclose(file);

    if (direction == 'b')
    {
        start -= length;
    }

    for (int i = 0;; i++)
    {
        if (i == length)
        {
            *(target + i) = '\0';
            break;
        }
        *(target + i) = *(content + i + start);
    }

    FILE *clipboard = fopen("clipboard.txt", "w");
    fprintf(clipboard, "%s", target);
    fclose(clipboard);
    return 0;
}

int cut(char *address, int line, int pos, int length, char direction)
{
    int copied = copy(address, line, pos, length, direction);
    if (copied == 1)
    {
        return 1;
    }
    else if (copied == 2)
    {
        return 2;
    }
    int removed = removeString(address, line, pos, length, direction);
    if (removed == 1)
    {
        return 1;
    }
    else if (removed == 2)
    {
        return 2;
    }
    return 0;
}

int paste(char *address, int line, int pos)
{
    FILE *clipboard = fopen("clipboard.txt", "r");
    char *content = (char *)calloc(1000, sizeof(char));
    for (int i = 0;; i++)
    {
        char c = fgetc(clipboard);
        if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }

        *(content + i) = c;
    }
    fclose(clipboard);

    return insert(address, content, line, pos);
}