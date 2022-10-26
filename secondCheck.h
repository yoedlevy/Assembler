#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "getCode.h"
#define commandLength 30
#define binaryCodeLength 10

typedef struct node2 * ICLinePtr; /*pointer to node at the linked list*/ 

typedef struct node2 /*creates the linked list of lines in the code*/
{
	int IC;
	int line;
	ICLinePtr next;
}ICLine;

ICLinePtr head = NULL;

char command[commandLength];
int index;
int secCheckIC = 100;
int lineNum = 1;
char binaryCodeLine[binaryCodeLength];

void secondCheck(linePtr);
int passSymbol(char[]);
void getCommand2(char[]);
void changeSymbolToEntry(char[]);
void getBinaryCode(char[]);
void addBinaryCode(char);
void addOpCodeLineToCode(int, int, int);
void addToICLineList(ICLinePtr *, int, int);
int getICFromICLineTable(int line);
