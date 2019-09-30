File: simple_shell.c
  Author: Andrew Ingson (aings1@umbc.edu)
  Date: 9/18/2019
  Project: CMSC 421 (Principles of Operating Sytems) HW 1

1. Present the user with a prompt at which he or she can enter commands.
    Print out "$: " to indicate input prompt. 
2. Accept command input of arbitrary length (meaning you cannot set a hard limit on command length).
    Take user input using a custom getline function, term_getline(). This function creates uses multiple cstring arrays, and realloc, to increase the size of the "line"(outputString) by the value of "adjustRate" each time the capacity of the cstring is bellow or at the needed postion. A pointer to the completed cstring is then returned and assigned to the var, "usrInput".
3. Parse command-line arguments from the user's input and pass them along to the program that the user requests to be started.
    Use the string tokenizer function, strtok_r(), to split up the cstring usrInput, into seperate tokens (using a space or ' ' as a delimeter), which are stored in an array of cstrings, "finalArgs". After tokenizing, the process is forked. From there, if the process is forked successfully, the users' input is executed using execvp.
4. The shell should exit with a successful return code when the user enters the command exit with no arguments.
    Inbetween tokenizing and forking, the program checks to see if the first item in finalArgs is equal to the string "exit", if it is it sets an exit flag to true and breaks the loop, allowing the main to return 0 (success).