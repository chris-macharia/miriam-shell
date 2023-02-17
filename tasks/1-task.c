#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int tokenize(char *line, char *tokens[])
{
    int num_tokens = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && num_tokens < MAX_NUM_TOKENS) {
        tokens[num_tokens] = token;
        num_tokens++;
        token = strtok(NULL, " \t\n");
    }
    tokens[num_tokens] = NULL;
    return num_tokens;
}

int main()
{
    char line[MAX_INPUT_SIZE];
    char *tokens[MAX_NUM_TOKENS];
    int status;

    while (1) {
        printf("prompt> ");
        fflush(stdout);

        if (fgets(line, MAX_INPUT_SIZE, stdin) == NULL) {
            break;
        }

        int num_tokens = tokenize(line, tokens);

        if (num_tokens == 0) {
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execvp(tokens[0], tokens);
            fprintf(stderr, "Error: Command not found\n");
            exit(1);
        } else {
            // Parent process
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

