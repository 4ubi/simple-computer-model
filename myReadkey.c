#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "lab4.h"
#include "lab2.h"
/*
#define KEY_ENTER "\n"
#define KEY_F5 "\E[15~"
#define KEY_F6 "\E[17~"
#define KEY_UP "\E[A"
#define KEY_DOWN "\E[B"
#define KEY_RIGHT "\E[C"
#define KEY_LEFT "\E[D"

enum keys {key_L = 0, key_S, key_NANI, key_R, key_MUDA, key_ORA, key_T, key_I,
			key_F5, key_F6, key_up, key_down, key_left, key_right,
			key_enter, key_esc, what};*/

struct termios term;


int rk_readkey(enum keys *key){
	char buf;
	int readRes;
	//*key = what;
	readRes = read(0, &buf, 1);
	if (!readRes){
		return -1;
	}
	switch (buf){
		case 'l': *key = key_L;
				break;
		case 's': *key = key_S;
				break;
		//case 'n' : *key = key_NANI;
		//		break;
		case 'r': *key = key_R;
				break;
		//case 'm' : *key = key_MUDA;
		//		break;
		//case 'o' : *key = key_ORA;
		//		break;
		case 't': *key = key_T;
				break;
		case 'i': *key = key_I;
				break;
		case '\n': *key = key_enter;
				break;
		case '\E': {
			*key = key_esc;
			readRes = read(0, &buf, 1);
			if (buf == '['){
				//*key = what;
				readRes = read(0, &buf, 1);
				if (!readRes){
					return -1;
				}
				switch (buf){
					case 'A' : *key = key_up;
					break;
					case 'B' : *key = key_down;
					break;
					case 'C' : *key = key_right;
						break;
					case 'D' : *key = key_left;
						break;
					case '1' : {
						readRes = read(0, &buf, 1);
						if(!readRes){
							return -1;
						}
						switch(buf){
							case '5' : {
								read(0, &buf, 1);
								*key = key_F5;
								break;
							}
							case '7' : {
								read(0, &buf, 1);
								*key = key_F6;
									break;
							}
						}
						break;
					}
				}
			}
			break;
		}
		//default: *key = what;
		//	break;
	}
	return 0;
}

int rk_mytermsave() {
    return tcgetattr(0, &term);
}

int rk_mytermrestore() {
    return tcsetattr(0, TCSAFLUSH, &term);
}

int rk_mytermregime (int canon, int vtime, int vmin, int echo, int sigint){
	struct termios setting;	
	if (canon > 0)
		canon = 1;
	if((canon < 0) || (echo > 1) || (echo < 0) || (sigint > 1) || (sigint < 0))
		return -1;
	if((vtime < 0) || (vmin < 0))
		return -1;	
	if ((tcgetattr (0, &setting)) ==-1) 
		return -1;
	setting.c_lflag &= ~ICANON;
	setting.c_lflag &= ~ECHO;
	setting.c_lflag &= ~ISIG;
	if(canon)
		setting.c_lflag |= ICANON;
	if(echo)
		setting.c_lflag |= ECHO;
	if(sigint)
		setting.c_lflag |= ISIG;
	setting.c_cc[VTIME] = vtime;
	setting.c_cc[VMIN] = vmin;	
	if ((tcsetattr (0,TCSANOW,&setting)) == -1)
		return -1; 
	return 0;       
}
/*
int main(){
	mt_clrscr();
	enum keys key = 1;
	rk_mytermsave();
	rk_mytermregime(0, 0, 0, 0, 1);	
	int flag = 1;	
	while(key != key_esc) {					
		flag = rk_readkey(&key);
		if (flag == 0){			
			switch (key) {
				case key_L : printf("L\n");
						break;
				case key_S : printf("S\n");
						break;
				case key_R : printf("R\n");
						break;
				case key_T : printf("T\n");
						break;
				case key_I : printf("I\n");
						break;
				case key_F5 : printf("F5\n");
						break;
				case key_F6 : printf("F6\n");
						break;
				case key_up : printf("UP\n");
						break;
				case key_down : printf("DOWN\n");
						break;
				case key_left : printf("LEFT\n");
						break;
				case key_right : printf("RIGHT\n");
						break;
				case key_enter : printf("ENTER\n");
						break;
				case key_esc : printf("ESC\n");
						break;
				case key_NANI : printf("何?\n");
						break;
				case key_MUDA : printf("MUDA\n");
						break;
				case key_ORA : printf("ORA\n");
						break;
				case what : printf("what?!\n");
			}			
			flag = 1;
		}			
	}
	rk_mytermrestore();
	printf("\n");
	return 0;
}*/
