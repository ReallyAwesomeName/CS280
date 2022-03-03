/* main.cpp
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include "lex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
//#include <set>


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
            vector<int> iconst_v;  // TODO: maybe just use sets?
            vector<float> rconst_v;
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
            // TODO: maybe use lists?
            while (((tok = getNextToken(file, linenum)) != Token::ERR)
                    && (tok != Token::DONE)){
                if (v_seen){
                    // FIXME: print all tokens followed by (lexeme)
                    // if (tok == Token::IDENT){
                    //     cout << tok.GetToken() << '(' << tok.GetLexeme() 
                    //     << ')' << endl;
                    // }
                    // else{
                    //     cout << tok.GetToken();
                    // }
                    cout << tok << endl;
                }
                if (tok.GetToken() == ICONST){
                    // put in a list?
                    iconst_v.push_back(tok.GetToken());
                }
                if (tok == Token::RCONST){
                    rconst_v.push_back(tok.GetToken());
                }
                if (tok == Token::SCONST){
                    sconst_v.push_back(tok.GetLexeme());
                }
                if (tok == Token::IDENT){
                    ident_v.push_back(tok.GetLexeme());
                }
                ++num_tokens;  // count all tokens
            }
            if (tok == ERR){  // if error print message and exit
                // cout << "Error in line " << tok.GetLinenum() <<
                // " (" << tok.GetLexeme() << ")" << endl;
                cout << tok << endl;
                exit(1);
            }
            if (tok == DONE){
                file.close();
                // summary
                cout << "Lines: " << tok.GetLinenum() << endl;
                cout << "Tokens: " << num_tokens << endl;
                // handle flags in order: -iconst, -rconst, -sconst, -idents
                if (iconst_seen){
                    if (iconst_v.size() > 0){
                        // vector to set to vector to remove duplicates
                        // TODO: maybe just use sets to begin with?
                        //set<int> tempset;
                        //copy(iconst_v.begin(), iconst_v.end(), tempset);
                        //iconst_v.clear();
                        //iconst_v.assign(tempset.begin(), tempset.end());
                        // sort before output

                        unique(iconst_v.begin(), iconst_v.end());
                        sort(iconst_v.begin(), iconst_v.end());

                        cout << "INTEGERS:" << endl;
                        for (int i : iconst_v){  // FIXME: i is fine right?
                            cout << i << endl;
                        }
                    }
                }
                if (rconst_seen){
                    if (rconst_v.size() > 0){
                        //set<float> tempset;
                        //copy(rconst_v.begin(), rconst_v.end(), tempset);
                        //rconst_v.clear();
                        //rconst_v.assign(tempset.begin(), tempset.end());

                        unique(rconst_v.begin(), rconst_v.end());
                        sort(rconst_v.begin(), rconst_v.end());

                        cout << "REALS:" << endl;
                        for (float i : rconst_v){
                            cout << i << endl;
                        }
                    }
                }
                if (sconst_seen){
                    if (sconst_v.size() > 0){
                        //set<string> tempset;
                        //copy(sconst_v.begin(), sconst_v.end(), tempset);
                        //sconst_v.clear();
                        //sconst_v.assign(tempset.begin(), tempset.end());

                        unique(sconst_v.begin(), sconst_v.end());
                        sort (sconst_v.begin(), sconst_v.end());

                        cout << "STRINGS:" << endl;
                        for (string i : sconst_v){
                            cout << i << endl;
                        }
                    }
                }
                if (ident_seen){
                    if (ident_v.size() > 0){
                        //set<string> tempset;
                        //copy(ident_v.begin(), ident_v.end(), tempset);
                        //ident_v.clear();
                        //ident_v.assign(tempset.begin(), tempset.end());

                        unique(ident_v.begin(), ident_v.end());
                        sort (ident_v.begin(), ident_v.end());

                        cout << "IDENTIFIERS:" << endl;
                        // FIXME: print comma separated list of every identifier 
                        // found in alpha order
                        for (string i : ident_v){
                            cout << i << ",";
                        }
                    }
                }
            }


        }
    }
    return 0;
}