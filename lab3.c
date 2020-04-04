#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "lab2.h"

#define MAX(x, y) (x >= y) ? x : y

//enum colors {black, red, green, yellow, blue, purple, aqua, white, NAN};

/*int big[2]={0x181818FF,0xFF181818}; //+

int mt_getscreensize(int *cols, int *rows)
{
	struct winsize ws;
	if (!ioctl(1, TIOCGWINSZ, &ws)){
		*cols = ws.ws_col;
		*rows = ws.ws_row;
	} else 
		return -1;
    return 0;
}


int mt_gotoXY (int X, int Y)
{
    char *command = malloc(sizeof(command) * 20);
    sprintf(command, "\E[%d;%dH", X, Y);
    write(1, command, sizeof(command));
    return 0;
}

int mt_setfgcolor(enum colors color)
{
	char *command = malloc(sizeof(command) * 6);
	sprintf(command, "\E[3%dm", color);
	write(1, command, sizeof(command));
	return 0;
}
int mt_setbgcolor(enum colors color)
{
	char *command = malloc(sizeof(command) * 6);
	sprintf(command, "\E[4%dm", color);
	write(1, command, sizeof(command));
	return 0;
}

int mt_clrscr(void)
{
    write(1, "\E[H\E[J",sizeof("\E[H\E[J"));
    return 0;
}
*/
int bc_printA (const char *str)
{
	write(1, "\033(0", sizeof("\033(0"));
	write(1, str, sizeof(const char) * strlen(str));
	write(1, "\033(B", sizeof("\033(B"));
 return 0;
}

int bc_box(int x1, int y1, int x2, int y2) {
	int i, cols, rows;
	mt_getscreensize(&cols, &rows);
	char *str = calloc(sizeof(char), MAX(x2, y2));
	mt_gotoXY(x1, y1);
	str[0] = 'l';//左の上のコーナー
	for (i = 1; i < x2; i++)
		str[i] = 'q';//横ライン
	str[i] = 'k';//右の上のコーナー
	str[i + 1] = '\0';
	bc_printA(str);
	mt_gotoXY(x1+y2, y1);
	str[0] = 'm';//左の下のコーナー
	//for (i = 1; i < x2; i++)
		//str[i] = 'q';
	str[x2] = 'j'; //右の下のコーナー
	bc_printA(str);
	mt_gotoXY(x1+1,y1);
	for (i = 1; i < y2; i++) {
		mt_gotoXY(x1 + i, y1);
		bc_printA("x");//縦ライン
		mt_gotoXY(x1 + i, y1 + x2);
		bc_printA("x");//縦ライン
	}
	return 0;
}

int bc_printbigchar(int *big, int x, int y, int fgcolor, int bgcolor)
{
	char *str = malloc(sizeof(str) * 72);
	int i,j, flag, complete = 0;
	mt_setfgcolor(fgcolor);
	mt_setbgcolor(bgcolor);
	for(i = 0; i < 2; i++) { 
		for(j = 31; j >= 0; j--) {
			if (j % 8 == 0) {
				mt_gotoXY(x++,y);
			}
			flag = (big[i] >> j) & 0x1;
			if (flag == 1) 
				strcat(str,"a");
			else 
				strcat(str," ");
			complete++;
			if (complete % 8 == 0) {
				bc_printA(str);
				complete = 0;
				str[0] = '\0';
			}
		}
	}
	return 0;
}

int bc_getbigcharpos(int * big, int x, int y, int * value)
{
	int bitnum;
	bitnum=8*(x-1)+(y-1);
	if (x>=5)
		if (big[1] & (1 << (bitnum)))
			*value=1; 
		else 
			*value=0;
		else
			if (big[0] & (1 << (bitnum))) *value=1; else *value=0;
	return 0;
}

int bc_bigcharwrite(int fd, int * big, int count)
{
	int i;
	for (i = 0; i < count; i++)
		write(fd, big, sizeof (int) * 2);
	return 0; 
}

int bc_bigcharread (int fd, int *big, int need_count, int * read_count)
{
	int temp, i, readed = 0;
	for (i = 0; i < need_count; i++){
		temp = read (fd, big, sizeof (int) * 2);
		if (temp > 0)
			readed++;
	}
	*read_count = readed;
	return 0;
}


/*
int main()
{
    mt_clrscr();
    int OTT[3][2] = {{0x1010101,0x1010101},{0xFF010101,0xFF8080FF}, {0xFF0101FF,0x10101FF}};
    //int XUI[3][2] = {{0x81422418,0x18244281}, {0x81422418,0x10204080},{0x18838589,0x91A1C181}};
    int lol[2] = {0xFE8181FE,0x818181FE};
    //bc_printbigchar(OTT[0], 5, 10, NAN, NAN);
    //bc_box(3, 3, 50, 11);
    //bc_bigcharwrite(OTT[1], 5, 25, NAN, NAN);
    //bc_printbigchar(OTT[2], 5, 40, NAN, NAN);
    int fd = open("bigchar.lab", O_WRONLY);
	if(fd == -1)
		printf("don't work");
	bc_bigcharwrite(fd, OTT[0], 1);
	//bc_bigcharwrite(fd, OTT[1], 1);
	//bc_bigcharwrite(fd, OTT[2], 1);
	close(fd);
	fd = open("bigchar.lab", S_IREAD);
	int count = 0;
	bc_bigcharread(fd, OTT[0], 2, &count);
	printf("%d", count);
    return 0;
}
int main()
{
	mt_clrscr();
	int XUI[4][2] = {{0x81422418,0x18244281}, {0x81422418,0x10204080},{0x18838589,0x91A1C181},{0x0,0x11172421}};
	int lol[2] = {0xFE8181FE,0x818181FE};
	bc_printbigchar(XUI[0], 5, 10, NAN, NAN);
	bc_box(3, 3, 50, 11);
	bc_printbigchar(XUI[1], 5, 25, NAN, NAN);
	bc_printbigchar(XUI[2], 5, 40, NAN, NAN);
	int fd = open("bigchar.lab", O_WRONLY);
	bc_bigcharwrite(fd, XUI[2], 1);
	bc_bigcharwrite(fd, XUI[3], 1);
	bc_bigcharwrite(fd, XUI[0], 1);
	close(fd);
	fd = open("bigchar.lab", S_IREAD);
	int count = 0;
	bc_bigcharread(fd, lol, 3, &count);
	bc_printbigchar(lol, 15, 10, NAN, NAN);
	printf("%d", count);
	return 0;
}*/
