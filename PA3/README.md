# CS 280: PA3 Implementation of a PLSL Interpreter

Spring 2022\
Programming Assignment 3\
April 14, 2022\
Due Date: Sunday, May 1st, 2022, 23:59\
Total Points: 20

In this programming assignment, you will be building an interpreter for our PLSL programming
language. The grammar rules of the language and its tokens were given in Programming
Assignments 1 and 2. However, one rule has been eliminated from the previous specifications, it
is the one for the For Statement. You are required to modify the parser you have implemented for
the PLSL language to implement an interpreter for it. The modified specifications of the grammar
rules are described in EBNF notations.

1. Prog ::= PROGRAM IDENT; DeclBlock ProgBody
2. DeclBlock ::= VAR {DeclStmt;}
3. DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
4. ProgBody ::= BEGIN {Stmt;} END
5. Stmt ::= AssigStmt | IfStmt | WriteLnStmt
6. WriteLnStmt ::= WRITELN (ExprList)
7. IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
8. AssignStmt ::= Var := Expr
9. ExprList ::= Expr {, Expr}
10. Expr ::= Term {(+|-) Term}
11. Term ::= SFactor {( * | / ) SFactor}
12. SFactor ::= [(+ | -)] Factor
13. LogicExpr ::= Expr (= | > | <) Expr
14. Var ::= IDENT
15. Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)

The following points describe the programming language. You have already implemented all of
the syntactic rules of the language as part of the parser implementation. The points related to the
dynamic semantics (i.e. run-time checks) of the language must be implemented in your interpreter.
These include points 5-12 in the following list. These points are:

1. The language has three types: INTEGER, REAL and STRING.
2. The precedence rules of the PSLP language are as shown in the table
3. The PLUS, MINUS, MULT, and DIV operators are left associative.
4. All variables have to be declared in declaration statements in the declaration block.
5. An IfStmt evaluates a logical expression (LogicExpr) as a condition. If the logical expression
value is true, then the Stmt block is executed, otherwise it is not. An else part for an IfSmt is
optional.
6. A WriteLnStmt evaluates the list of expressions (ExprList), and prints their values in order
from left to right followed by a newline.
7. The ASSOP operator (:=) in the AssignStmt assigns a value to a variable. It evaluates the Expr
on the right-hand side and saves its value in a memory location associated with the left-hand
side variable (Var). A left-hand side variable of a numeric type can be assigned a value of
either one of the numeric types (i.e., INTEGER, REAL) of the language. For example, an
integer variable can be assigned a real value, and a real variable can be assigned an integer
value. In either case, conversion of the value to the type of the variable must be applied.
However, A left-hand side STRING variable can only be assigned a STRING value.
8. The binary arithmetic operations for addition (PLUS), subtraction (MINUS), multiplication
(MULT), and division (DIV) are performed upon two numeric operands (i.e., INTEGER,
REAL) of the same or different types. If the operands are of the same type, the type of the
result is the same type as the operator’s operands. Otherwise, the type of the result is REAL.
9. When the PLUS operator is applied on STRING operands, the operator performs string
concatenation.
10. The EQUAL LTHAN and GTHAN relational operators operate upon two compatible
operands. The evaluation of a logical expression, based on EQUAL, LTHAN or GTHAN
operation, produces either a true or false value that controls whether the statement(s) of the
selection IfStmt is executed or not.
11. It is an error to use a variable in an expression before it has been assigned.
12. The unary sign operators (+ or -) are applied upon unary numeric operands (i.e., INTEGER,
REAL).

|Precedence |         |                                           |
|-----------|---------|-------------------------------------------|
|           | Highest | () parentheses                            |
|           |         | Unary +, unary -                          |
|           |         | *, / (Left Associative)                   |
|           |         | + (add), - (subtract) (Left Associative)  |
|           | Lowest  | =, <, > (relational operators)            |

## Interpreter Requirements

Implement an interpreter for the PLSL language based on the recursive-descent parser
developed in Programming Assignment 2. You need to modify the parser functions to include
the required actions of the interpreter for evaluating expressions, determining the type of
expression values, executing the statements, and checking run-time errors. You may use the
lexical analyzer you wrote for Programming Assignment 1 and the parser you wrote for
Programming Assignment 2. Otherwise you may use the provided implementations for the
lexical analyzer and parser when they are posted. Rename the parse.cpp file as parserInt.cpp
to reflect the applied changes on the current parser implementation for building an interpreter.
The interpreter should provide the following:

- It performs syntax analysis of the input source code statement by statement, then executes
the statement if there is no syntactic or semantic error.
- It builds information of variables types in the symbol table for all the defined variables.
- It evaluates expressions and determines their values and types. You need to implement the
overloaded operator functions for the Value class.
- The results of an unsuccessful parsing and interpreting are a set of error messages printed
by the parser/interpreter functions, as well as the error messages that might be detected by
the lexical analyzer.
- Any failures due to the process of parsing or interpreting the input program should cause
the process of interpretation to stop and return back.
- In addition to the error messages generated due to parsing, the interpreter generates error
messages due to its semantics checking. The assignment does not specify the exact error
messages that should be printed out by the interpreter. However, the format of the messages
should be the line number, followed by a colon and a space, followed by some descriptive
text, similar to the format used in Programming Assignment 2. Suggested messages of the
interpreter’s semantics check might include messages such as "Run-Time Error-Illegal
Mixed Type Operands", " Run-Time Error-Illegal Assignment Operation", "Run-Time
Error-Illegal Division by Zero", etc.

