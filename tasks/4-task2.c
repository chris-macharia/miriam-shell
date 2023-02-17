#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LEN 256
#define MAX_PATHS 64
#define MAX_PATH_LEN 256

char *PATH[MAX_PATHS];

void parse_path() {
    char *path_env = getenv("PATH");
    char *path = strtok(path_env, ":");
    int i = 0;
    while (path != NULL && i < MAX_PATHS) {
        PATH[i++] = path;
        path = strtok(NULL, ":");
    }
    PATH[i] = NULL;
}

char *find_executable(char *command) {
    for (int i = 0; PATH[i] != NULL; i++) {
        char *path = PATH[i];
        char full_path[MAX_PATH_LEN];
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", path, command);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path);
        }
    }
    return NULL;
}

void execute_command(char *command) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        char *executable = find_executable(command);
        if (executable == NULL) {
            printf("%s: command not found\n", command);
            exit(127);
        }
        int ret = execl(executable, command, NULL);
        if (ret == -1) {
            perror("execl");
            exit(126);
        }
    } else {
        int status;
        if (wait(&status) == -1) {
            perror("wait");
        } else {
            if (!WIFEXITED(status)) {
                printf("Command terminated abnormally\n");
            }
        }
    }
}

int main() {
    parse_path();
    char command[MAX_COMMAND_LEN];
    while (1) {
        printf("$ ");
        if (fgets(command, MAX_COMMAND_LEN, stdin) == NULL) {
            printf("\n");
            exit(0);
        }
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
        if (strcmp(command, "exit") == 0) {
            exit(0);
        }
        execute_command(command);
    }
    return 0;
}

