#include <stdbool.h>
#include <stdio.h>
#define symbolNameLength 30
#define valueLength 10

typedef struct symbolTable * sTablePtr; /*pointer to node at the linked list*/

typedef struct symbolTable /*creates the linked list of symbols*/
{
	char symbolName[symbolNameLength];
	int value;
	bool action;
	bool external;
	bool entry;
	sTablePtr next;
}sTable;

typedef struct dataTable * dTablePtr; /*pointer to node at the linked list*/

typedef struct dataTable /*creates the linked list of symbols*/
{
	int dc;
	char value[valueLength];
	dTablePtr next;
}dTable;

typedef struct codeTable * cTablePtr; /*pointer to node at the linked list*/

typedef struct codeTable /*creates the linked list of symbols*/
{
	int ic;
	char value[valueLength];
	cTablePtr next;
}cTable;

sTablePtr sTableHead = NULL;
dTablePtr dTableHead = NULL;
cTablePtr cTableHead = NULL;

void addSymbol(char[], int, bool, bool, bool);

void addSymbolToTable(sTablePtr *, char[], int, bool, bool, bool);

void changeSymbolEntry(char[]);

int getValueOfSymbol(char[]);

bool ifExtern(char[]);

void addICToDataSymbols(int);

void addICToData(int);

void printDataTable(FILE*);

void printCodeTable(FILE*);

void printCodeAndDataLen(FILE*);

bool ifSymbolExist(char[]);

void addCodeToTable(cTablePtr *, char[], int);

void addToCode(char[], int);

void addToData(char[], int);

void addDataToTable(dTablePtr *, char[], int);

bool ifThereIsEnt();

bool ifThereIsExt();

void addToExtFile(FILE*);

void addToEntFile(FILE*);
