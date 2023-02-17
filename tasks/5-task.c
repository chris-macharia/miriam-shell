#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_PATHS 50
#define MAX_PATH_LEN 256

/* Searches the directories in PATH for the given executable filename.
 * If found, returns the full path to the executable. Otherwise, returns NULL. */
char *search_executable(char *filename, char *paths[]) {
    char *full_path = malloc(MAX_PATH_LEN);
    if (full_path == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; paths[i] != NULL; i++) {
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", paths[i], filename);
        if (access(full_path, X_OK) == 0) {
            return full_path;
        }
    }
    free(full_path);
    return NULL;
}

/* Parses the PATH environment variable into an array of directory paths. */
void parse_path(char *path, char *paths[]) {
    char *path_copy = strdup(path);
    if (path_copy == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    char *dir = strtok(path_copy, ":");
    int i = 0;
    while (dir != NULL) {
        paths[i++] = dir;
        dir = strtok(NULL, ":");
    }
    paths[i] = NULL;
    free(path_copy);
}

/* The main function of the shell. */
int main(void) {
    char *args[MAX_LINE/2 + 1];
    char *paths[MAX_PATHS];
    char *path = getenv("PATH");
    parse_path(path, paths);

    while (1) {
        printf("simple_shell> ");
        fflush(stdout);

        char input[MAX_LINE];
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            break; // end of file (ctrl-d)
        }

        // remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        if (strcmp(input, "env") == 0) {
            char **env = environ;
            while (*env != NULL) {
                printf("%s\n", *env);
                env++;
            }
            continue;
        }

        args[0] = input;
        args[1] = NULL;

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // child process
            char *executable = search_executable(input, paths);
            if (executable == NULL) {
                fprintf(stderr, "simple_shell: %s: command not found\n", input);
                exit(EXIT_FAILURE);
            }
            if (execv(executable, args) == -1) {
                perror("execv");
                exit(EXIT_FAILURE);
            }
        } else {
            // parent process
            int status;
            if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }
            if (!WIFEXITED(status)) {
                printf("Child process terminated abnormally\n");
            }
        }
    }

    printf("Exiting shell...\n");
    return 0;
}

