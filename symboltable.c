#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "analyse.h"
#include "code.h"
#include "symboltable.h"



/*-------------------------------------------------------------------------------------------
 * InsertSymbol
 *
 * Add a new symbol (string and value pair) to a symbol table, sorted in alphabetic order
 * of strings.
 *
 * If the symbol already exists then the routine returns -1, otherwise it returns the place
 * at which the new element was inserted. Note that the search is done with a simple linear
 * method- for large symbol tables this should be replaced with a binary search.
 * ------------------------------------------------------------------------------------------*/

int InsertSymbol(SymTabType *newSymbol, SymTabType *symbolTable[], int *noOfSymbols)

{
register short i, comparison, insertPlace;

/* Here's the dumb search routine */
for (i=0,  comparison = 1000; i< *noOfSymbols && comparison > 0; i++)
    comparison = strcmp(newSymbol->entry,  symbolTable[i]->entry); 

if (comparison == 0)
    return -1;
else if ((comparison >0 && i== *noOfSymbols) || (i == 0))
    insertPlace = i ;
else 
    insertPlace = i-1;

for (i= *noOfSymbols; i > insertPlace ; i--)
     symbolTable[i] = symbolTable[i-1];

symbolTable[insertPlace] = (SymTabType *)malloc( sizeof(SymTabType ));
symbolTable[insertPlace]->entry = StrSave( newSymbol->entry);
symbolTable[insertPlace]->t = newSymbol->t;


(*noOfSymbols)++;

if (newSymbol->t == ID )
	Declare(newSymbol->entry);

return (insertPlace);
}

/*---------------------------------------------------------------------------------
 * FindSymbolStr
 *
 * Find the symbol string inside a symbol table and return the position of the 
 * symbol. If not found then  return a negative number.
 *--------------------------------------------------------------------------------*/

int FindSymbolStr(char *string, SymTabType *symbolTable[], int noOfSymbols)

{
register int high, low=0, middle=0;
int          comparison=0;

if (noOfSymbols == 0)
   return (-1);
else {
    high= noOfSymbols - 1;
    while ( low <= high ) {
        middle=(high+low)>>1 ;  /* fast divide by two routine */ 
        comparison = strcmp ( string, symbolTable[middle]->entry);
        if (comparison < 0)
            high = middle -1;
        else if ( comparison > 0)
            low = middle + 1;
        else
            return (middle);
       }
    }
/* if the routine gets here, it didn't find the string! */
return(-1);
}

char *StrSave( char *String )

/* ---------------------------------------------------------------------------
 * Save a string to a safe place in memory.  If the save is successful it
 * returns a pointer to the start of the new string, otherwise a NULL value
 * is returned.
 * Note: malloc() needs to be treated with care! Different C compilers
 * have various interpretations of it.
 * --------------------------------------------------------------------------- */

{

char *Ptr;

if ( (Ptr = (char *)malloc(strlen(String) +1)) != NULL)
    strcpy(Ptr, String);
return (Ptr);
}

