#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("Hello!\n");
    pid_t pid;

    pid = fork();

    if (pid == 0) {
        // Child
        // execlp("ls", "ls", "-l", NULL);
        char *args[] = {
            "ls",
            "ls",
            "-l",
            NULL
        };

        execvp(args[0], args);
        perror("exec");
        exit(0);
    } else {
        // Parent
        wait(NULL);
    }
    printf("Goodbye!\n");
}