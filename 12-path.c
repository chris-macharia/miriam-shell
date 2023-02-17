#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare the global variable "environ"
extern char **environ;

char* _getenv(const char *name) {
    char **envp = environ;
    size_t len = strlen(name);

    // Loop through the environment variables and search for the specified name
    while (*envp != NULL) {
        if (strncmp(name, *envp, len) == 0 && (*envp)[len] == '=') {
            // If the name is found, return a pointer to the value
            return &((*envp)[len+1]);
        }
        envp++;
    }

    // If the name is not found, return NULL
    return NULL;
}

int main() {
    // Test the _getenv function
    char *path = _getenv("PATH");
    if (path != NULL) {
        printf("PATH=%s\n", path);
    } else {
        printf("PATH not found\n");
    }

    return 0;
}

