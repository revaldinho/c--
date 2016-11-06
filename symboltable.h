//*******************************************************************
//							symtable.h
//-------------------------------------------------------------------

typedef struct
        {  
        char 	*entry;
        TOKEN	t;
        } SymTabType;
        

int InsertSymbol(SymTabType *a,SymTabType *b[], int *c);
int LookupStr(char *a,SymTabType *b[], int c);
char *StrSave( char *String );
int FindSymbolStr(char *string, SymTabType *symbolTable[], int noOfSymbols);
