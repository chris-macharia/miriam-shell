#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int _setenv(const char *name, const char *value, int overwrite) {
    char **env, *new_env;
    size_t len_name = strlen(name);
    size_t len_value = strlen(value);
    size_t len_env;

    for (env = environ; *env != NULL; env++) {
        if (strncmp(*env, name, len_name) == 0 && (*env)[len_name] == '=') {
            if (!overwrite) {
                return 0;
            }
            break;
        }
    }

    new_env = (char *)malloc(len_name + len_value + 2);
    if (new_env == NULL) {
        return -1;
    }

    sprintf(new_env, "%s=%s", name, value);

    if (*env != NULL) {
        *env = new_env;
    } else {
        *env = new_env;
        *(env + 1) = NULL;
    }

    return 0;
}

int main() {
    char *path = getenv("PATH");

    printf("Old PATH: %s\n", path);
    _setenv("PATH", "/usr/bin:/bin", 1);
    path = getenv("PATH");
    printf("New PATH: %s\n", path);

    return 0;
}

