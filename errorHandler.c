#include "header.h"
/*This file contains function which handle the errors list*/

/*pointer to head*/
static error * head = NULL;

/*This function adds an error to the errors list.
meant for internal use
HOW TO USE:
addError(err, line, str)
err - string which describes the error. no need to add newline or anything.
line - line number. a negative number means no line.
str - adds an error string (primarily for syntax errors)
*/
void addError(char * err, int line, char * str)
{
  /*create a new error node*/
  error * tmp = malloc(sizeof(error));
  checkAllocation(tmp);

  /*assign error stringdd*/
  tmp->str_error = err;
  /*assign error line*/
  tmp->line = line;
  /*if we received another string*/
  if (str)
  {
    /*check if last char is newline, if so, remove it*/
    if (str[strlen(str)-1] == '\n')
    {
      str[strlen(str)-1] = 0;
    }
    /*assign the string to str*/
    tmp->str = malloc(sizeof(char) * MAX_LINE);
    checkAllocation(tmp->str);
    strcpy(tmp->str, str);
  }
  /*if we didn't receive another string*/
  else
  {
    /*set str to null*/
    tmp->str = NULL;
  }
  /*set next to null*/
  tmp->next = NULL;
  if (!head)
  {
    /*point head to temp*/
    head = tmp;
  }
  /*if head is not null*/
  else
  {
    error * p = head;
    while (p->next)
      p = p->next;
    /*point it to the new error*/
    p->next = tmp;
  }

}


/*This function will return 0 if there's no error, and another value if there was an error*/
int isError()
{
  if (head)
    return TRUE;
  else
    return FALSE;
}

/*This function returns the last line an error appears in.
used in the error printing loop*/
int getMaxLine()
{
  error * p;
  int maxLine = 1;
  p = head;
  if (!p)
    return 0;
  while (p)
  {
    if (p->line > maxLine)
      maxLine = p->line;
    p = p->next;
  }
  return maxLine;
}

/*this function will write the errors to an errors file*/
void printErrors(char * filename)
{
  /*error file*/
  FILE * tmp = NULL;
  error * ptr = head;
  int maxLine = getMaxLine();
  int index = 1;
  /*create filename for this input file*/
  char error_filename[MAX_FILENAME];
  strcpy(error_filename, "errors-");
  strcat(error_filename, filename);
  /*pointer to go through all the errors*/

  /*open the file and check if succeeded*/
  if ((tmp = openFile(error_filename, "w", ".txt")))
  {
    if (!ptr)
    {
      fclose(tmp);
      return;
    }
    /*write the errors*/
    while (index <= maxLine)
    {
      ptr = head;
      while (ptr)
      {
        if (ptr->line == index)
        {
          if (ptr->str)
            fprintf(tmp, "ERROR, LINE %d:\t %s \"%s\".\n", ptr->line, ptr->str_error, ptr->str);
          else
          {
            fprintf(tmp, "ERROR, LINE %d:\t %s.\n", ptr->line, ptr->str_error);
          }
        }
        ptr = ptr->next;
      }
      index++;
    }
    ptr = head;
    while (ptr)
    {
      if (ptr->line < 0)
      {
        if (ptr->str)
          fprintf(tmp, "%s: \"%s\"\n.", ptr->str_error, ptr->str);
        else
        {
          fprintf(tmp, "%s.\n", ptr->str_error);
        }
      }
      ptr = ptr->next;
    }
    printf("Operation failed. Errors exported to %s.txt\n", error_filename);
  }
  fclose(tmp);

}

/*clean up the errors list*/
void freeErrors()
{
  error * ptr = head;
  if (!ptr)
    return;

    while (ptr)
    {
      error * tmp = ptr;
      ptr = ptr->next;
      free(tmp->str);
      free(tmp);
    }
    head = NULL;
}
