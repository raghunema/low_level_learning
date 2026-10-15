#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TOKENS 1024

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

        if (num_tokens == 0) {
            continue;
        }
        
    }

    free(line);
    return EXIT_SUCCESS;
}