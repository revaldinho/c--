//----------------------------------------------------------
// Header file for compiler.c module.
//----------------------------------------------------------

void	Expr ( void);
void	Statement (void);
void	Parse_If(void);
void	Parse_Assign(void);
void	Parse_Repeat(void);
void	Term(void);
void	Match(TOKEN wanted);