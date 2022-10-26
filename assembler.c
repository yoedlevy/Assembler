#include "assembler.h"

int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		printf("\nThis program can't get more then one file\n");
		exit(0);
	}

	struct node* head = NULL;
	struct node* headAM = NULL;
	head = (struct node *) malloc(sizeof(struct node));
	headAM = (struct node *) malloc(sizeof(struct node));
	struct node* tmp = NULL; 
	tmp = (struct node *) malloc(sizeof(struct node));
	strcpy(fileNameAS,argv[1]);
	strcpy(fileNameAM,argv[1]);
	strcpy(fileNameOBJ,argv[1]);
	strcat(fileNameAS, ".as");
	strcat(fileNameAM, ".am");
	strcat(fileNameOBJ, ".ob");
	
	//Opens the file to read and saves the code at head
	FILE *fpr = fopen(fileNameAS,"r");
	if(fpr == NULL)
		printf("\ncan't reading file\n");
	head = getCode(fpr);

	//Opens new file (.am) and writes there the code after opening macros
	FILE *fpw = fopen(fileNameAM,"w");
	tmp = copyList(head);
	getMacros(tmp,fpw);
	fclose(fpw);

	//Opens the file (.am) to read now and saves the code (after opening macros) at headAM and print him
	FILE *fprAM = fopen(fileNameAM,"r");
	if(fprAM == NULL)
		printf("\ncan't reading file\n");
	headAM = getCode(fprAM);
	printList(headAM);

	//Checks errors and go through the code first check and second check
	checkCodeErrors(headAM);
	firstCheck(headAM);
	secondCheck(headAM);

	//Opens new file (.obj) and writes there the code at base 32
	FILE *fpwOBJ = fopen(fileNameOBJ,"w");
	printCodeAndDataLen(fpwOBJ);
	printCodeTable(fpwOBJ);
	printDataTable(fpwOBJ);

	//If there is entry lines at the code creates new file (.ent)
	if(ifThereIsEnt())
	{
		strcpy(fileNameENT,argv[1]);
		strcat(fileNameENT, ".ent");
		FILE *fpwENT = fopen(fileNameENT,"w");
		addToEntFile(fpwENT);
		fclose(fpwENT);
	}
	
	//If there is extern lines at the code creates new file (.ext)
	if(ifThereIsExt())
	{
		strcpy(fileNameEXT,argv[1]);
		strcat(fileNameEXT, ".ext");
		FILE *fpwEXT = fopen(fileNameEXT,"w");
		addToExtFile(fpwEXT);
		fclose(fpwEXT);
	}

	//Closes files
	fclose(fpr);
	fclose(fprAM);
	fclose(fpwOBJ);

	//free
	free(head);
	free(headAM);
	free(tmp);
}

struct node* copyList(struct node *sourceList) /*gets linked list copies her to new linked list and returns the new one */
{
	if(sourceList==NULL) return;
	struct node * targetList=(struct node *) malloc(sizeof(struct node));
	for(int i = 0; i < 80; i++)
		targetList->data[i]=sourceList->data[i];
	targetList->next = copyList(sourceList->next);
	return targetList;
}
