#include "getCode.h"

linePtr getCode(FILE *fpr) /*gets pointer to a file, makes linked list of the lines in the file and returns the head of the list*/
{
	linePtr head = NULL;
	char c[LineLength];
	memset(c , '\0' , LineLength);
	int counter = 0;
	while(fgets(c, LineLength, fpr))
	{	
		addToList(&head,c);
		memset(c , '\0' , LineLength);
	}
	return head;
}

void addToList(linePtr *hptr, char c[]) /*gets pointer to head of list and code line and adds a new element to the list with this code line*/
{
	linePtr p1,tmp;
	tmp = (linePtr) malloc(sizeof(line));
	if(!tmp)
	{
		printf("\n cannot allocate memory \n");
		exit(0);
	}
	for(int i = 0; i < LineLength; i++)
		if(c[i])
			tmp->data[i] = c[i];
	if(!(*hptr)) /*checks if the list is empty*/
	{
		tmp->next = NULL;
		*hptr = tmp;
		return;
	}
	p1 = *hptr;
	while(p1->next) /*takes the pointer to the end of the list*/
	{
		p1 = p1->next;
	}
	p1->next = tmp;
}

void printList(linePtr h) /*gets head of linked list and prints the list*/
{
	printf("\n\nThe code after opening the macros is :\n\n");
	while(h)
	{
		for(int i = 0; i < LineLength; i++)
			if(h->data[i])
				printf("%c",h->data[i]);
		h = h->next;
	}
	printf("\n");
}
