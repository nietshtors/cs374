#include "myalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>


int first = 1;
block *head = NULL;  // Head of the list, empty
const int padded_struct_block_size = PADDED_SIZE(sizeof(block));


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
        head->prev = NULL;
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
        b2->prev = b;
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

    if (b->prev && b->prev->in_use == 0) {
        // Consolidate
        b = b->prev;
        b->size = b->size + b->next->size + padded_struct_block_size;
        b->next = b->next->next;
        if (b->next) {b->next->prev = b;}
    }

    if (b->next && b->next->in_use == 0) {
        // Consolidate
        b->size = b->size + b->next->size + padded_struct_block_size;
        b->next = b->next->next;
        if (b->next) {b->next->prev = b;}
    }
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
        // printf("[%p:%p:%p:%d,%s]", b->prev, b, b->next, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main() {
    // Example 4
    void *p, *q, *r, *s;

    p = myalloc(10); print_data();
    q = myalloc(20); print_data();
    r = myalloc(30); print_data();
    s = myalloc(40); print_data();

    myfree(q); print_data();
    myfree(p); print_data();
    myfree(s); print_data();
    myfree(r); print_data();

}