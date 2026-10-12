#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main() {

    bool run_shell = true;
    printf(">");

    char *line = NULL;
    size_t linecap = 0; //capacity of the line buffer
    ssize_t linelen; //length of the line

    //read a line, print a line
    while (run_shell) {

        linelen = getline(&line, &linecap, stdin); 
        if (linelen == -1) {
            printf("Unable to get line properly");
            return EXIT_FAILURE;
        }

        // char input[linelen-1];
        // (void)strncpy(input, line, sizeof(input)); //remove the '\n' character of better parsing?
        
        printf("line: %s, line length: %zu\n", line, linelen); //for debugging?
        printf(">");

        if ((strcmp(line, "exit()") == 0) || (strcmp(line, "^C") == 0)) {
            run_shell = false;
        }

        if (strcmp(line, "\n") == 0) {
            printf(">");
        }
    }

    return EXIT_SUCCESS;
}