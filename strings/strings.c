#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int test_char_one(void) {

    char test_char;
    printf("test_char: %c\n", test_char); //prints garbage (something already in memory)
    printf("test_char: %d\n", test_char); //since everyhing is stored as a number, this prints, doesnt actually mean anything yet
    printf("test_char address: %p\n", &test_char); //real memory address

    return EXIT_SUCCESS;
}

int test_str(void) {

    char* test_str_2 = "Hello";
    printf("test_str_2: %s\n", test_str_2); // "Hello"
    printf("test_str_2: %p\n", test_str_2); // <- technically its actual value; stores &test_str_2[0]
    printf("test_str_2 address: %p\n", &test_str_2); // An Address
    printf("indexing test_str_2[0]: %c\n", test_str_2[0]); // "H" - the [0] does the dereference
    printf("indexing test_str_2[0]: %c\n", *test_str_2); // "H"
    printf("test_str_2 address[0]: %p\n", &test_str_2[0]); // An Address (the same as %p of test_str_2)
    printf("indexing test_str_2[1]: %c\n", test_str_2[1]);  // "e"
    printf("indexing test_str_2[1]: %c\n", (*test_str_2) + 1); //I because it gets the int value of H (72) adds +1 and gives 73 (I)
    printf("indexing test_str_2[1]: %c\n", *(test_str_2 + 1));
    printf("test_str_2 address[1]: %p\n", &test_str_2[1]); // Address 1 byte above [0]

    return EXIT_SUCCESS;
}

int test_str_array(void) {

    char test_3[] = "World"; //An array of characters? No pointers involved
    printf("test 3: %s\n", test_3);
    printf("test 3: %p\n", (void*)test_3); //value is itself's address 
    printf("test 3 address : %p\n", (void*)&test_3);
    printf("indexing test 3: %c\n", test_3[0]);
    printf("indexing test 3: %c\n", *test_3); //same explict derefence from before
    printf("indexing test 3: %p\n", (void*)&test_3[0]);
    printf("indexing test 3: %c\n", test_3[1]);
    printf("indexing test 3: %p\n", (void*)&test_3[1]);

    return EXIT_SUCCESS;
}

int main(void) {
 
    //test_char_one();

    //test_str();

    test_str_array();

    return EXIT_SUCCESS;
}