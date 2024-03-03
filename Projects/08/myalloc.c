#include "myalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>


int first = 1;
block *head = NULL;  // Head of the list, empty
int padded_struct_block_size = PADDED_SIZE(sizeof(block));


void *myalloc(int size) {
    size = PADDED_SIZE(size);   // Pad size requested

    // Initialize linked list
    if (first == 1) {
        first--;
        void *heap = mmap(NULL, MMAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        head = heap;
        head->size = MMAP_SIZE - PADDED_SIZE(sizeof(block));
        head->in_use = 0;
        head->next = NULL;
    }

    block *b = head;
    // Looks for next available block or returns NULL if there are none
    while (b->size < size || b->in_use) {
        if (b->next) {b = b->next;}
        else {return NULL;}
    }
    
    b->in_use = 1;

    // If big enough to split
    if (b->size >= size + padded_struct_block_size + 16) {
        // Initialize new block
        block *b2 = PTR_OFFSET(b, padded_struct_block_size + size);
        // Set new block's data
        b2->size = b->size - size - padded_struct_block_size;
        b2->in_use = 0;
        b2->next = b->next;
        // Adjust old block
        b->size = size;
        b->next = b2;
    }

    return PTR_OFFSET(b, padded_struct_block_size);
}

void myfree(void *p) {
    block *b = head;
    // Looks for block that matches or returns NULL if there are none
    while (p != PTR_OFFSET(b, padded_struct_block_size)) {
        if (b->next) {b = b->next;}
        else {return;}
    }

    b->in_use = 0;
}

void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        // printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // // Example 1
    // void *p;

    // p = myalloc(512);
    // print_data();

    // myfree(p);
    // print_data();

    // // Example 2
    // myalloc(10); print_data();
    // myalloc(20); print_data();
    // myalloc(30); print_data();
    // myalloc(40); print_data();
    // myalloc(10000); print_data();
    // myalloc(50); print_data();

    // // Example 3
    // void *p;

    // myalloc(10);     print_data();
    // p = myalloc(20); print_data();
    // myalloc(30);     print_data();
    // myfree(p);       print_data();
    // myalloc(40);     print_data();
    // myalloc(10);     print_data();

}