#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_PATHS 10

/**
 * Parses a command line string into an array of arguments.
 * Returns the number of arguments.
 */
int parse_command_line(char *command_line, char *args[]) {
    int num_args = 0;
    char *token;

    token = strtok(command_line, " \t\n");
    while (token != NULL && num_args < MAX_ARGS - 1) {
        args[num_args++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[num_args] = NULL;

    return num_args;
}

/**
 * Searches for the given executable file in the given paths.
 * Returns the full path to the executable, or NULL if not found.
 */
char *search_executable(char *file, char *paths[]) {
    char *full_path = malloc(MAX_PATH_LEN);
    int i;

    for (i = 0; i < MAX_PATHS && paths[i] != NULL; i++) {
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", paths[i], file);
        if (access(full_path, X_OK) == 0) {
            return full_path;
        }
    }

    free(full_path);
    return NULL;
}

int main(int argc, char *argv[], char *envp[]) {
    char *command_line;
    char *args[MAX_ARGS];
    char *paths[MAX_PATHS];
    char *path_env;
    char *executable;
    int num_args;
    int status;

    // Parse PATH environment variable.
    path_env = getenv("PATH");
    if (path_env == NULL) {
        fprintf(stderr, "Error: PATH environment variable not set.\n");
        return 1;
    }
    int i;
    for (i = 0; i < MAX_PATHS; i++) {
        paths[i] = strsep(&path_env, ":");
        if (paths[i] == NULL) {
            break;
        }
    }
    paths[i] = NULL;

    while (1) {
        // Display prompt and read command line.
        printf("$ ");
        command_line = NULL;
        size_t len = 0;
        ssize_t read = getline(&command_line, &len, stdin);
        if (read == -1) {
            break;  // End of file or error.
        }

        // Parse command line into arguments.
        num_args = parse_command_line(command_line, args);

        // Check for built-in commands.
        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }

        // Search for executable in PATH.
        executable = search_executable(args[0], paths);
        if (executable == NULL) {
            fprintf(stderr, "%s: command not found\n", args[0]);
            continue;
        }

        // Fork and execute the executable.
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // Child process.
            execve(executable, args, envp);
            // If execve returns, there was an error.
            perror(executable);
            _exit(1);
        } else {
            // Parent process.
            if (wait(&status) == -1) {
                perror("wait");
                continue;
            }
            if (!WIFEXITED(status)) {