## Provided Files

You are given the following files for the process of building an interpretation. These are “lex.h’,
“lex.cpp”, “val.h”, “parseInt.h”, and “parseInt.cpp” with definitions and partial implementations
of some functions. You need to complete the implementation of the interpreter in the provided
copy of “parseInt.cpp. “parse.cpp” will be posted later on.

### “val.h” includes the following

- A class definition, called Value, representing a value object in the interpreted source code
for constants, variables or evaluated expressions. It includes:

  - Four data members of the Value class for holding a value as either an integer, float,
string, or boolean.
  - A data member holding the type of the value as an enum type defined as:
enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };
  - Getter methods to return the value of an object based on its type
  - Getter methods to return the type of the value of an object.
  - Setter methods to update the value of an object.
  - Overloaded constructors to initialize an object based on the type of their parameters.
  - Member methods to check the type of the Value object.
  - Overloaded operators’ functions for the arithmetic operators (+, -, *, and /)
  - Overloaded operators’ functions for the relational operators (==, >, and <)
  - A friend function for overloading the operator<< to display value of an object based on
its type.

- You are required to provide the implementation of the Value class in a separate file, called
“val.cpp”, which includes the implementations of the overloaded operator functions
specified in the Value class definition (operator+, operator-, operator*, operator/,
operator==, operator>, operator<).

### “parserInt.h”

- includes the prototype definitions of the parser functions as in “parse.h” header file with the following applied modifications:

  - extern bool Var(istream& in, int& line, LexItem & tok);
  - extern bool Expr(istream& in, int& line, Value & retVal);
  - extern bool LogicExpr(istream& in, int& line,Value & retVal);
  - extern bool Term(istream& in, int& line, Value & retVal);
  - extern bool SFactor(istream& in, int& line, Value & retVal);
  - extern bool Factor(istream& in, int& line, Value & retVal);

### "parse.cpp”

- Implementations of parser functions in “parse.cpp” from Programming Assignment 2.
  - It will be provided after the deadline of PA 2 submission (including any
extensions).

### “prog3.cpp”

- You are given the testing program “prog3.cpp” that reads a file name from the command
line. The file is opened for syntax analysis and interpretation, as a source code of the
language.
- A call to Prog() function is made. If the call fails, the program should stop and display a
message as "Unsuccessful Interpretation ", and display the number of errors detected. For
example:

    ```text
    Unsuccessful Interpretation
    Number of Syntax Errors: 3
    ```

- If the call to Prog() function succeeds, the program should stop and display the message
"Successful Execution", and the program stops.

## Vocareum Automatic Grading

- You are provided by a set of 14 testing files associated with Programming Assignment 3.
Vocareum automatic grading will be based on these testing files. You may use them to
check and test your implementation. These are available in compressed archive “PA3 Test
Cases.zip” on Canvas assignment. The testing case of each file is defined in the Grading
table below.
- Automatic grading of testing files with no errors will be based on checking against the
generated outputs by the executed PLSL source program and the output message:

    ```text
    Successful Execution
    ```

- In each of the other testing files, there is one semantic error at a specific line. The automatic
grading process will be based on the statement number at which this error has been found
and associated with one or more error messages.
- You can use whatever error message you like. There is no check against the contents of the
error messages.
- A check of the number of errors your parser has produced and the number of errors printed
out by the program is also made.

## Submission Guidelines

- Submit your “parseInt.cpp” implementation through Vocareum. The “lex.h”, “parseInt.h”,
“lex.cpp”, “val.h”, and “prog3.cpp” files will be propagated to your Work Directory.
- Submissions after the due date are accepted with a fixed penalty of 25%. No submission
is accepted after Wednesday 11:59 pm, May 4, 2022.

## Grading Table

|Item|Points|
|----|------|
Compiles Successfully | 1
testprog1: Undefined variable | 1
testprog2: Illegal mixed type operands for logic expressions | 1
testprog3: Divide by zero execution error | 1
testprog4: Illegal Operand Type for Sign Operator | 1
testprog5: Illegal mixed type operands in expressions | 1
testprog6: Illegal mixed type assignment | 1
testprog7: Successful execution with assignment and If statements | 1
testprog8: Successful execution with If-Then-Else statement | 1
testprog9: Testing If Statement condition with equality | 1
testprog10: Testing string concatenation | 2
testprog11: Testing nested if-then-else statement | 2
testprog12: Testing legal mixed type assignment | 2
testprog13: Testing expression evaluation with mixed type operands | 2
testprog14: Successful Execution | 2
**Total** | 20
