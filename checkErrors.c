#include "checkErrors.h"

void checkCodeErrors(linePtr head) /*gets head of code lines list and checks erros in the code*/
{
	linePtr tmp = head;
	while(tmp)
	{
		lineNumber++;
		checkLineErrors(tmp->data);
		tmp = tmp->next;
		index = 0;
	}
	if(ifThereIsError == true)
		exit(0);
}

void checkLineErrors(char codeLine[]) /*gets code line and checks errors in the code*/
{
	char tmp[labelLength];
	memset(tmp,'\0',labelLength);
	int tmpIndex = 0;
	int counter = 0;
	bool thereIsLabel = false;
	bool ifThereisComma, error;
	char op1[labelLength];
	char op2[labelLength];
	memset(op1,'\0',labelLength);
	memset(op2,'\0',labelLength);
	int op;
	char symbName[labelLength];
	memset(symbName,'\0',labelLength);
	while(isspace(codeLine[index]))
		index++;
	while(!isspace(codeLine[index]) && codeLine[index] != '\n')
	{
		symbName[index] = codeLine[index];
		counter++;
		index++;
	}
	if(codeLine[index - 1] == ':') // if there is a labal
	{
		thereIsLabel =true;
		symbName[index-1] = '\0';
		if(checkLabelName(symbName) == true)
			ifThereIsError = true;
	}
	else
		index = index - counter; // returns the index to before the command
	while(isspace(codeLine[index]) && codeLine[index] != '\n')
		index++;
	if(codeLine[index] == '\n')
	{
		printf("Line:%d, Error - the label '%s' is empty\n",lineNumber,symbName);
		ifThereIsError = true;
	}
	else
	{
		while(!isspace(codeLine[index]))
		{
			tmp[tmpIndex] = codeLine[index];
			tmpIndex++;
			index++;
		}
		if(ifSpecialCommand(tmp) == false)
		{
			op = getOpcode(tmp);
			if(op == -1)
			{
				printf("Line:%d, Error - '%s' is illegal command\n",lineNumber,tmp);
				ifThereIsError = true;
			}
			else
			{
				while(isspace(codeLine[index]) && codeLine[index] != '\n')
					index++;
				if(codeLine[index] == '\n') // if its the end of the line (there is no operands)
				{
					if(op != 14 && op != 15)
					{
						printf("Line:%d, Error - the command '%s' need to get operands\n",lineNumber,tmp);
						ifThereIsError = true;
					}
				}
				else
				{
					if(op == 14 || op == 15)
					{
						printf("Line:%d, Error - the command '%s' can't get operands\n",lineNumber,tmp);
						ifThereIsError = true;
					}
					tmpIndex = 0;
					while(!isspace(codeLine[index]) && codeLine[index] != ',')
					{
						op1[tmpIndex] = codeLine[index];
						tmpIndex++;
						index++; 
					}
					while(isspace(codeLine[index]) && codeLine[index] != '\n')
						index++;
					if(codeLine[index] == '\n') // if its the end of the line (there is one operand)
					{
						if(op == 0 || op == 1 || op == 2 || op == 3 || op == 6)
						{
							printf("Line:%d, Error - the command '%s' need to get 2 operands\n",lineNumber,tmp);
							ifThereIsError = true;
						}
						if(checkOperand(op1) == 1 && op != 12)
						{
							printf("Line:%d, Error - the command '%s' can't get number\n",lineNumber,tmp);
							ifThereIsError = true;
						}
						if(checkOperandErrors(op1) == true)
							ifThereIsError = true;
					}
					else // if there is another operand
					{
						if(op == 4 || op == 5 || (op >= 7 && op <= 13))
						{
							printf("Line:%d, Error - the command '%s' can't get 2 operands\n",lineNumber,tmp);
							ifThereIsError = true;
						}
						if(checkOperandErrors(op1) == true)
							ifThereIsError = true;
						tmpIndex = 0;
						while(isspace(codeLine[index]) || codeLine[index] == ',')
							index++;
						while(!isspace(codeLine[index]))
						{
							op2[tmpIndex] = codeLine[index];
							tmpIndex++;
							index++; 
						}
						if(checkOperandErrors(op2) == true)
							ifThereIsError = true;
						else if(checkTwoOperands(op1,op2,op) == true)
							ifThereIsError = true;	
						while(isspace(codeLine[index]) && codeLine[index] != '\n')
							index++;
						if(codeLine[index] != '\n')
						{
							printf("Line:%d, Error - '%s' - command can't get more then 2 operands\n",lineNumber,tmp);
							ifThereIsError = true;
						}	
					}
				}
			}
		}
		else
		{
			if(!strcmp(tmp,".entry") || !strcmp(tmp,".extern"))
			{
				if(thereIsLabel == true)
					printf("Line:%d, Warning - the '%s' label has no meaning \n",lineNumber,symbName);
				while(isspace(codeLine[index]) && codeLine[index] != '\n')
					index++;
				if(codeLine[index] == '\n')
				{
					printf("Line:%d, Error - the command '%s' should have a label\n",lineNumber,tmp);
					ifThereIsError = true;
				}
				else
				{
					tmpIndex = 0;
					while(!isspace(codeLine[index]))
					{
						op1[tmpIndex] = codeLine[index];
						tmpIndex++;
						index++; 
					}
					if(checkOperand(op1) != 3)
					{
						printf("Line:%d, Error - the command '%s' should have a label\n",lineNumber,tmp);
						ifThereIsError = true;
					}
					else if(checkLabelName(op1) == false)
					{
						while(isspace(codeLine[index]) && codeLine[index] != '\n')
							index++;
						if(codeLine[index] != '\n')
						{
							printf("Line:%d, Error - '%s' - command can't get more then one label\n",lineNumber,tmp);
							ifThereIsError = true;
						}
						else if(!strcmp(tmp,".entry"))
							addToLabelsList(&labelsToCheckHead, lineNumber, op1);
					}
				}
			}
			else if(!strcmp(tmp,".data"))
			{
				if(thereIsLabel == false)
					printf("Line:%d, Warning - the data is not saved in a label\n",lineNumber);
				while(isspace(codeLine[index]) && codeLine[index] != '\n')
					index++;
				if(codeLine[index] == '\n')
				{
					printf("Line:%d, Error - the command '%s' should have a numbers\n",lineNumber,tmp);
					ifThereIsError = true;
				}
				else
				{
					ifThereisComma = true;
					error = false;
					while(codeLine[index] != '\n')
					{
						if(ifThereisComma == false)
						{
							printf("Line:%d, Error - there is no ',' between the numbers\n",lineNumber);
							ifThereIsError = true;
						}
						ifThereisComma = false;
						if(codeLine[index] == '+' || codeLine[index] == '-' || isdigit(codeLine[index]))
							index++;
						else
						{
							error = true;
							index++;
						}
						while(codeLine[index] != ',' && !isspace(codeLine[index]))
						{
							if(isdigit(codeLine[index]))
								index++;
							else
							{
								error = true;
								index++;
							}
						}
						while(codeLine[index] == ',' || (isspace(codeLine[index]) && codeLine[index] != '\n'))
						{
							if(codeLine[index] == ',')
							{
								if(ifThereisComma == true)
								{
									printf("Line:%d, Error - there is too many ',' between the numbers\n",lineNumber);
									ifThereIsError = true;
								}
								else
									ifThereisComma = true;
							}
							index++;
						}	
					}
					if(error == true)
					{
						printf("Line:%d, Error - the command '%s' should have only numbers\n",lineNumber,tmp);
						ifThereIsError = true;
					}
				}
			}
			else // if its .string or .struct
			{
				if(thereIsLabel == false)
					printf("Line:%d, Warning - the data is not saved in a label\n",lineNumber);
				while(isspace(codeLine[index]) && codeLine[index] != '\n')
					index++;
				if(codeLine[index] == '\n')
				{
					printf("Line:%d, Error - the command '%s' should have an elements\n",lineNumber,tmp);
					ifThereIsError = true;
				}
				else
				{
					error = false;
					if(!strcmp(tmp,".struct"))
					{
						ifThereisComma = false;
						if(codeLine[index] == '+' || codeLine[index] == '-' || isdigit(codeLine[index]))
							index++;
						else if(error == false)
						{
							printf("Line:%d, Error - the command '%s' should have a number first\n",lineNumber,tmp);
							ifThereIsError = true;
							error = true;
						}
						while(codeLine[index] != ',' && !isspace(codeLine[index]))
						{
							if(isdigit(codeLine[index]))
								index++;
							else if(error == false)
							{
								printf("Line:%d, Error - the command '%s' should have a number first\n",lineNumber,tmp);
								ifThereIsError = true;
								error = true;
								index++;
							}
							else
								index++;
						}
						while(codeLine[index] == ',' || (isspace(codeLine[index]) && codeLine[index] != '\n'))
						{
							if(codeLine[index] == ',')
							{
								if(ifThereisComma == true && error == false)
								{
									printf("Line:%d, Error - there is too many ',' between the elements\n",lineNumber);
									ifThereIsError = true;
									error = true;
								}
								else
									ifThereisComma = true;
							}
							index++;
						}
						if(ifThereisComma == false && error == false)
						{
							printf("Line:%d, Error - there is no ',' between the elements\n",lineNumber);
							ifThereIsError = true;
							error = true;
						}
					}
					if(error == false)
					{				
						if(codeLine[index] != '"')
						{
							printf("Line:%d, Error - the string doesn't start with a %c\n",lineNumber,34);
							ifThereIsError = true;
						}
						else 
						{
							index++;
							while(codeLine[index] != '"' && codeLine[index] != '\n')
								index++;
							if(codeLine[index] == '\n')
							{
								printf("Line:%d, Error - the string doesn't end with a %c\n",lineNumber,34);
								ifThereIsError = true;
							}
							else
							{
								index++;
								while(isspace(codeLine[index]) && codeLine[index] != '\n')
									index++;
								if(codeLine[index] != '\n')
								{
									printf("Line:%d, Error - there is another element after the string\n",lineNumber);
									ifThereIsError = true;
								}
							}
						}
					}
				}
			}
		}
	}
}

