// myalloc.h

#ifndef MYALLOC_H
#define MYALLOC_H

#define ALIGNMENT 16   // Must be power of 2
#define MMAP_SIZE 1024
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

typedef struct block {
    int size;            // Size in bytes
    int in_use;          // Boolean
    struct block *next;
} block;

void *myalloc(int size);
void myfree(void *p);

#endif