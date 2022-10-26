#include "Tables.h"

void addSymbol(char name[], int value, bool action, bool external, bool entry)/*gets name, value(IC), action(bool), external(bool) and entry(bool) and adds a new element to the list with these variables*/
{
	addSymbolToTable(&sTableHead, name, value, action, external, entry);
}
	
void addSymbolToTable(sTablePtr * TableHead, char name[], int value, bool action, bool external, bool entry) /*gets pointer to head of labels list, name, value(IC), action(bool), external(bool) and entry(bool) and adds a new element to the list with these variables*/
{
	if(ifSymbolExist(name) == false)
	{
		sTablePtr p1,tmp;
		tmp = (sTablePtr) malloc(sizeof(sTable));
		if(!tmp)
		{
			printf("\n cannot allocate memory \n");
			exit(0);
		}
		tmp->value = value;
		tmp->action = action;
		tmp->external = external;
		tmp->entry = entry;
		for(int i = 0; i < symbolNameLength; i++)
			tmp->symbolName[i] = name[i];
		if(!(*TableHead)) /*checks if the list is empty*/
		{
			tmp->next = NULL;
			*TableHead = tmp;
			return;
		}
		p1 = *TableHead;
		while(p1->next) /*takes the pointer to the end of the list*/
		{
			p1 = p1->next;
		}
		p1->next = tmp;
	}
	else
	{
		printf("Error - the label '%s' is declared more than once\n", name);
		exit(0);
	}
}

void addToData(char value[], int dc)/*gets value(bits) and DC, and adds a new element to the list with these variables*/
{
	addDataToTable(&dTableHead, value, dc);
}
	
void addDataToTable(dTablePtr * TableHead, char value[], int dc) /*gets pointer to head of data list, value(bits) and DC, and adds a new element to the list with these variables*/
{
	dTablePtr p1,tmp;
	tmp = (dTablePtr) malloc(sizeof(dTable));
	if(!tmp)
	{
		printf("\n cannot allocate memory \n");
		exit(0);
	}
	tmp->dc = dc;
	for(int i = 0; i < valueLength; i++)
		tmp->value[i] = value[i];
	if(!(*TableHead)) /*checks if the list is empty*/
	{
		tmp->next = NULL;
		*TableHead = tmp;
		return;
	}
	p1 = *TableHead;
	while(p1->next) /*takes the pointer to the end of the list*/
	{
		p1 = p1->next;
	}
	p1->next = tmp;
}

void addICToDataSymbols(int ic) /*gets IC and adds him to the value of the data labels*/
{
	sTablePtr * head = sTableHead;
	while(sTableHead)
	{
		if(sTableHead->action == false && sTableHead->external == false)
			sTableHead->value += ic;
		sTableHead = sTableHead->next;
	}
	sTableHead = head;
}

void addICToData(int ic) /*gets IC and adds him to the value of the data in the data table*/
{
	dTablePtr * head = dTableHead;
	while(dTableHead)
	{
		dTableHead->dc += ic;
		dTableHead = dTableHead->next;
	}
	dTableHead = head;
}

void addToCode(char value[], int ic) /*gets value(bits) and IC, and adds a new element to the list with these variables*/
{
	addCodeToTable(&cTableHead, value, ic);
}
	
void addCodeToTable(cTablePtr * TableHead, char value[], int ic) /*gets pointer to head of code list, value(bits) and IC, and adds a new element to the list with these variables */
{
	cTablePtr p1,tmp;
	tmp = (cTablePtr) malloc(sizeof(cTable));
	if(!tmp)
	{
		printf("\n cannot allocate memory \n");
		exit(0);
	}
	tmp->ic = ic;
	for(int i = 0; i < valueLength; i++)
		tmp->value[i] = value[i];
	if(!(*TableHead)) /*checks if the list is empty*/
	{
		tmp->next = NULL;
		*TableHead = tmp;
		return;
	}
	p1 = *TableHead;
	while(p1->next) /*takes the pointer to the end of the list*/
	{
		p1 = p1->next;
	}
	p1->next = tmp;
}

bool ifSymbolExist(char name[]) /*gets name of label, checks if there is a label with the same name and returns true/false */
{
	sTablePtr tmp = sTableHead;
	while(tmp)
	{
		if(!strcmp(name, tmp->symbolName))
			return true;
		tmp = tmp->next;
	}
	return false;
}

void changeSymbolEntry(char name[]) /*gets name of label and changes the entry of this label to true */
{
	sTablePtr tmp = sTableHead;
	while(tmp)
	{
		if(!strcmp(name, tmp->symbolName))
			tmp->entry = true;
		tmp = tmp->next;
	}
}

int getValueOfSymbol(char name[]) /*gets name of label and returns the value(IC) of the label*/
{
	sTablePtr tmp = sTableHead;
	while(tmp)
	{
		if(!strcmp(name, tmp->symbolName))
			return tmp->value;
		tmp = tmp->next;
	}
	return -1;
}

