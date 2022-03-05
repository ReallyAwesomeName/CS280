/* RevNums.cpp
 * RA6
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include <iostream>
#include <string>

using namespace std;

int inum = 0;
int maxval = 0;
string outstring;
string output[5];
int counter = 0;

void reverse_print(string outarray[], int ind){
    if (ind < 0){  // end
        return;
    }
    cout << outarray[ind] << endl;
    reverse_print(outarray, ind-1);
}

void PrintRevNums(int sum){
    cin >> inum;
    outstring = "";
    if (inum == 0){  // end condition
        // start output
        if (maxval > 0){
            reverse_print(output, 4);
            cout << "The maximum value is: " << maxval << endl;
        }
        if (maxval == 0){
            cout << endl;
        }
    }
    else if (inum >= 1){
        sum += inum;  // track total

        outstring += to_string(inum);
        outstring += " Total: ";
        outstring += to_string(sum);

        output[counter] = outstring;
        counter += 1;
        if (inum > maxval){
            maxval = inum;  // track max
        }
        PrintRevNums(sum);
    }
}

// Given:
// 45 67 32 12 0

// 12 Total: 156
// 32 Total: 144
// 67 Total: 112
// 45 Total: 45
// The maximum value is: 67
