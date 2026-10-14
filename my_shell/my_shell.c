#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUFSIZ 1024

int main(void) {

    bool run_shell = true;

    char *line = NULL;
    size_t linecap = 0; //capacity of the line buffer
    ssize_t linelen; //length of the line

    char *tokensplitters = " \t\n"; //user for strtok
    char *tokens[BUFSIZ]; //intitializing an array of strings

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
        //tokens[num_tokens] = strtok(line, tokensplitters);

        //"Hello World"
        char* temp_token = strtok(line, tokensplitters); //a string defined as a char pointer
        printf("New Token address: %p\n", &temp_token); //0x16f81cb10
        printf("New Token actual value: %p\n", (void *)temp_token); //0x102941cf0
        printf("New Token str value: %s\n", temp_token); //"Hello"
        printf("New Token index 0: %c\n", temp_token[0]); //'H' or *(temp_token)
        printf("New Token index 0 address: %p\n", &temp_token[0]); //0x102941cf0

        //So tokens should be an array of char* 's
        // char* tokens[1000] //an array of 1000 (arbritary size) strings
        tokens[num_tokens] = temp_token;
        printf("Token one: %s\n", tokens[num_tokens]); //Hello
        printf("Token one actual value: %p\n", tokens[num_tokens]); //0x102941cf0

        //keep iterating
        while(temp_token != NULL && num_tokens < (BUFSIZ - 1)){
            tokens[num_tokens] = strtok(NULL, tokensplitters);
            printf("New Token: %s\n", tokens[num_tokens]);
            num_tokens++;
        }

    }

    free(line);
    return EXIT_SUCCESS;
}