//RA3: Counting words with names and integers

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

istream *in;
ifstream file;
int num_lines = 0;
int non_blanks = 0;
int num_words = 0;
int num_names = 0;
int num_ints = 0;

int main(int argc, char *argv []){
    if (argc == 1){
        cout << "No file name is found" << endl;
        exit(1);
    }
    if (argc >= 2){
        file.open(argv[1], ios::in);
        if (!file){
            cout << "File cannot be opened: " << argv[1] << endl;
            exit(1);
        }
        if (file.is_open()){
            string line;
            string word;
            in = &file;
            while(getline(file, line)){
                num_lines++;
                if (line.size()){
                    non_blanks++;
                    stringstream ss(line);
                    while (ss >> word){
                        num_words++;
                        //check if valid word etc
                        if (isalpha(word.at(0)) && (word.find('-') == string::npos)){
                            num_names++;
                            }
                        if (all_of(word.begin(), word.end(), ::isdigit)){
                            num_ints++;
                        }
                    }
                }
            }
        }
        //output
        cout << "Total Number of Lines: " << num_lines << endl;
        cout << "Number of non-blank lines: " << non_blanks << endl;
        cout << "Number of Words: " << num_words << endl;
        cout << "Number of Integers: " << num_ints << endl;
        cout << "Number of Names: " << num_names << endl;
    }
}

bool isname(string astring){
    bool is_a_name = true;
    if (isalpha(astring.at(0) && all_of(astring.begin(), astring.end(), ::isalnum))){

    }
    return is_a_name;
}
