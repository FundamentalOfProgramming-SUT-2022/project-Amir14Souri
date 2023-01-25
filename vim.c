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
        char trash = 'a';
        trash = getchar();

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
        else if (strcmp(command, "grep") == 0)
        {
            char *files[50];
            char *key = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(10, sizeof(char));
            int options = 0, n = 0;

            // -file, -str and options
            for (int i = 0; i < 3; i++)
            {
                scanf("%s ", attribute);
                if (strcmp(attribute, "-c") == 0)
                {
                    options = 1;
                }
                else if (strcmp(attribute, "-l") == 0)
                {
                    options = 2;
                }
                else if (strcmp(attribute, "-str") == 0)
                {
                    if (i == 0)
                    {
                        i++;
                    }
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
                            *(key + i) = '\0';
                            break;
                        }

                        *(key + i) = c;
                    }
                    if (end == '"')
                    {
                        getchar();
                    }
                }
                else if (strcmp(attribute, "-files") == 0)
                {
                    char c = ' ';
                    for (int j = 0; c != '\n'; j++)
                    {
                        files[n] = (char *)calloc(100, sizeof(char));
                        c = getchar();
                        char end = (c == '"') ? '"' : ' ';
                        if (end == '"')
                        {
                            getchar();
                        }
                        for (int k = 0;; k++)
                        {
                            c = getchar();
                            if (c == end || c == '\n')
                            {
                                *(files[n] + k) = '\0';
                                n++;
                                break;
                            }
                            *(files[n] + k) = c;
                        }
                        if (end == '"')
                        {
                            getchar();
                        }
                    }
                }
            }

            // result
            int result = grep(files, n, key, options);
            if (result == 1)
            {
                printf("Some invalid path\n");
            }
            else if (result == 2)
            {
                printf("Some files doesn't exist\n");
            }
            else if (result == 3)
            {
                printf("Not found!\n");
            }
        }
        else if (strcmp(command, "undo") == 0)
        {
            char *attribute = (char *)calloc(10, sizeof(char));
            char *file = (char *)calloc(100, sizeof(char));

            scanf("%s ", attribute);
            int result = undo(inputPath());
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
                printf("No changes has been made to this file!\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "auto-indent") == 0)
        {
            char *attribute = (char *)calloc(10, sizeof(char));
            scanf("%s ", attribute);

            // result
            int result = autoIndent(inputPath());
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
                printf("Incorrect set of curly brackets!\n");
            }
            else
            {
                printf("Success\n");
            }
        }
        else if (strcmp(command, "compare") == 0)
        {
            char *file1 = (char *)calloc(100, sizeof(char));
            char *file2 = (char *)calloc(100, sizeof(char));
            char *attribute = (char *)calloc(100, sizeof(char));

            // -file1 attribute
            scanf("%s ", attribute);
            file1 = inputPath();

            scanf("%s ", attribute);
            file2 = inputPath();

            int result = compare(file1, file2);
            if (result == 1)
            {
                printf("Invalid path for 1st file\n");
            }
            else if (result == 2)
            {
                printf("1st file doesn't exist\n");
            }
            else if (result == 3)
            {
                printf("Invalid path for 2nd file\n");
            }
            else if (result == 4)
            {
                printf("2nd file doesn't exist\n");
            }
            else if (result == 5)
            {
                printf("2 files are the same\n");
            }
        }
        else if (strcmp(command, "exit") == 0)
        {
            FILE *clipboard = fopen("root/.clipboard.txt", "w");
            fclose(clipboard);
            break;
        }
        else
        {
            if (trash == '\n')
            {
                printf("Invalid command\n");
                continue;
            }
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

char *fileName(char *address)
{
    char *name = (char *)calloc(50, sizeof(char));
    int last = -1;
    for (int i = 0;; i++)
    {
        if (*(address + i) == '/')
        {
            last = i;
        }
        else if (*(address + i) == '\0')
        {
            break;
        }
    }
    for (int i = 0;; i++)
    {
        if (*(address + last + i + 1) == '\0')
        {
            *(name + i) = '\0';
            break;
        }

        *(name + i) = *(address + last + i + 1);
    }

    return name;
}

char *filePath(char *address)
{
    char *path = (char *)calloc(100, sizeof(char));
    int last = -1;
    for (int i = 0;; i++)
    {
        if (*(address + i) == '/')
        {
            last = i;
        }
        else if (*(address + i) == '\0')
        {
            break;
        }
    }
    for (int i = 0;; i++)
    {
        if (i == last)
        {
            *(path + i) = '/';
            break;
        }

        *(path + i) = *(address + i);
    }

    return path;
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

    backupFile(address);
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
    backupFile(address);
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

    backupFile(address);
    FILE *new = fopen(address, "w");
    fprintf(new, "%s", newContent);
    fclose(new);
    return 0;
}

int grep(char *addresses[], int n, char *key, int options)
{
    int matches[200][2], count = 0;
    for (int i = 0; i < 200; i++)
    {
        matches[i][0] = -1;
        matches[i][1] = -1;
    }
    char *content = (char *)calloc(1000, sizeof(char));
    for (int i = 0; i < n; i++)
    {
        if (checkPath(addresses[i]) == 1)
        {
            return 1;
        }
        FILE *file = fopen(addresses[i], "r");
        if (file == NULL)
        {
            return 2;
        }

        int k = 0, line = 0, found = 0;
        for (int j = 0;; j++)
        {
            char c = fgetc(file);
            if (c == EOF)
            {
                *(content + j) = '\0';
                break;
            }

            if (key[k] == '\0')
            {
                matches[count][0] = i;
                matches[count][1] = line;
                found = 1;
                k = 0;
                count++;
            }
            if (c == '\n')
            {
                found = 0;
                line++;
            }

            if (c == key[k] && found == 0)
            {
                k++;
            }
            else
            {
                k = 0;
            }

            *(content + j) = c;
        }

        if (options == 0)
        {
            k = 0;
            line = 0;
            for (int j = 0;; j++)
            {
                if (matches[j][0] == -1)
                {
                    break;
                }
                if (matches[j][0] != i)
                {
                    continue;
                }

                while (1)
                {
                    char c = *(content + k);
                    if (line == matches[j][1])
                    {
                        putc(c, stdout);
                        if (c == '\n')
                        {
                            k++;
                            line++;
                            break;
                        }
                    }

                    if (c == '\n')
                    {
                        line++;
                    }
                    k++;
                }
            }
        }

        fclose(file);
    }

    switch (options)
    {
    case 0:
        if (count == 0)
        {
            return 3;
        }
        return 0;
        break;
    case 1:
        printf("%d\n", count);
        return 0;
        break;
    case 2:
        if (count == 0)
        {
            return 3;
        }
        int last = -1;
        for (int i = 0; matches[i][0] != -1; i++)
        {
            if (matches[i][0] != last)
            {
                printf("/%s\n", addresses[++last]);
            }
        }
        return 0;
        break;
    }
}

void backupFile(char *address)
{
    char *content = (char *)calloc(1000, sizeof(char));
    FILE *file = fopen(address, "r");
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

    char *backupAddress = filePath(address);
    strcat(backupAddress, ".");
    strcat(backupAddress, fileName(address));
    FILE *backup = fopen(backupAddress, "w");
    fprintf(backup, "%s", content);
    fclose(backup);
}

int undo(char *address)
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
    fclose(file);

    char *backupAddress = filePath(address);
    strcat(backupAddress, ".");
    strcat(backupAddress, fileName(address));
    FILE *backup = fopen(backupAddress, "r");
    if (backup == NULL)
    {
        return 3;
    }

    char *backupContent = (char *)calloc(1000, sizeof(char));
    for (int i = 0;; i++)
    {
        char c = fgetc(backup);
        if (c == EOF)
        {
            *(backupContent + i) = '\0';
            break;
        }
        *(backupContent + i) = c;
    }
    fclose(backup);

    backupFile(address);
    FILE *new = fopen(address, "w");
    fprintf(new, "%s", backupContent);
    fclose(new);
    return 0;
}

int autoIndent(char *address)
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

    char *newContent = (char *)calloc(1000, sizeof(char));
    char *content = (char *)calloc(1000, sizeof(char));
    int space = 0, ok = 0, bracket = 0;
    for (int i = 0;; i++)
    {
        char c;
        c = fgetc(file);

        if (c == '{' || c == '}')
        {
            if (bracket == 1)
            {
                space = 0;
            }
            if (c == '{')
            {
                ok++;
            }
            else
            {
                ok--;
            }
            if (ok < 0)
            {
                return 3;
            }

            bracket = 1;
            i -= space;
            *(content + i) = c;
            space = 0;
            continue;
        }

        if (c == EOF)
        {
            *(content + i) = '\0';
            break;
        }
        else if (c == ' ' || c == '\n')
        {
            space++;
        }
        else
        {
            space = 0;
        }

        if (bracket == 1)
        {
            if (c == ' ' || c == '\n')
            {
                i--;
            }
            else
            {
                bracket = 0;
            }
        }
        if (bracket == 0)
        {
            *(content + i) = c;
        }
    }
    fclose(file);

    if (ok != 0)
    {
        return 3;
    }

    int indent = 0, tab = 0, future = 1, imediate = 0;
    bracket = 0;
    for (int i = 0;; i++)
    {
        char c = *(content + i);
        if (c == '{')
        {
            if (i != 0 && bracket == 0)
            {
                *(newContent + indent + i) = ' ';
                indent++;
            }
            *(newContent + indent + i) = c;
            indent++;
            tab++;
            *(newContent + indent + i) = '\n';
            bracket = 1;
            imediate = 0;
            c = '\n';
        }
        else if (c == '}')
        {
            if (imediate == 0)
            {
                *(newContent + indent + i) = '\n';
                indent++;
            }
            else
            {
                indent -= 4 * tab;
            }
            tab--;
            for (int j = 0; j < 4 * tab; j++)
            {
                *(newContent + indent + i) = ' ';
                indent++;
            }
            *(newContent + indent + i) = c;
            indent++;
            *(newContent + indent + i) = '\n';
            bracket = 1;
            imediate = 1;
            c = '\n';
        }
        else if (c == '\0')
        {
            imediate = 0;
            *(newContent + indent + i) = c;
            break;
        }
        else
        {
            imediate = 0;
            bracket = 0;
        }

        if (bracket == 0)
        {
            *(newContent + indent + i) = c;
        }

        if (c == '\n')
        {
            if (*(content + i + future) == '\n')
            {
                continue;
            }
            else if (*(content + i + future) == ' ')
            {
                future++;
                i--;
                continue;
            }

            indent++;
            for (int j = 0; j < 4 * tab; j++)
            {
                *(newContent + indent + i) = ' ';
                indent++;
            }
            indent--;
            i += future - 1;
            indent -= future - 1;
            future = 1;
        }
    }

    backupFile(address);
    FILE *new = fopen(address, "w");
    fprintf(new, "%s", newContent);
    fclose(new);
    return 0;
}

