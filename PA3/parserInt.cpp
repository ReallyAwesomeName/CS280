/* Implementation of Recursive-Descent Parser + Interpreter
 * parseInt.cpp
 * Programming Assignment 3
 * Spring 2022
*/

#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"

using namespace std;

map<string, bool> defVar;  // map variables as declared or not with boolean
map<string, Token> SymTable;  // map declared variable strings to their token
map<string, Value> TempsResults;  // variable strings and values
vector<string> IdList;  // use for populating SymTable
queue <Value> *ValQue;  // for printing

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

bool IdentList(istream& in, int& line);


// Prog ::= PROGRAM IDENT; DeclBlock ProgBody
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					
					return true;  // Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}  // end Prog()


// ProgBody ::= BEGIN {Stmt;} END
bool ProgBody(istream& in, int& line){
	bool status;
		
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == BEGIN) {
		
		status = Stmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}
			
			status = Stmt(in, line);
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok == END)
		{
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}	
}  // end ProgBody()


// DeclBlock ::= VAR {DeclStmt;}
bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	LexItem t = Parser::GetNextToken(in, line);
	if(t == VAR)
	{
		status = DeclStmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	
}  // end DeclBlock()


// DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{
			// assign declared variable a type --> populate SymTable
			for (int i = 0; i < IdList.size(); i++){
				string current_variable = IdList[i];
				SymTable[current_variable] = t.GetToken();
			}
			// clear IdList
			IdList.clear();
			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}
	
}  // end DeclStmt()


// IdList:= IDENT {,IDENT}
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			// populate defVar
			defVar[identstr] = true;
			// populate IdList --> use to populate SymTable in DeclStmt()
			IdList.push_back(identstr);
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}  // end IdentList()
	

// Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
// Stmt ::= AssigStmt | IfStmt | WriteLnStmt
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;
	
	// not using for loops anymore
	// case FOR:
	// 	status = ForStmt(in, line);
		
	// 	break;
		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}  // end Stmt()


// WriteLnStmt ::= WRITELN (ExprList)
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>;
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
	
	
	// Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		// added check for printing undefined variable
		if (nextVal.IsErr()){
			ParseError(line, "Undefined Variable");
			return false;
		}
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	return ex;
}  // end WriteLnStmt()


// IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
bool IfStmt(istream& in, int& line) {
	bool status; 
	LexItem t;
	Value val;
	Value expval;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	LogicExpr(in, line, val);
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if((t != THEN) && (t != ELSE) && (t != SEMICOL))
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}
	if (val.IsBool()){  // condition check
		if(val.GetBool()){
			// condition true, run statement and skip ELSE
			Stmt(in, line);
			// skip to SEMICOL
			while (t != SEMICOL){
				t = Parser::GetNextToken(in, line);
			}
			Parser::PushBackToken(t);
		}
		else{
			// condition false - skip until else or semicolon
			while ((t != SEMICOL) && (t != ELSE)){
				t = Parser::GetNextToken(in, line);
			}
			Parser::PushBackToken(t);
			if (t == ELSE){
				// run ELSE statement
				Stmt(in, line);
			}
		}
	}

	t = Parser::GetNextToken(in, line);
	if( t == ELSE ) {
		status = Stmt(in, line);
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		return true;
	}
		
	Parser::PushBackToken(t);
	return true;
}  // end IfStmt()


