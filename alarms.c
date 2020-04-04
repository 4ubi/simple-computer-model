#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include <signal.h>
#include <sys/time.h>

#define MAX(x, y) (x >= y) ? x : y

int idx = 0, AKKUM = 0;
int plus[2]={0x181818FF,0xFF181818}; //+
int BIG[16][2] = { {0x818181FF, 0xFF818181}, {0x120A0602,  0x2020202}, {0x10101FF,  0xFF8080FF}, 
{0xFF0101FF, 0xFF010101}, {0xFF818181,  0x1010101}, {0x808080FF, 0xFF0101FF}, 
{0x808080FF, 0xFF8181FF}, {0x80402FF,  0x20202010}, {0xFF8181FF, 0xFF818181}, {0xFF8181FF, 0xFF010101}, 
{0x8181817E, 0x818181FF}, {0xFE8181FE, 0xFE8181FE}, {0x808080FF, 0xFF808080}, 
{0x818181FE, 0xFE818181},{0xFF8080FF, 0xFF8080FF}, {0xFF8080FF, 0x80808080} };

//int bigCh_p[2] = {0xFF181818, 0x181818FF};

void printInfo();
struct itimerval nval, oval;
enum flags {P, O, S, I, E, NONE};
int cols, rows, braked = 0;
void printFlags();

void printall(void) /*Вывод рамок и информационного текста*/
{
	int XUI[4][2] = {{0x81422418,0x18244281}, {0x81422418,0x10204080},{0x18838589,0x91A1C181},{0x0,0x11172421}};
	mt_setfgcolor(black);
	bc_box(2,2, 92, 25); //main frame
	printf("\n");
	bc_box(3, 4, 61, 11); //Memory
	bc_box(15, 4, 50, 10); //bigchar field
	bc_box(15, 54 + 3, 29, 8); //keys
	bc_box(3, 68, 20, 2); //Accumulator
	bc_box(6, 68, 20, 2); //Instruction counters
	bc_box(9, 68, 20, 2); //Operation
	bc_box(12, 68, 20, 2); //Flags
	mt_setfgcolor(blue);
	mt_gotoXY(3, 63 / 2);
	write(1,"Memory", 7);
	mt_gotoXY(3, 73 );
	write(1,"Accumulator", 12);
	mt_gotoXY(6, 70);
	write(1,"InstructionCounter", 19);
	mt_gotoXY(9, 73);
	write(1,"Operation", 10);
	mt_gotoXY(12, 73 );
	write(1,"Flags", 6);
	mt_gotoXY(15, 70);
	write(1,"Keys", 4);
	mt_setfgcolor(black);
	mt_gotoXY(16, 59);
	write(1,"l  - load", 9);
	mt_gotoXY(17, 59);
	write(1,"s  - save", 9);
	mt_gotoXY(18, 59);
	write(1,"r  - run", 8);
	mt_gotoXY(19, 59);
	write(1,"t  - step", 9);
	mt_gotoXY(20, 59);
	write(1,"i  - reset", 11);
	mt_gotoXY(21 , 59);
	write(1,"F5 - accumulator", 16);
	mt_gotoXY(22, 59);
	write(1,"F6 - instructionCounter", 24);
	//To Bigchar field
	//mt_gotoXY (180, 4);
	//bc_printbigchar(XUI[1], rows / 5 + rows / 11, 7, NAN, NAN);
	//bc_printbigchar(XUI[2], rows / 5 + rows / 11, 17, NAN, NAN);
	mt_gotoXY (cols - 5, rows);
}

void Break (int sig);

void setFlag(enum flags flag)
{
	//if ()
	mt_setbgcolor(NAN + 1);
	mt_gotoXY(13, 74 + flag + flag);
	mt_setfgcolor(red);
	
	switch (flag) {
		case P : write(1, "P", 1);
			break;
		case O : write(1, "0", 1);
			break;
		case S : write(1, "S", 1);
			break;
		case I : write(1, "I", 1);
			break;
		case E : write(1, "E", 1);
			break;
		case NONE : Break(0);
			break;
	}
	mt_setfgcolor(white);
}

