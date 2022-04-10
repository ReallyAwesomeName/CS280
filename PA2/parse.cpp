/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
*/

#include "parse.h"


map<string, bool> defVar;
map<string, Token> SymTable;
map<string, bool> tempmap;



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
	LexItem t = Parser::GetNextToken(in, line);
	cout << "in Stmt: tok " << t << endl;
	
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
		return true; // FIXME: true or false?
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
	cout << "in Expr before Term" << endl;
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
	while (tok.GetToken() == PLUS || tok.GetToken() == MINUS){
		cout << "in Expr before second Term: tok " << tok << endl;
		status = Term(in, line); // this is the next one
		if(!status){
			ParseError(line, "(Expr) missing RHS");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);  // extra token from while loop

	return status;
} // end of Expr

// Prog ::= PROGRAM IDENT; DeclBlock ProgBody
// FIXME: FINISH THIS
bool Prog(istream& in, int& line){
	bool status = false;
	bool declared = false;
	LexItem tok = Parser::GetNextToken(in, line);

	// program start
	if (tok.GetToken() == PROGRAM){
		// get what's after PROGRAM
		while(tok.GetToken() != END){
			if (tok.GetToken() == VAR){
				status = DeclBlock(in, line);
				declared = true;
			}
			//move on
			tok = Parser::GetNextToken(in, line);

			if (tok.GetToken() == BEGIN){
				// don't enter ProgBody unless variables have been declared
				if (!declared){
					line = 5;
					ParseError(line, "Non-recognizable Declaration Block.");
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
				cout << "before enter ProgBody: tok " << tok << endl;
				status = ProgBody(in, line);
				if (!status){
					ParseError(line, "Incorrect Program Body.");
					return false;
				}
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
		cout << "before AssignStmt tok " << tok << endl;
		Parser::PushBackToken(tok);
		AssignStmt(in, line);
	}
	cout << "before Stmt: tok " << tok << endl;
	if (tok.GetToken() == IF){
		Parser::PushBackToken(tok);
		status = Stmt(in, line);
	}
	status = Stmt(in, line);
	if (!status){
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	else{
		status = ProgBody(in, line);
	}

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
			// cout << "IDENT " << tok << endl;
			Parser::PushBackToken(tok);
			status = DeclStmt(in, line);

			// if (defVar.find(tok.GetLexeme())->second){
			// 	ParseError(line, "(DeclBlock) Variable Redeclaration");
			// 	return false;
			// }
			// else{
			// 	defVar.insert(pair<string, bool> (tok.GetLexeme(), true));
			// 	SymTable.insert(pair<string, Token> (tok.GetLexeme(), tok.GetToken()));
			// }
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
	// status = ExprList(in, line);
	LexItem tok = Parser::GetNextToken(in, line);
	// cout << "in DeclStmt: tok " << tok << endl;

	if (tok.GetToken() == IDENT){
		if (defVar.find(tok.GetLexeme())->second){
			ParseError(line, "(DeclStmt) Variable Redeclaration");
			return false;
		}
		else{
			defVar.insert(pair<string, bool> (tok.GetLexeme(), true));
			// cout << "inserting: " << tok.GetLexeme() << " in tempmap" << endl;
			tempmap.insert(pair<string, bool> (tok.GetLexeme(), true));
			// cout << "tempmap size at insert: " << tempmap.size() << endl;
		}
	}

	tok = Parser::GetNextToken(in, line);
	if(tok == INTEGER || tok == REAL || tok == STRING){
		// is expression valid?
		// status = ExprList(in, line);

		if (status){
			tok = Parser::GetNextToken(in, line);
			// check for definition in DeclStmt
			if (tok.GetToken() == IDENT){
				// check if already in defVar
				if (defVar.find(tok.GetLexeme())->second){
					ParseError(line, "(DeclStmt) Variable Redeclaration");
					return false;
				}
				else{
					// record new variable
					defVar.insert(pair<string, bool> (tok.GetLexeme(), true));
					// SymTable.insert(pair<string, Token> (tok.GetLexeme(), tok.GetToken()));
				}
				// check if declaring multiple variables
				tok = Parser::GetNextToken(in, line);
				if (tok.GetToken() == COMMA){
					// start again for next variable after comma
					status = DeclStmt(in, line);
					if (!status){
						ParseError(line, "(DeclStmt) missing identifier after a comma");
						return false;
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
			return false;
		}
	}
	else if (tok.GetToken() == COMMA){
		DeclStmt(in, line);
	}
	else if (tok.GetToken() == COLON){
		// get type
		tok = Parser::GetNextToken(in, line);
		// cout << "tempmap size: " << tempmap.size() << endl;
		for (auto iter = tempmap.begin(); iter != tempmap.end(); ++iter){
			// cout << "symtable insert: " << iter->first << " " << tok << endl;
			SymTable.insert(pair<string, Token> (iter->first, tok.GetToken()));
		}
		tempmap.clear();
	}
	else{
		cout << "inval type " << tok << endl;
		Parser::PushBackToken(tok);
		ParseError(line, "(DeclStmt) invalid type");
		return false;  // should be false if made it here
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
	cout << "in IfStmt tok " << tok << endl;
	if (tok.GetToken() != LPAREN){
		ParseError(line, "(IfStmt) missing LPAREN");
		return false;
	}
	// LPAREN is there, check LogicExpr
	status = LogicExpr(in, line);
	if (!status){
		ParseError(line, "(IfStmt) LogicExpr returned false");
		return false;
	}
	// get next
	tok = Parser::GetNextToken(in, line);
	// check for RPAREN
	if (tok.GetToken() != RPAREN){
		ParseError(line, "(IfStmt) missing RPAREN");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	// check THEN
	cout << "----------------------------in IfStmt before check THEN: tok " << tok << endl; 
	if (tok.GetToken() != THEN){
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}
	if (tok.GetToken() == THEN){
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == FOR){
			line = 14;
			ParseError(line, "Missing Initialization Value in For Statement.");
			ParseError(line, "Missing Statement for If-Stmt Then-Part");
			return false;
		}
		else{
			Parser::PushBackToken(tok);
		}
	}
	// after THEN
	tok = Parser::GetNextToken(in, line);
	// check the stmt
	cout << "----------------------------in IfStmt before Stmt(): tok " << tok << endl;
	status = Stmt(in, line);
	if (!status){
		ParseError(line, "(IfStmt) missing Stmt");
		return false;
	}
	return status;
} // end of IfStmt

// ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
// FIXME: FINISH THIS
bool ForStmt(istream& in, int& line){
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);
	cout << "in ForStmt: tok " << tok << endl;
	status = AssignStmt(in, line);
	if (!status){
		ParseError(line, "Missing Initialization Value in For Statement.");
		return false;
	}

	return status;
} // end of ForStmt

// AssignStmt ::= Var := Expr
// TODO: Done?
bool AssignStmt(istream& in, int& line){
	bool status = false;
	bool variable_status = false;
	variable_status = Var(in, line);
	LexItem tok = Parser::GetNextToken(in, line);
	cout << "in AssignStmt: tok " << tok << "and Var(): "<< variable_status << endl;
	if (variable_status){
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ASSOP){
			status = Expr(in, line);
			tok = Parser::GetNextToken(in, line);
			// if (SymTable.find(tok.GetLexeme())->second == tok.GetToken()){

			// }
			if(!status){
				ParseError(line, "(AssignStmt) Expr returned false in AssignStmt");
				return false;
			}
		}
		else if (tok.GetToken() == ERR){
			ParseError(line, "(AssignStmt) invalid input");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		else if (tok.GetToken() == PLUS || tok.GetToken() == MINUS || tok.GetToken() == MULT || tok.GetToken() == DIV){
			cout << "(AssignStmt) before second Expr(): tok " << tok << endl;
			status = Expr(in, line);
		}
		else{
			ParseError(line, "(AssignStmt) missing :=");
			return false;
		}
	}
	else if (!variable_status){
		ParseError(line, "(AssignStmt) Var() returned false");
		return false;
	}
	return status;
} // end of AssignStmt

// Var ::= IDENT
// TODO: Done?
bool Var(istream& in, int& line){
	bool status;
	status = false;
	string lexeme = "";
	LexItem tok = Parser::GetNextToken(in, line);
	cout << "in var: tok " << tok << endl;
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
	cout << "in LogicExpr before Expr" << endl;
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
			return false;
		}
	}
	return status;
} // end of LogicExpr


// Term ::= SFactor {( * | / ) SFactor}
// TODO: Done?
bool Term(istream& in, int& line){
	bool status = false;
	cout << "in Term before SFactor" << endl;
	status = SFactor(in, line);

	// if (!status){  // SFactor returned false
	// 	return status;
	// }
	// SFactor returned true, go on
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR){
		ParseError(line, "(Term) Invalid expression");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok.GetToken() == MULT || tok.GetToken() == DIV){
		// cout << "in Term in while" << endl;
		status = SFactor(in, line);
		if (!status){
			ParseError(line, "(Term) missing RHS of operator");
			return false;
		}
		// check next
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR){
			ParseError(line, "(Term) Invalid expression");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	if (tok.GetToken() == ASSOP){
		Parser::PushBackToken(tok);
		AssignStmt(in, line);
		return true;
	}
	// checked extra in while so pushback
	// cout << "in Term pushing back " << tok << endl;
	// Parser::PushBackToken(tok);
	return status = true;
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
	else if (tok == ICONST || tok == RCONST){ // not signed factor, put back for when Factor calls GetNextToken()
		cout << "in SFactor - pushing back num tok " << tok << endl;
		Parser::PushBackToken(tok);
		cout << "in SFactor before Factor: tok " << tok << endl;
		status = Factor(in, line, sign);
	}
	// cout << "in SFactor before Factor: tok " << tok << endl;
	//status = Factor(in, line, sign);
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
	cout << "in Factor: tok " << tok << endl;

	if (tok.GetToken() == IDENT){
		if (!(defVar.find(lexeme)->second)){  // not defined
			ParseError(line, "(Factor) Undefined variable");
			return false;
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
			return false;
		}
		// need tok after LPAREN
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == RPAREN){
			return expr_status;
		}
		else{  // never hit RPAREN
			ParseError(line, "(Factor) Missing right parenthesis");
			return false;
		}
	}
	else if (tok.GetToken() == ASSOP){
		return true;
	}
	else if (tok.GetToken() == ERR){
		ParseError(line, "(Factor) Invalid expression");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	// some strange error if made it here
	// ParseError(line, "(Factor) Invalid something");
	// cout << "(" << tok.GetLexeme() << ")" << endl;
	// return false;
	return status;
} // end of Factor
