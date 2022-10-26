#include "firstCheck.h"

void firstCheck(linePtr h) /*gets pointer to the first line of the code and performs the first check*/
{
	memset(symbolName , '\0' , commandLength);
	memset(command , '\0' , commandLength);
	linePtr tmp = h;
	while(tmp)
	{
		ICLengthOfThisLine = 0;
		if(ifSymbol(tmp->data))
			isSymbol = true;
		endCommandIndex = getCommand(tmp->data);
		if(!strcmp(command, ".data") || !strcmp(command, ".string") || !strcmp(command, ".struct"))
		{
			if(isSymbol)
			{
				addSymbol(symbolName, DC, false, false, false);
				
			}
			saveData(tmp->data);
		}
		else if(!strcmp(command, ".entry") || !strcmp(command, ".extern"))
		{
			if(!strcmp(command, ".extern"))
				getExterns(tmp->data);
		}
		else
		{
			if(isSymbol)
			{
				addSymbol(symbolName, IC, true, false, false);
			}
			int opCode = getOpcode(command);
			if(opCode == 14 || opCode == 15)
				IC++;
			else
			{			
				ICLengthOfThisLine = calcICLength(tmp->data, endCommandIndex+1);
				IC += ICLengthOfThisLine;
			}
		}
		tmp = tmp->next;
		isSymbol = false;
		memset(symbolName , '\0' , commandLength);
	}
	printf("\n");
	addICToDataSymbols(IC);
	addICToData(IC);
	ifLabelsAreExist();
}

bool ifSymbol(char codeLine[]) /*gets code line, checks if there is label at the line and returns true/false */
{
	int index = 0;
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		index++;
	}
	if(codeLine[index - 1] == ':')
	{
		for(int i = 0; i < index - 1; i++)
			symbolName[i] = codeLine[i];
		return true;
	}
	return false;
}

int getCommand(char codeLine[]) /*gets code line saves the command of this line at 'command' and returns index that points to after the command*/
{
	int index = 0, commandIndex = 0;
	memset(command , '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		command[commandIndex] = codeLine[index];
		commandIndex++;
		index++;
	}
	if(isSymbol) /*if there is label at this line*/
	{
		memset(command , '\0' , commandLength);
		commandIndex = 0;
		while(isspace(codeLine[index]))
			index++;
		while(!isspace(codeLine[index]) && codeLine[index] != '\n')
		{
			command[commandIndex] = codeLine[index];
			commandIndex++;
			index++;
		}
	}
	return index;
}

void getExterns(char codeLine[])/*gets extern code line and adds the label to the symbols table*/
{
	int index = 0, symbolIndex = 0;
	char symbol[commandLength];
	memset(symbol , '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
		index++;
	while(isspace(codeLine[index]))
		index++;
	while(codeLine[index] != '\n')
	{
		while(!isspace(codeLine[index]) && codeLine[index] != ','  && codeLine[index] != '\n')
		{
			symbol[symbolIndex] = codeLine[index];
			symbolIndex++;
			index++;
		}
		if(symbolIndex != 0)
		{
			addSymbol(symbol, NULL, false, true, false);
			memset(symbol , '\0' , commandLength);
			symbolIndex = 0;
		}
		else
			index++;
	}
}

void saveData(char codeLine[])/*gets code line of data and saves binary code of this code at the data table*/
{
	int index = 0, dataIndex = 0;
	char data[commandLength];
	memset(data , '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
		index++;
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
		index++;
	while(isspace(codeLine[index]))
		index++;
	if(!strcmp(command, ".data") || !strcmp(command, ".struct"))
	{
		while(codeLine[index] != '\n' && codeLine[index] != '"')
		{
			while(!isspace(codeLine[index]) && codeLine[index] != ','  && codeLine[index] != '\n')
			{
				data[dataIndex] = codeLine[index];
				dataIndex++;
				index++;
			}
			if(dataIndex != 0)
			{
				convertIntToBinary(atoi(data), &binaryCodeData, 10);
				addToData(binaryCodeData, DC);
				DC += 1;
				ICLengthOfThisLine += 1;
				memset(data , '\0' , commandLength);
				dataIndex = 0;
			}
			else
				index++;
		}
	}
	if(!strcmp(command, ".string") || !strcmp(command, ".struct"))
	{
		if(codeLine[index] == '"')
		{
			index++;
			while(codeLine[index] != '"'  && codeLine[index] != '\n')
			{
				char c = codeLine[index];
				int ascii = (int)c;
				convertIntToBinary(ascii, &binaryCodeData, 10);
				addToData(binaryCodeData, DC);
				DC += 1;
				ICLengthOfThisLine += 1;
				index++;
			}
			memset(binaryCodeData , '0' , binaryCodeLength);
			addToData(binaryCodeData, DC);
			DC += 1;
			ICLengthOfThisLine += 1;
		}
	}
}

int calcICLength(char codeLine[], int index) /*gets code line and index that points to after the command, calculates the IC length of this line and returns this length*/
{
	int len = 1;
	int op1, op2;
	int operandIndex = 0;
	char operand[commandLength];
	memset(operand, '\0' , commandLength);
	while(isspace(codeLine[index]))
		index++;
	while(codeLine[index] != ',' && !isspace(codeLine[index]))
	{
		operand[operandIndex] = codeLine[index];
		index++;
		operandIndex++;
	}
	op1 = checkOperand(operand);
	operandIndex = 0;
	memset(operand, '\0' , commandLength);
	if(op1 >= 1 && op1 <= 4)
		len++;
	if(op1 == 4)
		len++;
	while(isspace(codeLine[index]) && codeLine[index] != '\n')
		index++;
	if(codeLine[index] != '\n')
	{	
		if(codeLine[index] == ',')
		{
			index++;
			while(isspace(codeLine[index]))
				index++;
			while(!isspace(codeLine[index]))
			{
				operand[operandIndex] = codeLine[index];
				index++;
				operandIndex++;
			}
			op2 = checkOperand(operand);
			operandIndex = 0;
			memset(operand, '\0' , commandLength);
			if(op2 >= 1 && op2 <= 4)
				len++;
			if(op2 == 4)
				len++;
			if(op1 == 2 && op2 == 2)
				len--;
		}
	}
	return len;	
}

int checkOperand(char operand[])/*gets operand returns - 0:illegal operand or empty operand, 1:number, 2:register, 3:symbol, 4:struct symbol*/ 
{
	int index = 0;
	int result = 0;
	if(operand[index] == '\0')
		return 0;
	if(operand[index] == '#')
	{
		index++;
		if(operand[index] == '+' || operand[index] == '-' || isdigit(operand[index]) == 1)
		{
			while(operand[index] != '\0')
			{
				index++;
				if(isdigit(operand[index]) == 0 && operand[index] != '\0')
					return 0;
			}
		}
		else
			return 0;
		result = 1;
	}
	if(operand[index] == 'r')
	{
		index++;
		if((int)operand[index] >= 48 && (int)operand[index] <= 55 && operand[index+1] == '\0')
			result = 2;
	}
	if(result == 0)
	{
		while(operand[index] != '\0' && operand[index] != '.' && !isspace(operand[index]))
			index++;
		if(operand[index] == '.')
			result = 4;
		else
			result = 3;
	}
	return result;
}

