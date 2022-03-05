/* main.cpp
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include "lex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stdio.h>


using namespace std;

int main(int argc, char *argv[]){
    if (argc == 1){
    cout << "NO SPECIFIED INPUT FILE NAME." << endl;
    exit(1);
    }
    if (argc >= 2){
        ifstream file(argv[1]);
        if (!file){
            cout << "CANNOT OPEN the File " << argv[1] << endl;
            exit(1);
        }
        if (file.is_open()){
            if (file.peek() == ifstream::traits_type::eof()){
                // empty file
                cout << "Lines: 0" << endl;
                exit(1);
            }
                
            int linenum = 0;
            int num_tokens = 0;
            LexItem tok;
            LexItem kw_tok;

            // vectors for output
            vector<string> iconst_v;
            vector<string> rconst_v;
            vector<string> sconst_v;
            vector<string> ident_v;

            // ~~~~~~~~~~ check CLI arguments ~~~~~~~~~~ 
            bool v_seen = false;
            bool iconst_seen = false;
            bool rconst_seen = false;
            bool sconst_seen = false;
            bool ident_seen = false;

            for (int i = 2; i < argc; i++){  // start after filename
                string curarg = argv[i];  // to check for flag
                if (curarg[0] == '-'){  // it is a flag
                    if (curarg == "-v"){
                        // print all tokens followed by (lexeme)
                        v_seen = true;
                    }
                    else if (curarg == "-iconst"){
                        // print all unique integer constants in numeric order
                        iconst_seen = true;
                    }
                    else if (curarg == "-rconst"){
                        // print all unique real constants in numeric order
                        rconst_seen = true;
                    }
                    else if (curarg == "-sconst"){
                        // print all unique string constants in alpha order
                        sconst_seen = true;
                    }
                    else if (curarg == "-ident"){
                        // print all unique identifiers in alpha order
                        ident_seen = true;
                    }
                    else{  // has '-', but not a valid flag
                        cout << "UNRECOGNIZED FLAG " << curarg << endl;
                        exit(1);
                    }
                }
                else{  // not a flag, no more filenames allowed
                    cout << "ONLY ONE FILE NAME ALLOWED." << endl;
                    exit(1);
                }
            }

            // ~~~~~~~~~~ print stuff out ~~~~~~~~~~ 
            while (((tok = getNextToken(file, linenum)) != Token::ERR)
                    && (tok != Token::DONE)){
                if (v_seen){
                    // if -v, print each token as it is read and recognized
                    cout << tok << endl;
                }
                if (tok.GetToken() == ICONST){
                    iconst_v.push_back(tok.GetLexeme());
                }
                if (tok == Token::RCONST){
                    if (tok.GetLexeme() == ".0" || tok.GetLexeme() == "0.0"){
                        rconst_v.push_back("0");
                    }
                    else if (tok.GetLexeme()[0] == '.'){  // add 0 if not present
                        string temp = "0";
                        temp.append(tok.GetLexeme());
                        rconst_v.push_back(temp);
                    }
                    else{
                        rconst_v.push_back(tok.GetLexeme());
                    }
                }
                if (tok == Token::SCONST){
                    // add quotes back for output list
                    string temp = "'";
                    temp.append(tok.GetLexeme());
                    temp.append("'");
                    sconst_v.push_back(temp);
                }
                if (tok == Token::IDENT){
                    ident_v.push_back(tok.GetLexeme());
                }
                ++num_tokens;  // count all tokens
            }  // end while

            if (tok == ERR){  // if error print message and exit
                if (tok.GetLexeme() == "end comment err"){ // special case comment err
                    cout << endl << "Missing a comment end delimiters '*)' at line "
                    << tok.GetLinenum() + 1 << endl;
                    tok = LexItem(DONE, "end comment err", linenum + 1);
                }
                else{
                    cout << tok << endl;
                    exit(1);
                }
            }
            if (tok == DONE){
                file.close();
                // summary
                cout << "Lines: " << tok.GetLinenum() << endl;
                cout << "Tokens: " << num_tokens << endl;
                // handle flags in order: -iconst, -rconst, -sconst, -idents
                // NOTE: above order is incorrect according to vocareum
                // use order: -sconst, -iconst, -rconst, -idents
                if (sconst_seen){
                    if (sconst_v.size() > 0){
                        // sort and remove duplicates
                        sort (sconst_v.begin(), sconst_v.end());
                        unique(sconst_v.begin(), sconst_v.end());

                        cout << "STRINGS:" << endl;
                        for (string i : sconst_v){
                            cout << i << endl;
                        }
                    }
                }
                if (iconst_seen){
                    if (iconst_v.size() > 0){
                        // sort and remove duplicates
                        sort(iconst_v.begin(), iconst_v.end());
                        unique(iconst_v.begin(), iconst_v.end());
                        iconst_v.pop_back();  // FIXME: shitty workaround #3

                        cout << "INTEGERS:" << endl;
                        for (string i : iconst_v){
                            cout << i << endl;
                        }
                    }
                }
                if (rconst_seen){
                    if (rconst_v.size() > 0){
                        // sort and remove duplicates
                        sort(rconst_v.begin(), rconst_v.end());
                        unique(rconst_v.begin(), rconst_v.end());

                        cout << "REALS:" << endl;
                        for (string i : rconst_v){
                            cout << i << endl;
                        }
                    }
                }
                if (ident_seen){
                    if (ident_v.size() > 0){
                        if (ident_v.size() == 1){  // FIXME: shitty workaround #4
                            // only for when printing a single ident
                            cout << "IDENTIFIERS:" << endl;
                            cout << ident_v[0] << endl;
                        }
                        else{
                            sort (ident_v.begin(), ident_v.end());
                            unique(ident_v.begin(), ident_v.end());

                            cout << "IDENTIFIERS:" << endl;
                            // FIXME: size() - 2? whitespace entry?
                            for (int i = 0; i < ident_v.size() - 2; i++){
                                cout << ident_v[i] << ", ";
                            }
                            cout << ident_v[ident_v.size() - 2] << endl;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
