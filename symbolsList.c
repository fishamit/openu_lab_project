#include "header.h"


static symbol * head = NULL;

/*This function adds a symbol to the symbols list*/
void addSymbol(char * label, int address, int external, int operation, int line)
{
  /*Temp sybol*/
  symbol * tmp = NULL;
  /*This function will check if such symbol already exists*/
  checkSymbol(label, line);
  /*Allocate memory for new symbol*/
  tmp = malloc(sizeof(symbol));
  /*Check if allocation succeeded*/
  checkAllocation(tmp);
  /*Allocate memory for symbol's label*/
  tmp->label = malloc(sizeof(char)*MAX_LINE);
  /*Check if allocation succeeded*/
  checkAllocation(tmp->label);
  /*Copy the label from the input to this symbol*/
  strcpy(tmp->label, label);
  /*Copy value of DC*/
  tmp->address = address;
  /*Copy external flag*/
  tmp->external = external;
  /*Copy operation tab*/
  tmp->operation = operation;
  /*set next to NULL*/
  tmp->next = NULL;
  /*if list is empty*/
  if (!head)
  {
    /*make head point to temp*/
    head = tmp;
  }
  /*if list isn't empty*/
  else
  {
    /*create pointer to head*/
    symbol * p = head;
    /*get to the last node*/
    while (p->next)
      p = p->next;
    /*set last node's next to tmp*/
    p->next = tmp;
  }
}

/*This function will update the symbol's addresses to match with the IC counter,
after the first pass*/
void updateSymbols(int IC)
{
  symbol * ptr = head;
  while (ptr)
  {
    if ((ptr->external == FALSE) && (ptr->operation == FALSE))
    {
      ptr->address += IC;
    }
    ptr = ptr->next;
  }
}


/*This function cleans up the symbol list*/
void freeSymbols()
{
  symbol * ptr = head;
  if (!ptr)
    return;

    while (ptr)
    {
      symbol * tmp = ptr;
      ptr = ptr->next;
      free(tmp->label);
      free(tmp);
    }
    head = NULL;
}

/*This function checks if a symbol with the same name exists*/
void checkSymbol(char * label, int line)
{
  symbol * ptr = head;
  while (ptr)
  {
    if (!strcmp(ptr->label, label))
    {
      addError("Multiple symbols with label", line, label);
      return;
    }
    ptr = ptr->next;
  }
}

/*This function will return a symbol with the same label as the string
entered as a parameter. if no such symbol was found, it will return NULL*/
symbol * searchSymbol(char * label)
{
  symbol *ptr = head;
  while (ptr)
  {
    if (!strcmp(ptr->label, label))
    {
      return ptr;
    }
    ptr = ptr->next;
  }
  return NULL;
}
