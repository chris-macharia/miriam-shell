#include <stdlib.h>
#include <string.h>

// Declare the global variable "environ"
extern char **environ;

int _setenv(const char *name, const char *value, int overwrite) {
    // Check if the variable already exists
    for (int i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], name, strlen(name)) == 0 && environ[i][strlen(name)] == '=') {
            // Variable already exists
            if (!overwrite) {
                // Do not overwrite existing variable
                return 0;
            }
            // Overwrite existing variable
            char *new_env = malloc(strlen(name) + strlen(value) + 2);
            sprintf(new_env, "%s=%s", name, value);
            environ[i] = new_env;
            return 0;
        }
    }

    // Variable does not exist, create new one
    char *new_env = malloc(strlen(name) + strlen(value) + 2);
    sprintf(new_env, "%s=%s", name, value);

    // Count the number of existing environment variables
    int env_count = 0;
    while (environ[env_count] != NULL) {
        env_count++;
    }

    // Allocate memory for a new environment array that includes the new variable
    char **new_environ = malloc((env_count + 2) * sizeof(char *));
    if (new_environ == NULL) {
        // Allocation error
        return -1;
    }

    // Copy the existing environment variables to the new array
    for (int i = 0; i < env_count; i++) {
        new_environ[i] = environ[i];
    }

    // Add the new variable to the end of the new array
    new_environ[env_count] = new_env;
    new_environ[env_count + 1] = NULL;

    // Replace the old environment array with the new one
    environ = new_environ;

    return 0;
}

int main() {
    // Test the _setenv function
    char *path = getenv("PATH");
    printf("Old PATH: %s\n", path);
    _setenv("PATH", "/usr/bin", 1);
    path = getenv("PATH");
    printf("New PATH: %s\n", path);
    _setenv("MY_VAR", "hello", 0);
    char *my_var = getenv("MY_VAR");
    printf("MY_VAR: %s\n", my_var);

    return 0;
}

