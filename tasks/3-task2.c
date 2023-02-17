#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_PATH 1024

char *get_path();
char **parse_input(char *input);
void execute_command(char **args, char *path);

int main(void) {
    char *path = get_path();

    while (1) {
        char input[MAX_INPUT];
        printf("$ ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                exit(EXIT_SUCCESS);
            } else {
                perror("fgets");
                exit(EXIT_FAILURE);
            }
        }

        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;

        // Parse input into command and arguments
        char **args = parse_input(input);

        // If the input is empty, restart the loop
        if (args[0] == NULL) {
            free(args);
            continue;
        }

        execute_command(args, path);
        free(args);
    }

    return 0;
}

// Get the PATH environment variable
char *get_path() {
    char *path = getenv("PATH");
    if (path == NULL) {
        fprintf(stderr, "Unable to get PATH environment variable.\n");
        exit(EXIT_FAILURE);
    }

    return path;
}

// Parse the input string into a list of arguments
char **parse_input(char *input) {
    char **args = malloc(MAX_ARGS * sizeof(char *));
    if (args == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *arg = strtok(input, " ");
    int i = 0;
    while (arg != NULL) {
        args[i++] = arg;
        arg = strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}

// Execute the command
void execute_command(char **args, char *path) {
    char command[MAX_PATH];
    int command_found = 0;

    // Check if the command exists in the PATH
    char *dir = strtok(path, ":");
    while (dir != NULL) {
        snprintf(command, MAX_PATH, "%s/%s", dir, args[0]);
        if (access(command, X_OK) == 0) {
            command_found = 1;
            break;
        }
        dir = strtok(NULL, ":");
    }

    if (command_found) {
        // Execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            execv(command, args);
            perror(command);
            exit(EXIT_FAILURE);
        } else {
            int status;
            if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        printf("%s: command not found\n", args[0]);
    }
}

