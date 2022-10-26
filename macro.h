#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "getCode.h"
#define macroNameLength 20

typedef struct macroNode * macroPtr; /*pointer to node at the linked list*/

typedef struct macroNode /*creates the linked list of macros*/
{
	linePtr ln;
	char macroName[macroNameLength];
	macroPtr next;
}macro;

char * macroName[macroNameLength];
macroPtr macrosHead = NULL;
linePtr macroLines = NULL;

void getMacros(linePtr, FILE*); // gets the head of the linked list (first line at assembly code)

void addMacroToList(macroPtr *, linePtr, char * macroName[]);

void getMacroName(char[], char * []);

void openMacros(macroPtr, linePtr);

bool checkIfItsMacro(char [], macroPtr);

void changeMacroToLines(linePtr, char [], macroPtr);

bool ifItsNoteOrEmpty(char[]);
