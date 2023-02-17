#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare the global variable "environ"
extern char **environ;

// Define a linked list node structure
typedef struct PathNode {
    char *dir;
    struct PathNode *next;
} PathNode;

PathNode* build_path_list() {
    char *path = getenv("PATH");
    if (path == NULL) {
        // If the PATH environment variable is not set, return NULL
        return NULL;
    }

    PathNode *head = NULL;
    PathNode *tail = NULL;

    // Parse the directories from the PATH environment variable and add them to the list
    char *dir = strtok(path, ":");
    while (dir != NULL) {
        PathNode *node = malloc(sizeof(PathNode));
        node->dir = strdup(dir);
        node->next = NULL;

        if (head == NULL) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;

        dir = strtok(NULL, ":");
    }

    return head;
}

int main() {
    // Test the build_path_list function
    PathNode *node = build_path_list();
    while (node != NULL) {
        printf("%s\n", node->dir);
        node = node->next;
    }

    return 0;
}

