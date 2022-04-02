# RA 4 Submission: Directory of Punctuation Characters

- Due Feb 14 by 11:59pm
- Points 6
- Submitting: an external tool
- Available Feb 8 at 1:30pm - Feb 16 at 11:59pm 8 days

Write a C++ program that should read from a file name specified in the command line as an argument. However, if no file name is provided, the program should print on a new line "No file is found.", and exit. If the file cannot be opened, print on a new line "File cannot be opened: ", followed by the file name, and exit. The program should read from the file characters until the end of file. In this assignment, an empty file is defined as the file that does not include any non-whitespace characters. In case the file is empty, print out on a new line the message "File is empty." and then exit. The program should keep a record of the number of occurrences of each seen punctuation characters in the input file. The program should create a simple directory using the \<map> container that maps a character to an integer for the occurrences of each punctuation character. In this assignment, we follow the \<cctype> (ctype.h) C/C++ header file definition for the function ispunct(c), which checks whether c is a punctuation character. Punctuation characters are all graphic characters (as in isgraph() function), that are not alphanumeric (as in isalnum() function), as defined in the original 127-character ASCII set. For example, the subset of characters {!"#$%&'()*+,-./} and {:;<=>?@} are considered punctuation characters.

For more details, see the \<cctype> (ctype.h) C/C++ header files definition at:

<https://www.cplusplus.com/reference/cctype/>

After all input has been processed, the program should

- Print out the total non-whitespace characters in the file.
- Print each punctuation character and the number of times it was seen. The punctuation characters should be printed in order.
- Determine the punctuation character that has been seen the largest number of times in the file and print it out.
