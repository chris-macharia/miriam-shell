#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIMITERS " \t\r\n\a"

int main(void)
{
    char *input_buffer;
    char *command;
    char *argument;
    char *path;
    char **paths;
    int status;
    int i;

    input_buffer = malloc(BUFFER_SIZE * sizeof(char));
    if (input_buffer == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        command = strtok(input_buffer, TOKEN_DELIMITERS);
        if (command == NULL) {
            continue;
        }

        argument = strtok(NULL, TOKEN_DELIMITERS);

        path = getenv("PATH");
        if (path == NULL) {
            perror("getenv failed");
            exit(EXIT_FAILURE);
        }

        paths = malloc(BUFFER_SIZE * sizeof(char *));
        if (paths == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        i = 0;
        paths[i] = strtok(path, ":");
        while (paths[i] != NULL) {
            i++;
            paths[i] = strtok(NULL, ":");
        }

        for (i = 0; paths[i] != NULL; i++) {
            char *command_path = malloc(BUFFER_SIZE * sizeof(char));
            if (command_path == NULL) {
                perror("malloc failed");
                exit(EXIT_FAILURE);
            }

            snprintf(command_path, BUFFER_SIZE, "%s/%s", paths[i], command);

            if (access(command_path, F_OK) == 0) {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork failed");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0) {
                    char *arguments[] = { command, argument, NULL };
                    if (execv(command_path, arguments) == -1) {
                        perror("execv failed");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    waitpid(pid, &status, 0);
                    break;
                }
            }

            free(command_path);
        }

        free(paths);
    }

    free(input_buffer);
    exit(EXIT_SUCCESS);
}

