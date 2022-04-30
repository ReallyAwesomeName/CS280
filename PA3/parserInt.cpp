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

map<string, bool> defVar;
map<string, Token> SymTable;  // map declared variable strings to their token
map<string, Value> TempsResults;  // variable strings and values
vector<string> IdList;  // use for populating SymTable
queue <Value> *ValQue;  // for printing
// queue <string> VarQue;  // got rid of this, use IdList instead

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


//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
//Prog ::= PROGRAM IDENT; DeclBlock ProgBody
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
					
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
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


//ProgBody ::= BEGIN {Stmt;} END
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
		if(tok == END )
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
}//End of ProgBody()


//DeclBlock ::= VAR {DeclStmt;}
bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
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
	
}//end of DeclBlock()


//DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
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
	// if (defVar.find(t.GetLexeme())->second){
	// 	VarQue.push(t.GetLexeme());
	// }
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{
			// assign declared variable a type --> populate SymTable
			for (int i = 0; i < IdList.size(); i++){
				string current_variable = IdList[i];
				// cout << "populating SymTable --> " << current_variable << ", " << t.GetToken() << endl;
				SymTable[current_variable] = t.GetToken();
			}
			// clear IdList
			IdList.clear();

			// while (!VarQue.empty()){
			// 	cout << "assigning type: " << VarQue.front() << ", " << t.GetToken() << endl;
			// 	SymTable[VarQue.front()] = t.GetToken();
			// 	VarQue.pop();
			// }
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
	
}//End of DeclStmt()


//IdList:= IDENT {,IDENT}
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

			// SymTable populating was moved to DeclStmt
			// if (identstr[0] == 'S'){
			// 	cout << "***adding to SymTable[identstr]: identstr: " << identstr << " TYPE: " << STRING << endl;
			// 	SymTable[identstr] = STRING;  // record type
			// }
			// else{
			// 	cout << "adding to SymTable[identstr]: identstr: " << identstr << " tok.GetToken(): " << tok.GetToken() << endl;
			// 	SymTable[identstr] = tok.GetToken();  // record type
				
			// }
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
	
}//End of IdentList()
	

//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt ::= AssigStmt | IfStmt | WriteLnStmt
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
		// cout << "in Stmt(), pushing back token " << t << endl;
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
}//End of Stmt()


//WriteLnStmt ::= WRITELN (ExprList)
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
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
	
	
	//Evaluate: print out the list of expressions' values
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
}//End of WriteLnStmt()


