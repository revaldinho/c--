//---------------------------------------------------------
// Header file for code.c code generator module.
//---------------------------------------------------------

#define		NONE	0

void	Declare( char *s);
void	PrintOp( char *s);
void	PrintLabel(int x);
void 	InitCode(char *s);
void	Generate(TOKEN tok, int value);

