#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int *array_alloc(int size)
{
    int *array;
    array = malloc(size);
    return array; 
    
}

void array_free(int *array)
{
    free(array);  // IMPLEMENT ME
}

int array_get_size(void)
{
    int size;

    printf("Enter number of elements in array: ");
    fflush(stdout);
    scanf(" %d", &size);

    return size;
}

void array_populate(int *array, int size)
{
    srand(getpid() + getppid());

    for (int i = 0; i < size; i++) {
        array[i] = rand()%1000;
    }
}

void array_print(int *array, int size)
{
    printf("\nArray contains:\n");

    for (int i = 0; i < size; i++) {
        printf("  %sarray[%d] = %d\n", i < 10? " ": "", i, array[i]);
    }
}

int main(void)
{
    int size = array_get_size();
    int *array = array_alloc(size);
    array_populate(array, size);
    array_print(array, size);
    array_free(array);
}