#include "myalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int first = 1;
struct block *head = NULL;  // Head of the list, empty


void *myalloc(int size) {
    if (first == 1) {
        first--;
        void *heap = mmap(NULL, MMAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        head = heap;
        int data_size = MMAP_SIZE - PADDED_SIZE(sizeof(block));
        head->size = data_size;
        head->in_use = 0;
        head->next = NULL;
    }
    size = PADDED_SIZE(size);
    int padded_struct_block_size = PADDED_SIZE(sizeof(block));
    block *b = head;
    while (b->size < size || b->in_use) {
        if (b->next) {
            b = b->next;
        }
        else {
            return NULL;
        }
    }
    b->in_use = 1;

    if (/*Big enough to split*/) {
        // Split
    }

    return PTR_OFFSET(b, padded_struct_block_size);
}

void myfree(void *p) {
    (void)p;
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

    void *p;

    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(16);
    printf("%p\n", p);
}