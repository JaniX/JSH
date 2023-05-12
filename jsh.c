#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024

int main() {

    char input[MAX_CMD_LENGTH];
    char *args[MAX_CMD_LENGTH / 2 + 1];

    while (1) {
        printf(">>> ");
        // musl libc needs it
        fflush(stdout);

        fgets(input, MAX_CMD_LENGTH, stdin);

        if (feof(stdin)) {
            printf("\n");
            break;
        }

        char *token = strtok(input, " \n");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " \n");
        }
        args[i] = NULL;

        if (!strcmp(args[0], "exit")) {
            break;
        }

        pid_t pid = fork();
        if (pid < 0) {
            printf("error: could not fork process\n");
        } else if (pid == 0) {
            execvp(args[0], args);
            printf("error: could not execute command\n");
            exit(1);
        } else {
            // when the child ends unexepectedly
            wait(NULL);
        }
    }

    return 0;
}
