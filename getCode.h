#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LineLength 80

typedef struct node * linePtr; /*pointer to node at the linked list*/ 

typedef struct node /*creates the linked list of lines in the code*/
{
	char data[LineLength];
	linePtr next;
}line;

void addToList(linePtr *, char[]);
void printList(linePtr h);
linePtr getCode(FILE*);