bool checkLabelName(char symbName[]) /*gets label name checks if it's an illegal name for a label and returns true/false */
{
	bool ifError = false;
	int index = 0;
	while(symbName[index] != '\0')
	{
		if(isalnum(symbName[index]) == false)
		{
			printf("Line:%d, Error - '%s' - label name need to contain only alphanumeric chars\n",lineNumber,symbName);
			ifError = true;
		}
		index++;
	}
	if(symbName[0] == 'r' && (int)symbName[1] >= 48 && (int)symbName[1] <= 55 && symbName[2] == '\0')
	{
		printf("Line:%d, Error - '%s' - label name can't be the same as a register name\n",lineNumber,symbName);
		ifError = true;
	}
	return ifError;
}

bool checkTwoOperands(char op1[], char op2[], int opCode)/*gets 2 operands and opcode of the command, checks if it's an illegal combination and returns true/false */
{
	bool ifError = false;
	int op1Type = checkOperand(op1);
	int op2Type = checkOperand(op2);
	if(opCode == 0 || opCode == 2 || opCode == 3)
	{
		if(op2Type == 1)
		{
			printf("Line:%d, Error - this command can't get operand type of '%s'\n",lineNumber,op2);
			ifError = true;
		}
	}
	if(opCode == 6)
	{
		if((op1Type == 1 || op1Type == 2))
		{
			printf("Line:%d, Error - this command can't get operand type of '%s'\n",lineNumber,op1);
			ifError = true;
		}
		if(op2Type == 1)
		{
			printf("Line:%d, Error - this command can't get operand type of '%s'\n",lineNumber,op2);
			ifError = true;
		}
	}
	return ifError;
}

