#include <stdio.h>
#include <stdlib.h>

// Declare the global variable "environ"
extern char **environ;

int main() {
    char **envp = environ;

    // Loop through the environment variables and print them
    while (*envp != NULL) {
        printf("%s\n", *envp);
        envp++;
    }

    return 0;
}

