# RA 5 Submission: Listing Tokens

- Due Feb 28 by 11:59pm
- Points 5
- Submitting: an external tool
- Available Feb 23 at 10:30am - Mar 2 at 11:59pm 8 days

You are given a copy of “lex.h” from Programming Assignment 1, and a file called “tokensListing.cpp” as a driver program.

DO NOT CHANGE neither “lex.h” nor “tokensListing.cpp”.

Your implementation should include the following in another file, called “RA5.cpp”:

- The function

LexItem id_or_kw(const string& lexeme, int linenum);

id_or_kw () function accepts a reference to a string of a lexeme and a line number and returns a LexItem object. It searches for the lexeme in a directory that maps a string value of a keyword to its corresponding Token value, and it returns a LexItem object containing either the keyword Token if it is found, or the identifier token IDENT if not.

- The overloaded operator function operator<< for LexItem.

ostream& operator<<(ostream& out, const LexItem& tok);

The operator<<() function accepts a reference to an ostream object and a reference to a LexItem object, and returns a reference to the ostream object. The operator<< function should print out the string value of the Token in the tok object, followed by its lexeme and line number.

**Note that the implementation of operator<< function in RA5 differs from its implementation in PA1. See the examples below for the output format.**

Use the given driver program in “tokensListing.cpp” for testing your implementations. The driver program accepts two command line arguments, “-othertok” and “-idsonly”.

The “-othertok” flag is used to test your implementation of the overloaded operator<< function only, and the “-idsonly” is used to test your implementation of both functions. See the details of the outputs for the examples in the slides.