//IfStmt:= if (Expr) then Stm} [Else Stmt]
//TODO: IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
bool IfStmt(istream& in, int& line) {
	// bool ex=false;
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
	// ex = LogicExpr(in, line, val);
	// if(!ex){
	// 	ParseError(line, "Missing if statement Logic Expression");
	// 	return false;
	// }
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	// cout << "******** t: " << t << endl;
	if((t != THEN) && (t != ELSE) && (t != SEMICOL))
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}
	// status = Stmt(in, line);
	// if(!status)
	// {
	// 	ParseError(line, "Missing Statement for If-Stmt Then-Part");
	// 	return false;
	// }
	if (val.IsBool()){  // condition check
		// cout << "val.GetBool(): " << val.GetBool() << endl;
		if(val.GetBool()){
			// cout << "val.GetBool() was true: " << val.GetBool() << endl;
			Stmt(in, line);
			// bool s = Stmt(in, line);
			// if (!s){
			// 	ParseError(line, "(IfStmt) missing stmt in if");
			// 	return false;
			// }
			// skip to SEMICOL
			while (t != SEMICOL){
				t = Parser::GetNextToken(in, line);
			}
			Parser::PushBackToken(t);
		}
		else{
			// cout << "val.GetBool() was false: " << val.GetBool() << endl;
			// condition false - skip until else or semicolon
			while ((t != SEMICOL) && (t != ELSE)){
				t = Parser::GetNextToken(in, line);
			}
			Parser::PushBackToken(t);
			if (t == ELSE){
				// t = Parser::GetNextToken(in, line);
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
		//cout << "in IFStmt status of Stmt true" << endl;
		return true;
	}
		
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt()


// ============================= NO LONGER USED =============================
//ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
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
}//End of ForStmt()


//Var ::= IDENT
// TODO: ADDED (LexItem& idtok) parameter
bool Var(istream& in, int& line, LexItem& idtok)
{
	//called only from the AssignStmt function
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		// push back for assignstmt to get it again?
		Parser::PushBackToken(tok);
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var()


//AssignStmt ::= Var := Expr
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
	// cout << "temp: " << temp << endl;
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		if (t == ASSOP){
			// cout << "passing to Expr: " << val << endl;
			status = Expr(in, line, val);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			// added SymTable and TempsResults stuff here
			// TODO: populate TempsResults here --> check for correct type with SymTable
			// cout << "about to check SymTable[temp]: " << temp << " val.GetType(): " << val.GetType() << endl;
			if ((SymTable[temp] == INTEGER) && (val.GetType() == VREAL)){
				fv = val.GetReal();
				val.SetType(VINT);
				val.SetInt((int)fv);
				// cout << "adding temp, val: " << temp << "," << val << endl;
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
			
			// TODO: don't need this block?
			// else if (!(val.GetType() == VREAL || val.GetType() == VINT)){
			// 	ParseError(line, "(assignstmt) bad assign operation");
			// 	return false;
			// }
			if ((SymTable[temp] == STRING) && (val.GetType() == VSTRING)){
				// cout << "******adding to TempsResults: " << temp << ", " << val << endl;
				TempsResults[temp] = val.GetString();
			}
			// else{ // no change needed
			// 	cout << "******adding to TempsResults: " << temp << ", " << val << endl;
			// 	TempsResults[temp] = val;
			// }
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


//ExprList ::= Expr {, Expr}
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


//Expr ::= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	//cout << "in Expr" << endl;
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
	//Evaluate: evaluate the expression for addition or subtraction
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
}//end of Expr()


//Term ::= SFactor {( * | / ) SFactor}
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
				//cout << "(" << tok.GetLexeme() << ")" << endl;
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


//SFactor ::= [(+ | -)] Factor
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
	else  // FIXME: is this a prob?
		Parser::PushBackToken(t);
		
	status = Factor(in, line, sign, retVal);
	return status;
}  // end SFactor()


//LogicExpr ::= Expr (= | > | <) Expr
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
		// FIXME: THIS SHOULD BE DONE IN IfStmt() or LogicExpr()?
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
		// else if (!retVal.GetBool()){
		// 	cout << "new false" << endl;
		// 	return false;
		// }

		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}  // end LogicExpr()


//Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
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
		// added check for signed string
		// if ((sign != 0) && (retVal.IsString())){
		// 	ParseError(line, "Illegal Operand Type for Sign Operator");
		// 	return false;
		// }
		// added TempResults check for definition
		// if (TempsResults.find(lexeme) == TempsResults.end()){
		// 	ParseError(line, "(TempsResults) Using Undefined Variable");
		// 	cout << "lexeme: " << lexeme << endl;
		// 	return false;
		// }

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
			// FIXME: added redundant check for signed string
			// if (retVal.IsString()){
			// 	ParseError(line, "Illegal Operand Type for Sign Operator");
			// 	return false;
			// }
		}
		// if (sign == 1){
		// 	// FIXME: added another redundant check for signed string
		// 	if (retVal.IsString()){
		// 		ParseError(line, "Illegal Operand Type for Sign Operator");
		// 		return false;
		// 	}
		// }

		return true;
	}

	// TODO: Make sure this working
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
		// cout << "in Factor-SCONST: tok = " << tok << endl;
		if (sign != 0){
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if(tok == RCONST) {
		if (sign == -1){
			retVal = Value(-(stoi(tok.GetLexeme())));
		}
		else{
			retVal = Value(stoi(tok.GetLexeme()));
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

