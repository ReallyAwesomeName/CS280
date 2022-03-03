/* RA5.cpp
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include <map>
#include "lex.h"

using namespace std;

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
    {ICONST, "ICONST"},
    {RCONST, "RCONST"},
    {SCONST, "SCONST"},

    // operators
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {ASSOP, "ASSOP"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {EQUAL, "EQUAL"},
    {GTHAN, "GTHAN"},
    {LTHAN, "LTHAN"},

    // terminals
    {COMMA, "COMMA"},
    {SEMICOL, "SEMICOL"},
    {COLON, "COLON"},
    // {DOT, "."},

    // other
    {IDENT, "IDENT"},
    {ERR, "ERR"},
    {DONE, "DONE"}
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