#include "header.h"
/*This file contains the first pass function*/

/*The first pass function will go throgh each line.
It will first check for a label, then check for a directive.
if there's no directive, it will check for a command. It will add errors
to the errors list if found, and will go through the entire input file*/
void firstPass(FILE * f, int * IC, int * DC)
{
  /*Line buffer*/
  char line[MAX_LINE];
  /*Line counter*/
  int lineCounter = 0;
  /*Line fetching loop*/
  while (fgets(line, MAX_LINE, f))
  {
    /*pointer to token*/
    char * token = NULL;
    /*pointer to label*/
    char * label  = NULL;
    /*directive flag*/
    int directive = 0;
    /*get first token*/
    token = strtok(line, " \t\n");
    /*increment line counter*/
    lineCounter++;
    /*if line is empty, go to next line*/
    if (!token)
    {
      continue;
    }
    /*if line is comment, go to next line*/
    if (isComment(token))
    {
      continue;
    }
    /*If label is not NULL after calling getLabel()*/
    if ((label = getLabel(token)))
    {
      /*If getLabel returned error string*/
      if (!strcmp(label, "$"))
      {
        /*Add error*/
        addError("Illegal label", lineCounter, token);
        /*set label to null*/
        label = NULL;
      }
      else if (!strcmp(label, "@"))
      {
        addError("Label name is reserved", lineCounter, NULL);
      }
      /*Get next token.*/
      token = strtok(NULL, " \t\n");
      /*if next token is null, add error and go to next line*/
      if (!token)
      {
        addError("Must add something next to label", lineCounter, NULL);
        continue;
      }
    }
    /*Check for directive*/
    if ((directive = getDirective(token)))
    {
      /*if getDirective returned an error*/
      if (directive == DIRECTIVE_ERROR)
      {
        /*add error*/
        addError("Invalid directive", lineCounter, token);
        continue;
      }
      /*if directive is data directive*/
      else if (directive == DIRECTIVE_DATA)
      {
        /*if there's a label*/
        if (label)
        {
          addSymbol(label, *DC, FALSE, FALSE, lineCounter);
          splitNumbers(token, lineCounter, DC);
        }
        /*if there isn't a label*/
        else
        {
          splitNumbers(token, lineCounter, DC);
        }
      }
      /*if directive is string directive*/
      else if (directive == DIRECTIVE_STRING)
      {
        /*get next token*/
        token = strtok(NULL, "\n");
        /*if token is null*/
        if (!token)
        {
          /*add error and go to next line*/
          addError("Must specify a string", lineCounter, NULL);
          continue;
        }
        /*if there's a label*/
        if (label)
        {
          /*add to symbols list*/
          addSymbol(label, *DC, FALSE, FALSE, lineCounter);
          /*add to date*/
          addString(token, lineCounter, DC);
          /*go to next line*/
          continue;
        }
        /*if there is no label*/
        else
        {
          /*add to data*/
          addString(token, lineCounter, DC);
          /*go to next line*/
          continue;
        }
      }
      /*if directive is entry*/
      else if (directive == DIRECTIVE_ENTRY)
      {
        /*if we have a label, print a warning*/
        if (label)
          printf("Warning, line %d: label \"%s\" before entry directive is meaningless\n", lineCounter, label);
        /*get next token*/
        if (!(token = strtok(NULL, " \t\n")))
        {
          addError("Entry directive must have a label parameter", lineCounter, NULL);
          continue;
        }
        /*add an entry node*/
        addEntry(token, lineCounter);
        /*if there's a next token*/
        if ((token = strtok(NULL, "\n")))
        {
          /*add error and go to next line*/
          addError("Entry directive can only take one parameter", lineCounter, NULL );
          continue;
        }
        /*if next token is null*/
        else
        {
          /*go to next line*/
          continue;
        }
      }
      /*if directive is extern*/
      else if (directive == DIRECTIVE_EXTERN)
      {
        /*if there's a label, print a warning*/
        if (label)
          printf("Warning, line %d: label \"%s\" before extern directive is meaningless\n", lineCounter, label);
        /*get next token*/
        if (!(token = strtok(NULL, " \t\n")))
        {
          addError("Extern directive must have a label parameter", lineCounter, NULL);
          continue;
        }
        /*add extern symbol*/
        addSymbol(token, 0, TRUE, FALSE, lineCounter);
        /*if next token is not null*/
        if ((token = strtok(NULL, "\n")))
        {
          /*add error and go to next line*/
          addError("Extern directive can only take one parameter", lineCounter, NULL );
          continue;
        }
        /*if next token is null*/
        else
        {
          /*go to next line*/
          continue;
        }
      }
    }
    /*if the token is not a directive*/
    else
    {
      /*Analyze the operation*/
      analyzeOperation(token, lineCounter, IC, label);
    }
  }
  /*If the file is empty*/
  if (lineCounter == 0)
  {
    addError("ERROR: File is empty.", -1, NULL);
  }

}
