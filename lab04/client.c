#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX 60 
#define BASE_PORT 12345
#define NUM_GROUPS 5
#define MAX_NAME_LEN 15

char name[MAX_NAME_LEN];
int socks[NUM_GROUPS] = {0};
int eavesdroppers_pid[NUM_GROUPS] = {0};


struct sockaddr_in serv_addr;
int eavesdropper_pid;

int create_socket(int);
void to_lower_string (char*);
int handle_input(char*);
int find_group_socket(int);
int eavesdrop(int);


int main(int argc, char const *argv[]) {
    printf("Enter your name: ");
    scanf("%[^\n]%*c", name);
    strcat(name, ": ");

    // loop for sending message from client to server
    char out[MAX] = {0};
    while (true) {
        fflush(stdin);
        scanf("%[^\n]%*c", out);
        handle_input(out);
    } // TODO must be merged with handle_input
    return 0;
}


int handle_input(char* out) {
    int valread, valsend;
    // listen to client 
    // read message
    if (!strcmp(out, "quit")) {
        for (int i = 0; i < NUM_GROUPS; i++) {
            if (socks[i] != 0) {
                kill(eavesdroppers_pid[i], SIGTERM);
                close(socks[i]);
            }
        }
        exit(0);
    } 
    char* command = strtok(out, " ");
    to_lower_string(command);
    if (!strcmp(command, "send")) { // TODO
        int gid = atoi(strtok(NULL, " "));
        if (find_group_socket(gid) == 0) {
            printf("no group found\n");
            return 1;
        }
        char message[MAX + MAX_NAME_LEN] = {0};
        strcpy(message, name);
        strcat(message, strtok(NULL, "\n"));
        valsend = send(socks[gid], message, sizeof(message), 0);
         
        if (valsend < 0) {
            perror("   send() failed");
            return -1;
        }
    } else if(!strcmp(command, "join")) { 
        int gid = atoi(strtok(NULL, " "));
        if (gid >= NUM_GROUPS || gid < 0 || socks[gid] != 0) {
            return -1;
        }
        // connects to the server
        create_socket(BASE_PORT + gid);
        eavesdroppers_pid[gid] = eavesdrop(gid);
        /* TODO  connect to the group */
    } else if(!strcmp(command, "leave")) {
        int gid = atoi(strtok(NULL, " "));
        if (find_group_socket(gid) == -1) { // no such group
            printf("no such group\n");
            return 1;
        }
        close(socks[gid]);
        kill(eavesdroppers_pid[gid], SIGTERM);
        /* TODO disconnect from the group */

    } else {
        printf("Error: format not recognized\n");
    }
    return 0;
}

int find_group_socket(int gid) {
    if (gid >= NUM_GROUPS || gid < 0 || socks[gid] == 0)
      return 0;
    return socks[gid];
}

void to_lower_string(char* p) {
    for (; *p; p++) *p = tolower(*p);
}

int eavesdrop(int gid) {
    int pid = fork();
    if (pid < 0) {
        perror("    fork() failed");
        return 1;
    } if (pid == 0) {
        int rc, close_conn;
        char buffer[MAX + MAX_NAME_LEN] = {0};
        while (true) {
            rc = recv(socks[gid], buffer, sizeof(buffer), 0);
            if (rc < 0) {
                perror("recv");
                exit (-1);
            } 
            else if (rc == 0) {
                printf("    Connection closed\n");
                close(socks[gid]);
                exit (-1);
            }
            else {
                printf("%s\n", buffer);
            }
        }
    } else {
        return pid;
    }
    exit(0);
}

int create_socket(int port) {
    int i = port - BASE_PORT;
    if ((socks[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    int* TRUE = malloc(sizeof(int));
    *TRUE = 1;
    setsockopt(socks[i], SOL_SOCKET, SO_REUSEADDR, TRUE, sizeof(*TRUE));
    // sets all memory cells to zero
    memset(&serv_addr, '0', sizeof(serv_addr));
    // sets port and address family
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(socks[i], (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}

// TODO when sending wrong message formats seg fault occurs
// TODO wait for child
