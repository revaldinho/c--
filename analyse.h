//------------------------------------------------
// Analyse.h
//------------------------------------------------

//-------------------------------------------------------------------
enum  { TIF, TTHEN,TELSE,TENDIF,TREPEAT,TUNTIL,ID,DONE,NUM,DIVI, 
		MULT,TBEGIN,TEND,PLUS,MINUS,BECOMES,EQUALS,LBRACKET,
		RBRACKET,TLABEL,TGOTO,GOFALSE,SEMICOLON,GREATERTHAN } ;

typedef	int	TOKEN;
//------------------------------------------------------------------

TOKEN	Lex(FILE *fp) ;
void	InitLex(void);
void	Error(char *message) ;

