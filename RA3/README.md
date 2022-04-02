# RA 3 Submission: Counting Words with Names & Integers

- Due Feb 7 by 11:59pm
- Points 8
- Submitting: an external tool
- Available Feb 1 at 2:30pm - Feb 9 at 11:59pm 8 days

Write a C++ program that accepts one command line argument for a file name, and ignores any extra command line arguments that may be provided after the first one. If no file name is provided, the program should print on a new line "No file name is found", and exit. If the file cannot be opened, print on a new line "File cannot be opened: ", followed by the file name, and exit. The program should read from the file lines until the end of file is found. If the input file is empty, print out the message "File is empty." on a new line and then exit. The program should count the number of lines, the number of non-blank lines, the total number of words, the number of names, and the number of integers, seen in the file.

A word is defined as a sequence of one or more non-whitespace characters separated by whitespace. A word is defined as a name if it starts by a letter and followed by zero or more letters, digits, underscores ‘_’, ‘@’, or ‘.’ characters. For example, 'value', 'val@l9', 'num234ten', 'num_45' are valid names, but '9val', and '_num' are not. A word is defined as an integer if it starts by a digit and followed by zero or more digits (ignoring signed integers). For example, 2345 is an integer word, while 44.75 and 4today45 are not. Note that a line having only whitespace characters is a non-blank line as well.
