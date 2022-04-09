/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
*/

#include "parse.h"


map<string, bool> defVar;
map<string, Token> SymTable;



namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}


//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
// Stmt ::= AssigStmt | IfStmt | WriteLnStmt | ForStmt
// TODO: Done?
bool Stmt(istream& in, int& line) {
	bool status;
	//cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		//cout << "After WriteStmet status: " << (status? true:false) <<endl;
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;
		
	case FOR:
		status = ForStmt(in, line);
		
		break;
		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt


// WriteLnStmt ::= WRITELN (ExprList)
// TODO: Done?
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return ex;
} // end of WriteLnStmt


// ExprList ::= Expr {, Expr}
// TODO: Done?
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
} // end of ExprList

// Expr ::= Term {(+|-) Term}
// TODO: Done?
bool Expr(istream& in, int& line){
	bool status;
	LexItem tok;
	cout << "before Term" << endl;
	status = Term(in, line);
	if (!status){
		return status;
	}
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "(Expr) invalid input");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok.GetToken() == PLUS || tok.GetToken() || MINUS){
		status = Term(in, line); // this is the next one
		if(!status){
			ParseError(line, "(Expr) missing RHS");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return status;
		}
	}
	Parser::PushBackToken(tok);  // extra token from while loop

	return status;
} // end of Expr

// Prog ::= PROGRAM IDENT; DeclBlock ProgBody
// FIXME: FINISH THIS
bool Prog(istream& in, int& line){
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);

	// program start
	if (tok.GetToken() == PROGRAM){
		// get what's after PROGRAM
		while(tok.GetToken() != END){
			if (tok.GetToken() == VAR){
				status = DeclBlock(in, line);
			}
			//move on
			tok = Parser::GetNextToken(in, line);

			if (tok.GetToken() == BEGIN){
				cout << "before enter ProgBody: tok " << tok.GetLexeme() << endl;
				status = ProgBody(in, line);
			}
			if (tok.GetToken() == END && (error_count = 0)){
				return true;
			}
		}
		// cout << "end? " << tok.GetLexeme() << endl;
	}
	else if (tok.GetToken() != PROGRAM){
		ParseError(line, "Missing PROGRAM.");
		return false;
	}
	if (tok.GetToken() == ERR){
		ParseError(line, "(Prog) Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (error_count > 0){
		ParseError(line, "(Prog) Invalid statements");
		return false;
	}
	return status;
} // end of Prog

// ProgBody ::= BEGIN {Stmt;} END
// FIXME: FINISH THIS
bool ProgBody(istream& in, int& line){
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);
	if (defVar.find(tok.GetLexeme())->second){
		cout << "before AssignStmt tok " << tok.GetLexeme() << endl;
		Parser::PushBackToken(tok);
		AssignStmt(in, line);
	}
	cout << "before Stmt " << endl;
	status = Stmt(in, line);

	return status;
} // end of ProgBody

// DeclBlock ::= VAR {DeclStmt;}
// FIXME: FINISH THIS
bool DeclBlock(istream& in, int& line){
	bool status;
	status = false;
	LexItem tok = Parser::GetNextToken(in, line);
	while (tok.GetToken() != BEGIN && tok.GetToken() != ASSOP){
		if (tok.GetToken() == IDENT){
			cout << "IDENT " << tok.GetLexeme() << endl;
			if (defVar.find(tok.GetLexeme())->second){
				ParseError(line, "(DeclBlock) Variable Redeclaration");
				return false;
			}
			else{
				defVar.insert(pair<string, bool> (tok.GetLexeme(), true));
				SymTable.insert(pair<string, Token> (tok.GetLexeme(), tok.GetToken()));
			}
		}
		// get next
		tok = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(tok);
	return status;
} // end of DeclBlock

// DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
// TODO: Done?
bool DeclStmt(istream& in, int& line){
	bool status;
	status = false;

	LexItem tok = Parser::GetNextToken(in, line);

	if(tok.GetToken() == INTEGER || tok.GetToken() == REAL || tok.GetToken() == STRING){
		// is expression valid?
		status = ExprList(in, line);

		if (status){
			tok = Parser::GetNextToken(in, line);
			// check for definition in DeclStmt
			if (tok.GetToken() == IDENT){
				// check if already in defVar
				if (defVar.find(tok.GetLexeme())->second){
					ParseError(line, "(DeclStmt) Variable Redefinition");
					return false;
				}
				else{
					// record new variable
					defVar.insert(pair<string, bool> (tok.GetLexeme(), true));
					SymTable.insert(pair<string, Token> (tok.GetLexeme(), tok.GetToken()));
				}
				// check if declaring multiple variables
				tok = Parser::GetNextToken(in, line);
				if (tok.GetToken() == COMMA){
					// start again for next variable after comma
					status = DeclStmt(in, line);
					if (!status){
						ParseError(line, "(DeclStmt) missing identifier after a comma");
						return status;
					}
				}
				else{
					Parser::PushBackToken(tok);
					return true;
				}
			}
			else{
				ParseError(line, "(DeclStmt) missing declaration");
				return false;
			}
		}

		if (!status){
			ParseError(line, "(DeclStmt) ExprList() returned false");
			return status;
		}
	}

	else{
		Parser::PushBackToken(tok);
		ParseError(line, "(DeclStmt) invalid type");
		return status;  // should be false if made it here
	}
	// made through first if, skipped second, should be true
	return status;
} // end of DeclStmt

// IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
// TODO: Done?
bool IfStmt(istream& in, int& line){
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);
	// check for LPAREN
	if (tok.GetToken() != LPAREN){
		ParseError(line, "(IfStmt) missing LPAREN");
		return status;
	}
	// LPAREN is there, check LogicExpr
	status = LogicExpr(in, line);
	if (!status){
		ParseError(line, "(IfStmt) LogicExpr returned false");
		return status;
	}
	// get next
	tok = Parser::GetNextToken(in, line);
	// check for RPAREN
	if (tok.GetToken() != RPAREN){
		ParseError(line, "(IfStmt) missing RPAREN");
		return false;
	}
	// check the stmt
	status = Stmt(in, line);
	if (!status){
		ParseError(line, "(IfStmt) missing Stmt");
		return status;
	}
	return true;
} // end of IfStmt

// ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
// FIXME: FINISH THIS
bool ForStmt(istream& in, int& line){
	bool status = false;
	return status;
} // end of ForStmt

// AssignStmt ::= Var := Expr
// TODO: Done?
bool AssignStmt(istream& in, int& line){
	bool status = false;
	bool variable_status = false;
	variable_status = Var(in, line);
	LexItem tok = Parser::GetNextToken(in, line);
	cout << "in AssignStmt " << tok.GetLexeme() << endl;
	if (variable_status){
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ASSOP){
			status = Expr(in, line);
			if(!status){
				ParseError(line, "(AssignStmt) Expr returned false in AssignStmt");
				return status;
			}
		}
		else if (tok.GetToken() == ERR){
			ParseError(line, "(AssignStmt) invalid input");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		else{
			ParseError(line, "(AssignStmt) missing :=");
			return status;
		}
	}
	else if (!variable_status){
		ParseError(line, "(AssignStmt) missing LHS (Var() returned false)");
		return false;
	}
	return true;
} // end of AssignStmt

// Var ::= IDENT
// TODO: Done?
bool Var(istream& in, int& line){
	bool status;
	status = false;
	string lexeme = "";
	LexItem tok = Parser::GetNextToken(in, line);
	cout << "in var: tok " << tok.GetLexeme() << endl;
	if (tok.GetToken() == IDENT){
		cout << "in var IDENT" << endl;
		lexeme = tok.GetLexeme();

		if (!(defVar.find(lexeme)->second)){  // not declared
			ParseError(line, "(Var) Undeclared variable");
			return false;
		}
		else{  // variable is declared
			Parser::PushBackToken(tok);
			status = true;
		}
	}
	else if (tok.GetToken() == ERR){
		ParseError(line, "(Var) Invalid statement");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
} // end of Var

// LogicExpr ::= Expr (= | > | <) Expr
// TODO: Done?
bool LogicExpr(istream& in, int& line){
	bool status = false;
	status = Expr(in, line);
	if (!status){
		return status;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "(LogicExpr) invalid input");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if (tok.GetToken() == EQUAL || tok.GetToken() == GTHAN || tok.GetToken() == LTHAN){
		status = Expr(in, line);
		if (!status){
			ParseError(line, "(LogicExpr) missing RHS");
			return status;
		}
	}
	return status;
} // end of LogicExpr


// Term ::= SFactor {( * | / ) SFactor}
// TODO: Done?
bool Term(istream& in, int& line){
	bool status = false;
	cout << "before SFactor" << endl;
	status = SFactor(in, line);

	if (!status){  // SFactor returned false
		return status;
	}
	// SFactor returned true, go on
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR){
		ParseError(line, "(Term) Invalid expression");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok.GetToken() == MULT || tok.GetToken() == DIV){
		status = SFactor(in, line);
		if (!status){
			ParseError(line, "(Term) missing RHS of operator");
			return status;
		}
		// check next
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR){
			ParseError(line, "(Term) Invalid expression");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	// checked extra in while so pushback
	Parser::PushBackToken(tok);
	return status;
} // end of Term

// SFactor ::= [(+ | -)] Factor
// TODO: Done?
bool SFactor(istream& in, int& line){
	bool status = false;
	int sign = 0;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == PLUS){
		sign = 1;
	}
	else if (tok.GetToken() == MINUS){
		sign = -1;
	}
	else{ // not signed factor, put back for when Factor calls GetNextToken()
		Parser::PushBackToken(tok);
	}
	// get status with sign
	cout << "before Factor: tok " << tok.GetLexeme() << endl;
	status = Factor(in, line, sign);
	return status;
} // end of SFactor

// Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
// TODO: Done?
bool Factor(istream& in, int& line, int sign){  // what do with sign?
	bool status;
	LexItem tok;
	status = false;
	tok = Parser::GetNextToken(in, line);
	string lexeme = tok.GetLexeme();

	if (tok.GetToken() == IDENT){
		if (!(defVar.find(lexeme)->second)){  // not defined
			ParseError(line, "(Factor) Undefined variable");
			return status;
			}
		else{  // variable is defined
			status = true;
		}
	}
	else if (tok.GetToken() == ICONST || tok.GetToken() == RCONST || tok.GetToken() == SCONST){
		return status = true;
	}
	else if (tok.GetToken() == LPAREN){
		bool expr_status = Expr(in, line);
		if (!expr_status){
			ParseError(line, "(Factor) Missing expression in parenthesis");
			return expr_status;
		}
		// need tok after LPAREN
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == RPAREN){
			return expr_status;
		}
		else{  // never hit RPAREN
			ParseError(line, "(Factor) Missing right parenthesis");
			return status;
		}
	}
	else if (tok.GetToken() == ERR){
		ParseError(line, "(Factor) Invalid expression");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	// some strange error if made it here
	ParseError(line, "(Factor) Invalid something");
	cout << "(" << tok.GetLexeme() << ")" << endl;
	return status;
} // end of Factor
