#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main() {

    // open file
    FILE *file = fopen("input.txt", "r");

    //Correct path check
    if (file == NULL) {
        printf("File not found :(\n");
        return EXIT_FAILURE; // equivalent of returning 1;
    }

    char puzzleInput[10000];
    fgets(puzzleInput, 10000, file); //get puzzleInput from file

    // dynamic array to save each range as a seperate string
    char **ranges = NULL; //an array of character pointers? pointer of pointers
    int num_ranges = 0;

    //Conceptually an array is just one pointer, that we assume has more elements of the same data type in front
    //So a pointer of a pointer is just pointing to the first char*
    //And then we realloc it to have more elements
    
    //iterate through ranges that is stored as one string right now
    char* range = strtok(puzzleInput, ",");  //gives a pointer to the new word
    while (range != NULL) {
        //printf("Range: %s\n", range);

        //Make a new array that adds one more character pointer
        char** new_ranges = realloc(ranges, (num_ranges + 1) * sizeof(char*));
        if (new_ranges == NULL) {
            free(ranges);
            printf("Unable to grow ranges (reallocation failure)");
            return EXIT_FAILURE;
        }
        ranges = new_ranges; //Update the pointer of the old range to the new reallocated range
        ranges[num_ranges] = range;
        //printf("Range: %s\n", ranges[num_ranges]);

        //get next string
        range = strtok(NULL, ",");
        num_ranges++;
    }

    long long total = 0;

    //iterate through all the ranges
    for (int i = 0; i < num_ranges; i++) {
        char* range = ranges[i];
        char* temp_range = strdup(range); //string dupe so i can actually read it for later

        long long r_start = strtol(strtok(range, "-"), NULL, 10);
        long long r_end = strtol(strtok(NULL, "-"), NULL, 10);

        //printf("Range: %s, start: %d, end: %d\n", temp_range, r_start, r_end);

        //actual logic for the puzzle
        for (long long j = r_start; j < (r_end + 1); j++) {
            char buffer[32]; //Create a character buffer
            snprintf(buffer, sizeof(buffer), "%lld", j); //Create number into a word
            
            int num_digits = strlen(buffer);
            int mid = num_digits / 2;

            //if the number can be split into 2 absolute parts
            if (num_digits % 2 == 0) {

                char first_half[16]; //must be half or less of 32 so 16 is enought
                memcpy(first_half, buffer, mid); //copy the first half of the string into entirely
                first_half[mid] = '\0';

                char *second_half = &buffer[mid];

                // printf("Original length: %d\n", num_digits);
                // printf("First Half:  %s\n", first_half);
                // printf("Second Half: %s\n", second_half);

                if (strcmp(first_half, second_half) == 0) {
                    total += j;
                }

            }

        }

    }

    printf("Total: %lld\n", total);
    
    fclose(file);
    free(ranges);

    return EXIT_SUCCESS; //equivalent of returning 0
}