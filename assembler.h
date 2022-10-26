#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getCode.h"

#define FileNameLength 20

char fileNameAS[FileNameLength];
char fileNameAM[FileNameLength];
char fileNameOBJ[FileNameLength];
char fileNameEXT[FileNameLength];
char fileNameENT[FileNameLength];

struct node* copyList(struct node *);
