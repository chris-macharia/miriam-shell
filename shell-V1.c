#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT];
    const char* path = "/bin/";
    char full_path[MAX_INPUT];
    pid_t pid;

    while (1) {
        printf("$ ");
        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = 0; // remove trailing newline

        if (strcmp(input, "exit") == 0) {
            exit(0);
        }

        strcpy(full_path, path);
        strcat(full_path, input);

        pid = fork();
        if (pid < 0) {
            printf("Error: fork() failed.\n");
            exit(1);
        } else if (pid == 0) {
            execl(full_path, input, (char *) NULL);
            printf("Error: execl() failed.\n");
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

