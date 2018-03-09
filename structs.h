#ifndef STRUCTS_H
#define STRUCTS_H
/*This file contains */

/*definition of first word type*/
typedef struct firstWord
{
  /*ERA bits*/
  unsigned int era: 2;
  /*Destination operand bits*/
  unsigned int destination: 2;
  /*source operand bits*/
  unsigned int source: 2;
  /*Opcode bits*/
  unsigned int opcode: 4;
  /*Group bits*/
  unsigned int group: 2;
  /*Unused bits - always 111*/
  unsigned int unused: 3;
  /*Last bit, always 0*/
  unsigned int last: 1;
} firstWord;

/*definition of immediate word type:
  it is used to store either an immediate number or a memory address*/
typedef struct immediateWord
{
  /*ERA bits*/
  unsigned int era: 2;
  /*bits used to hold the value/address*/
  int number: 13;
  /*Last bit, always 0*/
  unsigned int last: 1;
} immediateWord;

/*definition of index register word type*/
typedef struct indexWord
{
  /*ERA bits*/
  unsigned int era: 2;
  /*Left operand bit - the register before the [*/
  unsigned int leftOperand: 6;
  /*right operand bit - the register after the [*/
  unsigned int rightOperand: 6;
  /*Unused bits - always 111*/
  unsigned int unused: 1;
  /*Last bit, always 0*/
  unsigned int last: 1;
} indexWord;

/*definition of register word type*/
typedef struct registerWord
{
  /*ERA bits*/
  unsigned int era: 2;
  /*Destination register bits*/
  unsigned int destination: 6;
  /*source register bits*/
  unsigned int source: 6;
  /*Unused bit - always 0*/
  unsigned int unused: 1;
  /*Last bit, always 0*/
  unsigned int last: 1;
} registerWord;

/*Word holder union, used to hold all of the words.
a union is used so that every type of word can be in a linked list together*/
typedef union wordHolder
{
  /*firsWord type*/
  firstWord first;
  /*Immediate word type*/
  immediateWord immediate;
  /*Index word type*/
  indexWord index;
  /*Register word type*/
  registerWord reg;
} wordHolder;

/*wordNode is the type used in the word list. it contains a holder union type,
an address which is used when exporting files,
the method identifier which tells which type of word it holds,
the line is is at,
missing label string (if it refers to a label),
external label string (if it refers to an external variable),
an external flag, and a pointer to the next node */
typedef struct wordNode
{
  /*The word itself*/
  wordHolder holder;
  /*The address assigned to it*/
  int address;
  /*Addressign method - which word is it?*/
  int method;
  /*Which line in input?*/
  int line;
  /*Missing label*/
  char * missingLabel;
  /*Label of external*/
  char * externalLabel;
  /*Is it external?*/
  int isExternal;
  /*Pointer to next*/
  struct wordNode * next;
} wordNode;

/*definition of opcode type
used internally in operations.c to loop through all commands*/
typedef struct opcode
{
  /*The operation string*/
  char * str;
  /*The opcode*/
  int code;
  /*The group*/
  int group;
} opcode;

/*definition of error type. used in the errors list*/
typedef struct error
{
  /*The error itself*/
  char * str_error;
  /*The line where the error was found*/
  int line;
  /*The string where the error was found*/
  char * str;
  /*Pointer to next*/
  struct error * next;
} error;

/*definition of symbol type
contains a string (label),
address, and external/operation flags*/
typedef struct symbol
{
  /*Label of the symbol*/
  char * label;
  /*Address of the symbol*/
  int address;
  /*Is it external?*/
  int external;
  /*is it an operation?*/
  int operation;
  /*pointer to next*/
  struct symbol * next;
} symbol;

/*definition of data type
contains an address and a value (which stores the number/char)*/
typedef struct data
{
  /*address of data word*/
  int address;
  /*Value*/
  short int value;
  /*pointer to next*/
  struct data * next;
} data;

/*definition of entry type
Contains a label, an address, an address flag, a line value,
and a pointer to the next node*/
typedef struct entry
{
  /*Label of entry*/
  char * label;
  /*Address*/
  short int address;
  /*Does it have an address? (for internal use)*/
  int hasAddress;
  /*Line of entry*/
  int line;
  /*pointer to next*/
  struct entry * next;
} entry;

#endif