// ============================= NO LONGER USED =============================
// ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
bool ForStmt(istream& in, int& line)
{
	LexItem t;
	bool status;
	
	status = Var(in, line, t);
	
	if(!status)
	{
		ParseError(line, "Missing For Statement Control Variable");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != ASSOP)
	{
		ParseError(line, "For Statement Syntax Error");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != ICONST)
	{
		ParseError(line, "Missing Initialization Value in For Statement.");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == DOWNTO || t == TO)
	{
		t = Parser::GetNextToken(in, line);
		if(t != ICONST)
		{
			ParseError(line, "Missing Termination Value in For Statement.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if(t != DO)
		{
			ParseError(line, "For Statement Syntax Error");
			return false;
		}
		
		status = Stmt(in, line);
	}
	else
	{
		ParseError(line, "For Statement Syntax Error");
		return false;
	}
	
	return status;
}  // end ForStmt()


// Var ::= IDENT
bool Var(istream& in, int& line, LexItem& idtok)
{
	// called only from the AssignStmt function
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		// push back for AssignStmt() to get it again
		Parser::PushBackToken(tok);
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}  // end Var()


// AssignStmt ::= Var := Expr
bool AssignStmt(istream& in, int& line) {
	string temp = "";
	Value val;
	float fv;
	int iv;
	bool varstatus = false, status = false;
	LexItem t;
	
	varstatus = Var(in, line, t);

	t = Parser::GetNextToken(in, line);
	temp = t.GetLexeme();
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		if (t == ASSOP){
			status = Expr(in, line, val);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			// added SymTable and TempsResults stuff here
			// populate TempsResults here --> check for correct type with SymTable
			if ((SymTable[temp] == INTEGER) && (val.GetType() == VREAL)){
				fv = val.GetReal();
				val.SetType(VINT);
				val.SetInt((int)fv);
				TempsResults[temp] = val;
			}
			if ((SymTable[temp] == REAL) && (val.GetType() == VINT)){
				iv = val.GetInt();
				val.SetType(VREAL);
				val.SetReal((float)iv);
				TempsResults[temp] = val;
			}
			if ((SymTable[temp] == INTEGER) && (val.GetType() == VINT)) {
				// no changes needed
				TempsResults[temp] = val;
			}
			if ((SymTable[temp] == REAL) && (val.GetType() == VREAL)) {
				// no changes needed
				TempsResults[temp] = val;
			}
			if ((SymTable[temp] == STRING) && (val.GetType() == VSTRING)) {
				// no changes needed
				TempsResults[temp] = val;
			}
			if ((SymTable[temp] == STRING) && (val.GetType() != VSTRING)){
				ParseError(line, "Illegal Assignment Operation");
				return false;
			}
			if ((SymTable[temp] == REAL) && ((val.GetType() != VREAL) && (val.GetType() != VINT))){
				ParseError(line, "Illegal Assignment Operation");
				return false;
			}
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}  // end AssignStmt()


// ExprList ::= Expr {, Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value val;
	
	status = Expr(in, line, val);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(val);
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
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
}  // end ExprList()


// Expr ::= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	bool t1 = Term(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	// Evaluate: evaluate the expression for addition or subtraction
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = Term(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		if(tok == PLUS)
		{
			retVal = retVal + val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal addition operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if(tok == MINUS)
		{
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal subtraction operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		
	}
	Parser::PushBackToken(tok);
	return true;
}  // end Expr()


// Term ::= SFactor {( * | / ) SFactor}
bool Term(istream& in, int& line, Value& retVal){
	Value val;
	Value val1;
	bool t1 = SFactor(in, line, val);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val;
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = SFactor(in, line, val1);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		// added execution here
		if (retVal.GetType() == VSTRING || val1.GetType() == VSTRING){
			ParseError(line, "(Term) run-time error - math op on string");
			return false;
		}
		if (tok == MULT){
			retVal = retVal * val1;
			if (retVal.IsErr()){
				ParseError(line, "Illegal multiplication operation");
				return false;
			}
		}
		else if (tok == DIV){
			// check for div by 0
			if (val1.IsInt()){
				if (val1.GetInt() == 0){
					ParseError(line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}
			}
			else if (val1.IsReal()){
				if (val1.GetReal() == 0){
					ParseError(line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}
			}
			// no div by 0
			retVal = retVal / val1;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}  // end Term()


// SFactor ::= [(+ | -)] Factor
bool SFactor(istream& in, int& line, Value& retVal) {
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = Factor(in, line, sign, retVal);
	return status;
}  // end SFactor()


// LogicExpr ::= Expr (= | > | <) Expr
bool LogicExpr(istream& in, int& line, Value& retVal) {
	Value val;
	Value val1;
	bool t1 = Expr(in, line, val);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		t1 = Expr(in, line, val1);
		if(!t1)
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		// added run-time exec here
		if (tok == GTHAN){
			retVal = (val > val1);
		}
		else if (tok == EQUAL){
			retVal = (val == val1);
		}
		else if (tok == LTHAN){
			retVal = (val < val1);
		}
		if (retVal.GetType() == VERR){
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands for a Logic Expression");
			return false;
		}

		if (retVal.GetBool()){
			return true;
		}

		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}  // end LogicExpr()


// Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
bool Factor(istream& in, int& line, int sign, Value& retVal) {
	LexItem tok = Parser::GetNextToken(in, line);
	
	if(tok == IDENT){
		string lexeme = tok.GetLexeme();
		// cout << "in Factor-IDENT: Lexeme = " << lexeme << endl;
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "(defVar) Using Undefined Variable");
			return false;	
		}

		// variable is defined
		// added sign handling
		retVal = TempsResults[lexeme];
		if (sign == -1){
			// make negative
			if (retVal.IsInt()){
				retVal.SetInt(-(retVal.GetInt()));
			}
			if (retVal.IsReal()){
				retVal.SetReal(-(retVal.GetReal()));
			}
		}

		return true;
	}
	else if(tok == ICONST) {
		if (sign == -1){
			retVal = Value(-(stoi(tok.GetLexeme())));
		}
		else{
			retVal = Value(stoi(tok.GetLexeme()));
		}
		return true;
	}
	else if(tok == SCONST) {
		if (sign != 0){
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if(tok == RCONST) {
		if (sign == -1){
			retVal = Value(-(stof(tok.GetLexeme())));
		}
		else{
			retVal = Value(stof(tok.GetLexeme()));
		}
		return true;
	}
	else if(tok == LPAREN) {
		bool ex = Expr(in, line, retVal);
		if(!ex) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if(Parser::GetNextToken(in, line) == RPAREN)
			return ex;
		else{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}  // end Factor()

