#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** split_string(char* str, const char* delim, int* count) {
    char** tokens = NULL;
    char* token = strtok(str, delim);
    int i = 0;

    while (token != NULL) {
        tokens = realloc(tokens, sizeof(char*) * (i+1));
        tokens[i] = token;
        i++;
        token = strtok(NULL, delim);
    }

    *count = i;
    return tokens;
}

int main() {
    char str[] = "Hello world! This is a test.";
    int count = 0;
    char** words = split_string(str, " ", &count);

    for (int i = 0; i < count; i++) {
        printf("%s\n", words[i]);
    }

    free(words);
    return 0;
}

