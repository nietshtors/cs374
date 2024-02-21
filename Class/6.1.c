#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void notes() {
    char *x = malloc(128);
    strcpy(x, "Hello!");
    char *p = x + 7;
    strcpy(p, "Another string!");
    *(p-1) = ' ';

    /* Hello!0Another string!0
     * ^     ^               ^
     * x    x+7             x+16
     *       p              v  q*/

    // x: Hello! Another string!
    printf("x: %s\n", x);
    // p: Another string!
    printf("p: %s\n", p);

    void *v = x + 16;
    int *q = v;

    *q = 3490;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    notes();
    
}