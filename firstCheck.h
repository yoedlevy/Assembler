#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "getCode.h"
#define commandLength 30
#define binaryCodeLength 10 

int IC = 100;
int DC = 0;
char symbolName[commandLength];
char binaryCodeData[binaryCodeLength];
bool isSymbol = false;
char command[commandLength];
int ICLengthOfThisLine;
int endCommandIndex;

void firstCheck(linePtr);
bool ifSymbol(char[]);
int getCommand(char[]);
void getExterns(char[]);
void saveData(char[]);
int calcICLength(char[], int);
int checkOperand(char[]);

