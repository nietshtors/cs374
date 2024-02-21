#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

const int SIZE=1024;  // bytes

struct bicycle {
    int wheel_count;
    char name[32];
};

int main(void)
{
    // Allocate some space from the OS
    void *heap = mmap(NULL, SIZE, PROT_READ|PROT_WRITE,
                  MAP_ANON|MAP_PRIVATE, -1, 0);

    // Challenge 1:
    //
    // Treat heap as an array of chars
    // Store a string there with strcpy() and print it
    // char *s = heap;
    // strcpy(s, "I'm a string!");
    // printf("%s\n\n", s);

    // // Challenge 2:
    // //
    // // Treat heap as an array of ints
    // // Use a loop to store the values 0, 10, 20, 30, 40 in it
    // // Use a loop to retrieve the value and print them
    // int *arr = heap;
    // for (int i = 0; i <= 4; i++) {
    //     arr[i] = i * 10;
    // }
    // for (int i = 0; i < 5; i++) {
    //     printf("%d\n", arr[i]);
    // }
    

    // // Challenge 3:
    // //
    // // Treat heap as an array of struct bicycles
    // // Store 3 bicycles
    // // Print out the bicycle data in a loop
    // struct bicycle *bikes = (struct bicycle*)heap;
    // // int bikeSize = sizeof(struct bicycle);
    // for (int i = 0; i < 3; i++) {
    //     bikes[i].wheel_count = 2;
    //     sprintf(bikes[i].name, "Bike %d", i + 1);
    //     printf("Name: %s\nWheel Count: %d\n", bikes[i].name, bikes[i].wheel_count);
    // }
    // printf("\n");
    

    // Challenge 4:
    //
    // If you used pointer notation, redo challenges 1-3 with array
    // notation, and vice versa.
    // Challenge 4.1:
    char *s1 = heap;
    strcpy(s1, "I'm a string!");
    printf("%s\n", s1);

    // Challenge 4.2:
    int *arr1 = heap;
    for (int i = 0; i <= 4; i++) {
        *(arr1+i) = i * 10;
    }
    for (int i = 0; i < 5; i++) {
        printf("%d\n", *arr1++);
    }

    // Challenge 4.3:
    struct bicycle *bikes = heap;
    for (int i = 0; i < 3; i++) {
        (bikes+i)->wheel_count = 2;
        sprintf((bikes+i)->name, "Bike %d", i + 1);
        printf("Name: %s\nWheel Count: %d\n", (bikes+i)->name, (bikes+i)->wheel_count);
    }
    printf("\n");


    // Challenge 5:
    //
    // Make the first 32 bytes of the heap a string (array of chars),
    // and follow that directly with an array of ints.
    //
    // Store a string in the string area of the heap
    // Use a loop to store the values 0, 10, 20, 30, 40 in the int array
    // Use a loop to retrieve the value and print them
    // Print the string
    char *arrChar = heap;
    int *arrInt = heap + 32;

    // Challenge 6:
    //
    // Make the first 32 bytes of the heap a string (array of chars),
    // followed by a struct bicycle, followed by an array of 7 ints,
    // followed by another struct bicycle.
    //
    // Make sure both struct bicycles are aligned on 16-byte boundaries.

    // Free it up
    munmap(heap, SIZE);
}
