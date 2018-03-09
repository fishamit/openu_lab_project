#include "header.h"

/*This function checks if pointer is null*/
void checkAllocation(void * ptr)
{
    if (!ptr)
    {
      printf("The program has quit due to a memory allocation error.\n");
      exit(0);
    }
}

/*This function checks for a label.
if there is no label, it will return NULL.
if there's an illegal label, it will return $.
if there's a legal label, it will return the label.*/
char * getLabel(char * token)
{
  char * ptr = token;
  /*if last char is ':'*/
  if (token[strlen(token) - 1] == ':')
  {
    /*if first letter is valid*/
    if (isalpha(*token))
    {
      /*set null terminator instead of ':'*/
      token[strlen(token) - 1] = 0;

      /*If there's a char which is not a digit or a letter, return an error flag*/
      while ((*ptr) != '\0')
      {
        if (!isalpha(*ptr))
        {
          if (!isdigit(*ptr))
          {
            return "$";
          }
        }
        ptr++;
      }
      /*If the token is an operation, return reserved word flag*/
      if (isOperation(token))
      {
        return "@";
      }
      /*If the token is a register, return reserved word flag*/
      if (isRegister(token))
      {
        return "@";
      }
      return token;
    }
    /*if first letter is invalid, return error flag*/
    else
    {
      return "$";
    }
  }
  /*if last letter is not ':', return null*/
  else
  {
    return NULL;
  }
}

/*This function determines if we received a directive.
it will return an enum int of the proper directive, or an error flag
if there's no such directive. if the string doesn't start with ".",
it will return an enum defined as NONE, meaning it is not a directive*/
int getDirective(char * token)
{
  /*if first char is '.'*/
  if (*token == '.')
  {
    /*return enum corresponding to the directive*/
    if (!strcmp(token, STR_DIRECTIVE_DATA))
    {
      return DIRECTIVE_DATA;
    }
    else if (!strcmp(token, STR_DIRECTIVE_STRING))
    {
      return DIRECTIVE_STRING;
    }
    else if (!strcmp(token, STR_DIRECTIVE_ENTRY))
    {
      return DIRECTIVE_ENTRY;
    }
    else if (!strcmp(token, STR_DIRECTIVE_EXTERN))
    {
      return DIRECTIVE_EXTERN;
    }
    /*if string is unknown*/
    else
    {
      /*return error*/
      return DIRECTIVE_ERROR;
    }
  }
  /*if this isn't a directive*/
  else
  {
    return DIRECTIVE_NONE;
  }
}

/*this function checks if token is a comment*/
int isComment(char * token)
{
  if (*token == ';')
    return 1;
  else
    return 0;
}
