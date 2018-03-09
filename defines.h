#ifndef DEFINES_H
#define DEFINES_H

/*This file contains enumerations and defines of constants*/

/*boolean enum*/
enum boolean {FALSE, TRUE};

/*Directive enum - used during the first pass to determine which directive has been recieved.
NONE means no directive has been recieved and ERROR means that the string was recognized as a directive but the syntax was wrong*/
enum directive {DIRECTIVE_NONE, DIRECTIVE_DATA, DIRECTIVE_STRING, DIRECTIVE_ENTRY, DIRECTIVE_EXTERN, DIRECTIVE_ERROR};

/*Addressing method enum - NONE means first word*/
enum addresing_method {NONE = -1, IMMEDIATE = 0, DIRECT = 1, INDEX = 2, REGISTER = 3};


/*Directive string constants, to be compared with input*/
#define STR_DIRECTIVE_DATA ".data"
#define STR_DIRECTIVE_STRING ".string"
#define STR_DIRECTIVE_ENTRY ".entry"
#define STR_DIRECTIVE_EXTERN ".extern"

/*Maximum line buffer size*/
#define MAX_LINE 80

/*Maximum filename size*/
#define MAX_FILENAME 50

/*Number of registers*/
#define NUM_OF_REGISTERS 8

/*Maximum and minimum values that can be stored in 13 bits*/
#define MAX_NUMBER_DATA 16383
#define MIN_NUMBER_DATA -16384
#define MAX_NUM_IMMEDIATE 4095
#define MIN_NUMBER_IMMEDIATE -4096
/*Amoount of opcodes*/
#define OPCODES_AMOUNT 16

/*Extrnsions for file i/o*/
#define AS_EXTENSION ".as"
#define TXT_EXTENSION ".txt"
#define OB_EXTENSION ".ob"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"

/*Opcodes*/
#define OPCODE_MOV 0
#define OPCODE_CMP 1
#define OPCODE_ADD 2
#define OPCODE_SUB 3
#define OPCODE_NOT 4
#define OPCODE_CLR 5
#define OPCODE_LEA 6
#define OPCODE_INC 7
#define OPCODE_DEC 8
#define OPCODE_JMP 9
#define OPCODE_BNE 10
#define OPCODE_RED 11
#define OPCODE_PRN 12
#define OPCODE_JSR 13
#define OPCODE_RTS 14
#define OPCODE_STOP 15

#endif
