#include "header.h"

/*This file contains function used for file handling*/

/*openInputFile function - used to open files.
  takes 3 strings - filename, mode and extension.
  returns a pointer to the opened file, or null if failed*/
FILE * openFile(char* filename, char * mode, char * extension)
{
  /*Temp file to be returned*/
  FILE * tmp = NULL;
  /*Full filename string*/
  char full_filename[MAX_FILENAME];
  /*Copy the filename from argv to the full filename string*/
  strcpy(full_filename, filename);
  /*Add the .as extension to the full filename.*/
  strcat(full_filename, extension);
  /*Open the file. if fopen return null, exit the function and print error to screen*/
  if (!(tmp = fopen(full_filename, mode)))
  {
    printf("Can't open file %s.\n", full_filename);
    return NULL;
  }
  /*Return pointer to the file*/
  return tmp;
}

/*This function exports the output files*/
void exportFiles(int IC, int DC, char * name)
{
  /*FILE pointers*/
  FILE * object = NULL;
  FILE * entries = NULL;
  FILE * externs = NULL;
  /*Word holder*/
  unsigned short int * toPrint;
  /*Extern flag*/
  int nExtern = FALSE;
  /*Entry flag*/
  int nEntry = 0;
  /*Pointer to wordNode*/
  wordNode * p = NULL;
  /*Pointer to entry*/
  entry * pe = NULL;
  /*Pointer to data*/
  data * pd = NULL;
  /*open an object file in write mode*/
  object = openFile(name, "w", OB_EXTENSION);
  /*assign wordNode pointer with wordNode head*/
  p = getHeadWord();
  /*Print the number of words and number of data in hexadecimal*/
  fprintf(object, "%x %x\n", countWords(), countData());
  /*go through the entire words list*/
  while (p)
  {
    /*Assign toPrint with corresponding word*/
    switch(p->method)
    {
      case NONE:
        toPrint = (unsigned short int *)(&(p->holder.first));
      case IMMEDIATE:
        toPrint = (unsigned short int *)(&(p->holder.immediate));
      case DIRECT:
        toPrint = (unsigned short int *)(&(p->holder.immediate));
      case INDEX:
        toPrint = (unsigned short int *)(&(p->holder.index));
      case REGISTER:
        toPrint = (unsigned short int *)(&(p->holder.reg));
    }
    /*Print the address and word in hexadecimal*/
    fprintf(object, "%02x %04x\n", p->address, *toPrint);
    /*If we found an external word, set flag to 0*/
    if (p->isExternal == TRUE)
    {
      nExtern=TRUE;
    }
    p = p->next;
  }
  /*Assign pointer with data list head*/
  pd = getHeadData();
  /*print all the data & addresses in hexadecimal*/
  while (pd)
  {
    fprintf(object, "%02hx %04hx\n", pd->address, pd->value);
    pd = pd->next;
  }
  /*Assign pointer with entry list head*/
  pe = getHeadEntry();
  /*If the list is not empty*/
  if (pe)
  {
    /*open a file in write mode for entries*/
    entries = openFile(name, "w", ENT_EXTENSION);
    /*go through the entry list and write the labels and addresses in hexadecimal*/
    while (pe)
    {
      fprintf(entries, "%s %x\n", pe->label, pe->address);
      pe = pe->next;
      nEntry++;
    }
  }
  /*If we found an extern word earlier*/
  if (nExtern)
  {
      /*open an extern file in write mode*/
      externs = openFile(name, "w", EXT_EXTENSION);
      /*assign pointer with word list head*/
      p = getHeadWord();
      /*Go through the list*/
      while (p)
      {
        /*if the word is external*/
        if (p->isExternal == TRUE)
        {
          /*print the label and address in hexadecimal*/
          fprintf(externs, "%s %02x\n", p->externalLabel, p->address);
        }
        p = p->next;
      }
  }
  printf("Success! files exported.\n");
  /*Clean up*/
  if (object)
    fclose(object);
  if (entries)
    fclose(entries);
  if (externs)
    fclose(externs);
}
