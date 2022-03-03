/* lex.cpp
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include <map>

#include "lex.h"

using namespace std;

// #TODO: MIGHT BE MISSING TOKENS
static map<Token,string> tokenAll = {
    // reserved words
    {PROGRAM, "PROGRAM"},
    {WRITELN, "WRITELN"},
    {INTEGER, "INTEGER"},
    {BEGIN, "BEGIN"},
    {END, "END"},
    {IF, "IF"},
    {REAL, "REAL"},
    {STRING, "STRING"},
    {VAR, "VAR"},
    {ELSE, "ELSE"},
    {FOR, "FOR"},
    {THEN, "THEN"},
    {DO, "DO"},
    {TO, "TO"},
    {DOWNTO, "DOWNTO"},

    {IDENT, "IDENT"},
    {ERR, "ERROR"},
    {DONE, "DONE"},

    // operators
    {PLUS, "+"},
    {MINUS, "-"},
    {MULT, "*"},
    {DIV, "/"},
    {ASSOP, ":="},
    {LPAREN, "("},
    {RPAREN, ")"},
    {EQUAL, "="},
    {GTHAN, ">"},
    {LTHAN, "<"},

    // terminals
    {COMMA, ","},
    {SEMICOL, ";"},
    {COLON, ":"},
    {DOT, "."}
};

static map<string, Token> kwmap{
    {"PROGRAM", PROGRAM},
    {"WRITELN", WRITELN},
    {"INTEGER", INTEGER},
    {"BEGIN", BEGIN},
    {"END", END},
    {"IF", IF},
    {"REAL", REAL},
    {"STRING", STRING},
    {"VAR", VAR},
    {"ELSE", ELSE},
    {"FOR", FOR},
    {"THEN", THEN},
    {"DO", DO},
    {"TO", TO},
    {"DOWNTO", DOWNTO}
};
// check lexeme against kwmap, returns LexItem with appropriate token
LexItem id_or_kw(const string& lexeme, int linenum){
	Token tok = IDENT;

	auto kmIt = kwmap.find(lexeme);
	if (kmIt != kwmap.end()){
		tok = kmIt->second;
    }
	return LexItem(tok, lexeme, linenum);
}

// The operator<< function should print out the string value of 
// the Token in the tok object, followed by its lexeme and 
// line number.
ostream& operator<<(ostream& out, const LexItem& tok) {
	Token t = tok.GetToken();
	out << tokenAll[t]
        << " (" << tok.GetLexeme() << ")"
        << " Line #: " << tok.GetLinenum() << endl;

	return out;
}

LexItem getNextToken(istream& in, int& linenum){
    enum TokState{START, INID, INSTRING, ININT, INREAL, INCOMMENT}
    lexstate = START;
    Token tok = ERR;
    string lexeme;
    char ch;
    bool escaped;
    while (in.get(ch)){
        switch (lexstate){
            case START:
                lexeme = ch;
                if (isspace(ch)){
                    continue;
                }
                if (ch == '\n'){
                    linenum++;
                }
                if (isalpha(ch)){
                    lexstate = INID;
                }
                else if (ch == '\''){ // TODO: make sure this works
                    escaped = false;
                    lexstate = INSTRING;
                }
                else if (isdigit(ch)){
                    lexstate = ININT;
                }
                else if (ch == '.'){
                    auto temp = in.peek();
                    if (isdigit(temp)){
                        lexstate = INREAL;
                    }  // TODO: else what? return ERR?
                }
                else if (ch == '(' && in.peek() == '*'){
                    lexstate = INCOMMENT;
                    continue;
                }
                else{
                    // TODO: ? Token tok = ERR;
                    switch (ch){
                        case '+':
                            tok = PLUS;
                            break;
                        case '-':
                            tok = MINUS;
                            break;
                        case '*':
                            tok = MULT;
                            break;
                        case '/':
                            tok = DIV;
                            break;
                        case ':':
                            tok = COLON;
                            if (in.peek() == '='){
                                tok = ASSOP;
                            }
                            break;
                        case '(':
                            tok = LPAREN;
                            break;
                        case ')':
                            tok = RPAREN;
                            break;
                        case '=':
                            tok = EQUAL;
                            break;
                        case '>':
                            tok = GTHAN;
                            break;
                        case '<':
                            tok = LTHAN;
                            break;
                        case ',':
                            tok = COMMA;
                            break;
                        case ';':
                            tok = SEMICOL;
                            break;
                        case '.':
                            tok = DOT;
                            break;    
                    }
                    return LexItem(tok, lexeme, linenum);
                }

            // TODO: FINISH LEXSTATES
            case INID:
                if (isalpha(ch) || isdigit(ch)){
                    lexeme += ch;
                }
                else{  // done with ID
                    in.putback(ch);  // need to check ch again
                    // check if keyword
                    return id_or_kw(lexeme, linenum);
                }
                break;

            case INSTRING:
                if (ch == '\\'){
                    escaped = true;
                    break;
                }
                if (escaped){
                    // TODO: finished?
                    if (ch == '\''){  // escaped, so still in string 
                        lexeme += ch;
                        escaped = false;
                        break;
                    }
                }
                if (ch == '\''){  // not escaped - done with string
                    // just an end marker - don't putback(ch)
                    return LexItem(STRING, lexeme, linenum);
                }
                lexeme += ch;
                break;

            case ININT:
                if (isdigit(ch)){
                    lexeme += ch;
                }
                else{  // done with integer
                    in.putback(ch);  // need to check ch again
                    return LexItem(INTEGER, lexeme, linenum);
                }
                break;

            case INREAL:
                if (isdigit(ch)){
                    lexeme += ch;
                }
                else if (ch == '.'){  // can't have another . in real
                    return LexItem(ERR, lexeme, linenum);
                }
                else{  // done with real
                    in.putback(ch); // need to check ch again
                    return LexItem(REAL, lexeme, linenum);
                }
                break;

            case INCOMMENT:
                // it's a comment - count lines and wait for end
                if (ch == '\n'){  // can have multiline comment
                    linenum++;
                }
                if (ch == '*' && in.peek() == ')'){  // done with comment
                    lexstate = START;
                }
                break;
        }
    }
    if (in.eof()){
        return LexItem(DONE, "", linenum);
    }

    return LexItem(ERR, "some error", linenum);
}