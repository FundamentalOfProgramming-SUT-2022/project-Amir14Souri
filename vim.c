#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ncurses.h>
#include "prototypes.h"

WINDOW *commandBar;
int map[100];
int openStatus = 0, exist = 0, preLine = 0, selectLength = 0, saved = 1;
char *currentFileName;

int main()
{
    currentFileName = (char *)calloc(200, sizeof(char));
    for (int i = 0; i < 100; i++)
    {
        map[i] = -1;
    }
    initscr();
    keypad(stdscr, true);
    cbreak();
    noecho();

    // setting background color
    start_color();
    init_pair(BG_COLOR, 15, 17);
    bkgd(COLOR_PAIR(BG_COLOR));
    refresh();
    initWindow(commandBar);

    // NORMAL mode

    while (1)
    {
        char c = getch();
        if (c == ':')
        {
            echo();
            move(LINES - 1, 0);
            attron(COLOR_PAIR(BAR_COLOR));
            addch(':');
            if (inputAndCallCommand() == 0)
            {
                break;
            }
            attroff(COLOR_PAIR(BAR_COLOR));
            noecho();
        }
        else if (c == 'l')
        {
            int x, y;
            getyx(stdscr, y, x);
            if (map[y + preLine] >= x - 2 && x > 3)
            {
                move(y, x + 1);
            }
        }
        else if (c == 'j')
        {
            int x, y;
            getyx(stdscr, y, x);
            if (map[y + preLine + 1] != -1)
            {
                if (map[y + preLine + 1] >= x - 3)
                {
                    move(y + 1, x);
                }
                else
                {
                    move(y + 1, map[y + preLine + 1] + 3);
                }
            }
        }
        else if (c == 'h')
        {
            int x, y;
            getyx(stdscr, y, x);
            if (x > 4)
            {
                move(y, x - 1);
            }
        }
        else if (c == 'k')
        {
            int x, y;
            getyx(stdscr, y, x);
            if (y > 0)
            {
                if (map[y + preLine - 1] >= x - 3)
                {
                    move(y - 1, x);
                }
                else
                {
                    move(y - 1, map[y + preLine - 1] + 3);
                }
            }
        }
        else if (c == 'v')
        {
            // VISUAL mode
            int xStart, yStart;
            getyx(stdscr, yStart, xStart);
            changeMode("VISUAL");
            move(yStart, xStart);
            while (1)
            {
                int x, y;
                c = getch();
                if (c == 'l')
                {
                    getyx(stdscr, y, x);
                    if (map[y + preLine] >= x - 2)
                    {
                        move(y, x + 1);
                        getyx(stdscr, y, x);
                        clearHighlight();
                        highlight(xStart, yStart, x, y);
                    }
                }
                else if (c == 'j')
                {
                    getyx(stdscr, y, x);
                    if (map[y + preLine + 1] != -1)
                    {
                        if (map[y + preLine + 1] >= x - 3)
                        {
                            move(y + 1, x);
                        }
                        else
                        {
                            move(y + 1, map[y + preLine + 1] + 3);
                        }
                        getyx(stdscr, y, x);
                        clearHighlight();
                        highlight(xStart, yStart, x, y);
                    }
                }
                else if (c == 'h')
                {
                    getyx(stdscr, y, x);
                    if (x > 4)
                    {
                        move(y, x - 1);
                        getyx(stdscr, y, x);
                        clearHighlight();
                        highlight(xStart, yStart, x, y);
                    }
                }
                else if (c == 'k')
                {
                    getyx(stdscr, y, x);
                    if (y > 0)
                    {
                        if (map[y + preLine - 1] >= x - 3)
                        {
                            move(y - 1, x);
                        }
                        else
                        {
                            move(y - 1, map[y + preLine - 1] + 3);
                        }
                        getyx(stdscr, y, x);
                        clearHighlight();
                        highlight(xStart, yStart, x, y);
                    }
                }
                else if (c == 'c')
                {
                    // nothing selected
                    if (yStart == y && xStart == x)
                    {
                        attron(COLOR_PAIR(BAR_COLOR));
                        move(LINES - 1, 0);
                        printw("Nothing selected");
                        getch();
                        clearBar();
                        changeMode("NORMAL");
                        move(xStart, yStart);
                        attroff(COLOR_PAIR(BAR_COLOR));
                        break;
                    }

                    // direction
                    char direction = 'f';
                    if (yStart > y || (yStart == y && xStart > x))
                    {
                        direction = 'b';
                    }
                    copy(".unsaved.txt", yStart + 1, xStart - 4, selectLength, direction);
                    changeMode("NORMAL");
                    clearHighlight();
                    move(y, x);
                    break;
                }
                else if (c == 'd')
                {
                    // nothing selected
                    if (yStart == y && xStart == x)
                    {
                        attron(COLOR_PAIR(BAR_COLOR));
                        move(LINES - 1, 0);
                        printw("Nothing selected");
                        getch();
                        clearBar();
                        changeMode("NORMAL");
                        move(xStart, yStart);
                        attroff(COLOR_PAIR(BAR_COLOR));
                        break;
                    }

                    // direction
                    char direction = 'f';
                    if (yStart > y || (yStart == y && xStart > x))
                    {
                        direction = 'b';
                    }
                    removeString(".unsaved.txt", yStart + 1, xStart - 4, selectLength, direction);
                    cat(".unsaved.txt", stdout);
                    changeMode("NORMAL");
                    clearHighlight();
                    saved = 0;
                    showFileStatus();
                    if (direction == 'f')
                    {
                        x = xStart;
                        y = yStart;
                    }
                    move(y, x);
                    break;
                }
                else if (c == 'x')
                {
                    // nothing selected
                    if (yStart == y && xStart == x)
                    {
                        attron(COLOR_PAIR(BAR_COLOR));
                        move(LINES - 1, 0);
                        printw("Nothing selected");
                        getch();
                        clearBar();
                        changeMode("NORMAL");
                        move(xStart, yStart);
                        attroff(COLOR_PAIR(BAR_COLOR));
                        break;
                    }

                    // direction
                    char direction = 'f';
                    if (yStart > y || (yStart == y && xStart > x))
                    {
                        direction = 'b';
                    }
                    cut(".unsaved.txt", yStart + 1, xStart - 4, selectLength, direction);
                    cat(".unsaved.txt", stdout);
                    changeMode("NORMAL");
                    clearHighlight();
                    saved = 0;
                    showFileStatus();
                    if (direction == 'f')
                    {
                        x = xStart;
                        y = yStart;
                    }
                    move(y, x);
                    break;
                }
                else if (c == 'n')
                {
                    changeMode("NORMAL");
                    clearHighlight();
                    move(yStart, xStart);
                    break;
                }
            }
        }
        else if (c == 'p')
        {
            int x, y;
            getyx(stdscr, y, x);
            paste(".unsaved.txt", y + 1, x - 4);
            cat(".unsaved.txt", stdout);
            saved = 0;
            showFileStatus();
            move(y, x);
        }
        else if (c == 'u')
        {
            undo(".unsaved.txt");
            cat(".unsaved.txt", stdout);
            move(0, 4);
            saved = 0;
            showFileStatus();
        }
        else if (c == '/')
        {
            int x, y;
            getyx(stdscr, y, x);

            char *input = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
            char *key = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
            echo();
            move(LINES - 1, 0);
            attron(COLOR_PAIR(BAR_COLOR));

            // input
            addch('/');
            getstr(input);

            // key
            int indent = 0;
            for (int i = 0;; i++)
            {
                c = *(input + indent + i);
                if (c == '\\')
                {
                    indent++;
                    c = *(input + indent + i);
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
                else if (c == '\0')
                {
                    *(key + i) = '\0';
                    break;
                }

                *(key + i) = c;
            }

            clearBar();
            int interval[INPUT_MAX_CHAR][2];
            for (int i = 0; i < INPUT_MAX_CHAR; i++)
            {
                interval[i][0] = -1;
                interval[i][1] = -1;
            }
            replace(".unsaved.txt", key, "", 4, 1, interval);
            for (int i = 0; interval[i][0] != -1; i++)
            {
                int x1, x2, y1, y2;
                int *px1 = &x1, *py1 = &y1, *px2 = &x2, *py2 = &y2;
                locToLineAndPos(interval[i][0], py1, px1);
                locToLineAndPos(interval[i][1] + 1, py2, px2);
                // mvprintw(12, 12, "%d %d %d %d", y1, x1, y2, x2);
                // getch();
                highlight(x1, y1, x2, y2);
            }
            move(y, x);
            attroff(COLOR_PAIR(BAR_COLOR));
            noecho();

            // move on found expressions
            while (1)
            {
                if (getch() == 'n')
                {
                    int found = 0;
                    int xx = -1, yy = -1;
                    int *pxx = &xx, *pyy = &yy;
                    for (int i = 0; interval[i][0] != -1; i++)
                    {
                        locToLineAndPos(interval[i][0], pyy, pxx);
                        if (yy > y || (yy == y && xx > x))
                        {
                            found = 1;
                            y = yy;
                            x = xx;
                            break;
                        }
                    }
                    if (found == 0 && interval[0][0] != -1)
                    {
                        locToLineAndPos(interval[0][0], pyy, pxx);
                        y = yy;
                        x = xx;
                        move(y, x);
                    }
                    else if (found == 1)
                    {
                        move(y, x);
                    }
                }
                else
                {
                    getyx(stdscr, y, x);
                    clearHighlight();
                    move(y, x);
                    break;
                }
            }
        }
        else if (c == '=')
        {
            if (autoIndent(".unsaved.txt") == 3)
            {
                attron(COLOR_PAIR(BAR_COLOR));
                mvprintw(LINES - 1, 0, "Invalid set of curly brackets!");
                getch();
                clearBar();
                attroff(COLOR_PAIR(BAR_COLOR));
                move(0, 4);
                continue;
            }
            cat(".unsaved.txt", stdout);
            move(0, 4);
            saved = 0;
            showFileStatus();
        }
    }

    // getch();
    endwin();
    return 0;
}

int inputAndCallCommand()
{
    int pipe = 0;
    int n = 0;
    int *loc = &n;
    char *command = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
    char *input = (char *)calloc(500, sizeof(char));

    char trash;
    if (pipe == 0)
    {
        getstr(input);
        trash = scan(command, input, loc);
    }

    if (strcmp(command, "newfile") == 0)
    {
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(NULL, input, loc);

        // result
        int result = newFile(file);
        clearBar();
        if (result == 1)
        {
            printw("This file already exists");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            printw("Success");
            getch();
            clearBar();
            move(0, 4);
        }
    }
    else if (strcmp(command, "insert") == 0)
    {
        int line, pos;
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *data = (char *)calloc(FILE_MAX_CHAR, sizeof(char));

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(NULL, input, loc);

        // -str attribute
        if (pipe == 1)
        {
            data = readPipe();
            pipe = 0;
        }
        else
        {
            scan(attribute, input, loc);
            char c = *(input + *loc);
            char end = (c == '"') ? '"' : ' ';
            if (end == '"')
            {
                (*loc)++;
            }
            for (int i = 0;; i++)
            {
                c = *(input + *loc + i);
                if (c == '\\')
                {
                    (*loc)++;
                    c = *(input + *loc + i);
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
                else if (c == end || c == '\0')
                {
                    *loc += i + 1;
                    *(data + i) = '\0';
                    break;
                }

                *(data + i) = c;
            }
            if (end == '"')
            {
                (*loc)++;
            }
        }

        // -pos attribute
        scan(attribute, input, loc);
        line = scanNumber(NULL, input, loc);
        pos = scanNumber(NULL, input, loc);
        if (line <= 0 || pos < 0)
        {
            clearBar();
            printw("Invalid position");
            getch();
            clearBar();
            move(0, 4);
            pipe = 0;
            return 1;
        }

        // result
        int result = insert(file, data, line, pos);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            printw("Success");
            if (strcmp(file, currentFileName) == 0)
            {
                cat(file, stdout);
                saved = 1;
                showFileStatus();
            }
            getch();
            clearBar();
            move(0, 4);
        }
    }
    else if (strcmp(command, "cat") == 0)
    {
        char *c = (char *)calloc(1, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        char *file = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(c, input, loc);

        // pipe possibility
        if (*c == ' ')
        {
            scan(attribute, input, loc);
            if (strcmp(attribute, "=D") == 0)
            {
                pipe = 1;
                scan(command, input, loc);
            }
        }

        // result
        int result;
        if (pipe == 0)
        {
            result = cat(file, stdout);
        }
        else
        {
            FILE *pipe = fopen(".pipe.txt", "w");
            result = cat(file, pipe);
            fclose(pipe);
        }
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            if (openStatus == 1)
            {
                save();
            }
            *currentFileName = '\0';
            clearBar();
            move(0, 4);
            openStatus = 0;
            exist = 1;
            saved = 0;
            showFileStatus();
        }
        if (pipe == 1 && result != 0)
        {
            pipe = 0;
            while (1)
            {
                (*loc)++;
                if (*(input + *loc) == '\0')
                {
                    break;
                }
            }
        }
    }
    else if (strcmp(command, "open") == 0)
    {
        char *file = (char *)calloc(FILE_MAX_CHAR, sizeof(char));

        // file name
        open(inputPath(NULL, input, loc));
        clearBar();
        move(0, 4);
        openStatus = 1;
        saved = 1;
        exist = 1;
        showFileStatus();
    }
    else if (strcmp(command, "remove") == 0)
    {
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int line, pos, length;
        char direction;

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(NULL, input, loc);

        // -pos attribute
        scan(attribute, input, loc);
        line = scanNumber(NULL, input, loc);
        pos = scanNumber(NULL, input, loc);
        if (line <= 0 || pos < 0)
        {
            clearBar();
            printw("Invalid position");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // -size attribute
        scan(attribute, input, loc);
        length = scanNumber(NULL, input, loc);
        if (length <= 0)
        {
            clearBar();
            printw("Invalid size");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // -f or -b flag
        scan(attribute, input, loc);
        direction = attribute[1];

        // result
        int result = removeString(file, line, pos, length, direction);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            printw("Success");
            if (strcmp(file, currentFileName) == 0)
            {
                cat(file, stdout);
                saved = 1;
                showFileStatus();
            }
            getch();
            clearBar();
            move(0, 4);
        }
    }
    else if (strcmp(command, "copy") == 0)
    {
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int line, pos, length;
        char direction;

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(NULL, input, loc);

        // -pos attribute
        scan(attribute, input, loc);
        line = scanNumber(NULL, input, loc);
        pos = scanNumber(NULL, input, loc);
        if (line <= 0 || pos < 0)
        {
            clearBar();
            printw("Invalid position");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // -size attribute
        scan(attribute, input, loc);
        length = scanNumber(NULL, input, loc);
        if (length <= 0)
        {
            clearBar();
            printw("Invalid size");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // -f or -b flag
        scan(attribute, input, loc);
        direction = attribute[1];

        // result
        int result = copy(file, line, pos, length, direction);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            printw("Success");
            getch();
            clearBar();
            move(0, 4);
        }
    }
    else if (strcmp(command, "cut") == 0)
    {
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int line, pos, length;
        char direction;

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(NULL, input, loc);

        // -pos attribute
        scan(attribute, input, loc);
        line = scanNumber(NULL, input, loc);
        pos = scanNumber(NULL, input, loc);
        if (line <= 0 || pos < 0)
        {
            clearBar();
            printw("Invalid position");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // -size attribute
        scan(attribute, input, loc);
        length = scanNumber(NULL, input, loc);
        if (length <= 0)
        {
            clearBar();
            printw("Invalid size");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // -f or -b flag
        scan(attribute, input, loc);
        direction = attribute[1];

        // result
        int result = cut(file, line, pos, length, direction);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            printw("Success");
            if (strcmp(file, currentFileName) == 0)
            {
                cat(file, stdout);
                saved = 1;
                showFileStatus();
            }
            getch();
            clearBar();
            move(0, 4);
        }
    }
    else if (strcmp(command, "paste") == 0)
    {
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int line, pos;

        // -file attribute
        scan(attribute, input, loc);
        file = inputPath(NULL, input, loc);

        // -pos attribute
        scan(attribute, input, loc);
        line = scanNumber(NULL, input, loc);
        pos = scanNumber(NULL, input, loc);
        if (line <= 0 || pos < 0)
        {
            clearBar();
            printw("Invalid position");
            getch();
            clearBar();
            move(0, 4);
            return 1;
        }

        // result
        int result = paste(file, line, pos);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            printw("Success");
            if (strcmp(file, currentFileName) == 0)
            {
                cat(file, stdout);
                saved = 1;
                showFileStatus();
            }
            getch();
            clearBar();
            move(0, 4);
        }
    }
    else if (strcmp(command, "find") == 0)
    {
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *key = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int options = 0, at = 1;

        // -str attribute
        if (pipe == 1)
        {
            pipe = 0;
            key = readPipe();
        }
        else
        {
            scan(attribute, input, loc);
            char c = *(input + *loc);
            char end = (c == '"') ? '"' : ' ';
            if (end == '"')
            {
                (*loc)++;
            }
            for (int i = 0;; i++)
            {
                c = *(input + *loc + i);
                if (c == '\\')
                {
                    (*loc)++;
                    c = *(input + *loc + i);
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
                else if (c == end || c == '\0')
                {
                    *loc += i + 1;
                    *(key + i) = '\0';
                    break;
                }

                *(key + i) = c;
            }
            if (end == '"')
            {
                (*loc)++;
            }
        }

        // -file attribute
        scan(attribute, input, loc);
        char *last = (char *)calloc(1, sizeof(char));
        file = inputPath(last, input, loc);
        char c = *last;

        // possible options (or pipe)
        for (int i = 0; i < 5; i++)
        {
            if (c == ' ')
            {
                c = scan(attribute, input, loc);
                if (strcmp(attribute, "-count") == 0)
                {
                    options += 8;
                }
                else if (strcmp(attribute, "-at") == 0)
                {
                    options += 4;
                    at = scanNumber(last, input, loc);
                    if (at <= 0)
                    {
                        clearBar();
                        printw("Invalid number for -at attribute");
                        getch();
                        clearBar();
                        move(0, 4);
                    }
                    c = *last;
                }
                else if (strcmp(attribute, "-byword") == 0)
                {
                    options += 2;
                }
                else if (strcmp(attribute, "-all") == 0)
                {
                    options++;
                }
                else if (strcmp(attribute, "=D") == 0)
                {
                    pipe = 1;
                    scan(command, input, loc);
                    break;
                }
            }
            else
            {
                break;
            }
        }

        // result
        int result;
        if (pipe == 0)
        {
            FILE *space = fopen(".space.txt", "w");
            result = find(file, key, options, at, space);
            fclose(space);
        }
        else
        {
            FILE *pipe = fopen(".pipe.txt", "w");
            result = find(file, key, options, at, pipe);
            fclose(pipe);
        }
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 3)
        {
            printw("Invalid set of options!");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 4)
        {
            printw("Not found!");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            if (openStatus == 1)
            {
                save();
            }
            cat(".space.txt", stdout);
            *currentFileName = '\0';
            clearBar();
            move(0, 4);
            openStatus = 0;
            exist = 1;
            saved = 0;
            showFileStatus();
        }
        if (pipe == 1 && result != 0)
        {
            pipe = 0;
            while (1)
            {
                if (getch() == '\n')
                {
                    break;
                }
            }
        }
    }
    else if (strcmp(command, "replace") == 0)
    {
        int thisFile = 0;
        int interval[INPUT_MAX_CHAR][2];
        for (int i = 0; i < INPUT_MAX_CHAR; i++)
        {
            interval[i][0] = -1;
            interval[i][1] = -1;
        }
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *key = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
        char *alternate = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int options = 0, at = 1;

        // -str1 (key) attribute
        if (pipe == 1)
        {
            pipe = 0;
            key = readPipe();
        }
        else
        {
            scan(attribute, input, loc);
            char c = *(input + *loc);
            char end = (c == '"') ? '"' : ' ';
            if (end == '"')
            {
                (*loc)++;
            }
            for (int i = 0;; i++)
            {
                c = *(input + *loc + i);
                if (c == '\\')
                {
                    (*loc)++;
                    c = *(input + *loc + i);
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
                else if (c == end || c == '\0')
                {
                    *loc += i + 1;
                    *(key + i) = '\0';
                    break;
                }

                *(key + i) = c;
            }
            if (end == '"')
            {
                (*loc)++;
            }
        }

        // -str2 (alternate) attribute
        scan(attribute, input, loc);
        char c = *(input + *loc);
        char end = (c == '"') ? '"' : ' ';
        if (end == '"')
        {
            (*loc)++;
        }
        for (int i = 0;; i++)
        {
            c = *(input + *loc + i);
            if (c == '\\')
            {
                (*loc)++;
                c = *(input + *loc + i);
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
            else if (c == end || c == '\0')
            {
                *loc += i + 1;
                *(alternate + i) = '\0';
                break;
            }

            *(alternate + i) = c;
        }
        if (end == '"')
        {
            (*loc)++;
        }

        // -file and possible options
        char *last = (char *)calloc(1, sizeof(char));
        for (int i = 0; i < 3; i++)
        {
            if (c == ' ')
            {
                c = scan(attribute, input, loc);
                if (strcmp(attribute, "-file") == 0)
                {
                    file = inputPath(last, input, loc);
                    c = *last;
                }
                else if (strcmp(attribute, "-at") == 0)
                {
                    options += 2;
                    at = scanNumber(last, input, loc);
                    if (at <= 0)
                    {
                        clearBar();
                        printw("Invalid number for -at attribute");
                        getch();
                        clearBar();
                        move(0, 4);
                    }
                    c = *last;
                }
                else if (strcmp(attribute, "-all") == 0)
                {
                    options++;
                }
            }
            else
            {
                break;
            }
        }

        if (*file == '\0')
        {
            thisFile = 1;
            file = ".unsaved.txt";
        }

        // result
        int result = replace(file, key, alternate, options, at, interval);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 3)
        {
            printw("Invalid set of options!");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 4)
        {
            printw("Not found!");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result != 5)
        {
            if (thisFile == 0)
            {
                printw("Success");
                if (strcmp(file, currentFileName) == 0)
                {
                    cat(file, stdout);
                    saved = 1;
                    showFileStatus();
                }
                getch();
                clearBar();
                move(0, 4);
            }
            else
            {
                cat(".unsaved.txt", stdout);
                clearBar();
                saved = 0;
                showFileStatus();
                if (options == 0 || options == 1)
                {
                    if (interval[0][0] != -1)
                    {
                        int x, y;
                        int *px = &x, *py = &y;
                        locToLineAndPos(interval[0][0], py, px);
                        move(y, x);
                    }
                }
                else if (options == 2)
                {
                    if (interval[at - 1][0] != -1)
                    {
                        int x, y;
                        int *px = &x, *py = &y;
                        locToLineAndPos(interval[at - 1][0], py, px);
                        move(y, x);
                    }
                }

                return 1;
            }
        }
    }
    else if (strcmp(command, "grep") == 0)
    {
        char *files[50];
        char *key = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int options = 0, n = 0;
        char c;

        // -files, -str and options
        for (int i = 0; i < 3; i++)
        {
            scan(attribute, input, loc);
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
                c = *(input + *loc + i);
                char end = (c == '"') ? '"' : ' ';
                if (end == '"')
                {
                    (*loc)++;
                }
                for (int i = 0;; i++)
                {
                    c = *(input + *loc + i);
                    if (c == '\\')
                    {
                        (*loc)++;
                        c = *(input + *loc + i);
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
                    else if (c == end || c == '\0')
                    {
                        *loc += i + 1;
                        *(key + i) = '\0';
                        break;
                    }

                    *(key + i) = c;
                }
                if (end == '"')
                {
                    c = *(input + *loc);
                    (*loc)++;
                }
            }
            else if (strcmp(attribute, "-files") == 0)
            {
                if (i == 0)
                {
                    i++;
                }
                if (pipe == 1)
                {
                    i++;
                    pipe = 0;
                    key = readPipe();
                }
                c = ' ';
                for (int j = 0; c != '\0'; j++)
                {
                    files[n] = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
                    c = *(input + *loc);
                    (*loc)++;
                    char end;
                    if (c == '"')
                    {
                        end = '"';
                    }
                    else if (c == '=')
                    {
                        c = ' ';
                        break;
                    }
                    else
                    {
                        end = ' ';
                    }
                    if (end == '"')
                    {
                        (*loc)++;
                    }
                    for (int i = 0;; i++)
                    {
                        c = *(input + *loc + i);
                        if (c == end || c == '\0')
                        {
                            *loc += i + 1;
                            *(files[n] + i) = '\0';
                            break;
                        }
                        *(files[n] + i) = c;
                    }
                    if (end == '"')
                    {
                        c = *(input + *loc);
                        (*loc)++;
                    }
                    n++;
                }
            }
        }

        // pipe possibility
        if (c == ' ')
        {
            scan(attribute, input, loc);
            if (strcmp(attribute, "D") == 0)
            {
                pipe = 1;
                scan(command, input, loc);
            }
        }

        // result
        int result;
        if (pipe == 0)
        {
            FILE *space = fopen(".space.txt", "w");
            result = grep(files, n, key, options, space);
            fclose(space);
        }
        else
        {
            FILE *pipe = fopen(".pipe.txt", "w");
            result = grep(files, n, key, options, pipe);
            fclose(pipe);
        }
        clearBar();
        if (result == 1)
        {
            printw("Some invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("Some files doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 3)
        {
            printw("Not found!");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            if (openStatus == 1)
            {
                save();
            }
            cat(".space.txt", stdout);
            *currentFileName = '\0';
            clearBar();
            move(0, 4);
            openStatus = 0;
            exist = 1;
            saved = 0;
            showFileStatus();
        }
        if (pipe == 1 && result != 0)
        {
            pipe = 0;
            while (1)
            {
                if (getch() == '\n')
                {
                    break;
                }
            }
        }
    }
    else if (strcmp(command, "undo") == 0)
    {
        int thisFile = 0;
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));

        // -file attirbute
        if (trash == ' ')
        {
            scan(attribute, input, loc);
            file = inputPath(NULL, input, loc);
        }
        else if (trash == '\0')
        {
            thisFile = 1;
            file = ".unsaved.txt";
        }

        // result
        int result = undo(file);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 3)
        {
            printw("No changes has been made to this file!");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            if (thisFile == 0)
            {
                printw("Success");
                if (strcmp(file, currentFileName) == 0)
                {
                    cat(file, stdout);
                    saved = 1;
                    showFileStatus();
                }
                getch();
                clearBar();
                move(0, 4);
            }
            else
            {
                cat(".unsaved.txt", stdout);
                clearBar();
                move(0, 4);
                saved = 0;
                showFileStatus();
                return 1;
            }
        }
    }
    else if (strcmp(command, "auto-indent") == 0)
    {
        int thisFile = 0;
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));

        if (trash == ' ')
        {
            scan(attribute, input, loc);
            file = inputPath(NULL, input, loc);
        }
        else if (trash == '\0')
        {
            thisFile = 1;
            file = ".unsaved.txt";
        }

        // result
        int result = autoIndent(file);
        clearBar();
        if (result == 1)
        {
            printw("Invalid path");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("File doesn't exist");
            getch();
            clearBar();
            move(0, 4);
        }
        else if (result == 3)
        {
            printw("Incorrect set of curly brackets!");
            getch();
            clearBar();
            move(0, 4);
        }
        else
        {
            if (thisFile == 0)
            {
                printw("Success");
                if (strcmp(file, currentFileName) == 0)
                {
                    cat(file, stdout);
                    saved = 1;
                    showFileStatus();
                }
                getch();
                clearBar();
                move(0, 4);
            }
            else
            {
                cat(".unsaved.txt", stdout);
                clearBar();
                move(0, 4);
                saved = 0;
                showFileStatus();
                return 1;
            }
        }
    }
    else if (strcmp(command, "compare") == 0)
    {
        char *c = (char *)calloc(1, sizeof(char));
        char *file1 = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *file2 = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        char *attribute = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));

        // -file1 attribute
        file1 = inputPath(c, input, loc);

        // -file2 attribute
        file2 = inputPath(c, input, loc);

        // pipe possibility
        if (*c == ' ')
        {
            scan(attribute, input, loc);
            if (strcmp(attribute, "=D") == 0)
            {
                pipe = 1;
                scan(command, input, loc);
            }
        }

        // result
        int result;
        if (pipe == 0)
        {
            FILE *space = fopen(".space.txt", "w");
            result = compare(file1, file2, space);
            fclose(space);
        }
        else
        {
            FILE *pipe = fopen(".pipe.txt", "w");
            result = compare(file1, file2, pipe);
            fclose(pipe);
        }
        clearBar();
        if (result == 1)
        {
            printw("Invalid path for 1st file");
            getch();
            clearScreen();
            move(0, 4);
        }
        else if (result == 2)
        {
            printw("1st file doesn't exist");
            getch();
            clearScreen();
            move(0, 4);
        }
        else if (result == 3)
        {
            printw("Invalid path for 2nd file");
            getch();
            clearScreen();
            move(0, 4);
        }
        else if (result == 4)
        {
            printw("2nd file doesn't exist");
            getch();
            clearScreen();
            move(0, 4);
        }
        else if (result == 5)
        {
            printw("2 files are the same");
            getch();
            clearScreen();
            move(0, 4);
        }
        else
        {
            if (openStatus == 1)
            {
                save();
            }
            cat(".space.txt", stdout);
            *currentFileName = '\0';
            clearBar();
            move(0, 4);
            openStatus = 0;
            exist = 1;
            saved = 0;
            showFileStatus();
        }
        if (pipe == 1 && result != 0)
        {
            pipe = 0;
            while (1)
            {
                if (getch() == '\n')
                {
                    break;
                }
            }
        }
    }
    else if (strcmp(command, "tree") == 0)
    {
        int depth;
        char *attribute = (char *)calloc(ATTR_MAX_CHAR, sizeof(char));
        int history[200] = {0};

        // depth
        char *last = (char *)calloc(1, sizeof(char));
        depth = scanNumber(last, input, loc);
        char c = *last;

        // pipe possibility
        if (c == ' ')
        {
            scan(attribute, input, loc);
            if (strcmp(attribute, "=D") == 0)
            {
                pipe = 1;
                scan(command, input, loc);
            }
        }

        // result
        int result;
        if (pipe == 0)
        {
            FILE *space = fopen(".space.txt", "w");
            result = tree("root", 0, depth, history, space);
            fclose(space);
        }
        else
        {
            FILE *pipe = fopen(".pipe.txt", "w");
            result = tree("root", 0, depth, history, pipe);
            fclose(pipe);
        }

        if (result == 1)
        {
            printw("Invalid depth!");
            getch();
            clearScreen();
            move(0, 4);
        }
        else
        {
            if (openStatus == 1)
            {
                save();
            }
            cat(".space.txt", stdout);
            *currentFileName = '\0';
            clearBar();
            move(0, 4);
            openStatus = 0;
            exist = 1;
            saved = 0;
            showFileStatus();
        }
        if (pipe == 1 && result != 0)
        {
            pipe = 0;
            while (1)
            {
                if (getch() == '\n')
                {
                    break;
                }
            }
        }
    }
    else if (strcmp(command, "save") == 0)
    {
        save();
        clearBar();
        printw("Success");
        getch();
        clearBar();
        move(0, 4);
    }
    else if (strcmp(command, "saveas") == 0)
    {
        char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));

        // new name for saving
        file = inputPath(NULL, input, loc);

        saveAs(file);
        clearBar();
        printw("Success");
        getch();
        clearBar();
        move(0, 4);
    }
    else if (strcmp(command, "exit") == 0)
    {
        if (openStatus == 1 && saved == 0)
        {
            save();
        }
        FILE *clipboard = fopen(".clipboard.txt", "w");
        fclose(clipboard);
        FILE *pipe = fopen(".pipe.txt", "w");
        fclose(pipe);
        FILE *unsaved = fopen(".unsaved.txt", "w");
        fclose(unsaved);
        FILE *backup = fopen(".backup.txt", "w");
        fclose(backup);
        FILE *space = fopen(".space.txt", "w");
        fclose(space);

        return 0;
    }
    else
    {
        clearBar();
        printw("Invalid command");
        getch();
        clearBar();
        move(0, 4);
        return 1;
    }

    return 1;
}

