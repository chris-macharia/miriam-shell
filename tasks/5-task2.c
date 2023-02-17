#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_COMMAND_LEN 100
#define MAX_PATH_LEN 100
#define MAX_ARGS 10

extern char **environ;

char* get_command() {
    static char buf[MAX_COMMAND_LEN];
    printf("simple_shell$ ");
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return NULL;
    }
    buf[strcspn(buf, "\n")] = '\0'; // remove newline
    return buf;
}

char* get_full_path(char* command) {
    char* path = getenv("PATH");
    char* dir = strtok(path, ":");
    while (dir != NULL) {
        char* full_path = malloc(MAX_PATH_LEN);
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0) {
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    return NULL;
}

int execute_command(char* command) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (pid == 0) { // child process
        char* full_path = get_full_path(command);
        if (full_path == NULL) {
            printf("%s: command not found\n", command);
            exit(127);
        }
        char* args[MAX_ARGS+1] = {full_path};
        for (int i = 1; i < MAX_ARGS+1; i++) {
            char* arg = strtok(NULL, " ");
            if (arg == NULL) {
                args[i] = NULL;
                break;
            }
            args[i] = arg;
        }
        execve(full_path, args, environ);
        perror("execve"); // should never get here
        exit(126);
    }
    else { // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return -1;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status)) {
            printf("terminated by signal %d\n", WTERMSIG(status));
            return -1;
        }
        else { // should never get here
            printf("unexpected status %d\n", status);
            return -1;
        }
    }
}

int main() {
    while (1) {
        char* command = get_command();
        if (command == NULL) {
            printf("\n");
            exit(0);
        }
        execute_command(command);
    }
    return 0;
}

