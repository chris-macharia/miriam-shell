#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int _unsetenv(const char *name)
{
    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        return -1;
    }

    int i = 0;
    char **envp = environ;

    while (envp[i] != NULL) {
        if (strncmp(name, envp[i], strlen(name)) == 0 && envp[i][strlen(name)] == '=') {
            // Remove the environment variable from the array by shifting all
            // elements after it one position to the left
            for (int j = i; envp[j] != NULL; j++) {
                envp[j] = envp[j + 1];
            }

            return 0;
        }

        i++;
    }

    return 0;
}

