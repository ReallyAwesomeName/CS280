# CS 280: RA8 Value Class Testing

Spring 2022\
Recitation Assignment 8\
April 7, 2022\
Due Date: Monday, April 11, 2022, 23:59\
Total Points: 4

In  this  recitation  assignment,  you  are  given  the  definition  of  a  Class,  called  Value,  which
represents  values  of  different  PLSL  language  types  of  operands,  as  (INTEGER,  REAL,
STRING) and Boolean, through using C++ types as (int, float, string, and bool)  for four data
members. The objective of defining the Value class is to facilitate constructing an interpreter
for the PLSL language which evaluates expressions and executes PLSL statements.
In  this  assignment,  you  are  required  to  implement  some  of  the  overloaded  operators  of  the
Value  class  in  order  to  enable  testing  this  class  separately  as  a  unit  before  using  it  in  the
construction  of  the  PLSL  interpreter  in  PA3.  You  are  required  to  implement the overloaded
operators for Addition, Multiplication and Equality. The semantic rules governing the
evaluation of expressions in the PLSL language are summarized below:

- The binary arithmetic operations for addition (PLUS), subtraction (MINUS),
multiplication  (MULT),  and  division  (DIV)  are  performed  upon  two  numeric  operands
(i.e.,  INTEGER,  REAL)  of  the  same  or  different  types.  If  the  operands  are  of  the  same
type, the type of the result is the same type as the operator’s operands. Otherwise, the type
of the result is REAL.  
- When  the  PLUS  operator  is  applied  on  STRING  operands,  the  operator  performs  string
concatenation.
- The  EQUAL  LTHAN  and  GTHAN  relational  operators  operate  upon  two  compatible
operands. The evaluation of a logical expression, based on EQUAL, LTHAN or GTHAN
operation,  produces  either  a  true  or  false  value  that  controls  the  execution  of  the  IfStmt
Then-part or Else-part statement.

## Note

It is recommended to implement the other overloaded operators in the Value class and
testing them before using the class implementation in the PLSL interpreter project.

## Vocareum Automatic Grading

- A driver  program  is provided for testing the implementation, called “RA8prog.cpp”, on
Vocareum. The “RA8prog.cpp” will be propagated to your Work directory.
- You are provided by a test  case file associated with Recitation Assignment  8. Vocareum
automatic grading will  be based on the produced output of  your  implementations  for the
required overloaded operators compared with the test case file. You may use them to check
and  test  your  implementation.  These  are  available  in  a  compressed archive “RA8  Test
Cases.zip” on Canvas assignment.  
- “RA8prog.cpp” is available with the other assignment material on Canvas.

## Submission Guidelines

- Please upload your implementation to Vocareum as a “val.cpp” file. The file should include
the implementations of the Value overloaded operators functions for addition,
multiplication and equality.

- Submissions after the due date are accepted with a fixed penalty of 25%. No submission
is accepted after Wednesday 11:59 pm, April 13, 2022.
