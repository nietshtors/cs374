#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, const char *argv[]) {
    if (argc > 2) {
        printf("Too many arguments: %d > 2\n", argc);
        return -1;
    }

    // Create pipe
    int pfd[2];
    if (pipe(pfd) < 0) {
        perror("pipe");
        return 1;
    }

    // Create fork
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
        return 2;
    }

    // Child
    if (pid == 0) {
        dup2(pfd[0], 0);
        close(pfd[1]);
        execlp("wc", "wc", "-l", NULL);
        exit(0);
    }

    // Parent
    dup2(pfd[1], 1);
    close(pfd[0]);
    execlp("ls", "ls", "-1a", argv[1], NULL);
    exit(0);
}