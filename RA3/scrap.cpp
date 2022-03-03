#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

istream *in;
ifstream fin;
string line;

int main(){

    fin.open("test.txt");
    char word[30];
    int count=0;
    while(getline(fin, line))
    {
        stringstream ss(line);
        while(ss >> word){
            count++;
            cout << word << " ";
        }
        
    }
    cout<<"Number of words in file are "<<count;
    fin.close();

}