int compare(char *address1, char *address2)
{
    if (checkPath(address1) == 1)
    {
        return 1;
    }
    FILE *file1 = fopen(address1, "r");
    if (file1 == NULL)
    {
        return 2;
    }

    if (checkPath(address2) == 1)
    {
        return 3;
    }
    FILE *file2 = fopen(address2, "r");
    if (file2 == NULL)
    {
        return 4;
    }

    char *sentence1 = (char *)calloc(1000, sizeof(char));
    char *sentence2 = (char *)calloc(1000, sizeof(char));
    char *word1 = (char *)calloc(1000, sizeof(char));
    char *word2 = (char *)calloc(1000, sizeof(char));
    int count = 0, word = 0, line = 1, last = -1, lastLine = -1, modified = 0, saved = 0;
    char c1 = ' ', c2 = ' ';
    for (int i = 0;; i++)
    {
        if (c1 == ' ')
        {
            fscanf(file1, "%s", word1);
            c1 = fgetc(file1);
        }
        else
        {
            *word1 = '\0';
        }
        if (c2 == ' ')
        {
            fscanf(file2, "%s", word2);
            c2 = fgetc(file2);
        }
        else
        {
            *word2 = '\0';
        }

        if (strcmp(word1, word2) != 0)
        {
            last = word;
            count++;
        }
        if (c1 == EOF && c2 == EOF)
        {
            if (*sentence1 != '\0')
            {
                printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n%s", lastLine, line - 1, sentence1);
            }
            else if (*sentence2 != '\0')
            {
                printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n%s", lastLine, line - 1, sentence2);
            }
            break;
        }

        if (c1 == ' ')
        {
            strcat(word1, " ");
        }
        if (c2 == ' ')
        {
            strcat(word2, " ");
        }
        strcat(sentence1, word1);
        strcat(sentence2, word2);
        word++;

        if (c1 == '\n' && c2 == '\n')
        {
            if (count > 0)
            {
                if (count == 1)
                {
                    char *copy1 = (char *)calloc(1000, sizeof(char));
                    char *copy2 = (char *)calloc(1000, sizeof(char));
                    int space = 0, indent = 0;
                    for (int j = 0;; j++)
                    {
                        char c = *(sentence1 + j);

                        if (last == 0 && j == 0)
                        {
                            *copy1 = '>';
                            *(copy1 + 1) = '>';
                            *(copy1 + 2) = c;
                            indent = 2;
                            continue;
                        }
                        if (c == ' ' || c == '\0')
                        {
                            space++;
                            if (space == last)
                            {
                                *(copy1 + j) = ' ';
                                *(copy1 + j + 1) = '>';
                                *(copy1 + j + 2) = '>';
                                indent = 2;
                                continue;
                            }
                            if (space == last + 1)
                            {
                                *(copy1 + indent + j) = '<';
                                *(copy1 + indent + j + 1) = '<';
                                *(copy1 + indent + j + 2) = ' ';
                                indent = 4;
                                continue;
                            }
                        }
                        if (c == '\0')
                        {
                            *(copy1 + indent + j) = '\0';
                            break;
                        }

                        *(copy1 + indent + j) = c;
                    }

                    space = 0, indent = 0;
                    for (int j = 0;; j++)
                    {
                        char c = *(sentence2 + j);

                        if (last == 0 && j == 0)
                        {
                            *copy2 = '>';
                            *(copy2 + 1) = '>';
                            *(copy2 + 2) = c;
                            indent = 2;
                            continue;
                        }
                        if (c == ' ' || c == '\0')
                        {
                            space++;
                            if (space == last)
                            {
                                *(copy2 + j) = ' ';
                                *(copy2 + j + 1) = '>';
                                *(copy2 + j + 2) = '>';
                                indent = 2;
                                continue;
                            }
                            if (space == last + 1)
                            {
                                *(copy2 + indent + j) = '<';
                                *(copy2 + indent + j + 1) = '<';
                                *(copy2 + indent + j + 2) = ' ';
                                indent = 4;
                                continue;
                            }
                        }
                        if (c == '\0')
                        {
                            *(copy2 + indent + j) = '\0';
                            break;
                        }

                        *(copy2 + indent + j) = c;
                    }

                    printf("============ #%d ============\n%s\n%s\n", line, copy1, copy2);
                }
                else
                {
                    printf("============ #%d ============\n%s\n%s\n", line, sentence1, sentence2);
                }
                modified = 1;
            }
            c1 = ' ';
            c2 = ' ';
            *sentence1 = '\0';
            *sentence2 = '\0';
            *word1 = '\0';
            *word2 = '\0';
            last = -1;
            line++;
            count = 0;
            word = 0;
        }
        else if (c1 == '\n' && c2 == EOF)
        {
            c1 = ' ';
            strcat(sentence1, "\n");
            *word1 = '\0';
            if (saved == 0)
            {
                lastLine = line;
                saved = 1;
            }
            line++;
            count = 0;
            word = 0;
            modified = 1;
        }
        else if (c1 == EOF && c2 == '\n')
        {
            c2 = ' ';
            strcat(sentence2, "\n");
            *word2 = '\0';
            if (saved == 0)
            {
                lastLine = line;
                saved = 1;
            }
            line++;
            count = 0;
            word = 0;
            modified = 1;
        }
    }

    if (modified == 0)
    {
        return 5;
    }
    return 0;
}
