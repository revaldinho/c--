// ****************************************************************************
//							compiler.c
//
// ----------------------------------------------------------------------------
//
// Generic compiler based on Martin Davies' skeleton Modula-2 code in
// the "Code generation" article from PCW, Sept 1988.
//
// ----------------------------------------------------------------------------
// Date     : Comment
// ----------------------------------------------------------------------------
// 28.02.94 : First adaptation from Modula 2 original
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "analyse.h"
#include "symboltable.h"
#include "code.h"
#include "compiler.h"
#include "Boolean.h"
//----------------------------Const. definitions-------------------------------
#define kMaxSymbols		512		// maximum number of entries in Symbol table
// ----------------------------External Globals--------------------------------
extern TOKEN  	gTokenVal;
extern int    	gNextLabel;
extern Boolean	gLoaded;
// ----------------------------Global Variables--------------------------------
FILE			*gFp;							// a file pointer for input stream
TOKEN			gNextToken;				  		// ?

SymTabType		*gSymbolTable[kMaxSymbols];		// pointers to Symbol table entries
int				gNoOfSymbols;					// no of symbol table entries
//*****************************************************************************
// main 
//-----------------------------------------------------------------------------
int	main( int argc, char **argv)
{

	if ((gFp = fopen( *++argv, "r")) == NULL) {
		printf("Couldn't open input file %s.\n", *argv);
		exit(1);
		}
	InitLex( );
	InitCode("xxx");
		
	gNextToken = Lex( gFp );
	Match(TBEGIN);
	while (gNextToken != DONE && gNextToken != TEND) {
		Statement();
		}
	Match(TEND);
}

//*****************************************************************************
// Statement
//-----------------------------------------------------------------------------
void Statement(void)
{
switch	(gNextToken) {
	case TIF:		Parse_If() ; break;
	case TREPEAT:	Parse_Repeat() ; break;
	case ID:		Parse_Assign() ; break;
	default:		break;
	}
Match(SEMICOLON);
}

//*****************************************************************************
// Parse_Assign
//-----------------------------------------------------------------------------
void Parse_Assign()
{
int	tmp;

tmp = gTokenVal;			// store identifier
Match(ID);					
Match(BECOMES);
Expr();						// parse expression
Generate(ID, tmp);			// generate code for the assignment
Generate(BECOMES, NONE);	// after generating the expression
}

//*****************************************************************************
// Parse_Repeat
//-----------------------------------------------------------------------------
void Parse_Repeat()
{
int start;

Match(TREPEAT);
start = gNextLabel++;		// generate label for start of loop
Generate(TLABEL, start);
do {
	Statement();				// parse statements
   } while (gNextToken != TUNTIL && gNextToken != DONE );
Match(TUNTIL);
Expr();						// parse expression
Generate(GOFALSE,start);	// generate goto start of loop
}

//*****************************************************************************
// Parse_If
//-----------------------------------------------------------------------------

void Parse_If()
{
int		out, endif;

Match(TIF);						// find 'if'
Expr();
out = gNextLabel++;				// get a unique label
Generate(GOFALSE, out);			// go to it if result is false
Match(TTHEN);
Statement();					// parse a statement
if (gNextToken == TELSE) {		// if there's an else cause
	Match(TELSE);
	endif = gNextLabel++;		// get another label
	Generate(TGOTO, endif);
	Generate(TLABEL, out);
	Statement();
	Generate(TLABEL, endif);
	}
else {
	Generate(TLABEL, out);
	}
Match(TENDIF);
}

//*****************************************************************************
// Expr - parse an expression
//-----------------------------------------------------------------------------
// An expression is a term followed by zero or more repetitions of an operator,
// followed by a term.
//-----------------------------------------------------------------------------

void Expr()
{
TOKEN	lastToken;
Boolean loopOn;

loopOn = true;
gLoaded = false; 							// register has to be loaded
Term();										// parse a term
do {									
	switch( gNextToken) {
		case PLUS :
		case MINUS:
		case DIVI :
		case MULT :
		case EQUALS:	lastToken = gNextToken;
						Match(gNextToken);
						Term();
						Generate(lastToken, NONE);
						break;
		default:		loopOn = false; break;			// end of expression
		}
	} while (loopOn);
}

//*****************************************************************************
// Term - check that terms are well formed
//-----------------------------------------------------------------------------

void Term()

{
int	temp;

switch (gNextToken) {
	case LBRACKET : Match(LBRACKET);
					Generate(LBRACKET, NONE);
					Expr();
					Match(RBRACKET);
					Generate(RBRACKET, NONE);
					break;
	case NUM:		Generate(NUM, gTokenVal);
					Match(NUM);
					break;
	case ID:		Generate(ID, gTokenVal);
					Match(ID);
					break;
	default:		Error("Syntax error.");
	}
}

//*****************************************************************************
// Match - move to next token
//-----------------------------------------------------------------------------
void Match( TOKEN wanted)
{
if (wanted == gNextToken)
	gNextToken = Lex( gFp);
else 
	Error("Syntax Error.");
}
