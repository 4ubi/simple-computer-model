#ifndef LAB4_H
#define LAB4_H

#define KEY_ENTER "\n"
#define KEY_F5 "\E[15~"
#define KEY_F6 "\E[17~"
#define KEY_UP "\E[A"
#define KEY_DOWN "\E[B"
#define KEY_RIGHT "\E[C"
#define KEY_LEFT "\E[D"

enum keys {key_L = 0, key_S, /*key_NANI,*/ key_R, /*key_MUDA, key_ORA,*/ key_T, key_I,
			key_F5, key_F6, key_up, key_down, key_left, key_right,
			key_enter, key_esc};//, what};

int rk_readkey(enum keys *key);

int rk_mytermsave();

int rk_mytermrestore();

int rk_mytermregime (int canon, int vtime, int vmin, int echo, int sigint);

#endif