char *inputPath(char *lastChar, char *input, int *loc)
{
    char *file = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
    char c = *(input + *loc);
    (*loc)++;
    char end = (c == '"') ? '"' : ' ';
    if (end == '"')
    {
        (*loc)++;
    }
    for (int i = 0;; i++)
    {
        c = *(input + *loc + i);
        if (c == end || c == '\0')
        {
            *loc += i + 1;
            *(file + i) = '\0';
            break;
        }
        *(file + i) = c;
    }
    if (end == '"')
    {
        c = *(input + *loc);
        (*loc)++;
    }
    if (lastChar != NULL)
    {
        *lastChar = c;
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
    char *path = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
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
    char *path = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
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

    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

    if (strcmp(address, ".unsaved.txt") != 0 && strcmp(address, ".backup.txt") != 0)
    {
        backupFile(address);
    }
    FILE *new = fopen(address, "w");
    fprintf(file, "%s", content);
    fclose(new);
    return 0;
}

int cat(char *address, FILE *where)
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

    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char c = '\n';
    int line = 1, nChar = 0;
    if (where == stdout)
    {
        clearScreen();
        for (int i = 0; i < 100; i++)
        {
            map[i] = -1;
        }
        move(0, 0);
    }
    for (int i = 0;; i++)
    {
        if (c == '\n' && where == stdout)
        {
            initLine(line);
        }
        c = fgetc(file);
        nChar++;
        if (c == '\n' && where == stdout)
        {
            map[line - 1] = nChar;
            line++;
            nChar = 0;
        }
        else if (c == EOF)
        {
            if (where == stdout)
            {
                map[line - 1] = 1;
            }
            *(content + i) = '\0';
            break;
        }

        if (where == stdout)
        {
            attron(COLOR_PAIR(BG_COLOR));
            addch(c);
            attroff(COLOR_PAIR(BG_COLOR));
        }
        *(content + i) = c;
    }
    fclose(file);

    if (where == stdout)
    {
        FILE *unsaved = fopen(".unsaved.txt", "w");
        fclose(unsaved);
        insert(".unsaved.txt", content, 1, 0);
        if (strcmp(address, ".unsaved.txt") != 0 && strcmp(address, ".space.txt") != 0)
        {
            char *backup = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
            strcat(backup, filePath(address));
            strcat(backup, ".");
            strcat(backup, fileName(address));
            FILE *fakeBackup = fopen(".backup.txt", "w");
            cat(backup, fakeBackup);
            fclose(fakeBackup);
        }
    }
    else
    {
        fprintf(where, "%s", content);
    }
    return 0;
}

