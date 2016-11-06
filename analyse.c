//********************************************************************
// 							Analyse.h
//--------------------------------------------------------------------
//
// Lexical analyser for a generic compiler, based on Martin Davies's
// "Code compilation" article in Personal Computer World of Sep '88.
//
//--------------------------------------------------------------------

#include 	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	"analyse.h"
#include	"symboltable.h"
#include        "Boolean.h"

//------------------------- Constant Decl.  --------------------------
#define		kBufferSize  30					// size of input buffer
//------------------------- Module Globals ---------------------------
static int		gLineNo = 1;					// current line no.
TOKEN			gTokenVal;

//-------------------------External Globals---------------------------
extern	SymTabType		*gSymbolTable[];	// the symbol table itself
extern	int				gNoOfSymbols;				// number of entries


//********************************************************************
// Error : print an error message.
//--------------------------------------------------------------------
void Error ( char *message)
{
printf("%s  Line: %d\n", message, gLineNo);
}


//********************************************************************
// Lex : 	the main lexical analysis procedure, reads in a token from
// 			the input file and returns the token's value.
//
// Parameter fp is a file already opened for reading, pointing to the
// next character to be read;
//--------------------------------------------------------------------
TOKEN	Lex(FILE *fp)

{
int				tokenSize;					// size of token found
char			ident[kBufferSize];			// declare buffer
static Boolean	unGetChar = false;			// set if the buffer is full
static int		ch;							// one character buffer (must be int to detect EOF)

while (true) {
	if (!unGetChar)
		ch = fgetc(fp);
	else
		unGetChar = false;
	
	if ( isdigit(ch) ) {
	// It's a digit so expect a decimal constant, work out its value and
	// return the fact with NUM.
		gTokenVal = 0;
		while ( isdigit(ch)) {
			gTokenVal = (ch - '0') + gTokenVal * 10;
			ch = fgetc(fp);
			}
		unGetChar = true;
		return( NUM );
		}
	else if (isalpha(ch)) {
	// It's a letter so expect a Identifier/keyword.
		tokenSize = 0;
		do	{
			ident[tokenSize++] = ch;
			ch = fgetc(fp);
			} while ( (isalnum(ch) || (ch == '_')) && ( tokenSize < kBufferSize));
		if (tokenSize >= kBufferSize) {
			Error("Token too large.");
			exit(1);
			}
		
		ident[tokenSize] = '\0';	// mark end of string
		gTokenVal = FindSymbolStr(ident, gSymbolTable, gNoOfSymbols); 	// Is it in the symbol table?
		if (gTokenVal == -1 ) { 		//  if not then add it to table
			SymTabType newSymbol;
			
			newSymbol.entry = StrSave(ident);
			newSymbol.t = ID;
			gTokenVal = InsertSymbol(&newSymbol, gSymbolTable, &gNoOfSymbols);
			unGetChar = true;
			return (ID);
			}
		else {
			unGetChar = true;
			return (gSymbolTable[gTokenVal]->t);		
			}
		}
	else	// must be a special character
		{
		switch (ch) {
			case '(' : return(LBRACKET); break;
			case ')' : return(RBRACKET); break;
			case '*' : return(MULT) ; break;
			case '/' : return(DIVI); break;
			case '+' : return(PLUS); break;
			case '-' : return(MINUS) ; break;
			case ';' : return(SEMICOLON); break;
			case '>' : return(GREATERTHAN); break;
			case '=' : return(EQUALS); break;
			case ':' : if ( (ch = fgetc(fp)) != '=')   
							Error("equals expected."); // expects ":=" assigment
					   else
					   		return( BECOMES );
					   break;
			case '\n': gLineNo++; break;			     // newline just increments line counter
			case EOF : fclose(fp) ; return (DONE); break;
			case '\t':
			case ' ' :
			case '\f': break;
			default	 : Error("Unknown character."); break;
			}
		}
	}
}
		

//**************************************************************************
// InitLex : add some reserved words to the symbol table before we start.
//--------------------------------------------------------------------------

void InitLex()
{
static SymTabType reservedSymbol[] = 
	{{"IF", TIF},  {"THEN", TTHEN},  {"ELSE", TELSE}, {"ENDIF", TENDIF}, {"REPEAT",TREPEAT},
	 {"UNTIL", TUNTIL}, {"BEGIN", TBEGIN}, {"END", TEND}};
		
#define kNoOfResWords 8

int i;
for (i = 0 ; i < kNoOfResWords ; i++) {	
	InsertSymbol(&reservedSymbol[i], gSymbolTable, &gNoOfSymbols);
	}
}



	
