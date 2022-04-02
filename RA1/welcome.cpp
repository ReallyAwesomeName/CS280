#include <iostream>
#include <string>
using namespace std;

string uname;

int main() {
    cout << "Welcome to CS 280.\n";
    cout << "What is your first and last names?\n";
    getline (cin, uname);
    cout << "Hello, " << uname;
    cout << "\n";
}