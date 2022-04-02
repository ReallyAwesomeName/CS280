# RA 7 Submission: Infix to Postfix Conversion

Write a C++ function, called infToPostfix(), that accepts a string as a parameter for an infix expression, converts it to postfix notation, and displays it on the output. The infix expression is formed from single lowercase characters as operands (i.e., ‘a’-‘z’), and the binary operators ‘+’ for addition, ‘-‘ for subtractions, ‘*’ for multiplications, ‘/’ for division, ‘(‘, and ‘)’ for the left and right parentheses. The function assumes the string for the infix expression is syntactically correct. The conversion process allows the use of parentheses, and applies the operator precedence rules, assuming all the operators having left-to-right association. Where, the precedence of ‘*’ and ‘/’ are higher than ‘+’ and ‘-‘. For example, an input and output dialog might be as follows:

```text
Please enter an infix notation expression using single lowercase characters:

(a + b) * c/d - e

a b + c * d / e –
```

The infToPostfix()function has the following header definition:

```text
void infToPostfix(string infixstr);
```

## Vocareum Automatic Grading

- A driver program is provided for testing the implementation, called “RA7prog.cpp”, on Vocareum. The “RA7prog.cpp” will be propagated to your Work directory. The program prompts the user to enter an infix expression using single lowercase characters as opernads, then calls the infToPostfix()
- You are provided by a set of 5 testing files associated with Recitation Assignment 7. Vocareum automatic grading will be based on these testing files. You may use them to check and test your implementation. These are available in a compressed archive “RA7 Test Cases.zip” on Canvas assignment. The testing case of each file is defined in the Grading table below.
- “RA7prog.cpp” is available with the other assignment material on Canvas.
