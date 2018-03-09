#ifndef HEADER_H
#define HEADE_H
/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "defines.h"

/*
Maman 14 by Amit Fisher
How it works:
  Read a line from input.
    -get first token.
    -Determine if first token is a label. if it is, store it in the symbols list.
    -get next token.
    -Is it a directive?
      -If it's .string or .data, store it in the data list and increment DC.
      -If it's .entry, store a new entry with the next token as label.
      -If it's .extern, store it in the symbols list, with next token as label.
    -If it's not a directive, check for command.
    -If it's a command, analize the command and count # of words. increment IC,
      and add the command to the words list.
  Go back to read another line. If there are no more lines:

    Now we have all the symbols and data. Now we'll update every data\string symbols's address.
    If there were errors, we will create an error file and write the errors to that file.
    if there aren't any errors, we will create an object file, and extern/entry files if needed.
*/

/*files.c functions*/
FILE * openFile(char* filename, char * mode, char * extension);
void exportFiles(int IC, int DC, char * name);

/*firstPass.c functions*/
void firstPass(FILE * f, int * counter, int * DC);

/*secondPass.c functions*/
void secondPass();

/*errorHandler.c functions*/
void addError(char * err, int line, char * str);
void printErrors(char * filename);
void freeErrors();
int isError();
int getMaxLine();

/*symbolList.c functions*/
void addSymbol(char * label, int address, int external, int operation, int line);
void freeSymbols();
void checkSymbol(char * label, int line);
symbol * searchSymbol(char * label);
void updateSymbols(int IC);

/*dataList.c functions*/
void addNumber(char * number, int line, int * DC);
void splitNumbers(char * token, int line, int * DC);
void freeData();
void addString(char * string, int line, int *DC);
void addChar(char c);
data * getHeadData();
int countData();
void updateDataAddresses(int IC);

/*utilsFirstPass.c*/
void checkAllocation(void * ptr);
char * getLabel(char * token);
int getDirective(char * token);
int isComment(char * token);

/*entryList.c functions*/
void addEntry(char * label, int line);
void freeEntries();
int checkEntry(char * label);
entry * getEntry();
entry * getHeadEntry();

/*operations.c functions*/
void analyzeOperation(char * token, int line, int *IC, char *label);
int isRegister(char * token);
int isIndex(char * token);
int isOperation(char * token);

/*wordsList.c functions*/
void addWordNode(wordNode * node);
void freeWords();
wordNode * getMissingLabel();
wordNode * getHeadWord();
int countWords();
void updateAddresses();

#endif
