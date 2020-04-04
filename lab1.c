#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lab1.h"
#include "BSTree.h"


/*/bits:
#define ZF 1 //zero flag, zero division
#define PF 2 //parity flag (偶＼ぐう)
#define ACF 4 //Auxiliary carry flag (補助＼ほじょ), segmentation fault
#define TF 8 //Trap flag (tracing) system flag, invalid command
#define RF 16 //Resume flag system flag*/

struct bstree *tree;

void PrintBuffer();

/*int buffer[100], reg,*/ int commands[39] = {0x10, 0x11, 0x20, 0x21, 0x30, 0x31, 0x32, 0x33, 0x40, 0x41, 0x42,
0x43, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
0x67, 0x68, 0x69, 0x70, 0x71, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76};


int InitBSTree()
{
    int i;
    tree = bstree_create(commands[0], "READ");
    for (i = 1; i < 39; i++)
	bstree_add(tree, commands[i], "command");
    return 0;
}

int CheckCommand(int command)
{
	if(bstree_lookup(tree, command) == NULL)
		return -1;
	return 0;
}

int sc_memoryInit()
{
	int i;
	if (buffer == NULL)
		return -1;
	for (i = 0; i < 100; i++)
			buffer[i] = 0;
	return 0;
}

int sc_memorySet(int adress, int value)
{
	if (adress > 99 || adress < 0)
		return -1;
	buffer[adress] = value;
	return 0;
}

int sc_memoryGet(int adress, int *value)
{
	if (value == NULL || adress < 0 || adress > 99) {
		sc_regSet(ACF, 1);
		return -1;
	}
	*value = buffer[adress];
	return 0;
}

int sc_memorySave(const char *filename)
{
	if (filename == NULL || filename[0] == '\0')
		return -1;
	ssize_t bytes_written;
	int fd;
	fd = open(filename, O_WRONLY);
	if(fd == -1)
		return -1;
	bytes_written = write(fd, buffer, sizeof(buffer));
	close(fd);
	if(bytes_written == -1)
		return -1;
	return 0;
}

int sc_memoryLoad(const char *filename)
{
	if (filename == NULL)
		return -1;
	ssize_t bytes_readed;
	int fd;
	fd = open(filename, S_IREAD);
	if(fd == -1)
		return -1;
	bytes_readed = read(fd, buffer, sizeof(buffer));
	close(fd);
	if(bytes_readed == -1)
		return -1;
	return 0;
}

int sc_regInit()
{
	reg = 0x0;
	return 0;
}

int sc_regSet(int regrank, int value)
{
	if(value < 0)
		return -1;
	if (value >= 1) {
		value = 1;
		reg |= regrank;
	} else
		reg &= ~regrank;
	return 0;
}

int sc_regGet(int regrank, int *value)
{
	if(value == NULL)
		return -1;
	*value = (reg & regrank) == 0 ? 0 : 1;
	return 0;
}

int sc_commandEncode(int command, int operand, int *value)
{
	if (command > 0x7F || operand > 0x7F)
		return -1;
	if (value == NULL)
		return -1;
	if (CheckCommand(command))
		sc_regSet(TF, 1);
	command <<= 8;
	*value = command | operand;
	return 0;
}

int sc_commandDecode(int value, int *command, int *operand)
{
	if (value > 0x7FFF)
		return -1;
	if (command == NULL || operand == NULL)
		return -1;
	*operand = value & 0x7F;
	*command = value & (0x7F << 8);
	*command >>= 8;
	sc_regSet(TF, 0);
	if (CheckCommand(*command))
		sc_regSet(TF, 1);
	return 0;
}

void PrintBuffer()
{
    int i;
	for (i = 0; i < 100; i++)
		printf("%d ", buffer[i]);
}
/*
int main()
{
	int value = 0, command = 0, operand = 0;
	sc_commandEncode(0x6F, 0x05, &value);
	sc_commandDecode(value, &command, &operand);
	printf("%X %X", command, operand);
	return 0;
}
*/
/*
int main()
{
	int value = 1, command = 0, operand = 0;
	struct bstree *node;
	if (sc_memoryInit())
		printf("init error");
	sc_regInit();
	if (sc_memoryInit())
		printf("init error");
	sc_memorySet(1, 5);
	sc_regGet(ACF, &value);
	if (value == 1) {
		printf("Segmentation fault");
		exit(EXIT_FAILURE);
	}
	sc_memoryGet(1, &value);
	sc_regGet(ACF, &value);
	if(value == 1) {
		printf("error has been occured in getting memory");
		exit(EXIT_FAILURE);
	}
	printf("%d", value);
	if (sc_memorySave("testfile.dat") == -1)
		printf("Save error");
	if (sc_memoryLoad("testfile.dat") == -1)
		printf("Load error");
	printf("\n");
	
	//sc_regSet(ZF, 1); //001
	//sc_regSet(ACF, 1); //101
	//sc_regSet(ACF, 0); //00001
	//printf("set bits: %d\n", reg);
	//sc_regGet(RF, &value);
	//printf("Got: %d\n", value);
	InitBSTree();
	node = bstree_lookup(tree, 0x133);
	if (node != NULL)
		printf("0x33: Value = %s\n",  node->value);
	sc_commandEncode(0x33, 0x59, &value);
	sc_regGet(TF, &value);
	if (value == 1) {
		printf("Encoding was fauled with signal 'INVALID COMMAND'\n");
		exit(EXIT_FAILURE);
	}
	printf("Encoded value: %x\n", value);
	sc_commandDecode(0x25D7, &command, &operand);
	sc_regGet(TF, &value);
	if (value == 1) {
		printf("Decoding was fauled with signal 'INVALID COMMAND'\n");
		exit(EXIT_FAILURE);
	}
	printf("Decoded command: %x and operand %x\n", command, operand);
	PrintBuffer();
	int i;
	sc_regSet(ZF, 1);
	printf("\n");
	printf("%d\n", reg);
	for (i = 0; i < 5; i++){
		sc_regGet(RF, &value);
		printf("%d", value);
		reg <<= 1;
	}
	return 0;
}*/
