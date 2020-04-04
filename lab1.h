#ifndef LAB1_H
#define LAB1_H
#include "BSTree.h"

#define ZF 1 //zero flag
#define PF 2 //parity flag (偶＼ぐう)
#define ACF 4 //Auxiliary carry flag (補助＼ほじょ)
#define TF 8 //Trap flag (tracing) system flag
#define RF 16 //Resume flag system flag

int buffer[100], reg;/*, commands[39] = {0x10, 0x11, 0x20, 0x21, 0x30, 0x31, 0x32, 0x33, 0x40, 0x41, 0x42,
0x43, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
0x67, 0x68, 0x69, 0x70, 0x71, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76};*/


int CheckCommand(int command);
int sc_memoryInit();
int sc_memoryGet(int adress, int *value);
int sc_memorySet(int adress, int value);
int sc_memorySave(const char *filename);
int sc_memoryLoad(const char *filename);
int sc_regInit();
int sc_regSet(int regrank, int value);
int sc_regGet(int regrank, int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value, int *command, int *operand);


#endif
