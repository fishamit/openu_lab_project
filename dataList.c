#include "header.h"
/*This file contains functions which handle the data list*/

/*pointer to the first node*/
static data * head = NULL;

/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
if the string recieved is null, it will add an error. otherwise, it will loop through the string's
tokens and send each one to the addNumber function*/
void splitNumbers(char * token, int line, int * DC)
{
  /*if the token is null*/
  if (!(token = strtok(NULL, " ,\t\n")))
  {
    /*add error and return*/
    addError("You must specify numbers", line, NULL);
    return;
  }
  /*if token is not null*/
  else
  {
    /*send first token to addNumber()*/
    addNumber(token, line, DC);
    /*send the rest tokens to addNumber()*/
    while ((token = strtok(NULL, " ,\t\n")))
    {
      addNumber(token, line, DC);
    }
  }
}

/*This function adds a number to the data list.
It will check if the string is a valid number, and if the number is within range.
It will convert the string to a long int and then store it in the data node*/
void addNumber(char * number, int line, int * DC)
{
  /*pointer for strtol error checking*/
  char * ptr = NULL;
  /*temporary data node*/
  data * tmp = NULL;
  /*temporary holder for the converted number*/
  long int tmpNum;


  /*convert the string to a number*/
  tmpNum = strtol(number, &ptr, 10);
  /*if strtok returned an error*/
  if (*ptr != '\0')
  {
  /*add an error and exit function*/
  addError("Invalid number", line, number);
  return;
  }

  /*if number is out of 15-bit range*/
  if ((tmpNum < MIN_NUMBER_DATA) || (tmpNum > MAX_NUMBER_DATA))
  {
    /*add error and exit function*/
    addError("Number is out of range", line, number);
    return;
  }
  /*create new pointer to data, allocate memory*/
  tmp = malloc(sizeof(data));
  checkAllocation(tmp);
  /*Cast tmpNum(long) to a short int type, and assign to tmp*/
  tmp->value = (short int)tmpNum;
  tmp->next = NULL;
  /*if the list is empty*/
  if (!head)
  {
    /*make head point to temp*/
    head = tmp;
  }
  /*if list is not empty*/
  else
  {
    /*pointer to head*/
    data * p = head;
    /*make p point to last node*/
    while (p->next)
      p = p->next;
    /*make tmp the last node*/
    p->next = tmp;
  }
  (*DC)++;
}

/*This function adds an individual character to the symbols list.
Used when adding a string*/
void addChar(char c)
{
  /*temporary data node*/
  data * tmp = malloc(sizeof(data));
  /*Assign node with character and set next to NULL*/
  tmp->value = c;
  tmp->next = NULL;
  /*if list is empty*/
  if (!head)
  {
    /*make head point to temp*/
    head = tmp;
  }
  /*if list is not empty*/
  else
  {
    /*pointer to head*/
    data * p = head;
    /*make p point to last node*/
    while (p->next)
      p = p->next;
    /*make tmp the last node*/
    p->next = tmp;
  }
}

/*This function adds a string to the data list*/
void addString(char * string, int line, int *DC)
{
  /*if we only recieved a "*/
  if (!strcmp(string, "\""))
  {
    /*add error and return*/
    addError("Must specify a string", line, NULL);
    return;
  }
  /*Check if string is surrounded by quotation marks*/
  if ((*string == '"') && (string[strlen(string) - 1] == '"'))
  {
    /*change right quotation marks to null terminator*/
    string[strlen(string) - 1] = '\0';
    /*point string to character after left quotation mark*/
    string++;
  }
  /*if it's not surrounded by quotation marks*/
  else
  {
    /*add error and return*/
    addError("Strings must be surrounded by quotation marks", line, NULL);
    return;
  }

  /*Loop adding each character to the data list*/
  while (*string != '\0')
  {
    /*add the character*/
    addChar(*string);
    /*point string to next char*/
    string++;
    /*increment data count*/
    (*DC)++;
  }
  /*add null terminator*/
  addChar(0);
  /*increment data count*/
  (*DC)++;
}

/*This functions counts the number of nodes in the data list*/
int countData()
{
  /*Temp counter variable*/
  int counter = 0;
  /*pointer to head*/
  data * ptr = head;
  /*if head is null, return counter (0)*/
  if (!ptr)
    return counter;

    /*go through the list and count each node*/
    while (ptr)
    {
      counter++;
      ptr = ptr->next;
    }
  /*Return the number of nodes*/
  return counter;
}

/*This function updates the data addresses AFTER we have all our operation words*/
void updateDataAddresses(int IC)
{
  /*pointer to head*/
  data * ptr;
  ptr = head;
  /*if head is null, return*/
  if (!ptr)
  {
    return;
  }
  /*go through every node and assign it an address*/
  while (ptr)
  {
    ptr->address = IC;
    /*increment the address to be assigned to the next node*/
    IC++;
    ptr = ptr->next;
  }
}
/*Clear the data list*/
void freeData()
{
  /*temp pointer to head*/
  data * ptr = head;
  /*if head is null, return*/
  if (!ptr)
    return;
    /*free each data node*/
    while (ptr)
    {
      data * tmp = ptr;
      ptr = ptr->next;
      free(tmp);
    }
    /*make the head pointer null*/
    head = NULL;
}

/*return pointer to head. used when exporting files*/
data * getHeadData()
{
  data * tmp = head;
  return tmp;
}
