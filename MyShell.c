/***************************************************************************//**
  @file         MyShell.c
  @author       Chloe Duncan

*******************************************************************************/

/**
DONOT change the existing function definitions. You can add functions, if necessary.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


/**
  @brief Fork a child to execute the command using execvp. The parent should wait for the child to terminate
  @param args Null terminated list of arguments (including program).
  @return returns 1, to continue execution and 0 to terminate the MyShell prompt.
 */
int execute(char **args) {

    // fork
    int pid = fork();
    
    // fork failed
    if (pid < 0) {
        fprintf(stderr, "error executing fork\n");
        free(args[0]);
        exit(1);
    }
    
    // child process
    else if (pid == 0) {
        // execvp
        int status = execvp(args[0], args);
        
        if (status < 0) {
            fprintf(stderr, "error executing command: No such file or directory\n");
            free(args[0]);
            exit(0);
        }
    }
    
    // parent process
    else {
        // wait for child process to terminate
        wait(NULL);
    }
    
    return 1;
}


/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
char** parse(void) {
    // user input
    char* input = NULL;
    size_t inputSize = 0;

    getline(&input, &inputSize, stdin);    
        
    // split into tokens
    char** tokenArray = calloc(inputSize, sizeof(char));
    char* token = strtok(input, " ");
    
    int numTok = 0;
    
    while (token != NULL) {
        // put token into an array of strings
        tokenArray[numTok] = token;       
        token = strtok(NULL, " ");      
        numTok = numTok + 1;
    }
    
    // get last elem of array and get rid of new line character
    int len = strlen(tokenArray[numTok - 1]);
    tokenArray[numTok - 1][len - 1] = '\0';   
    
    return tokenArray;
}


/**
   @brief Main function should run infinitely until terminated manually using CTRL+C or typing in the exit command
   It should call the parse() and execute() functions
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv) {

    int loop = 1;

    while (loop == 1) {
        printf("MyShell> ");
        // parse
        char** tokenArray = parse();
        
        // if user types exit
        if (strcmp(tokenArray[0], "exit") == 0) {
            free(tokenArray[0]);
            free(tokenArray);
            exit(0);
        }
        
        // if user doesn't type empty command, execute
        if (strcmp(tokenArray[0], "\0") != 0) {
            // execute
            loop = execute(tokenArray);
        }
        
        // free
        int len = sizeof(tokenArray) / (sizeof(char *) * 1);    
        for (int i = 0; i < len; i++) {
            free(tokenArray[i]);
        }
        
        free(tokenArray);
    }

    return EXIT_SUCCESS;
}
