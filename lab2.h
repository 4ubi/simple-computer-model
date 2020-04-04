#ifndef LAB2_H
#define LAB2_H

enum colors {black, red, green, yellow, blue, purple, aqua, white, NAN};

int mt_clrscr(void);
int mt_gotoXY (int X, int Y);
int mt_setfgcolor(enum colors color);
int mt_setbgcolor(enum colors color);
int mt_getscreensize(int *cols, int *rows);


#endif