void open(char *address)
{
    if (openStatus == 1 && saved == 0)
    {
        save();
    }

    currentFileName = address;
    newFile(address);
    cat(address, stdout);
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

    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char *newContent = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char *target = (char *)calloc(length + 1, sizeof(char));
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

    FILE *clipboard = fopen(".clipboard.txt", "w");
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
    FILE *clipboard = fopen(".clipboard.txt", "r");
    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

int find(char *address, char *key, int options, int at, FILE *where)
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

    int startA[INPUT_MAX_CHAR][2];
    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    for (int i = 0; i < INPUT_MAX_CHAR; i++)
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
            return 4;
        }
        else
        {
            printFind(startA, at, (options / 2) % 2, where);
        }
        break;
    case 1:
    case 3:
        if (startA[0][0] == -1)
        {
            return 4;
        }
        else
        {
            for (int i = 0; startA[i][0] != -1; i++)
            {
                printFind(startA, i, (options / 2) % 2, where);
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
        fprintf(where, "Number of matches: %d\n", count);
        break;
    }

    return 0;
}

void printFind(int startA[][2], int i, int by, FILE *where)
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
    fprintf(where, "match %2d (%s): %3d\n", i + 1, byStr, startA[i][by]);
}

int replace(char *address, char *key, char *alternate, int options, int at, int interval[][2])
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

    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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
    if (options == 4)
    {
        return 5;
    }

    at--;
    char *newContent = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

