#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int run_command(char **args, int num_args) {
    // Check if command is "cd"
    if (!strcmp(args[0], "cd")) {
        if (num_args != 2) {
            fprintf(stderr, "usage: cd directory\n");
        }
        if ((chdir(args[1])) == -1) {
            perror(args[1]);
        }
        return 1;
    }
    // Check if command is "exit"
    // this below needs work
    // (args[1] == NULL) ? 0 : *args[1] - 48 
    else if (!strcmp(args[0], "exit")) {exit((args[1] == NULL) ? 0 : *args[1] - 48);}

    pid_t pid;
    pid = fork();

    // Child
    if (pid == 0) {
        execvp(args[0], args);
        perror("exec");
        exit(1);
    }

    // Parent waits for Child to finish
    wait(NULL);
    return 1;
}


int main() {
    while (1){
        char input[2048];
        char *args[128];
        char cwd[256];

        // Get current working directory
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
        }

        // Current working directory with unique separator 
        printf("%s ( ͡° ͜ʖ ͡°) ", cwd);
        fflush(stdout);
        fgets(input, sizeof(input), stdin);

        // Exit on end-of-file
        if (feof(stdin)) {exit(0);}

        char *token;
        int num_args = 0;

        // Tokenize the input into args array
        if ((token = strtok(input, " \t\n\r")) != NULL) {
            do {
                args[num_args++] = token;
            } while ((token = strtok(NULL, " \t\n\r")) != NULL);
        }

        // Add NULL terminator
        args[num_args] = NULL;

        run_command(args, num_args);
    }
}