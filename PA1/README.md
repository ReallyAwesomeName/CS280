# CS280 PA1

CS 280
Spring 2022
Programming Assignment 1
February 17, 2022
Due Date: Sunday, March 6, 2022, 23:59
Total Points: 20

In this programming assignment, you will be building a lexical analyzer for small programming
language and a program to test it. This assignment will be followed by two other assignments to
build a parser and interpreter to the same language. Although, we are not concerned about the
syntax definitions of the language in this assignment, we intend to introduce it ahead of
Programming Assignment 2 in order to show the language reserved words, constants, and
operators. The syntax definitions of the small programming language, called Pascal-Like Simple
Language (PLSL), are given below using EBNF notations. The PLSL syntax has some features
similar to the well-known Pascal Language. However, the details of the meanings (i.e. semantics)
of the language constructs will be given later on.

1. Prog ::= PROGRAM IDENT; DeclBlock ProgBody
2. DeclBlock ::= VAR {DeclStmt;}
3. DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
4. ProgBody ::= BEGIN {Stmt;} END
5. Stmt ::= AssigStmt | IfStmt | WriteLnStmt | ForStmt
6. WriteLnStmt ::= WRITELN (ExprList)
7. IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
8. ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
9. AssignStmt ::= Var := Expr
10. ExprList ::= Expr {, Expr}
11. Expr ::= Term {(+|-) Term}
12. Term ::= SFactor {( * | / ) SFactor}
13. SFactor ::= [(+ | -)] Factor
14. LogicExpr ::= Expr (= | > | <) Expr
15. Var ::= IDENT
16. Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)

Based on the language definitions, the lexical rules of the language and the assigned tokens to the
terminals are as follows:

1. The language has identifiers, referred to by IDENT terminal, which are defined to be a letter
followed by zero or more letters or digit. It is defined as:
IDENT := [Letter _] {(Letter|Digit|_)}
Letter := [a-z A-Z]
Digit := [0-9]
2. Integer constants, referred to by ICONST terminal, are defined as one or more digits. It is
defined as:
ICONST := [0-9]+
3. Real constants, referred to by RCONST terminal, are defined as zero or more digits followed
by a decimal point (dot) and one or more digits. It is defined as:
RCONST := ([0-9]*)\.([0-9]+)
For example, real number constants such as 12.0 and .2 are accepted, but 2. and 2.45.2 are not.
4. String literals, referred to by SCONST terminal, are defined as a sequence of characters
delimited by single quotes, that should all appear on the same line. For example,
‘Hello to CS 280.’ is a string literal. While, “Hello to CS 280.” Or ‘Hello
to CS 280.” are not.
5. The reserved words of the language are: program, end, begin, writeLn, if, integer, real, string,
for, to, downto, else, var. These reserved words have the following tokens, respectively:
PROGRAM, END, BEGIN, WRITELN, IF, INTEGER, REAL, STRING, FOR, TO,
DOWNTO, ELSE and VAR.
6. The operators of the language are: +, -, *, /, :=, (, ), =, >, <. These operators are for plus,
subtract, multiply, divide, assignment, left parenthesis, right parenthesis, equality, greater than,
and less than operations. They have the following tokens, respectively: PLUS, MINUS,
MULT, DIV, ASSOP, LPAREN, RPAREN, EQUAL, GTHAN, LTHAN.
7. The semicolon, colon and comma characters are terminals with the following tokens:
SEMICOL, COLON, and COMMA.
8. A comment is defined by all the characters following the sequence of characters “(\*” as starting
delimiters to the closing delimiters “*)”. Comments may overlap one line, as multi-line
comments. A recognized comment is skipped and does not have a token.
9. White spaces are skipped. However, white spaces between tokens are used to improve
readability and can be used as a one way to delimit tokens.
10. An error will be denoted by the ERR token.
11. End of file will be denoted by the DONE token.
