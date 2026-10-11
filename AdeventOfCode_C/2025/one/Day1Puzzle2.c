#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>

int main() {

    // open file
    FILE *file = fopen("puzzleInput.txt", "r");

    if (file == NULL) {
        printf("File not found :(\n");
        return EXIT_FAILURE; // equivalent of returning 1;
    }

    int current_location = 50;
    int password = 0;

    char rotation[10];
    while (fgets(rotation, 10, file)) {
        char r_direction = rotation[0];
        int r_distance = atoi(rotation + 1); // will always be a positive number
        printf("Direction: %c, Distance: %d\n", r_direction, r_distance);
         
        if (r_direction == 'R'){

            //you have to wrap around a.k.a. go through 0
            password += (current_location + r_distance) / 100;
            current_location = (current_location + r_distance) % 100;
        } else {
            password += ((100 - current_location) % 100 + r_distance) / 100;
            current_location = ((current_location - r_distance) % 100 + 100) % 100;
        }

        printf("New Location: %d\n", current_location);
    }
 
    //close out the file
    fclose(file);

    printf("\nPassword: %d\n", password);
    return EXIT_SUCCESS;
}