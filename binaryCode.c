#include "binaryCode.h"

int getOpcode(char command[]) /*gets string with command and returns the opcode of the command*/
{
	if(!strcmp(command, "mov"))
		return 0;
	if(!strcmp(command, "cmp"))
		return 1;
	if(!strcmp(command, "add"))
		return 2;
	if(!strcmp(command, "sub"))
		return 3;
	if(!strcmp(command, "not"))
		return 4;
	if(!strcmp(command, "clr"))
		return 5;
	if(!strcmp(command, "lea"))
		return 6;
	if(!strcmp(command, "inc"))
		return 7;
	if(!strcmp(command, "dec"))
		return 8;
	if(!strcmp(command, "jmp"))
		return 9;
	if(!strcmp(command, "bne"))
		return 10;
	if(!strcmp(command, "get"))
		return 11;
	if(!strcmp(command, "prn"))
		return 12;
	if(!strcmp(command, "jsr"))
		return 13;
	if(!strcmp(command, "rts"))
		return 14;
	if(!strcmp(command, "hlt"))
		return 15;
	return -1;
}

void convertIntToBinary(int num ,char * binaryCode, int bits) /*gets number, address of string and number of bits and puts the number in the string at amount of bits received */
{
	int c,k,index = 0;
	for (c = bits-1; c >= 0; c--) {
        	k = num >> c;
        	if (k & 1)
         	   binaryCode[index] = '1';
        	else
         	   binaryCode[index] = '0';
		index++;
   	 }
}

void convertToBinarySecReg(int num ,char * binaryCode) /*gets number and address of string and puts the number in the string at the right 5 bits*/
{
	int c,k,index = 0;
	for (c = 3; c >= 0; c--) {
        	k = num >> c;
        	if (k & 1)
         	   binaryCode[index+4] = '1';
        	else
         	   binaryCode[index+4] = '0';
		index++;
   	 }
}

int convertBinaryToInt(int bits) /*gets binary number(int) and returns the number at decimal*/
{
	int dec = 0, rem = 0;
	int weight = 1;
	while(bits != 0)
	{
		rem = bits % 10;
		dec = dec + (rem * weight);
		bits = bits / 10;
		weight = weight * 2;
	}
	return dec;
}

char convertIntToBase32(int num) /*gets number (0-31) and return the correct char at base 32*/
{
	if(num == 0)
		return '!';
	if(num == 1)
		return '@';
	if(num == 2)
		return '#';
	if(num == 3)
		return '$';
	if(num == 4)
		return '%';
	if(num == 5)
		return '^';
	if(num == 6)
		return '&';
	if(num == 7)
		return '*';
	if(num == 8)
		return '<';
	if(num == 9)
		return '>';
	if(num == 10)
		return 'a';
	if(num == 11)
		return 'b';
	if(num == 12)
		return 'c';
	if(num == 13)
		return 'd';
	if(num == 14)
		return 'e';
	if(num == 15)
		return 'f';
	if(num == 16)
		return 'g';
	if(num == 17)
		return 'h';
	if(num == 18)
		return 'i';
	if(num == 19)
		return 'j';
	if(num == 20)
		return 'k';
	if(num == 21)
		return 'l';
	if(num == 22)
		return 'm';
	if(num == 23)
		return 'n';
	if(num == 24)
		return 'o';
	if(num == 25)
		return 'p';
	if(num == 26)
		return 'q';
	if(num == 27)
		return 'r';
	if(num == 28)
		return 's';
	if(num == 29)
		return 't';
	if(num == 30)
		return 'u';
	if(num == 31)
		return 'v';
	return ' ';
}
