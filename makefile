maman14: main.o files.o firstPass.o secondPass.o errorHandler.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o
	gcc -Wall -ansi -pedantic main.o files.o firstPass.o secondPass.o errorHandler.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o -o maman14

main.o:	main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o

files.o: files.c
	gcc -c -Wall -ansi -pedantic files.c -o files.o

firstPass.o: firstPass.c
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o

secondPass.o: secondPass.c
	gcc -c -Wall -ansi -pedantic secondPass.c -o secondPass.o

errorHandler.o: errorHandler.c
	gcc -c -Wall -ansi -pedantic errorHandler.c -o errorHandler.o

symbolsList.o: symbolsList.c
	gcc -c -Wall -ansi -pedantic symbolsList.c -o symbolsList.o

utilsFirstPass.o: utilsFirstPass.c
	gcc -c -Wall -ansi -pedantic utilsFirstPass.c -o utilsFirstPass.o

dataList.o: dataList.c
	gcc -c -Wall -ansi -pedantic dataList.c -o dataList.o

entryList.o: entryList.c
	gcc -c -Wall -ansi -pedantic entryList.c -o entryList.o

operations.o: operations.c
	gcc -c -Wall -ansi -pedantic operations.c -o operations.o

wordsList.o: wordsList.c
	gcc -c -Wall -ansi -pedantic wordsList.c -o wordsList.o
