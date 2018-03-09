#include "header.h"

int main(int argc, char* argv[])
{
  /*Pointer to input file*/
  FILE * f = NULL;
  /*Index used to loop through the input files*/
  int index;
  /*Line counter*/
  int IC = 100;
  int DC = 0;

  /*Check if there are no arguments*/
  if (argc == 1)
  {
    /*Print error to screen*/
    printf("You did not provide any input files.\n");
  }
  else
  {
    /*Loop which goes through all of argv's elements*/
    for (index = 1; index < argc; index++)
    {
      printf("%s.as: ", argv[index]);
      /*Open the file and do everything*/
      if ((f = openFile(argv[index], "r", AS_EXTENSION)))
      {
        /*first pass*/
        firstPass(f, &IC, &DC);
        /*update symbols*/
        updateSymbols(IC);
        /*Update words list addresses*/
        updateAddresses();
        /*update data list addresses*/
        updateDataAddresses(IC);
        /*Second pass*/
        secondPass(IC);
        /*If there were no errors, export the files*/
        if (!isError())
        {
        exportFiles(IC, DC, argv[index]);
        }
        /*If there were errors, export the error file*/
        else
        {
          printErrors(argv[index]);
          /*Free the errors list*/
          freeErrors();
        }
        /*Cleanup*/
        freeData();
        freeSymbols();
        freeEntries();
        freeWords();
      }
    }

  }
  return 0;
}