void printBIG_Char(int value)
{
	int command = 0, operand = 0, pos[4];
	sc_commandDecode(value, &command, &operand);
	pos[1] = command & 0xF;
	pos[0] = (command >> 4) & 0xF;
	pos[3] = operand & 0xF;
	pos[2] = (operand >> 4) & 0xF;
	bc_printbigchar(BIG[pos[0]], 16, 6, NAN + 1, NAN + 1);
	bc_printbigchar(BIG[pos[1]], 16, 16, NAN + 1, NAN + 1);
	bc_printbigchar(BIG[pos[2]], 16, 26, NAN + 1, NAN + 1);
	bc_printbigchar(BIG[pos[3]], 16, 36, NAN + 1, NAN + 1);
}

void printWindow()
{
	if (idx > 99) {
		printFlags();
		setFlag(S);
		idx = 99;
	}
	else
		printFlags();
	char *chvalue = calloc(sizeof(chvalue), 4);
	//int cols, rows;
	//mt_getscreensize(&cols, &rows);
	int i = 0, j = 1, x = 4, y = 5;//cols - (cols / 2) + cols / 6;
	int value = 0;
	mt_setfgcolor(white);
	mt_gotoXY(x, y);
	for (i = 0; i < 100; i++) {
		if (i % 10 == 0 && i != 0) {
			mt_gotoXY(x + j, y);
			j++;
		}
		sc_memoryGet(i, &value);
		if (idx == i) {
			//mt_gotoXY(7, cols - cols / 4);
			mt_setbgcolor(yellow);
			//mt_gotoXY(x + j, y);
		} else
			mt_setbgcolor(purple);
		int len = sprintf(chvalue, "+%.2X%.2X " , (value >> 8), (value & 0x7F));
		write(1, chvalue, len);
		mt_setfgcolor(white);
	}
}

		

void printFlags()
{
	int value = 0, command = 0, operand = 0;
	mt_setbgcolor(NAN + 1);
	mt_setfgcolor(black);
	mt_gotoXY(13, 74 + P);
	write(1, "P", 1);
	mt_gotoXY(13, 75 + O);
	write(1, "O", 1);
	mt_gotoXY(13, 76 + S);
	write(1, "S", 1);
	mt_gotoXY(13, 77 + I);
	write(1, "I", 1);
	mt_gotoXY(13, 78 + E);
	write(1, "E", 1);
	sc_regSet(TF, 0);
	sc_memoryGet(idx, &value);
	sc_commandDecode(value, &command, &operand);
	sc_regGet(TF, &value);	
	if(value)
		setFlag(E);
}

void Break (int sig)
{
	if (!braked)
		idx = 0;
	else
		idx--;
	printWindow();
	printInfo();
	printFlags();
}

void signalhandler (int signo)
{
    //signal (SIGUSR1, handler2);
    //printf ("%d\n", idx);
	/*if (signo == SIGUSR1) {
		alarm(0);
		idx = 0;
		printWindow();
		printInfo();
	}*/
	//if (signo == SIGALRM)
		
		printWindow();
		setFlag(I);
		printInfo();
		idx++;
	/*if (idx == 99) {
	    //alarm(0);
		nval.it_interval.tv_sec = 0;
		nval.it_interval.tv_usec = 0;
		nval.it_value.tv_sec = 0;
		nval.it_value.tv_usec = 0;
		setitimer (ITIMER_REAL, &nval, &oval);
		idx = 0;
		return;
	    //raise(SIG_DFL);//SIGUSR1);
	}
    idx++;*/
}
void run (void)
{
    int i;
    signal (SIGALRM, signalhandler);
    signal (SIGUSR1, Break);
    nval.it_interval.tv_sec = 1; //And set the interval to 3second, 500 usecond repeatly
    nval.it_interval.tv_usec = 200;
    nval.it_value.tv_sec = 1;//from 1 to 0 second and 0 usecond
    nval.it_value.tv_usec = 0;
    /* Запускаем таймер */

     setitimer (ITIMER_REAL, &nval, &oval);
    //printf("%d", ITIMER_REAL);
	enum keys key;
	rk_mytermregime(0, 0, 0, 0, 1);
     for (i = idx; i <= 100; i++) {
		//if (idx == 99) {
			//raise(SIGALRM);
			rk_readkey(&key);
			if (key == key_esc) {
				braked = 1;
				break;
			}
			pause();
		//}
	}
	alarm(0);
	raise(SIGUSR1);
	//printWindow();
	//printInfo();
}

