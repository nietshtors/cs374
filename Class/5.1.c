#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct node {
    int value;
    struct node *next;
}node;

int main() {
    char *s = "16";
    int x = atoi(s);
    printf("%d\n", x);
}