bool checkOperandErrors(char operand[]) /*gets operand, checks if it's an illegal operand and returns true/false (if it's a label saves the name at the list of labels to check)*/
{
	bool ifError = false; 
	int opIndex = 0;
	int operandType = checkOperand(operand);
	char tmp[labelLength];
	memset(tmp,'\0',labelLength);
	if(operandType == 0)
	{
		printf("Line:%d, Error - the operand '%s' is illegal\n",lineNumber,operand);
		ifError = true;
	}
	else if(operandType == 3 || operandType == 4) /*if it's a label*/
	{
		if(operandType == 4) /*if it's a struct label*/
		{
			while(operand[opIndex] != '.')
			{
				tmp[opIndex] = operand[opIndex];
				opIndex++;
			}
			if((operand[opIndex + 1] == '1' || operand[opIndex + 1] == '2') && checkLabelName(tmp) == false)
				addToLabelsList(&labelsToCheckHead, lineNumber, tmp);
			else
			{
				printf("Line:%d, Error - the operand '%s' is illegal\n",lineNumber,operand);
				ifError = true;
			}
		}
		else if(checkLabelName(operand) == false)
			addToLabelsList(&labelsToCheckHead, lineNumber, operand);
	}
	return ifError;
}

bool ifSpecialCommand(char name[])/*gets name checks if it's data/string/struct/entry/extern command and returns true/false */
{
	if(!strcmp(name,".data") || !strcmp(name,".string") || !strcmp(name,".struct") || !strcmp(name,".entry") || !strcmp(name,".extern"))
		return true;
	return false;
}

void addToLabelsList(labelsToCheckPtr* h, int line, char labelName[]) /*gets pointer to head of labels to check list, line number and label name and adds a new element to the list with these variables*/
{
	labelsToCheckPtr p1,tmp;
	tmp = (labelsToCheckPtr) malloc(sizeof(labelsToCheck));
	if(!tmp)
	{
		printf("\n cannot allocate memory \n");
		exit(0);
	}
	tmp->line = line;
	for(int i = 0; i < labelLength; i++)
		if(labelName[i])
			tmp->label[i] = labelName[i];
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

void ifLabelsAreExist() /*Checks if all the labels used are exist */
{
	labelsToCheckPtr tmp = labelsToCheckHead;
	while(tmp)
	{
		if(ifSymbolExist(tmp->label) == false)
		{
			printf("Line:%d, Error - label '%s' is not exist\n",tmp->line,tmp->label);
			exit(0);
		}
		tmp = tmp->next;
	}
}

void addToExtFile(FILE *fpw) /*gets pointer to extern file and prints there the extern labels and the IC of their use*/
{
	labelsToCheckPtr tmp = labelsToCheckHead;
	int ic;
	char c1,c2;
	while(tmp)
	{
		if(ifExtern(tmp->label) == true)
		{
			ic = getICFromICLineTable(tmp->line);
			c1 = convertIntToBase32((ic)/32);
			c2 = convertIntToBase32((ic)%32);
			fprintf(fpw,"%s   %c%c\n",tmp->label,c1,c2);
		}
		tmp = tmp->next;
	}
}
