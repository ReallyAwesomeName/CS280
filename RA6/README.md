# CS 280: RA6

Fall 2021
Recitation Assignment 6
March 3rd, 2022
Due Date: Monday, March 7, 2022, 23:59
Total Points: 5

Write a recursive C++ function, called PrintRevNums, that reads a positive integer number
from the keyboard in each call to the function, until a 0 value is entered (base case). In each call
the function keeps a running total of the numbers being entered and the current maximum value.
After a 0 value is entered, the function outputs the numbers entered in reverse order; such that,
each printed number in reverse order has to be followed by a running total of the numbers in
reverse order too. At the end, the function should print out the maximum value entered. For
example, an input and output dialog might be the following:

Enter any sequence of positive numbers, then enter 0 to end: 45 67 32 12 0
12 Total: 156
32 Total: 144
67 Total: 112
45 Total: 45
The maximum value is: 67

The PrintRevNums has the following header definition:
void PrintRevNums(int sum);

## Vocareum Automatic Grading

• Implement the PrintRevNums() function in a file, called “PrintRevNums.cpp” and
upload it to Vocareum.
• You can implement any driver program for testing your implementation. However, a driver
program is provided on Vocareum for testing the implementation, called “prog.cpp”. The
“prog.cpp” will be propagated to your work directory. The program asks the user to enter
any sequence of positive numbers, then enter 0 to end, then calls the
PrintRevNums()function.
• The testing is based on the correct results returned by the call to your recursive function
PrintRevNums(), and the satisfaction of using recursive approach for the solution of
the problem, rather than an iterative one. Test cases files will be used instead of the input
from the keyboard on Vocareum.
• “prog.cpp” is available with the other assignment material on Canvas.

## Submission Guidelines

• Please upload your implementation of the function to Vocareum as a
“PrintRevNums().cpp” or any other file name you choose. The file should include the
implementation of the function PrintRevNums().
• Submissions after the due date are accepted with a fixed penalty of 25%. No submission
is accepted after Wednesday 11:59 pm, March 9, 2022.