int grep(char *addresses[], int n, char *key, int options, FILE *where)
{
    int matches[200][2], count = 0;
    for (int i = 0; i < 200; i++)
    {
        matches[i][0] = -1;
        matches[i][1] = -1;
    }
    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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
                        fputc(c, where);
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
        fprintf(where, "%d\n", count);
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
                fprintf(where, "/%s\n", addresses[matches[i][0]]);
                last = matches[i][0];
            }
        }
        return 0;
        break;
    }
}

void backupFile(char *address)
{
    if (strcmp(address, ".backup.txt") == 0)
    {
        return;
    }

    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

    char *backupAddress = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
    if (strcmp(address, ".unsaved.txt") == 0)
    {
        strcat(backupAddress, ".backup.txt");
    }
    else
    {
        strcat(backupAddress, filePath(address));
        strcat(backupAddress, ".");
        strcat(backupAddress, fileName(address));
    }
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

    char *backupAddress = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
    if (strcmp(address, ".unsaved.txt") == 0)
    {
        strcat(backupAddress, ".backup.txt");
    }
    else
    {
        strcat(backupAddress, filePath(address));
        strcat(backupAddress, ".");
        strcat(backupAddress, fileName(address));
    }
    FILE *backup = fopen(backupAddress, "r");
    if (backup == NULL)
    {
        return 3;
    }

    char *backupContent = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

    char *newContent = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

int compare(char *address1, char *address2, FILE *where)
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

    char *sentence1 = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char *sentence2 = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char *word1 = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
    char *word2 = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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
                fprintf(where, "<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n%s", lastLine, line - 1, sentence1);
            }
            else if (*sentence2 != '\0')
            {
                fprintf(where, ">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n%s", lastLine, line - 1, sentence2);
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
                    char *copy1 = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
                    char *copy2 = (char *)calloc(FILE_MAX_CHAR, sizeof(char));
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

                    fprintf(where, "============ #%d ============\n%s\n%s\n", line, copy1, copy2);
                }
                else
                {
                    fprintf(where, "============ #%d ============\n%s\n%s\n", line, sentence1, sentence2);
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

int tree(char *dir, int depth, int limit, int history[], FILE *where)
{
    if (limit < -1)
    {
        return 1;
    }

    indentTree(depth, history, where);
    fprintf(where, "%s\n", fileName(dir));
    depth++;
    if (limit != -1 && depth == limit + 1)
    {
        return 0;
    }

    int count = 0;
    DIR *folder = opendir(dir);
    struct dirent *entry;
    while ((entry = readdir(folder)) != NULL)
    {
        char *name = entry->d_name;
        if (*name != '.')
        {
            count++;
        }
    }
    rewinddir(folder);
    for (int i = 0; (entry = readdir(folder)) != NULL; i++)
    {
        char *name = entry->d_name;
        if (*name == '.')
        {
            i--;
            continue;
        }

        if (entry->d_type == DT_REG)
        {
            indentTree(depth, history, where);
            fprintf(where, "%s\n", name);
        }
        else if (entry->d_type == DT_DIR)
        {
            char *subDir = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
            strcat(subDir, dir);
            strcat(subDir, "/");
            strcat(subDir, name);
            if (i != count - 1)
            {
                history[depth - 1] = 1;
            }
            else if (i == count - 1)
            {

                history[depth - 1] = 0;
            }
            tree(subDir, depth, limit, history, where);
        }
    }
    closedir(folder);
    return 0;
}

void indentTree(int depth, int history[], FILE *where)
{
    for (int i = 0; i < depth; i++)
    {
        if (i == depth - 1)
        {
            fprintf(where, "|--- ");
        }
        else if (history[i] == 1)
        {
            fprintf(where, "|    ");
        }
        else
        {
            fprintf(where, "     ");
        }
    }
}

char *readPipe()
{
    FILE *pipe = fopen(".pipe.txt", "r");
    char *content = (char *)calloc(FILE_MAX_CHAR, sizeof(char));

    for (int i = 0;; i++)
    {
        char c = fgetc(pipe);
        if (c == EOF)
        {
            *(content + i - 1) = '\0';
            break;
        }
        *(content + i) = c;
    }
    fclose(pipe);
    return content;
}

void initWindow(WINDOW *bar)
{
    // initialize boxes
    WINDOW *lineNo = newwin(LINES - 2, 4, 0, 0);
    init_pair(LINE_NO_COLOR, COLOR_BLACK, 26);
    wbkgd(lineNo, COLOR_PAIR(LINE_NO_COLOR));
    WINDOW *mode = newwin(1, 10, LINES - 2, 0);
    init_pair(MODE_COLOR, 15, COLOR_GREEN);
    wbkgd(mode, COLOR_PAIR(MODE_COLOR));
    WINDOW *fileName = newwin(1, COLS - 10, LINES - 2, 10);
    init_pair(FILE_NAME_COLOR, COLOR_BLACK, 15);
    wbkgd(fileName, COLOR_PAIR(FILE_NAME_COLOR));
    WINDOW *commandBar = newwin(1, COLS, LINES - 1, 0);
    init_pair(BAR_COLOR, COLOR_WHITE, 16);
    wbkgd(commandBar, COLOR_PAIR(BAR_COLOR));
    bar = commandBar;

    // box texts
    wattron(mode, A_BOLD);
    mvwprintw(mode, 0, 2, "NORMAL");
    wattroff(mode, A_BOLD);

    wrefresh(lineNo);
    wrefresh(mode);
    wrefresh(fileName);
    wrefresh(commandBar);
    return;
}

char scan(char *text, char *input, int *loc)
{
    char c;
    for (int i = 0;; i++)
    {
        c = *(input + *loc + i);
        if (c == ' ' || c == '\0')
        {
            *loc += i + 1;
            *(text + i) = '\0';
            break;
        }

        *(text + i) = c;
    }

    return c;
}

int scanNumber(char *last, char *input, int *loc)
{
    int num = 0;
    char c = *(input + *loc);
    if (c == '-')
    {
        return -1;
    }
    for (int i = 0;; i++)
    {
        c = *(input + *loc + i);
        if (c == ' ' || c == ':' || c == '\0')
        {
            *loc += i + 1;
            if (last != NULL)
            {
                *last = c;
            }
            break;
        }
        num *= 10;
        num += (c - '0');
    }

    return num;
}

void clearBar()
{
    attron(COLOR_PAIR(BAR_COLOR));
    for (int i = 0; i < COLS; i++)
    {
        mvaddch(LINES - 1, i, ' ');
    }
    attron(COLOR_PAIR(BAR_COLOR));
    move(LINES - 1, 0);
    return;
}

void clearName()
{
    attron(COLOR_PAIR(FILE_NAME_COLOR));
    for (int i = 10; i < COLS; i++)
    {
        mvaddch(LINES - 2, i, ' ');
    }
    attron(COLOR_PAIR(FILE_NAME_COLOR));
    return;
}

void clearLine(int line)
{
    attron(COLOR_PAIR(LINE_NO_COLOR));
    for (int i = 0; i < 4; i++)
    {
        mvaddch(line, i, ' ');
    }
    attroff(COLOR_PAIR(LINE_NO_COLOR));
    attron(COLOR_PAIR(BG_COLOR));
    for (int i = 4; i < COLS; i++)
    {
        mvaddch(line, i, ' ');
    }
    attroff(COLOR_PAIR(BG_COLOR));
    move(0, 4);
    return;
}

void clearScreen()
{
    for (int i = 0; i < LINES - 2; i++)
    {
        clearLine(i);
    }
    return;
}

void initLine(int no)
{
    attron(COLOR_PAIR(LINE_NO_COLOR));
    printw("%3d ", no);
    attroff(COLOR_PAIR(LINE_NO_COLOR));
}

void highlight(int x1, int y1, int x2, int y2)
{
    selectLength = 0;
    int x, y, xx, yy;
    if (y2 < y1 || (y1 == y2 && x2 < x1))
    {
        x = x2;
        y = y2;
        xx = x1;
        yy = y1;
    }
    else
    {
        x = x1;
        y = y1;
        xx = x2;
        yy = y2;
    }

    init_pair(HIGHLIGHT_COLOR, 15, 12);
    attron(COLOR_PAIR(HIGHLIGHT_COLOR));
    char c;
    int j = y, i = x, thisLine = 0;
    for (;; j++)
    {
        if (j == yy)
        {
            thisLine = 1;
        }
        for (;; i++)
        {
            if (i == map[j + preLine] + 4)
            {
                i = 4;
                break;
            }
            if (thisLine == 1 && i == xx)
            {
                break;
            }

            c = mvinch(j, i);
            mvaddch(j, i, c);
            selectLength++;
        }
        if (thisLine == 1)
        {
            break;
        }
    }

    move(y2, x2);
    attroff(COLOR_PAIR(HIGHLIGHT_COLOR));
}

void clearHighlight()
{
    char c;
    attron(COLOR_PAIR(BG_COLOR));
    for (int i = 0; i < LINES - 2; i++)
    {
        for (int j = 4; j < COLS; j++)
        {
            c = mvinch(i, j);
            mvaddch(i, j, c);
        }
    }
    attron(COLOR_PAIR(BG_COLOR));
}

void changeMode(char *mode)
{
    attron(COLOR_PAIR(MODE_COLOR) | A_BOLD);
    mvprintw(LINES - 2, 0, "  %s  ", mode);
    attroff(COLOR_PAIR(MODE_COLOR) | A_BOLD);
}

void save()
{
    if (*currentFileName == '\0')
    {
        int n = 0;
        int *loc = &n;
        char *input = (char *)calloc(INPUT_MAX_CHAR, sizeof(char));
        clearBar();
        attron(COLOR_PAIR(BAR_COLOR));
        mvprintw(LINES - 1, 0, ":");
        getstr(input);
        currentFileName = inputPath(NULL, input, loc);
        attroff(COLOR_PAIR(BAR_COLOR));
    }

    newFile(currentFileName);
    backupFile(currentFileName);
    FILE *file = fopen(currentFileName, "w");
    cat(".unsaved.txt", file);
    fclose(file);
    saved = 1;
    showFileStatus();
}

void saveAs(char *address)
{
    if (strcmp(address, currentFileName) == 0)
    {
        backupFile(address);
    }
    newFile(address);
    FILE *file = fopen(address, "w");
    cat(".unsaved.txt", file);
    fclose(file);
    saved = 1;
    open(address);
    showFileStatus();
}

void showFileStatus()
{
    clearName();
    attron(COLOR_PAIR(FILE_NAME_COLOR) | A_BOLD);
    mvprintw(LINES - 2, 12, "%s", fileName(currentFileName));
    attroff(A_BOLD);
    if (saved == 1)
    {
        mvprintw(LINES - 2, COLS - 7, "SAVED");
    }
    attroff(COLOR_PAIR(FILE_NAME_COLOR));
    move(0, 4);
}

void locToLineAndPos(int loc, int *y, int *x)
{
    int sum = 0;
    int last = 0;
    for (int i = 0;; i++)
    {
        sum += map[i];
        if (loc < sum)
        {
            *y = i;
            *x = loc - last + 4;
            break;
        }
        last += map[i];
    }
    return;
}