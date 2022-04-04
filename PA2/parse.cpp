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
}


// ExprList ::= Expr {, Expr}
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
}

// Prog ::= PROGRAM IDENT; DeclBlock ProgBody
bool Prog(istream& in, int& line){
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR){
		ParseError(line, "(Prog) Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	if (error_count > 0){
		ParseError(line, "(Prog) Invalid statements");
		return status;
	}

	// program start
	if (tok.GetToken() == PROGRAM){
		// get what's after PROGRAM
		// go into ProgBody()?
		tok = Parser::GetNextToken(in, line);

		if (tok.GetToken() == IDENT){
			status = DeclBlock(in, line);

			// go on
			tok = Parser::GetNextToken(in, line);
		}
	}

	return status;
}

// DeclBlock ::= VAR {DeclStmt;}
bool DeclBlock(istream& in, int& line){
	bool status = false;
}

// DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
bool DeclStmt(istream& in, int& line){
	bool status = false;

	LexItem tok = Parser::GetNextToken(in, line);

	if(tok.GetToken() == INTEGER || tok.GetToken() == REAL || tok.GetToken() == STRING){
		// is expression valid?
		status = ExprList(in, line);

		if (status){
			//TODO: WORK ON THIS, add to defVar here?
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == IDENT){
				// check if already in defVar
				if (defVar.find(tok.GetLexeme())->second){
					ParseError(line, "Variable Redefinition");
					return false;
				}
			}
		}

		if (!status){
			ParseError(line, "(DeclStmt) Problem calling ExprList()");
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
}

// ProgBody ::= BEGIN {Stmt;} END
bool ProgBody(istream& in, int& line){
	bool status = false;
}

// IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
bool IfStmt(istream& in, int& line){
	bool status = false;
}

// ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
bool ForStmt(istream& in, int& line){
	bool status = false;
}

// AssignStmt ::= Var := Expr
bool AssignStmt(istream& in, int& line){
	bool status = false;
	bool variable_status = false;
	variable_status = Var(in, line);
	LexItem tok = Parser::GetNextToken(in, line);

	if (variable_status){
		if (tok.GetToken() == ASSOP){
			// call Expr?
		}
	}
}

// Var ::= IDENT
bool Var(istream& in, int& line){
	bool status = false;
	string lexeme = "";
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == IDENT){
		lexeme = tok.GetLexeme();

		if (!(defVar.find(lexeme)->second)){  // not defined
			ParseError(line, "(Var) Undefined variable");
			return status;
		}
		else{  // variable is defined
			status = true;
		}
	}
	else if (tok.GetToken() == ERR){
		ParseError(line, "(Var) Invalid statement");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
}

// LogicExpr ::= Expr (= | > | <) Expr
bool LogicExpr(istream& in, int& line){
	bool status = false;
}

// Expr ::= Term {(+|-) Term}
bool Expr(istream& in, int& line){
	bool status = false;
	// status = Term()?
}

// Term ::= SFactor {( * | / ) SFactor}
bool Term(istream& in, int& line){
	bool status = false;
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

}

// SFactor ::= [(+ | -)] Factor
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
	status = Factor(in, line, sign);
	return status;
}

// Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
bool Factor(istream& in, int& line, int sign){  // what do with sign?
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);
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
		status = true;
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
	return status;
}
