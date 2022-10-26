#include "secondCheck.h"

void secondCheck(linePtr h) /*gets pointer to the first line of the code and performs the second check*/
{
	linePtr tmp = h;
	while(tmp)
	{
		index = passSymbol(tmp->data);
		getCommand2(tmp->data);
		if(strcmp(command, ".data") && strcmp(command, ".string") && strcmp(command, ".struct") && strcmp(command, ".extern"))
		{
			if(!strcmp(command, ".entry"))
			{
				changeSymbolToEntry(tmp->data);
			}
			else
			{
				getBinaryCode(tmp->data);
			}	
		}
		tmp = tmp->next;
		index = 0;
		lineNum++;
	}
}

int passSymbol(char codeLine[]) /*gets code line and returns index to after the label (if there is no label returns 0)*/
{
	int symbolIndex = 0;
	while(isspace(codeLine[symbolIndex]))
		symbolIndex++;
	while(!isspace(codeLine[symbolIndex]) && codeLine[symbolIndex] != '\n')
	{
		symbolIndex++;
	}
	if(codeLine[symbolIndex - 1] == ':')
	{
		return symbolIndex;
	}
	return 0;
}
	
void getCommand2(char codeLine[]) /*gets code line ,saves the command of the line at 'command' and advances the index to after the command*/
{
	int commandIndex = 0;
	memset(command , '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		command[commandIndex] = codeLine[index];
		commandIndex++;
		index++;
	}
}

void changeSymbolToEntry(char codeLine[]) /*gets code line, checks the label and changes the label at symbol table to entry*/
{
	char entrySymbol[commandLength];
	int symbolIndex = 0;
	memset(entrySymbol , '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		entrySymbol[symbolIndex] = codeLine[index];
		symbolIndex++;
		index++;
	}
	changeSymbolEntry(entrySymbol);
}

void getBinaryCode(char codeLine[]) /*gets code line, calculates and adds the binary code of the line in 10 bits to the code table*/
{
	int opCode = getOpcode(command);
	char firstOperand[commandLength];
	char secondOperand[commandLength];
	int operandIndex = 0;
	int value;
	int typeFirstOp,typeSecondOp;
	memset(firstOperand , '\0' , commandLength);
	memset(secondOperand , '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n' && codeLine[index] != ',')
	{
		firstOperand[operandIndex] = codeLine[index];
		operandIndex++;
		index++;
	}
	operandIndex = 0;
	
	if(firstOperand[0] != '\0') // if there is first operand
	{
		while((isspace(codeLine[index]) || codeLine[index] == ',') && codeLine[index] != '\n')
			index++;
		while(!isspace(codeLine[index]) && codeLine[index] != '\n')
		{
			secondOperand[operandIndex] = codeLine[index];
			operandIndex++;
			index++;
		}
		typeFirstOp = checkOperand(firstOperand);
		typeSecondOp = checkOperand(secondOperand);
		addOpCodeLineToCode(opCode, typeFirstOp, typeSecondOp);
		if(typeFirstOp == 1) // if its a number
		{
			memmove(firstOperand, firstOperand+1, strlen(firstOperand));
			value = atoi(firstOperand);
			convertIntToBinary(value, &binaryCodeLine, 8);
			addBinaryCode('A');
		}
		if(typeSecondOp != 2 && typeFirstOp == 2) // if there is no second operand and the first one is register
		{
			memmove(firstOperand, firstOperand+1, strlen(firstOperand));
			value = atoi(firstOperand);
			convertIntToBinary(value, &binaryCodeLine, 4);
			convertToBinarySecReg(0, &binaryCodeLine);
			addBinaryCode('A');
		}
		if(typeFirstOp == 3) // if its a symbol
		{
			value = getValueOfSymbol(firstOperand);
			convertIntToBinary(value, &binaryCodeLine, 8);
			if(ifExtern(firstOperand))
				addBinaryCode('E');
			else
				addBinaryCode('R');
			
		}
		if(typeFirstOp == 4) // if its a struct symbol
		{
			int firstOpIndex = 0;
			char symbolName[commandLength];
			memset(symbolName , '\0' , commandLength);
			while(firstOperand[firstOpIndex] != '.')
			{
				symbolName[firstOpIndex] = firstOperand[firstOpIndex];
				firstOpIndex++;
			}
			value = getValueOfSymbol(symbolName);
			convertIntToBinary(value, &binaryCodeLine, 8);
			if(ifExtern(symbolName))
				addBinaryCode('E');
			else
				addBinaryCode('R');
			memmove(firstOperand, firstOperand+firstOpIndex+1, strlen(firstOperand));
			value = atoi(firstOperand);
			convertIntToBinary(value, &binaryCodeLine, 8);
			addBinaryCode('A');
		}
	}
	if(secondOperand[0] != '\0') // if there is second operand
	{
		typeSecondOp = checkOperand(secondOperand);
		if(typeSecondOp == 1)  // if its a number
		{
			memmove(secondOperand, secondOperand+1, strlen(secondOperand));
			value = atoi(secondOperand);
			convertIntToBinary(value, &binaryCodeLine, 8);
			addBinaryCode('A');
		}
		if(typeSecondOp == 2) // if its a register
		{
			if(typeFirstOp == 2) // if the first operand is also register
			{
				memmove(firstOperand, firstOperand+1, strlen(firstOperand));
				value = atoi(firstOperand);
				convertIntToBinary(value, &binaryCodeLine, 4);
				memmove(secondOperand, secondOperand+1, strlen(secondOperand));
				value = atoi(secondOperand);
				convertToBinarySecReg(value, &binaryCodeLine);
				addBinaryCode('A');
			}
			else
			{
				memmove(secondOperand, secondOperand+1, strlen(secondOperand));
				value = atoi(secondOperand);
				convertIntToBinary(0, &binaryCodeLine, 4);
				convertToBinarySecReg(value, &binaryCodeLine);
				addBinaryCode('A');
			}
		}
		if(typeSecondOp == 3) // if its a symbol
		{
			value = getValueOfSymbol(secondOperand);
			convertIntToBinary(value, &binaryCodeLine, 8);
			if(ifExtern(secondOperand))
				addBinaryCode('E');
			else
				addBinaryCode('R');
		}
		if(typeSecondOp == 4) // if its a struct symbol
		{
			int secOpIndex = 0;
			char symbolName[commandLength];
			memset(symbolName , '\0' , commandLength);
			while(secondOperand[secOpIndex] != '.')
			{
				symbolName[secOpIndex] = secondOperand[secOpIndex];
				secOpIndex++;
			}
			value = getValueOfSymbol(symbolName);
			convertIntToBinary(value, &binaryCodeLine, 8);
			if(ifExtern(symbolName))
				addBinaryCode('E');
			else
				addBinaryCode('R');
			memmove(secondOperand, secondOperand+secOpIndex+1, strlen(secondOperand));
			value = atoi(secondOperand);
			convertIntToBinary(value, &binaryCodeLine, 8);
			addBinaryCode('A');
		}
		
	}
	else if(secondOperand[0] == '\0' && firstOperand[0] == '\0')
		addOpCodeLineToCode(opCode, 0, 0);
}

