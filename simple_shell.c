/*
  File: simple_shell.c
  Author: Andrew Ingson (aings1@umbc.edu)
  Date: 9/18/2019
  Project: CMSC 421 (Principles of Operating Sytems) HW 1
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <wait.h>

/*
    term_getline()
    
    - No Parameters
    - Ouputs char* cstring
    - Reads user input from the terminal and returns it in a dynamic,
      size adjusted cstring
*/
char* term_getline() {
    // create c string arrays so we change sizes
    char* outputString = NULL;
    char* tempString = NULL;
    // create size vars 
    size_t currSize = 0;
    size_t loc = 0;
    // var to set how much to increase string capacity by each time is is full
    int adjustRate = 16;
    // create beggining input char
    int inputChar = EOF;

    while (inputChar) {
        // get a character from the input
        inputChar = fgetc(stdin);

        // see if we are out of data
        if (inputChar == EOF || inputChar == '\n') {
            // set the current char to size
            inputChar = 0;
        }
       
       // see if we need to increase the size of the outputString
        if (currSize <= loc) {
            // update size
            currSize += adjustRate;
            // realloc string to new size
            tempString = realloc(outputString, currSize);
            // change output to new and improved big string
            outputString = tempString;
        }
        // add the input character to the output array
        outputString[loc] = inputChar;
        loc++;
    }
    //free(tempString);
    return outputString;   
}

int main() {
    // create exit flag and exit string to check against
    bool exit = false;
    const char exitChar[] = "exit";

    // loop until we break or return
    while(1) {
        // check if it's time to exit
        if (exit == true) {
            break;
        }
        // present the user with the prompt and take input
        printf("$: ");
        char* usrInput = term_getline();
        
        // Validation Sequence!

        if (usrInput != NULL) {
            // create strings and other vars to tokenize data into
            char* charSplit = NULL;
            // cast to results so we dont mess with usrInput
            char* results = usrInput;
            // var for how big the arg string array should be
            int inputSize = strlen(usrInput) * 2;
            // create final arg array with inputSize size
            char* finalArgs[inputSize];
            // how many args do we have?
            int argCount = 0;

            // loop to intialize final args
            for (int i = 0; i < inputSize; i++){
                finalArgs[i] = NULL;
            //    free(finalArgs[i]);
            }

            // tokenize the user input into final args (2d array of chars)
            while ((charSplit = __strtok_r(results, " ", &results))){
                finalArgs[argCount] = charSplit;
                argCount++;
            }
            // check to see if we need to exit and clean up memory
            if (strcmp(finalArgs[0], exitChar) == 0) {
                exit = true;
                break;
            }

            // 
            // FORK!!!
            // 

            pid_t pid;
            // fork proccess
            pid = fork();

            // Check if fork failed
            if (pid < 0) {
                fprintf(stderr, "Fork Failed");
                return 1;
            } else if (pid == 0) {
                // Child Proccess

                // execute user args
                // if exec is unsuccessful, prompt user with error and kill child
                if (execvp(finalArgs[0],finalArgs) != 0) {
                    printf( " %s\n", "Unknown Command. " );
                    return 0;
                }
                // exit child if we need to exit
                if (exit == 0) {
                    return 0;
                }
            } else {
                // parent proccess

                // wait for child to complete
                wait(NULL);
            }

            // clean up cstrings for next loop
            for (int i = 0; i < inputSize; i++){
                finalArgs[i] = NULL;
                free(finalArgs[i]);
            }
            free(charSplit);
        
        }
    // clean up input string for next loop
     free(usrInput);
    }

    // successful exit
    return 0;
}
