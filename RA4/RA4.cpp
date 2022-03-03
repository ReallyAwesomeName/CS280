//RA4: Directory of Punctuation Characters

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>

using namespace std;

int non_whitespace_chars = 0;
int max_occ = 0;
char max_punc;
map<char,int> counters;

int main(int argc, char *argv []){
    if (argc == 1){
        cout << "No file is found." << endl;
        exit(1);
    }
    if (argc >= 2){
        ifstream file(argv[1]);
        //file.open(argv[1], ios::in);
        if (!file){
            cout << "File cannot be opened: " << argv[1] << endl;
            exit(1);
        }
        if (file.is_open()){
            auto ss = ostringstream();
            ss << file.rdbuf();
            string fcontent = ss.str();
            // fcontent.assign((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
            if (all_of(fcontent.begin(), fcontent.end(), ::isspace) || !fcontent.size()){
                cout << "File is empty." << endl;
                exit(1);
            }
            for (char i : fcontent){
                if (!isspace(i)){
                    non_whitespace_chars++;
                    if (ispunct(i)){
                        counters[i]++;
                    }
                }
            }
            cout << "Number of characters in the file: " << non_whitespace_chars << endl;
            cout << "Punctuation characters in the file and their occurrences:" << endl;
            cout << endl;
            for (auto i : counters){
                cout << i.first << ": " << i.second << endl;
                if (i.second > max_occ){
                    max_occ = i.second;
                    max_punc = i.first;
                }
            }
            cout << endl;
            cout << "The punctuation character with maximum occurrences is \""
            << max_punc << "\" found " << max_occ << " times." << endl;

        }
    }
}