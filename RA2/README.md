# RA 2 Submission: Counting Lines

- Due Jan 31 by 11:59pm
- Points 7
- Submitting: an external tool
- Available Jan 25 at 8am - Feb 2 at 11:59pm 9 days

Write a C++ program that acts like a simple counting tool for collecting information from textual files of documents prepared for a simple word processing software. An input file for the simple word processor includes three types of data, commented lines, command lines and general text of the document to be formatted. A command line is recognized by the ‘%’ at the beginning of the line and followed by a formatting command. The simple word processor software includes three commands only for formatting textual documents. These are “bold”, “italic”, and “regular”, which would affect formatting the following text of the document. A commented line is recognized by “#” at the beginning of the line, and would be skipped by the word processor. The simple counting tool should collect data about the total number of lines read from the file, the number of commented lines, the number of command lines, and the number of bold, italic, and regular commands.

Write a C++ program for the simple counting tool that reads lines from a file until the end of file. The program should prompt the user for the file name to read from. The program should open the file for reading, and if the file cannot be opened, it should print the message "File cannot be opened ", followed by the filename, and exit. The program should consider the first word following the ‘%’ character of a command line as a command name. If no word is found, or a word is not recognized as one of the three commands, an error message should be printed out followed by the line number. See the example below for the format of the error message.

After reading the contents of the input file, the program should print out the total number of lines, the number of commented lines, the number of command lines, and the number of bold, italic, and regular commands in the file. An example of an input file and the expected results after processing the file are shown below.
