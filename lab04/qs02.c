#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


#define PORT 12345
#define SERVER_COUNT 5

int main() {
    char port[10];
    for (int i = 0; i < SERVER_COUNT; i++) {
        sprintf(port, "%d", i + PORT);
        char *args[] = {"./server.o", port, NULL};
        int pid = fork();
        if (pid < 0) {
            perror("fork()\n");
            return -1;
        } else if (pid == 0) {
            execvp(args[0], args);
            exit(0);
        }
    }
    pid_t pid;
    int response;
    for (int i = 0; i < SERVER_COUNT; i++) {
        pid = wait(&response);
    }
    return 0;
}

