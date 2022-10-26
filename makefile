assembler: binaryCode.o checkErrors.o macro.o firstCheck.o secondCheck.o getCode.o Tables.o assembler.o
	gcc -g -ansi assembler.o binaryCode.o checkErrors.o macro.o firstCheck.o secondCheck.o getCode.o Tables.o -o assembler -lm
assembler.o: assembler.c assembler.h getCode.h checkErrors.h Tables.h firstCheck.h secondCheck.h macro.h
	gcc -c -Wall -ansi -pedantic -std=c99 assembler.c -o assembler.o -lm
binaryCode.o: binaryCode.c binaryCode.h
	gcc -c -Wall -ansi -pedantic binaryCode.c -o binaryCode.o -lm
checkErrors.o: checkErrors.c checkErrors.h binaryCode.h firstCheck.h Tables.h secondCheck.h getCode.h
	gcc -c -Wall -ansi -pedantic -std=c99 checkErrors.c -o checkErrors.o -lm
macro.o: macro.c macro.h getCode.h
	gcc -c -Wall -ansi -pedantic -std=c99 macro.c -o macro.o -lm
firstCheck.o: firstCheck.c firstCheck.h getCode.h Tables.h binaryCode.h checkErrors.h
	gcc -c -Wall -ansi -pedantic -std=c99 firstCheck.c -o firstCheck.o -lm
secondCheck.o: secondCheck.c secondCheck.h firstCheck.h Tables.h binaryCode.h
	gcc -c -Wall -ansi -pedantic -std=c99 secondCheck.c -o secondCheck.o -lm
getCode.o: getCode.c getCode.h
	gcc -c -Wall -ansi -pedantic -std=c99 getCode.c -o getCode.o -lm
Tables.o: Tables.c Tables.h binaryCode.h 
	gcc -c -Wall -ansi -pedantic -std=c99 Tables.c -o Tables.o -lm
