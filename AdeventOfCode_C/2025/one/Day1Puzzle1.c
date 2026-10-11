#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>

int update_location (int curr_loc, char* rot) {

    //printf("%s\n", rot);
    //Get only the distance from rotation
    char *distance_word = rot + 1;
   
    int new_loc = 0;

    if (rot[0] == 'R'){
        new_loc = (curr_loc + atoi(distance_word)) % 100;
    } else {
        new_loc = (curr_loc - atoi(distance_word)) % 100;
    }

    if (new_loc == 0){
        printf("Update Password! New Location:%d\n", new_loc);
    }

    return new_loc;
}


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
        current_location = update_location(current_location, rotation);
        printf("%d\n", current_location);

        if ((int)current_location == 0) {
            password = password + 1;
            printf("Updated password!");
        }
    }

    //close out the file
    fclose(file);

    printf("\nPassword: %d\n", password);
    return password;
}