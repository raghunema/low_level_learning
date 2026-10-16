#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h> //to get global errno var
#include <unistd.h> //for execvp function
#include <sys/wait.h> //for waitpud function

#define MAX_TOKENS 1024

// Actually launches the programs once we parse the input
int launch(char **tokens) {

    pid_t childPid = fork(); //create a copy of the request
    if (childPid == -1) {
        int error = errno;

        //fprintf allows us to send message to Standard Error Stream
        fprintf(stderr, "Fork process failure, error number: %d, error: %s\n", error, strerror(error));
        return EXIT_FAILURE;
    } else if (childPid == 0) {
        //childPid == 0 if I am in the child process -> only then run execVP

        int executed_process = execvp(tokens[0], tokens);
        if (executed_process == -1) {
            int error = errno;

            fprintf(stderr, "Process ExecVp failure, error number: %d, error: %s\n", error, strerror(error));
            _exit(error); //exits the child process if execute failed
        }

    } else if (childPid > 0) {
        //only if child process actually started wait for it

        int childStatus;
        pid_t wait_child = waitpid(childPid, &childStatus, 0);

        if (wait_child == -1) {
            int error = errno;

            fprintf(stderr, "Wait process failure, error number: %d, error: %s\n", error, strerror(error));
            return EXIT_FAILURE;
        }

        if (WIFEXITED(childStatus)) {
           return WEXITSTATUS(childStatus);
        } else if (WIFSIGNALED(childStatus)) {
            return 128 + WTERMSIG(childStatus); //terminaition signal? Why is convetion to add 128?
        }
    }

    return EXIT_FAILURE;
}
 
int main(void) {

    bool run_shell = true;

    char *line = NULL;
    size_t linecap = 0; //capacity of the line buffer
    ssize_t linelen; //length of the line

    char *tokensplitters = " \t\n"; //user for strtok
    char *tokens[MAX_TOKENS]; //intitializing an array of strings

    //read a line, print a line
    while (run_shell) {
        printf(">");
        fflush(stdout); //flush the memory buffer that prints to terminal forcefully, otherwise output will just sit in the buffer

        linelen = getline(&line, &linecap, stdin); 
        if (linelen == -1) { //for crtl-d as it sends End-of-File signal (no input)
            printf("\n");
            break;
        }

        const char *invalid_chars = "\n";
        //will return how many bytes before new line?
        size_t valid_len = strcspn(line, invalid_chars);
        line[valid_len] = '\0'; //replace new line with terminator?

        printf("line: %s, line length: %zu\n", line, valid_len); //for debugging?

        if ((strcmp(line, "exit") == 0)) {
            run_shell = false; //keeping this for sanity's sake
            break;
        }

        //parse the line
        size_t num_tokens = 0;
        char* temp_token = strtok(line, tokensplitters); //a string defined as a char pointer
       
        //keep iterating
        while(temp_token != NULL && num_tokens < (MAX_TOKENS - 1)){
            tokens[num_tokens] = temp_token;
            //printf("Token %s at index: %zu\n", tokens[num_tokens], num_tokens); //Hello
            
            num_tokens++;
            temp_token = strtok(NULL, tokensplitters); //a string defined as a char pointer
        }

        tokens[num_tokens] = NULL;

        //skip this since we will never get to fork?
        if (num_tokens == 0) {
            continue;
        } 

        //pass in the address of tokens, not a full copy?
        int child_process = launch(tokens);
        
    }

    free(line);
    return EXIT_SUCCESS;
}