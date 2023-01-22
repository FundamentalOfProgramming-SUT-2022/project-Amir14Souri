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
        getchar();

        if (strcmp(command, "newfile") == 0)
        {
            char *attribute = (char *)calloc(10, sizeof(char));
            scanf("%s ", attribute);

            // result
            if (newFile(inputPath()) == 1)
            {
                printf("This file already exists\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "insert") == 0)
        {
            int line, pos;
            char *attribute = (char *)calloc(10, sizeof(char));
            char *file = (char *)calloc(100, sizeof(char));
            char *data = (char *)calloc(1000, sizeof(char));

            // -file attribute
            scanf("%s ", attribute);
            file = inputPath();
            getchar();

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
                else if (c == end || c == '\n')
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
            scanf("%s ", attribute);
            scanf("%d:%d", &line, &pos);
            getchar();

            // result
            int result = insert(file, data, line, pos);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "cat") == 0)
        {

            char *attribute = (char *)calloc(10, sizeof(char));
            scanf("%s ", attribute);

            // result
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

            // -file attribute
            scanf("%s ", attribute);
            file = inputPath();

            // -pos attribute
            scanf("%s ", attribute);
            scanf("%d:%d", &line, &pos);
            getchar();

            // -size attribute
            scanf("%s ", attribute);
            scanf("%d", &length);
            getchar();

            // -f or -b flag
            scanf("%s", attribute);
            direction = attribute[1];
            getchar();

            // result
            int result = removeString(file, line, pos, length, direction);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "copy") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos, length;
            char direction;

            // -file attribute
            scanf("%s ", attribute);
            file = inputPath();

            // -pos attribute
            scanf("%s ", attribute);
            scanf("%d:%d", &line, &pos);
            getchar();

            // -size attribute
            scanf("%s ", attribute);
            scanf("%d", &length);
            getchar();

            // -f or -b flag
            scanf("%s", attribute);
            direction = attribute[1];
            getchar();

            // result
            int result = copy(file, line, pos, length, direction);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "cut") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos, length;
            char direction;

            // -file attribute
            scanf("%s ", attribute);
            file = inputPath();

            // -pos attribute
            scanf("%s ", attribute);
            scanf("%d:%d", &line, &pos);
            getchar();

            // -size attribute
            scanf("%s ", attribute);
            scanf("%d", &length);
            getchar();

            // -f or -b flag
            scanf("%s", attribute);
            direction = attribute[1];
            getchar();

            // result
            int result = cut(file, line, pos, length, direction);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "paste") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int line, pos;

            // -file attribute
            scanf("%s ", attribute);
            file = inputPath();

            // -pos attribute
            scanf("%s ", attribute);
            scanf("%d:%d", &line, &pos);
            getchar();

            // result
            int result = paste(file, line, pos);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "find") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *key = (char *)calloc(1000, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int options = 0, at = 1;

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
                    case '*':
                        *(key + i) = '\\';
                        c = '*';
                        i++;
                        break;
                    }
                }
                else if (c == end || c == '\n')
                {
                    *(key + i) = '\0';
                    break;
                }

                *(key + i) = c;
            }
            if (end == '"')
            {
                getchar();
            }

            // -file attribute
            scanf("%s ", attribute);
            c = getchar();
            end = (c == '"') ? '"' : ' ';
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
            if (end == '"')
            {
                c = getchar();
            }

            // possible options
            for (int i = 0; i < 4; i++)
            {
                if (c == ' ')
                {
                    scanf("%s", attribute);
                    if (strcmp(attribute, "-count") == 0)
                    {
                        options += 8;
                        c = getchar();
                    }
                    else if (strcmp(attribute, "-at") == 0)
                    {
                        options += 4;
                        scanf(" %d", &at);
                        c = getchar();
                    }
                    else if (strcmp(attribute, "-byword") == 0)
                    {
                        options += 2;
                        c = getchar();
                    }
                    else if (strcmp(attribute, "-all") == 0)
                    {
                        options++;
                        c = getchar();
                    }
                }
                else
                {
                    break;
                }
            }

            // result
            int result = find(file, key, options, at);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else if (result == 3)
            {
                printf("Invalid set of options!\n");
            }
        }
        else if (strcmp(command, "replace") == 0)
        {
            char *file = (char *)calloc(100, sizeof(char));
            char *key = (char *)calloc(1000, sizeof(char));
            char *alternate = (char *)calloc(1000, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int options = 0, at = 1;

            // -str1 (key) attribute
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
                    case '*':
                        *(key + i) = '\\';
                        c = '*';
                        i++;
                        break;
                    }
                }
                else if (c == end || c == '\n')
                {
                    *(key + i) = '\0';
                    break;
                }

                *(key + i) = c;
            }
            if (end == '"')
            {
                getchar();
            }

            // -str2 (alternate) attribute
            scanf("%s ", attribute);
            c = getchar();
            end = (c == '"') ? '"' : ' ';
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
                else if (c == end || c == '\n')
                {
                    *(alternate + i) = '\0';
                    break;
                }

                *(alternate + i) = c;
            }
            if (end == '"')
            {
                getchar();
            }

            // -file attribute
            scanf("%s ", attribute);
            c = getchar();
            end = (c == '"') ? '"' : ' ';
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
            if (end == '"')
            {
                c = getchar();
            }

            // possible options
            for (int i = 0; i < 2; i++)
            {
                if (c == ' ')
                {
                    scanf("%s", attribute);
                    if (strcmp(attribute, "-at") == 0)
                    {
                        options += 2;
                        scanf(" %d", &at);
                        c = getchar();
                    }
                    else if (strcmp(attribute, "-all") == 0)
                    {
                        options++;
                        c = getchar();
                    }
                }
                else
                {
                    break;
                }
            }

            // result
            int result = replace(file, key, alternate, options, at);
            if (result == 1)
            {
                printf("Invalid path\n");
            }
            else if (result == 2)
            {
                printf("File doesn't exist\n");
            }
            else if (result == 3)
            {
                printf("Invalid set of options!\n");
            }
            else if (result == 4)
            {
                printf("Not found!\n");
            }
            else
            {
                printf("Success\n");
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
    if (end == '"')
    {
        getchar();
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

    FILE *clipboard = fopen("root/.clipboard.txt", "w");
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
    FILE *clipboard = fopen("root/.clipboard.txt", "r");
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

int find(char *address, char *key, int options, int at)
{
    switch (options)
    {
    case 5:
    case 7:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return 3;
        break;
    }

    if (checkPath(address) == 1)
    {
        return 1;
    }
    FILE *file = fopen(address, "r");
    if (file == NULL)
    {
        return 2;
    }

    int startA[100][2];
    char *content = (char *)calloc(1000, sizeof(char));
    for (int i = 0; i < 100; i++)
    {
        startA[i][0] = -1;
        startA[i][1] = -1;
    }
    int j = 0, start = -1, last = -1, n = 0, word = 0, startWord = -1;
    int star = 0, endStar = 0, firstStar = 0, realStar = 0;
    for (int i = 0;; i++)
    {
        char c;
        if (*(content + i) == 0)
        {
            c = fgetc(file);
            if (c == ' ' || c == '\n')
            {
                word++;
            }
        }
        else
        {
            c = *(content + i);
        }
        if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }
        *(content + i) = c;

        if (key[j] == '\\')
        {
            if (key[++j] == '*')
            {
                realStar = 1;
                if (j == 1)
                {
                    start = i;
                    startWord = word;
                }
            }
        }
        if (key[j] == '\0' && endStar == 0)
        {
            startA[n][0] = start;
            startA[n][1] = startWord;
            star = 0;
            endStar = 0;
            firstStar = 0;
            realStar = 0;
            last = -1;
            j = 0;
            start = -1;
            startWord = -1;
            n++;
        }
        if (key[j] == '*' && realStar == 0)
        {
            star = 1;
            j++;
            if (key[j] == '\0')
            {
                endStar = 1;
            }
            else
            {
                firstStar = 1;
                if (j == 1)
                {
                    start = i;
                    startWord = word;
                }
            }
        }

        if (c != key[j] && j > 0 && star == 0)
        {
            last = -1;
            start = -1;
            startWord = -1;
            j = 0;
            endStar = 0;
            firstStar = 0;
            realStar = 0;
            star = 0;
        }
        if (firstStar == 1 && last == -1 && c != key[j] && (c == '\n' || c == ' '))
        {
            last = -1;
            start = -1;
            startWord = -1;
            j = 0;
            endStar = 0;
            firstStar = 0;
            realStar = 0;
            star = 0;
        }

        if (c == key[j] || star == 1)
        {
            if (j == 0)
            {
                start = i;
                startWord = word;
            }

            if (firstStar == 1 && c == key[j])
            {
                last = i;
            }

            if (star == 0)
            {
                j++;
                if (realStar == 1)
                {
                    realStar = 0;
                }
            }

            if (firstStar == 1 && (c == ' ' || c == '\n') && (last != -1 || key[j] == ' ' || key[j] == '\n'))
            {
                j++;
                star = 0;
                firstStar = 0;
                endStar = 0;
                realStar = 0;
                i = last;
                last = -1;
            }
            if (endStar == 1 && (c == ' ' || c == '\n'))
            {
                startA[n][0] = start;
                startA[n][1] = startWord;
                last = -1;
                j = 0;
                start = -1;
                startWord = -1;
                star = 0;
                firstStar = 0;
                endStar = 0;
                realStar = 0;
                n++;
            }
        }
    }
    fclose(file);

    at--;
    switch (options)
    {
    case 0:
    case 2:
    case 4:
    case 6:
        if (startA[at][0] == -1)
        {
            printf("Not found!\n");
        }
        else
        {
            printFind(startA, at, (options / 2) % 2);
        }
        break;
    case 1:
    case 3:
        if (startA[0][0] == -1)
        {
            printf("Not found!\n");
        }
        else
        {
            for (int i = 0; startA[i][0] != -1; i++)
            {
                printFind(startA, i, (options / 2) % 2);
            }
        }
        break;
    case 8:
    case 10:
        int count = 0;
        for (int i = 0; startA[i][0] != -1; i++)
        {
            count++;
        }
        printf("Number of matches: %d\n", count);
        break;
    }

    return 0;
}

void printFind(int startA[][2], int i, int by)
{
    char *byStr = (char *)calloc(8, sizeof(char));
    if (by == 0)
    {
        byStr = "by char";
    }
    else
    {
        byStr = "by word";
    }
    printf("match %2d (%s): %3d\n", i + 1, byStr, startA[i][by]);
}

int replace(char *address, char *key, char *alternate, int options, int at)
{
    if (options == 3)
    {
        return 3;
    }

    if (checkPath(address) == 1)
    {
        return 1;
    }
    FILE *file = fopen(address, "r");
    if (file == NULL)
    {
        return 2;
    }

    int interval[100][2];
    char *content = (char *)calloc(1000, sizeof(char));
    for (int i = 0; i < 100; i++)
    {
        interval[i][0] = -1;
        interval[i][1] = -1;
    }
    int j = 0, start = -1, last = -1, n = 0, line = 1, pos = 0;
    int star = 0, endStar = 0, firstStar = 0, realStar = 0;
    for (int i = 0;; i++)
    {
        char c;
        if (*(content + i) == 0)
        {
            c = fgetc(file);
        }
        else
        {
            c = *(content + i);
        }
        if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }
        *(content + i) = c;

        if (key[j] == '\\')
        {
            if (key[++j] == '*')
            {
                realStar = 1;
                if (j == 1)
                {
                    start = i;
                }
            }
        }
        if (key[j] == '\0' && endStar == 0)
        {
            interval[n][0] = start;
            interval[n][1] = i - 1;
            star = 0;
            endStar = 0;
            firstStar = 0;
            realStar = 0;
            last = -1;
            j = 0;
            start = -1;
            n++;
        }
        if (key[j] == '*' && realStar == 0)
        {
            star = 1;
            j++;
            if (key[j] == '\0')
            {
                endStar = 1;
            }
            else
            {
                firstStar = 1;
                if (j == 1)
                {
                    start = i;
                }
            }
        }

        if (c != key[j] && j > 0 && star == 0)
        {
            last = -1;
            start = -1;
            j = 0;
            endStar = 0;
            firstStar = 0;
            realStar = 0;
            star = 0;
        }
        if (firstStar == 1 && last == -1 && c != key[j] && (c == '\n' || c == ' '))
        {
            last = -1;
            start = -1;
            j = 0;
            endStar = 0;
            firstStar = 0;
            realStar = 0;
            star = 0;
        }

        if (c == key[j] || star == 1)
        {
            if (j == 0)
            {
                start = i;
            }

            if (firstStar == 1 && c == key[j])
            {
                last = i;
            }

            if (star == 0)
            {
                j++;
                if (realStar == 1)
                {
                    realStar = 0;
                }
            }

            if (firstStar == 1 && (c == ' ' || c == '\n') && (last != -1 || key[j] == ' ' || key[j] == '\n'))
            {
                j++;
                star = 0;
                firstStar = 0;
                endStar = 0;
                realStar = 0;
                i = last;
                last = -1;
            }
            if (endStar == 1 && (c == ' ' || c == '\n'))
            {
                interval[n][0] = start;
                interval[n][1] = i - 1;
                last = -1;
                j = 0;
                start = -1;
                star = 0;
                firstStar = 0;
                endStar = 0;
                realStar = 0;
                n++;
            }
        }
    }
    fclose(file);

    at--;
    char *newContent = (char *)calloc(1000, sizeof(char));
    switch (options)
    {
    case 0:
    case 2:
        if (interval[at][0] == -1)
        {
            return 4;
        }
        else
        {
            for (int i = 0; i < interval[at][0]; i++)
            {
                *(newContent + i) = *(content + i);
            }
            *(newContent + interval[at][0]) = '\0';
            strcat(newContent, alternate);
            int indent = strlen(alternate) - interval[at][1] + interval[at][0] - 1;
            for (int i = interval[at][1] + 1;; i++)
            {
                if (*(content + i) == '\0')
                {
                    *(newContent + indent + i) = '\0';
                    break;
                }
                *(newContent + indent + i) = *(content + i);
            }
        }
        break;
    case 1:
        if (interval[0][0] == -1)
        {
            return 4;
        }
        else
        {
            int j = 0, indent = 0;
            for (int i = 0;; i++)
            {
                if (i == interval[j][0])
                {
                    *(newContent + indent + i) = '\0';
                    strcat(newContent, alternate);
                    i += interval[j][1] - interval[j][0] + 1;
                    indent += strlen(alternate) - interval[j][1] + interval[j][0] - 1;
                    j++;
                }
                if (*(content + i) == '\0')
                {
                    *(newContent + indent + i) = '\0';
                    break;
                }

                *(newContent + indent + i) = *(content + i);
            }
        }
        break;
    }

    FILE *new = fopen(address, "w");
    fprintf(new, "%s", newContent);
    fclose(new);
    return 0;
}