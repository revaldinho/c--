//*****************************************************************************
//						          Code.c
//-----------------------------------------------------------------------------
//
// Code generator module for generic assembler example.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "analyse.h"
#include "symboltable.h"
#include "compiler.h"
#include "code.h"
#include "Boolean.h"

// ----------------------------External Globals--------------------------------
extern SymTabType 	*gSymbolTable[];
extern int	      	gNoOfSymbols;

// ----------------------------Global Variables--------------------------------
int					gFreeSpace;		// Marks free data area
Boolean				gPopped;		// last operation was a pop
int					gNextLabel;
Boolean				gLoaded;
FILE				*gOutFp;			// output file pointer
struct				{
					TOKEN	tok;
					int		value;
					}	gLast;		// Last token
					
//*****************************************************************************
// Declare - allocate space for variables 
//-----------------------------------------------------------------------------
void	Declare(char *s)
{
fprintf(gOutFp, "%s EQ %X\n", s, gFreeSpace);
gFreeSpace += 2;								// assume byte addressing
}


//*****************************************************************************
// PrintOp - print operator
//-----------------------------------------------------------------------------
void	PrintOp(char *operator)
{
fprintf(gOutFp, "%s", operator);
if (gPopped) {
	fprintf(gOutFp,"_TMP"); // last op was a pop so load from TMP
	gPopped = false;
	}
else if (gLast.tok == NUM) {
	fprintf(gOutFp, "%d", gLast.value);	// a literal
	}
else {
	fprintf(gOutFp,"%s", gSymbolTable[gLast.value]->entry);
	}
fprintf(gOutFp, "\n");
}

//*****************************************************************************
// PrintLabel - 
//-----------------------------------------------------------------------------
void	PrintLabel(int x)
{
fprintf(gOutFp,"L%d:\n", x);
}

//*****************************************************************************
// Generate - generates code for each construct in a simple pseudo-assembler.
// All operations are of the form <LABEL:> operator operand
//-----------------------------------------------------------------------------

void	Generate(TOKEN tok, int value)
{
switch (tok) {
	case PLUS	: PrintOp(" ADD "); break;
	case MINUS	: 
	case GREATERTHAN :PrintOp(" SUB "); break;
	case MULT	: PrintOp(" MUL "); break;
	case DIVI	: PrintOp(" DIV "); break;
	case BECOMES: PrintOp(" STORE "); break;
	case EQUALS : PrintOp(" XOR "); break;
	case LBRACKET: fprintf(gOutFp, " PUSH\n"); break;
	case RBRACKET: fprintf(gOutFp, " POP _TMP\n"); gPopped = true; break;
	case ID		: if (!gLoaded)  // start of expression
						fprintf(gOutFp," LOAD %s\n",gSymbolTable[value]->entry);
				  break;
	case NUM	: if (!gLoaded)  // start of expression
						fprintf(gOutFp," LOAD %d\n",value);
				  break;
	case TLABEL	: PrintLabel(value); break;
	case GOFALSE: fprintf(gOutFp," JNZ "); PrintLabel(value); break;
	case TGOTO	: fprintf(gOutFp," JMP "); PrintLabel(value); break;
	default		: break;	// don't do anything with unrecognized token types...
	}
	
gLast.tok = tok;
gLast.value = value;
gLoaded = true;
}

//*****************************************************************************
// InitCode - initialise globals in the code module
//-----------------------------------------------------------------------------
void	InitCode(char *theFileName)
{
//if ( (gOutFp = fopen("theFileName, "w")) == NULL) {
//    printf("AArrgh: couldn't open the output file %s.\n", theFileName);
//    exit(1);
//    }

gOutFp = stdout;  
gFreeSpace = 1000;	// start of data area
Declare("_TMP");	// set aside temporary storage
gNextLabel = 1;		// set first label's value
gLoaded = false;	// accumulator is not loaded
}
						
	
