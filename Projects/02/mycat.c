#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 2048

int main(int argc, const char *argv[]) {
    int fd;

    for (int i = (argc == 1) ? 0: 1; i <= argc; i++) {
        if (i == 0) {
            fd = 0;
        }
        else {
            fd = open(argv[i], O_RDWR);
        }

        int size = 1;
        char* buf = malloc(MAX_SIZE);
        while (size > 0) {
            size = read(fd, buf, MAX_SIZE);
            if (size > 0) {
                write(1, buf, size);
            }
        }
        close(fd);
    }
    
}
