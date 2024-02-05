#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    // Check for enough arguments
    if (argc < 3) {
        printf("Too few arguments: %d < 3\n", argc);
        return -1;
    }

    // Open output file
    int fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    // Anything from stdout goes to file
    dup2(fd, 1);
    execvp(argv[2], &argv[2]);
}