#include "header.h"
/*This file contains functions which handle input analyzing for operations.*/

/*Array holding information about each opcode, for internal use*/
static opcode opCodes[16] = {{"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2},
{"sub", 3, 2}, {"not", 4, 1}, {"clr", 5, 1}, {"lea", 6, 2}, {"inc", 7, 1},
{"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"red", 11, 1}, {"prn", 12, 1},
{"jsr", 13, 1}, {"rts", 14, 0}, {"stop", 15, 0}};

/*Array of register string, for comparing with input*/
static char * registers[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*This function will split the input string to tokens and will analyze each token.
It will create the corresponding word nodes, add them to the word list,
and increment the IC as needed.*/
void analyzeOperation(char * token, int line, int *IC, char * label)
{
  /*Index for loops*/
  int i;
  /*Found a command flag*/
  int found = FALSE;
  /*How much do we want to increment IC?*/
  int increment_value_IC;
  /*Pointers to word nodes*/
  wordNode * first = NULL;
  wordNode * second = NULL;
  wordNode * third = NULL;
  /*Check if command is known*/
  for (i = 0; i < OPCODES_AMOUNT; i++)
  {
    /*if it is, create a new word object, initiate it and assign it with everything*/
    if (!strcmp(token, opCodes[i].str))
    {
      /*Set "found command" flag to true*/
      found = TRUE;
      /*Allocate memory for the first word*/
      first = malloc(sizeof(wordNode));
      checkAllocation(first);
      /*Set all of first word's parameters*/
      first->method = NONE;
      first->isExternal = FALSE;
      first->missingLabel = NULL;
      first->externalLabel = NULL;
      first->line = line;
      first->holder.first.last = 0;
      first->holder.first.unused = 7;
      first->holder.first.group = opCodes[i].group;
      first->holder.first.opcode = opCodes[i].code;
      first->holder.first.source = 0;
      first->holder.first.destination = 0;
      first->holder.first.era = 0;
      first->next = NULL;
      /*Add one to the increment value*/
      increment_value_IC++;
    }
  }
  /*if the command wasnt found, error*/
  if (!found)
  {
    addError("Unknown command", line, token);
    return;
  }

  /*
  A command word exists.
  If we have parameters after the command*/
  if ((token = strtok(NULL, " ,\t\n")))
  {
    /*If the command is group 2*/
    if (first->holder.first.group == 2)
    {
      /*If first token is an immediate number*/
      if (*token == '#')
      {
        int num;
        char * ptr;
        char * errorptr;
        ptr = token;
        ptr++;
        num = strtol(ptr, &errorptr, 10);
        /*if command is LEA, error&quit*/
        if (first->holder.first.opcode == OPCODE_LEA)
        {
          addError("This command can't take an immediate number as a first operand", line, NULL);
          free(first);
          return;
        }
        /*if number conversion failes*/
        if (*errorptr != 0)
        {
          addError("Invalid number", line, NULL);
          free(first);
          return;
        }
        /*if the number isn't within range*/
        if (num > MAX_NUM_IMMEDIATE || num < MIN_NUMBER_IMMEDIATE)
        {
          addError("Immediate number is out of range", line, NULL);
          free(first);
          return;
        }
        /*If syntax is correct*/
        else
        {
          /*Update first word and create a second word*/
          first->holder.first.source = IMMEDIATE;
          second = malloc(sizeof(wordNode));
          second->method = IMMEDIATE;
          second->isExternal = FALSE;
          second->line = line;
          second->missingLabel = NULL;
          second->externalLabel = NULL;
          second->holder.immediate.last = 0;
          second->holder.immediate.number = num;
          second->holder.immediate.era = 0;
          second->next = NULL;
          increment_value_IC++;
        }
      }
      /*if first token is register*/
      else if (isRegister(token))
      {
        /*If the command is lea, add error and return*/
        if (first->holder.first.opcode == OPCODE_LEA)
        {
          addError("This command can't take an immediate number as a first operand", line, NULL);
          free(first);
          return;
        }
        /*Update first word and create a second word*/
        first->holder.first.source = REGISTER;
        second = malloc(sizeof(wordNode));
        second->method = REGISTER;
        second->isExternal = FALSE;
        second->line = line;
        second->missingLabel = NULL;
        second->externalLabel = NULL;
        second->holder.reg.last = 0;
        second->holder.reg.unused = 0;
        second->holder.reg.source = (token[1] - '0');
        second->holder.reg.destination = 0;
        second->holder.reg.era = 0;
        second->next = NULL;
        increment_value_IC++;
      }
      /*if first token is index*/
      else if (isIndex(token))
      {
        int left = (token[1] - '0');
        int right = (token[4] - '0');

        /*if numbers are not legit registers*/
        if ((left < 0 || left > 7 ) || (right < 0 || right > 7))
        {
          addError("Illegal register", line, NULL);
          free(first);
          return;
        }
        /*if the numbers are legit registers*/
        else
        {
          /*if first is odd and second is even*/
          if (((left % 2 ) != 0) && ((token[4] - '0') % 2 == 0))
          {
            first->holder.first.source = INDEX;
            second = malloc(sizeof(wordNode));
            second->method = INDEX;
            second->isExternal = FALSE;
            second->line = line;
            second->missingLabel = NULL;
            second->externalLabel = NULL;
            second->holder.index.last = 0;
            second->holder.index.unused = 0;
            second->holder.index.rightOperand = right;
            second->holder.index.leftOperand = left;
            second->holder.index.era = 0;
            second->next = NULL;
            increment_value_IC++;
          }
          else
          {
            addError("Left register must be odd and right register must be even", line, token);
            free(first);
            return;
          }
        }
      }
      /*if we didnt recieve immediate number/register/index, it must be a label*/
      else
      {
        char * ptr;
        ptr = token;
        /*check if first char is letter (valid label)*/
        if (isalpha(*ptr))
        {
          /*checks for invalid chars*/
          while (*ptr != '\0')
          {
            if ((!isdigit(*ptr)) && (!isalpha(*ptr)))
            {
              addError("Invalid character", line, token);
              free(first);
              return;
            }
            ptr++;
          }
          /*create memory address opcode*/
          first->holder.first.source = DIRECT;
          second = malloc(sizeof(wordNode));
          second->method = DIRECT;
          second->isExternal = FALSE;
          second->line = line;
          second->missingLabel = malloc(sizeof(char)*MAX_LINE);
          second->externalLabel = NULL;
          strcpy(second->missingLabel, token);
          second->holder.immediate.last = 0;
          second->holder.immediate.number = 0;
          second->holder.immediate.era = 1;
          second->next = NULL;
          increment_value_IC++;
        }
        else
        {
          addError("Labels must start with a letter", line, token);
          free(first);
          return;
        }
      }
      /*Move to second token*/
      if ((token = strtok(NULL, " ,\t\n")))
      {
        if (*token == '#')
        {
          int num;
          char * ptr;
          char * errorptr;
          ptr = token;
          ptr++;
          num = strtol(ptr, &errorptr, 10);
          /*if command is LEA, error&quit*/
          if (first->holder.first.opcode != OPCODE_CMP)
          {
            addError("This command can't take an immediate number as a second operand", line, NULL);
            free(first);
            free(second);
            return;
          }
          /*if number conversion failes*/
          if (*errorptr != 0)
          {
            addError("Invalid number", line, NULL);
            free(first);
            free(second);
            return;
          }
          /*if the number isn't within range*/
          if (num > MAX_NUM_IMMEDIATE || num < MIN_NUMBER_IMMEDIATE)
          {
            addError("Immediate number is out of range", line, NULL);
            free(first);
            return;
          }
          /*If syntax is correct*/
          else
          {
            first->holder.first.destination = IMMEDIATE;
            third = malloc(sizeof(wordNode));
            third->method = IMMEDIATE;
            third->isExternal = FALSE;
            third->line = line;
            third->missingLabel = NULL;
            third->externalLabel = NULL;
            third->holder.immediate.last = 0;
            third->holder.immediate.number = num;
            third->holder.immediate.era = 0;
            third->next = NULL;
            increment_value_IC++;
          }
        }
        else if (isRegister(token))
        {
          if (second->method == REGISTER)
          {
            first->holder.first.destination = REGISTER;
            second->holder.reg.destination = (token[1] - '0');
          }
          else
          {
            first->holder.first.destination = REGISTER;
            third = malloc(sizeof(wordNode));
            third->method = REGISTER;
            third->isExternal = FALSE;
            third->line = line;
            third->missingLabel = NULL;
            third->externalLabel = NULL;
            third->holder.reg.last = 0;
            third->holder.reg.unused = 0;
            third->holder.reg.source = 0;
            third->holder.reg.destination = (token[1] - '0');
            third->holder.reg.era = 0;
            third->next = NULL;
            increment_value_IC++;
          }
        }
        else if (isIndex(token))
        {
          int left = (token[1] - '0');
          int right = (token[4] - '0');
          /*if numbers are not legit registers*/
          if ((left < 0 || left > 7 ) || (right < 0 || right > 7))
          {
            addError("Illegal register", line, NULL);
            free(first);
            return;
          }
          /*if the numbers are legit registers*/
          else
          {
            /*if first is odd and second is even*/
            if (((left % 2 ) != 0) && ((token[4] - '0') % 2 == 0))
            {
              first->holder.first.destination = INDEX;
              third = malloc(sizeof(wordNode));
              third->method = INDEX;
              third->isExternal = FALSE;
              third->line = line;
              third->missingLabel = NULL;
              third->externalLabel = NULL;
              third->holder.index.last = 0;
              third->holder.index.rightOperand = right;
              third->holder.index.leftOperand = left;
              third->holder.index.era = 0;
              third->next = NULL;
              increment_value_IC++;
            }
            else
            {
              addError("Left register must be odd and right register must be even", line, token);
              free(first);
              free(second);
              return;
            }
          }
        }
        /*if token is not immediate/index/register*/
        else
        {
          char * ptr;
          ptr = token;
          /*check if first char is letter (valid label)*/
          if (isalpha(*ptr))
          {
            /*checks for invalid chars*/
            while (*ptr != '\0')
            {
              if ((!isdigit(*ptr)) && (!isalpha(*ptr)))
              {
                addError("Invalid character", line, token);
                free(first);
                return;
              }
              ptr++;
            }
            /*create memory address opcode*/
            first->holder.first.destination = DIRECT;
            third = malloc(sizeof(wordNode));
            third->method = DIRECT;
            third->isExternal = FALSE;
            third->line = line;
            third->missingLabel = malloc(sizeof(char)*MAX_LINE);
            third->externalLabel = NULL;
            strcpy(third->missingLabel, token);
            third->holder.immediate.last = 0;
            third->holder.immediate.number = 0;
            third->holder.immediate.era = 1;
            third->next = NULL;
            increment_value_IC++;
          }
          /*if the label didn't start with a letter*/
          else
          {
            addError("Labels must start with a letter", line, token);
            free(first);
            free(second);
            return;
          }
        }
        /*check if the string has more arguments*/
        if ((token = strtok(NULL, " ,\t\n")))
        {
          addError("Too many arguments", line, NULL);
          free(first);
          free(second);
          free(third);
          return;
        }
      }
      /*Check if there's less than 2 arguments*/
      else
      {
        addError("This command must take 2 parameters", line, NULL);
        free(first);
        free(second);
        return;
      }
    }
    /*if the command is group 1*/

    else if (first->holder.first.group == 1)
    {
      /*If first token is an immediate number*/
      if (*token == '#')
      {
        int num;
        char * ptr;
        char * errorptr;
        ptr = token;
        ptr++;
        num = strtol(ptr, &errorptr, 10);
        /*if command is LEA, error&quit*/
        if (first->holder.first.opcode != OPCODE_PRN)
        {
          addError("This command can't take an immediate number as a first operand", line, NULL);
          free(first);
          return;
        }
        /*if number conversion failes*/
        if (*errorptr != 0)
        {
          addError("Invalid number", line, NULL);
          free(first);
          return;
        }
        /*if the number isn't within range*/
        if (num > MAX_NUM_IMMEDIATE || num < MIN_NUMBER_IMMEDIATE)
        {
          addError("Immediate number is out of range", line, NULL);
          free(first);
          return;
        }
        /*If syntax is correct*/
        else
        {
          first->holder.first.destination = IMMEDIATE;
          second = malloc(sizeof(wordNode));
          second->method = IMMEDIATE;
          second->isExternal = FALSE;
          second->line = line;
          second->missingLabel = NULL;
          second->externalLabel = NULL;
          second->holder.immediate.last = 0;
          second->holder.immediate.number = num;
          second->holder.immediate.era = 0;
          second->next = NULL;
          increment_value_IC++;
        }
      }
      /*Check if token is a register*/
      else if (isRegister(token))
      {
        if (first->holder.first.opcode == OPCODE_LEA)
        {
        first->holder.first.destination = REGISTER;
        second = malloc(sizeof(wordNode));
        second->method = REGISTER;
        second->isExternal = FALSE;
        second->line = line;
        second->missingLabel = NULL;
        second->externalLabel = NULL;
        second->holder.reg.last = 0;
        second->holder.reg.unused = 0;
        second->holder.reg.source = (token[1] - '0');
        second->holder.reg.destination = 0;
        second->holder.reg.era = 0;
        second->next = NULL;
        increment_value_IC++;
        }
      }
      /*Check if token is index*/
      else if (isIndex(token))
      {
        int left = (token[1] - '0');
        int right = (token[4] - '0');
        /*if numbers are not legit registers*/
        if ((left < 0 || left > 7 ) || (right < 0 || right > 7))
        {
          addError("Illegal register", line, NULL);
          free(first);
          return;
        }
        /*if the numbers are legit registers*/
        else
        {
          /*if first is odd and second is even*/
          if (((left % 2 ) != 0) && ((token[4] - '0') % 2 == 0))
          {
            first->holder.first.destination = INDEX;
            second = malloc(sizeof(wordNode));
            second->method = INDEX;
            second->isExternal = FALSE;
            second->line = line;
            second->missingLabel = NULL;
            second->externalLabel = NULL;
            second->holder.index.last = 0;
            second->holder.index.rightOperand = right;
            second->holder.index.leftOperand = left;
            second->holder.index.era = 0;
            second->next = NULL;
            increment_value_IC++;
          }
          /*If left register isn't odd or right register isn't even*/
          else
          {
            addError("Left register must be odd and right register must be even", line, token);
            free(first);
            return;
          }
        }
      }
      /*If token not immediate/register/token*/
      else
      {
        char * ptr;
        ptr = token;
        /*check if first char is letter (valid label)*/
        if (isalpha(*ptr))
        {
          /*checks for invalid chars*/
          while (*ptr != '\0')
          {
            if ((!isdigit(*ptr)) && (!isalpha(*ptr)))
            {
              addError("Invalid character", line, token);
              free(first);
              return;
            }
            ptr++;
          }

          /*create memory address opcode*/
          first->holder.first.destination = DIRECT;
          second = malloc(sizeof(wordNode));
          second->line = line;
          second->method = DIRECT;
          second->isExternal = FALSE;
          second->missingLabel = malloc(sizeof(char)*MAX_LINE);
          second->externalLabel = NULL;
          strcpy(second->missingLabel, token);
          second->holder.immediate.last = 0;
          second->holder.immediate.number = 0;
          second->holder.immediate.era = 1;
          second->next = NULL;
          increment_value_IC++;
        }
        /*if the label didn't start with a letter*/
        else
        {
          addError("Labels must start with a letter", line, token);
          free(first);
          free(second);
          return;
        }
      }
      /*if string has more arguments*/
      if ((token = strtok(NULL, " ,\t\n")))
      {
        addError("Too many arguments", line, NULL);
        free(first);
        free(second);
        free(third);
        return;
      }
    }
    /*if the command is group 0*/
    else
    {
      addError("This command cannot take parameters", line, NULL);
      free(first);
      return;
    }
  }
  /*If we didn't recieve any parameters after the command*/
  else
  {
    if (first->holder.first.opcode != OPCODE_RTS && first->holder.first.opcode != OPCODE_STOP)
    {
      addError("This command must take parameters", line, NULL);
      free(first);
      return;
    }
  }
  /*Check if there's a label with the command. if there is, add it to the symbols table*/
  if (label)
  {
    addSymbol(label, *IC, FALSE, TRUE, line);
  }
  /*Add each word (as needed) to the words list*/
  if (first)
    addWordNode(first);
  if (second)
    addWordNode(second);
  if (third)
    addWordNode(third);
  /*Increment the IC as needed*/
  *IC += increment_value_IC;
}

/*This function recieves a string and returns:
  1 if the string is a registers
  0 if the string is not a register*/
int isRegister(char * token)
{
  int i;
  if (strlen(token) == 2)
  {
    if (token[0] == 'r')
    {
      if (isdigit(token[1]))
      {
        for (i=0; i<NUM_OF_REGISTERS; i++)
        {
          if (!strcmp(token, registers[i]))
          {
            return 1;
          }
        }

      }
    }
  }
  return 0;
}

/*This function recieves a string and returns:
  1 if the string is an operation
  0 if the string is not an operation*/
int isOperation(char * token)
{
  int i;

  for (i = 0; i < OPCODES_AMOUNT; i++)
  {
    if (!strcmp(token, opCodes[i].str))
    {
      return 1;
    }
  }
  return 0;
}

/*This function recieves a string and returns:
  1 if the string is an index
  0 if the string is not an index*/
int isIndex(char * token)
{
  if (strlen(token) == 6)
  {
    if (*token == 'r')
    {
      if (isdigit(token[1]))
      {
        if (token[2] == '[')
        {
          if (token[3] == 'r')
          {
            if (isdigit(token[4]))
            {
              if (token[5] == ']')
              {
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}
