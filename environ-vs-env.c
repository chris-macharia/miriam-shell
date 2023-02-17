#include <stdio.h>

// Declare the global variable "environ"
extern char **environ;

int main(int argc, char *argv[], char *env[]) {
    // Print the address of "env"
    printf("Address of env: %p\n", (void*)env);

    // Print the address of "environ"
    printf("Address of environ: %p\n", (void*)environ);

    return 0;
}

