#ifndef LAB3_H
#define LAB3_H

int bc_printA (const char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(int *big, int x, int y, int fgcolor, int bgcolor);
int bc_getbigcharpos(int * big, int x, int y, int * value);
int bc_bigcharwrite(int fd, int * big, int count);
int bc_bigcharread (int fd, int *big, int need_count, int * read_count);

#endif
