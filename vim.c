#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
void inputAndCallCommand();
int newFile(char *address);

int main()
{
   inputAndCallCommand();

    return 0;
}

void inputAndCallCommand()
{
    while (1)
    {
        char *command = (char *)malloc(sizeof(char *));
        fflush(stdin);
        scanf("%s ", command);

        if (strcmp(command, "newfile") == 0)
        {
            char *file = (char *)malloc(sizeof(char *));
            char *attribute = (char *)malloc(sizeof(char *));
            scanf("%s ", attribute);
            char c = getchar();
            char end = (c == '"') ? '"' : '\n';
            for (int i = 0;; i++)
            {
                c = getchar();
                if (c == end)
                {
                    *(file + i) = '\0';
                    break;
                }
                *(file + i) = c;
            }

            if (newFile(file) == 1)
            {
                printf("This file already exitsts");
            }
        }
    }
}

int newFile(char *address)
{
    char *path = (char *)malloc(sizeof(char *));
    for (int i = 0;; i++)
    {
        if (address[i] == '/')
        {
            mkdir(path, 0777);
        } else if (address[i] == '\0')
        {
            path[i] = '\0';
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