void printInfo()
{
	int value = 0, command = 0, operand = 0;
	char *counter = calloc(sizeof(counter), 10);
	//mt_getscreensize(&cols, &rows);
	mt_setbgcolor(NAN + 1);
	mt_setfgcolor(black);
	mt_gotoXY(4, 75);
	int len = sprintf(counter, "%.2X", AKKUM);
	write(1, counter, len);
	mt_gotoXY(7, 75);
	len = sprintf(counter, "%2.2d", idx);
	mt_setbgcolor(NAN + 1);
	mt_setfgcolor(black);
	write(1, counter, len);
	sc_memoryGet(idx, &value);
	sc_commandDecode(value, &command, &operand);
	mt_gotoXY(10, 74);
	len = sprintf(counter, "+%.2X:%.2X", command, operand);
	mt_setbgcolor(NAN + 1);
	mt_setfgcolor(black);
	write(1, counter, len);
	/*mt_gotoXY(13, cols - cols / 5 + P);
	write(1, "P", 1);
	mt_gotoXY(13, cols - cols / 5 + O + 1);
	write(1, "O", 1);
	mt_gotoXY(13, cols - cols / 5 + S + 2);
	write(1, "S", 1);
	mt_gotoXY(13, cols - cols / 5 + I + 3);
	write(1, "I", 1);
	mt_gotoXY(13, cols - cols / 5 + E + 4);
	write(1, "E", 1);*/
	printBIG_Char(value);
	mt_gotoXY(cols, rows);
	//sc_regGet(TF, &value);
	//if (value == 1) {
		
	//}
}
/*
void loadMemory(int sig)
{
			char *filename = calloc(sizeof(filename), 100);
			mt_gotoXY(30, 2);
			//rk_mytermregime(1, 0, 0, 0, 1);
			write(1, "Input name of file: ", sizeof("Input name of file: "));
			mt_gotoXY(30, 10);
			scanf("%s", filename)read(0, filename, sizeof(filename));
			if (sc_memoryLoad(filename) == -1)
				printf("Load error");
}*/

void reset()
{
	sc_memoryInit();
	mt_clrscr();
	printall();
	printWindow();
	printInfo();
	printFlags();
	mt_gotoXY(30,5);
}

