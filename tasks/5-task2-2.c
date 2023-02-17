#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 1024

void print_prompt() {
    printf("simple_shell$ ");
    fflush(stdout);
}

int main(int argc, char *argv[], char *envp[]) {
    char command[MAX_COMMAND_LENGTH];
    char *path = getenv("PATH");

    while (1) {
        print_prompt();

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        // Remove the trailing newline character
        command[strcspn(command, "\n")] = '\0';

        // Check for built-in commands
        if (strcmp(command, "exit") == 0) {
            exit(0);
        } else if (strcmp(command, "env") == 0) {
            char **env;
            for (env = envp; *env != NULL; env++) {
                printf("%s\n", *env);
            }
            continue;
        }

        // Tokenize the command
        char *tokens[2];
        tokens[0] = strtok(command, " ");
        tokens[1] = strtok(NULL, " ");

        // Search for the command in the directories specified in PATH
        int found = 0;
        if (tokens[0] != NULL) {
            char *p = strtok(path, ":");
            while (p != NULL) {
                char command_path[MAX_COMMAND_LENGTH];
                snprintf(command_path, MAX_COMMAND_LENGTH, "%s/%s", p, tokens[0]);

                if (access(command_path, X_OK) == 0) {
                    found = 1;
                    pid_t pid = fork();
                    if (pid == 0) {
                        // Child process
                        char *args[] = {tokens[0], tokens[1], NULL};
                        execv(command_path, args);
                    } else if (pid > 0) {
                        // Parent process
                        wait(NULL);
                        break;
                    } else {
                        // Fork failed
                        perror("fork");
                        exit(1);
                    }
                }

                p = strtok(NULL, ":");
            }
        }

        if (!found) {
            printf("%s: command not found\n", tokens[0]);
        }
    }

    return 0;
}

