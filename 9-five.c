#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num_processes = 5;
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Error: fork() failed.\n");
            exit(1);
        } else if (pid == 0) {
            printf("Child %d (PID %d) executing ls -l /tmp\n", i+1, getpid());
            execl("/bin/ls", "ls", "-l", "/tmp", (char *) NULL);
            printf("Error: execl() failed.\n");
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            printf("Child %d (PID %d) finished executing.\n", i+1, pid);
        }
    }

    printf("All child processes finished executing.\n");
    return 0;
}