int main()
{
	//char *filename = calloc(sizeof(filename), 100);
	int flag = 0, value = 0;//,idx = 0;
	mt_getscreensize(&cols, &rows);
	mt_clrscr();
	sc_regInit();
	printall();
	printWindow();
	printInfo();
	printFlags();
	sc_memorySet(44, 0x6F05);
	/*sc_memorySet(98, 5);
	sc_memorySet(99, 10);
	sc_memorySet(1, 35);
	sc_memorySet(2, 63);
	sc_memorySet(3, 0x7F);
	sc_memorySet(4, 11);
	sc_memorySet(10, 2);
	sc_memorySet(15, 1);
	sc_memorySet(50, 100);*/
	//if (sc_memorySave("testfile.dat") == -1)
		//printf("Save error");
	//if (sc_memoryLoad("testfile.dat") == -1)
	//	printf("Load error");
	//printWindow(0);
	enum keys key;
	rk_mytermregime(0, 0, 0, 0, 1);
	while(key != key_esc) {
		flag = rk_readkey(&key);
		if (flag == 0){
			switch (key) {
		case key_L : { 
			if (sc_memoryLoad("testfile.dat") == -1) {
				mt_gotoXY(30, 2);
				write(1, "Load error", sizeof("Load error"));
			}
			printWindow();
			printInfo();
			//rk_mytermregime(0, 0, 0, 0, 1);
		    }
			break;
		case key_S : { 
			if (sc_memorySave("testfile.dat") == -1) {
				mt_gotoXY(30, 2);
				write(1, "Save error", sizeof("Save error"));
			}
			printWindow();
			printInfo();
			}
			break;
		case key_R : run();
			break;
		case key_T : {
			idx++;
			/*if (idx > 99) {
				idx = 99;
				setFlag(S);
				
			}*/
			printWindow();
			printInfo();
			//printFlags();
			}
			break;
		case key_I : reset();
			break;
		case key_F5 : {
				rk_mytermregime(1, 0, 0, 1, 1);
				printf("Input Accumulator: ");
				scanf("%X", &value);
				AKKUM = value;
				rk_mytermregime(0, 0, 0, 0, 1);
				mt_clrscr();
				printall();
				printWindow();
				printInfo();
			}
			break;
		case key_F6 : {
			rk_mytermregime(1, 0, 0, 1, 1);
		    printf("Input InstructionCounter: ");
		    scanf("%d", &idx);
		    rk_mytermregime(0, 0, 0, 0, 1);
		    mt_clrscr();
		    printall();
		    printWindow();
		    printInfo();
			}
			break;
		case key_up : {
			if (idx / 10 == 0) idx += 90;
			else
				idx -= 10;
			printWindow();
			printInfo();
		}
			break;
		case key_down : {
			if (idx / 10 == 9) {
				idx -= 90;
			} else
				idx += 10;
			printWindow();
			printInfo();
		}
			break;
		case key_left : {
			if (idx < 0 || idx % 10 == 0) idx += 9;
			else
				idx--;
			printWindow();
			printInfo();
		}
			break;
		case key_right : {
			if ((idx - 9) % 10 == 0) idx -= 9;
			else
				idx++;
			printWindow();
			printInfo();
		}
			break;
		case key_enter : {
					rk_mytermregime(1,0,0,1,1);
					int value = 0, command = 0, operand = 0;
					mt_gotoXY(30, 40);
					printf("\nInput command: ");
					scanf("%X", &command);
					printf("Input operand: ");
					scanf("%X", &operand);
					if (command > 0x7F || operand > 0x7F)
						setFlag(P);
					int e = sc_commandEncode(command, operand, &value);
					if (e) {
						mt_clrscr();
					printall();
					printWindow();
					printInfo();
						rk_mytermregime(0, 0, 0, 0, 1);
						break;
					}
					sc_memorySet(idx, value);
					sc_regGet(TF, &value);
					
					rk_mytermregime(0, 0, 0, 0, 1);
					mt_clrscr();
					printall();
					printWindow();
					printInfo();
					if(value) {
						setFlag(E);
						//printall();
						//printWindow();
						//printInfo();
					}
					
		}
			break;
//		case key_esc : printf("ESC\n");
//			break;
//		case key_NANI : printf("何?\n");
//			break;
//		case key_MUDA : printf("MUDA\n");
//			break;
//		case key_ORA : printf("ORA\n");
//			break;
//		case what : printf("what?!\n");
		}
		flag = 1;
	}
	}
	
	mt_gotoXY(cols, rows);
	/*int XUI[4][2] = {{0x81422418,0x18244281}, {0x81422418,0x10204080},{0x18838589,0x91A1C181},{0x0,0x11172421}};
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
	printf("%d", count);*/
	return 0;
}