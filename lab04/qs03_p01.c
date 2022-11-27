#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MSGSIZE 256

int main() {
    char in_buffer[MSGSIZE];
    int p01[2];
    int p02[2];
    char* message = "This Is First Process";

    if (pipe(p01) < 0) {
        perror("pipe 1");
        exit(1);
    }
    if (pipe(p02) < 0) {
        perror("pipe 2");
        exit(1);
    }
    write(p01[1], message, strlen(message));
    int pid = fork();
    if (pid == 0) {
        read(p01[0], in_buffer, strlen(message));
        int i = 0;
        for (; i < sizeof(in_buffer); i++) {
            if (islower(in_buffer[i])) in_buffer[i] = toupper(in_buffer [i]);
            else if (isupper(in_buffer[i])) in_buffer[i] = tolower(in_buffer[i]);
        }
        write(p02[1], in_buffer, strlen(message));
        exit(0);
    }
    wait(NULL);
    //read(p02[0], in_buffer, strlen(message)); 
    //printf("%s\n", in_buffer);
    int len = strlen(message) + 1;
    message = malloc(sizeof(char) * len);
    read(p02[0], message, len); 
    printf("%s\n", message);
    return 0;
}
