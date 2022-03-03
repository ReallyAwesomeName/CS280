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

    {SEMICOL, "SEMICOL"},
    {COLON, "COLON"},
    {DOT, "DOT"},

    // terminals
    {ERR, "ERROR"},
    {DONE, "DONE"},
    {COMMA, "COMMA"},

    // // operators
    // {PLUS, "+"},
    // {MINUS, "-"},
    // {MULT, "*"},
    // {DIV, "/"},
    // {ASSOP, ":="},
    // {LPAREN, "("},
    // {RPAREN, ")"},
    // {EQUAL, "="},
    // {GTHAN, ">"},
    // {LTHAN, "<"},

    // // terminals
    // {COMMA, ","},
    // {SEMICOL, ";"},
    // {COLON, ":"},
    // {DOT, "."}


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

// lexemes to upper for id_or_kw
string makeUpper(string &x){
    for (int i = 0; i < x.length(); i++){
        x[i] = toupper(x[i]);
    }
    return x;
}

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
    if (t == ERR){
        out << "Error in line " << tok.GetLinenum() + 1 << " (" 
        << tok.GetLexeme() << ")";
    }
    else if (t == ICONST || t == RCONST || t == SCONST || t == IDENT){
        out << tokenAll[t] << "(" << tok.GetLexeme() << ")";
    }
    else{
        out<<tokenAll[t];
    }
	// out << tokenAll[t]
    //     << "(" << tok.GetLexeme() << ")"
    //     << " Line #: " << tok.GetLinenum() << endl;

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
            case START:  // new lexeme
                if (ch == '\n'){  // doesn't work below space??
                    linenum++;
                }
                if (isspace(ch)){
                    continue;
                }
                lexeme = ch;  // starting new lexeme
                if (isalpha(ch) || ch == '_'){
                    lexstate = INID;
                }
                else if (ch == '\''){
                    escaped = false;
                    lexstate = INSTRING;
                    continue;
                }
                else if (isdigit(ch)){
                    lexstate = ININT;
                }
                else if (ch == '.'){
                    auto temp = in.peek();
                    if (isdigit(temp)){
                        lexstate = INREAL;
                        continue;
                    }
                    else{
                        lexeme += temp;
                        return LexItem(ERR, lexeme, linenum);
                    }
                }
                // else if (ch == '(' && in.peek() == '*'){
                //     lexstate = INCOMMENT;
                //     continue;
                // }
                else{
                    tok = ERR;
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
                                in.get(ch);
                                lexeme += ch;
                                tok = ASSOP;
                            }
                            break;
                        case '(':
                            tok = LPAREN;
                            if (in.peek() == '*'){
                                lexstate = INCOMMENT;
                                continue;
                            }
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
                    }  // switch(ch) end
                    return LexItem(tok, lexeme, linenum);
                }
                break;

            // identified lexstate cases
            case INID:
                if (isalpha(ch) || isdigit(ch) || ch == '_'){
                    lexeme += ch;
                }
                else{  // done with ID
                    lexstate = START;
                    in.putback(ch);  // need to check ch again
                    // check if keyword - needs upper
                    string temp = makeUpper(lexeme);
                    return id_or_kw(temp, linenum);
                }
                break;

            case INSTRING:
                if (ch == '\\'){
                    escaped = true;
                    continue;
                }
                if (escaped){
                    // TODO: finished?
                    if (ch == '\''){  // escaped, so still in string 
                        lexeme += ch;
                        escaped = false;
                        break;
                    }
                    // if (ch == 'n'){
                    //     ch = '\n';  // now start case linenum++ should work?
                    //     escaped = false;
                    //     break;
                    // }
                }
                if (ch == '\"'){  // FIXME: shitty workaround
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                }
                if (ch == '\n'){  // FIXME: shitty workaround #2
                    return LexItem(ERR, lexeme, linenum);
                }
                lexeme += ch;  // not escaped so just add to string lexeme
                if (ch == '\''){  // done with string
                    // just an end marker - don't putback(ch)
                    if (lexeme.size() >= 2){  // remove quotes
                        lexeme = lexeme.substr(1, lexeme.size() - 2);
                    }
                    return LexItem(SCONST, lexeme, linenum);
                }
                break;

            case ININT:
                if (isdigit(ch)){
                    lexeme += ch;
                }
                else if (ch == '.'){
                    lexeme += ch;
                    lexstate = INREAL;
                }
                // else if (isalpha(ch)){
                //     in.putback(ch);
                //     lexstate = INID;
                //     return LexItem(ICONST, lexeme, linenum);
                // }
                else{  // done with integer
                    lexstate = START;
                    in.putback(ch);  // need to check ch again
                    return LexItem(ICONST, lexeme, linenum);
                }
                break;

            case INREAL:
                if (isdigit(ch)){
                    lexeme += ch;
                }
                else if (ch == '.'){  // can't have another . in real
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                }
                else{  // done with real
                    lexstate = START;
                    in.putback(ch); // need to check ch again
                    return LexItem(RCONST, lexeme, linenum);
                }
                break;

            case INCOMMENT:
                // it's a comment - count lines and wait for end
                if (in.peek() == -1){  // eof before close comment
                    return LexItem(ERR, "end comment err", linenum);
                }
                if (ch == '\n'){  // can have multiline comment
                    linenum++;
                }
                if (ch == '*' && in.peek() == ')'){  // done with comment
                    in.get(ch);
                    lexstate = START;
                }
                continue;
        }  // switch(lexstate) end
    }  // loop end
    if (in.eof()){
        return LexItem(DONE, "", linenum);
    }

    return LexItem(ERR, "some error", linenum);
}