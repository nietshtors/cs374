#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int pfd[2];
    char buf[128] = {0};

    if (pipe(pfd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 2;
    }

    if (pid == 0) {
        // This happens in the child
        read(pfd[0], buf, 128);
        printf("Buf is \"%s\"\n", buf);
        exit(0);
    }

    // This happens in the parent
    //write(pfd[1], "Hi there!", 9);

    close(1);
    dup(pfd[1]);

    //write(1, "Hi there!", 9);  // this should go in the pipe
    printf("Another test");
    fflush(stdout);  // Since stdout is line buffered

    wait(NULL);
}