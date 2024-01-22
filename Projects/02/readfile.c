#include <unistd.h>
#include <fcntl.h>

#define MAX_SIZE 2048

int main(int argc, const char *argv[]) {
    int file = open(argv[1], O_RDWR);

    char buf[MAX_SIZE];
    int size = read(file, buf, MAX_SIZE);

    write(1, buf, size);

    close(file);
}
