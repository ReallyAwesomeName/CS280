// Command line: %<command> <text> (bold, italic, regular)
// Commented line: #<text>
// Count total lines, commented lines, command lines, number of bold, italic, regular
// First word following % is command name
// If no command or invalid command, print "Error: Unrecognizable command in line #"
// Prompt user for file name to read from
// Open the file for reading
// If cannot be opened, print "File cannot be opened ", followed by filename, then exit
// Print total # of lines, number of commented lines, number of command lines, number
// of each commands

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

fstream infile;
string filename, outstring;
int num_lines, num_comments, num_commands, num_bolds, num_italics, num_regulars;

int main(){
    cout << "Enter the name of a file to read from: " << endl;
    cin >> filename;
    cout << endl;

    infile.open(filename.c_str(), ios::in);
    if (!infile){
        cout << "File cannot be opened ";
        cout << filename << endl;
        exit(1);
    }
    string line;
    while(getline(infile, line)){
        num_lines += 1;

        if (line.size() && line.at(0) == '#'){
            num_comments += 1;
        }

        if (line.size() && line.at(0) == '%'){
            num_commands += 1;
            while (line.at(1) == ' '){
                line.erase(1, 1);
            }

            string command = line.substr(1, line.find(' '));
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            command.erase(remove(command.begin(),command.end(),' '),command.end());
            
            if (command == "bold"){
                num_bolds += 1;
            }

            else if (command == "italic"){
                num_italics += 1;
            }

            else if (command == "regular"){
                num_regulars += 1;
            }

            else{
                cout << "Error: Unrecognizable command in line " << num_lines << endl;
                cout << endl;
            }

        }

    }

    infile.close();

    cout << "Total lines: " << num_lines << endl;
    cout << "Commented lines: " << num_comments << endl;
    cout << "Command lines: " << num_commands << endl;
    cout << "Bold commands: " << num_bolds << endl;
    cout << "Italic commands: " << num_italics << endl;
    cout << "Regular commands: " << num_regulars << endl;

    return 0;
}
