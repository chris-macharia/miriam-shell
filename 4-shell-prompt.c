#include <stdio.h>

int main() {
    char command[100];

    while (1) {
        printf("$ ");
        fgets(command, 100, stdin);
        printf("%s", command);
    }

    return 0;
}

