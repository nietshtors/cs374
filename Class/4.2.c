#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd = open("2files.txt", O_CREAT|O_TRUNC|O_WRONLY, 0644);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    char *s = pid == 0? "C\n": "P\n";

    for (int i = 0; i < 10000; i++) {
        write(fd, s, 2);
    }

    close(fd);
}