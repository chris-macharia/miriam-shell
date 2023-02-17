#include <stdlib.h>
#include <string.h>

extern char **environ;

int _unsetenv(const char *name) {
    int i, j;
    size_t len = strlen(name);

    // Find the variable in the environment
    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=') {
            // Found it, delete it
            for (j = i; environ[j] != NULL; j++) {
                environ[j] = environ[j+1];
            }
            return 0;
        }
    }

    // Variable not found
    return -1;
}

