#include "macro.h"

void getMacros(linePtr h, FILE *fpw) /*gets head of code lines list and ponter to a file, opens all the macros in the code and saves the new code at the file */
{
	bool isMacro = false;
	bool ifNext = false;
	linePtr tmp = h;
	linePtr tmp2;
	while(h->next)
	{
		tmp2 = h->next->next;
		if(ifItsNoteOrEmpty(h->next->data))
		{
			h->next = tmp2;
			ifNext = true;
		}
		else if(isMacro == false)
		{
			if(ifItsStartOrEndMacroLine(h->next->data) == 1)
			{
				memset(macroName , '\0' , macroNameLength);
				isMacro = true;
				getMacroName(h->next->data, macroName);
				h->next = tmp2;
				ifNext = true;
			}	
		}	
		else 
		{
			int firstWordCheck = ifItsStartOrEndMacroLine(h->next->data);
			if(firstWordCheck == 0)
			{
				addToList(&macroLines, h->next->data);
				h->next = tmp2;
				ifNext = true;
			}
			if(firstWordCheck == 2)
			{
				addMacroToList(&macrosHead, macroLines, macroName);
				macroLines = NULL;
				isMacro = false;
				h->next = tmp2;
				ifNext = true;
			}
		}
		if(ifNext == false)
			h = h->next;
		ifNext = false;
	}
	macroPtr tmpMacro = macrosHead;
	openMacros(tmpMacro, tmp);
	while(tmp)
	{
		fprintf(fpw, "%s", tmp->data);
		tmp = tmp->next;
	}
}

bool ifItsNoteOrEmpty(char codeLine[]) /*gets code line, checks if its a note/empty line and returns true/false */
{
	int index = 0;
	while(isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		index++;
	}
	if(codeLine[index] == '\n' || codeLine[index] == ';')
		return true;
	else
		return false;
}

int ifItsStartOrEndMacroLine(char codeLine[]) /*gets code line and returns 1 if it's line of open macro or 2 if it's line of ending macro else returns 0 */
{
	int index = 0, firstWordIndex = 0;
	char firstWord[LineLength];
	memset(firstWord , '\0' , LineLength);
	while(isspace(codeLine[index]))
	{	
		index++;
	}	
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		firstWord[firstWordIndex] = codeLine[index];
		firstWordIndex++;
		index++;
	}
	if(!strcmp(firstWord, "macro"))
		return 1;
	if(!strcmp(firstWord, "endmacro"))
		return 2;
	return 0;
}

void addMacroToList(macroPtr * macrosHead, linePtr macroLines, char * macroName[]) /*gets poiter to head of macros list, head of code lines list and pointer to macro name and adds a new element to the macros list with these variables*/
{
	macroPtr p1,tmp;
	tmp = (macroPtr) malloc(sizeof(macro));
	if(!tmp)
	{
		printf("\n cannot allocate memory \n");
		exit(0);
	}
	tmp->ln = macroLines;
	for(int i = 0; i < macroNameLength; i++)
		tmp->macroName[i] = macroName[i];
	if(!(*macrosHead)) /*checking if the list is empty*/
	{
		tmp->next = NULL;
		*macrosHead = tmp;
		return;
	}
	p1 = *macrosHead;
	while(p1->next) /*takes the pointer to the end of the list*/
	{
		p1 = p1->next;
	}
	p1->next = tmp;
}

void getMacroName(char codeLine[], char * macroName[]) /*gets code line and poiter to macro name and saves at macroName the name of the macro in this line*/
{
	int index = 0, nameIndex = 0;
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
		index++;
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		macroName[nameIndex] = codeLine[index];
		nameIndex++;
		index++;
	}
}

void openMacros(macroPtr macrosHeadTmp, linePtr tmp) /*gets head of macros list and head of code list and changes all the calls to macros in the code to the code lines of these macros*/
{
	linePtr h = tmp;
	while(h->next)
	{
		int index = 0, nameIndex = 0;
		char name[macroNameLength];
		memset(name , '\0' , macroNameLength);
		while(isspace(h->next->data[index]))
			index++;
		while(!isspace(h->next->data[index]) && h->next->data[index] != '\n')
		{
			name[nameIndex] = h->next->data[index];
			nameIndex++;
			index++;
		}
		if(checkIfItsMacro(name,macrosHeadTmp))
			changeMacroToLines(h,name,macrosHeadTmp);
		else if(h->next->data[index - 1] == ':')
		{
			memset(name , '\0' , macroNameLength);
			nameIndex = 0;
			while(isspace(h->next->data[index])  && h->next->data[index] != '\n')
				index++;
			if(h->next->data[index] != '\n')
			{
				while(!isspace(h->next->data[index]))
				{
					name[nameIndex] = h->next->data[index];
					nameIndex++;
					index++;
				}
				if(checkIfItsMacro(name,macrosHeadTmp))
					changeMacroToLines(h,name,macrosHeadTmp);
			}
		}
		memset(name , '\0' , macroNameLength);
		h = h->next;
	}
}

void changeMacroToLines(linePtr h, char name[], macroPtr macroTmp) /*gets head of list, name and head of macros list and changes at the list the macro with this name to the code lines of this macro */
{
	linePtr tmp = NULL;
	macroPtr macroHeadTmp = macroTmp;
	
	if(h->next->next)
		tmp = h->next->next;
	while(macroHeadTmp)
	{
		if(strcmp(name,macroHeadTmp->macroName) == 0)
			{
				while(macroHeadTmp->ln)
				{
					h->next = macroHeadTmp->ln;
					h = h->next;
					macroHeadTmp->ln = macroHeadTmp->ln->next;	
				}
			}
		macroHeadTmp = macroHeadTmp->next;
	}
	while(h->next)
		h = h -> next;
	h->next = tmp;
}

bool checkIfItsMacro(char name[], macroPtr macroTmp) /*gets name and head of macros list, checks if this name its a name of macro and returns true/false */
{
	macroPtr tmp = macroTmp;
	while(tmp)
	{
		if(strcmp(name,tmp->macroName) == 0)
		{
			return true;
		}
		tmp = tmp -> next;
	}
	return false;
}