void addBinaryCode(char ARE) /*gets char 'A','R' or 'E' adds accordingly the 2 last bits to the binary code of the current line, adds the binary code to the code table and increase the IC by 1*/
{
	if(ARE == 'A')
	{
		binaryCodeLine[8] = '0';
		binaryCodeLine[9] = '0';
	}
	if(ARE == 'R')
	{
		binaryCodeLine[8] = '1';
		binaryCodeLine[9] = '0';
	}
	if(ARE == 'E')
	{
		binaryCodeLine[8] = '0';
		binaryCodeLine[9] = '1';
	}
	addToCode(binaryCodeLine, secCheckIC);
	if(ARE == 'E')
		addToICLineList(&head, secCheckIC, lineNum);
	secCheckIC++;
	memset(binaryCodeLine , '\0' , binaryCodeLength);
}

void addOpCodeLineToCode(int opCode, int typeFirOp, int typeSecOp) /*gets opcode of command and types of 2 operands, calculates addressing methods and adds the binary code of the current line to the code table*/
{
	convertIntToBinary(opCode, &binaryCodeLine, 4);
	if(typeFirOp == 0)
		convertToBinarySecReg(0, &binaryCodeLine);
	if(typeFirOp == 1)
	{
		if(typeSecOp == 0 || typeSecOp == 1)
			convertToBinarySecReg(0, &binaryCodeLine);
		if(typeSecOp == 2)
			convertToBinarySecReg(3, &binaryCodeLine);
		if(typeSecOp == 3)
			convertToBinarySecReg(1, &binaryCodeLine);
		if(typeSecOp == 4)
			convertToBinarySecReg(2, &binaryCodeLine);
	}
	if(typeFirOp == 2)
	{
		if(typeSecOp == 0)
			convertToBinarySecReg(3, &binaryCodeLine);
		if(typeSecOp == 1)
			convertToBinarySecReg(12, &binaryCodeLine);
		if(typeSecOp == 2)
			convertToBinarySecReg(15, &binaryCodeLine);
		if(typeSecOp == 3)
			convertToBinarySecReg(13, &binaryCodeLine);
		if(typeSecOp == 4)
			convertToBinarySecReg(14, &binaryCodeLine);
	}
	if(typeFirOp == 3)
	{
		if(typeSecOp == 0)
			convertToBinarySecReg(1, &binaryCodeLine);
		if(typeSecOp == 1)
			convertToBinarySecReg(4, &binaryCodeLine);
		if(typeSecOp == 2)
			convertToBinarySecReg(7, &binaryCodeLine);
		if(typeSecOp == 3)
			convertToBinarySecReg(5, &binaryCodeLine);
		if(typeSecOp == 4)
			convertToBinarySecReg(6, &binaryCodeLine);
	}
	if(typeFirOp == 4)
	{
		if(typeSecOp == 0)
			convertToBinarySecReg(2, &binaryCodeLine);
		if(typeSecOp == 1)
			convertToBinarySecReg(8, &binaryCodeLine);
		if(typeSecOp == 2)
			convertToBinarySecReg(11, &binaryCodeLine);
		if(typeSecOp == 3)
			convertToBinarySecReg(9, &binaryCodeLine);
		if(typeSecOp == 4)
			convertToBinarySecReg(10, &binaryCodeLine);
	}
	addBinaryCode('A');
}

void addToICLineList(ICLinePtr *h, int IC, int Line) /*gets pointer to head of linked list, IC and line number and adds a new element to the list with these variables*/
{
	ICLinePtr p1,tmp;
	tmp = (ICLinePtr) malloc(sizeof(ICLine));
	if(!tmp)
	{
		printf("\n cannot allocate memory \n");
		exit(0);
	}
	tmp->IC = IC;
	tmp->line = Line;
	if(!(*h)) /*checks if the list is empty*/
	{
		tmp->next = NULL;
		*h = tmp;
		return;
	}
	p1 = *h;
	while(p1->next) /*takes the pointer to the end of the list*/
	{
		p1 = p1->next;
	}
	p1->next = tmp;
}

int getICFromICLineTable(int line) /*gets line number and returns the IC at this line*/
{
	ICLinePtr tmp = head;
	while(tmp)
	{
		if(tmp->line == line)
			return tmp->IC;
		tmp = tmp->next;
	}
}