bool ifExtern(char name[]) /*gets name of label, checks if its an extern label and returns true/false */
{
	sTablePtr tmp = sTableHead;
	while(tmp)
	{
		if(!strcmp(name, tmp->symbolName))
			return tmp->external;
		tmp = tmp->next;
	}
	return false;
}

void printDataTable(FILE *fpw) /*gets pointer to file and prints there the code of the data at base 32*/
{
	dTablePtr tmp = dTableHead;
	char c1,c2;
	char c3[2];
	int val;
	int binary1,binary2;
	while(tmp)
	{
		val = atoi(tmp->value);
		val = convertBinaryToInt(val);
		binary1 = (val&992);
		binary1 >>= 5;
		binary2 = (val&31);
		c1 = convertIntToBase32(binary1);
		c2 = convertIntToBase32(binary2);
		c3[0] = convertIntToBase32((tmp->dc)/32);
		c3[1] = convertIntToBase32((tmp->dc)%32);
		fprintf(fpw,"%c%c, %c%c",c3[0],c3[1],c1,c2);
		fprintf(fpw,"\n");
		tmp = tmp->next;
	}
}

void printCodeTable(FILE *fpw) /*gets pointer to file and prints there the code at base 32 (without the data code)*/
{
	cTablePtr tmp = cTableHead;
	char c1,c2;
	char c3[2];
	int val;
	int binary1,binary2;
	while(tmp)
	{
		val = atoi(tmp->value);
		val = convertBinaryToInt(val);
		binary1 = (val&992);
		binary1 >>= 5;
		binary2 = (val&31);
		c1 = convertIntToBase32(binary1);
		c2 = convertIntToBase32(binary2);
		c3[0] = convertIntToBase32((tmp->ic)/32);
		c3[1] = convertIntToBase32((tmp->ic)%32);
		fprintf(fpw,"%c%c, %c%c",c3[0],c3[1],c1,c2);
		fprintf(fpw,"\n");
		tmp = tmp->next;
	}
}

void printCodeAndDataLen(FILE* fpw) /*gets pointer to file and prints there the  IC and the DC at base 32*/
{
	char CodeC[2];
	char DataC[2];
	memset(CodeC, '\0', 2);
	memset(DataC, '\0', 2);
	dTablePtr tmpD = dTableHead;
	cTablePtr tmpC = cTableHead;
	int countD = 0;
	int countC = 0;
	while(tmpD)
	{
		countD++;
		tmpD = tmpD->next;
	}
	while(tmpC)
	{
		countC++;
		tmpC = tmpC->next;
	}  
	if(countD > 32)
	{
		DataC[0] = convertIntToBase32((countD)/32);
		DataC[1] = convertIntToBase32((countD)%32);
	}
	else
		DataC[0] = convertIntToBase32(countD);
	if(countC > 32)
	{
		CodeC[0] = convertIntToBase32((countC)/32);
		CodeC[1] = convertIntToBase32((countC)%32);
	}
	else
		CodeC[0] = convertIntToBase32(countC);
	if(countD > 32 && countC > 32)
		fprintf(fpw,"%c%c, %c%c\n",CodeC[0],CodeC[1],DataC[0],DataC[1]);
	if(countD <= 32 && countC > 32)
		fprintf(fpw,"%c%c, %c\n",CodeC[0],CodeC[1],DataC[0]);
	if(countD > 32 && countC <= 32)
		fprintf(fpw,"%c, %c%c\n",CodeC[0],DataC[0],DataC[1]);
	if(countD <= 32 && countC <= 32)
		fprintf(fpw,"%c, %c\n",CodeC[0],DataC[0]);
}

bool ifThereIsExt() /*checks is there is extern labals and returns true/false */
{
	sTablePtr tmp = sTableHead;
	bool ifExt = false;
	while(tmp)
	{
		if(tmp->external == true)
			ifExt = true;
		tmp = tmp->next;
	}
	return ifExt;
}

bool ifThereIsEnt() /*checks is there is entry labals and returns true/false */
{
	sTablePtr tmp = sTableHead;
	bool ifEnt = false;
	while(tmp)
	{
		if(tmp->entry == true)
			ifEnt = true;
		tmp = tmp->next;
	}
	return ifEnt;
}

void addToEntFile(FILE *fpw) /*gets pointer to entry file and prints there the entry labels and the IC of them declaration*/
{
	sTablePtr tmp = sTableHead;
	char c1,c2;
	while(tmp)
	{
		if(tmp->entry == true)
		{
			c1 = convertIntToBase32((tmp->value)/32);
			c2 = convertIntToBase32((tmp->value)%32);
			fprintf(fpw,"%s   %c%c\n",tmp->symbolName,c1,c2);
		}
		tmp = tmp->next;
	}
}
