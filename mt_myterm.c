#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "lab2.h"

//enum colors {black, red, green, yellow, blue, purple, aqua, white, NAN};

int mt_clrscr(void)
{
    write(1, "\E[H\E[J", sizeof("\E[H\E[J"));
    return 0;
}

int mt_gotoXY (int X, int Y)
{
    char *command = malloc(sizeof(command) * 20);
    int len = sprintf(command, "\E[%d;%dH", X, Y);
    write(1, command, len);
    return 0;
}

int mt_setfgcolor(enum colors color)
{
    char *command = malloc(sizeof(command) * 6);
    int len = sprintf(command, "\E[3%dm", color);
    write(1, command, len);
    return 0;
}
int mt_setbgcolor(enum colors color)
{
    char *command = malloc(sizeof(command) * 6);
    int len = sprintf(command, "\E[4%dm", color);
    write(1, command, len);
    return 0;
}

int mt_getscreensize(int *cols, int *rows)
{
    struct winsize ws;
    if (!ioctl(1, TIOCGWINSZ, &ws)) {
	//printf ("Число строк – %d\nЧисло столбцов – %d\n", ws.ws_row, ws.ws_col);
	*cols = ws.ws_col;
	*rows = ws.ws_row;
    } else
	fprintf (stderr, "Ошибка получения размера экрана.\n");
    return 0;
}
/*
int main()
{
    mt_clrscr();
    int cols, rows;
    mt_getscreensize(&cols, &rows);
    mt_setfgcolor(yellow);
    mt_setbgcolor(black);
    char *smth = "Roma KADRITELASIATOK2000";
    mt_gotoXY(cols / 2, rows / 2);
    write(1, smth, strlen(smth));
    //mt_getscreensize();
    
    return 0;
}*/
