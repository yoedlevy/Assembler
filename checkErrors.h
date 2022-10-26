#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getCode.h"
#define labelLength 30

typedef struct labels * labelsToCheckPtr; /*pointer to node at the linked list*/

typedef struct labels /*creates the linked list of symbols*/
{
	int line;
	char label[labelLength];
	labelsToCheckPtr next;
}labelsToCheck;

labelsToCheckPtr labelsToCheckHead = NULL;

int index = 0;
int lineNumber = 0;
bool ifThereIsError = false;

bool ifSpecialCommand(char[]);
void addToLabelsList(labelsToCheckPtr*, int, char[]);
void checkCodeErrors(linePtr);
void checkLineErrors(char[]);
bool checkOperandErrors(char[]);
bool checkTwoOperands(char[],char[],int);
bool checkLabelName(char[]);
void ifLabelsAreExist();
