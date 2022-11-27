#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/mman.h>


#define PORT 8080
#define MAX 256
#define NUM_GROUPS 3
#define MAX_NUM_USERS 5

static int *groups;
static int *counts; // automatically initialized to zero
static int *count;

void debug(void);
void* create_shared_memory(int);
int handle_client(int);
void init_2darray(int val, int arr[], int row, int col);
void to_lower_string (char*);

int main(int argc, char const *argv[]) {
    // creates socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    // host to network -- coverts the ending of the given integer
    address.sin_port = htons(PORT); 
    // binding
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // listening on server socket with backlog size 3.
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listen on %s:%d\n",
            inet_ntoa(address.sin_addr),
            ntohs(address.sin_port));


    // initialize shared memory
    // repetition can be seen here
    int shm_size;

    shm_size = sizeof(int) * NUM_GROUPS * MAX_NUM_USERS;
    groups = (int*)create_shared_memory(shm_size);
    init_2darray(-1, groups, NUM_GROUPS, MAX_NUM_USERS);

    shm_size = sizeof(counts[0]) * NUM_GROUPS;
    counts = (int*)create_shared_memory(shm_size);

    count = (int*)create_shared_memory(sizeof(int));
    *count = 0;

    int pid = fork();
    if (pid == 0) {
        const int addrlen = sizeof(address);
        // accepting client
        // accept returns client socket
        // and fills given address and 
        // addrlen with client address information.
        while (true) {
            int client_socket, valread;
            if (*count < MAX_NUM_USERS && 
                (client_socket = 
                accept(server_fd, 
                        (struct sockaddr *)&address, 
                        (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            } else if (*count == MAX_NUM_USERS) {
                int status;
                int pid = wait(&status);
                count--;
            }
            count++;
            int child_pid = fork();
            if (child_pid < 0) {
                perror("child");
            } else if (child_pid == 0) {
                handle_client(client_socket);
                //count--;
                exit(EXIT_SUCCESS);
            } // must add check for fork() < 0
        }
    }
    int status;
    pid = wait(&status);
    return 0;
}

int handle_client(int sock) {
    char commands[4][6] = {"join", "send", "leave", "quit"} ;
    // listen to client 
    char in[MAX] = {0};
    while (true) {
        // read message
        int valread = read(sock, in, MAX); 
        printf("recieved: %s\n", in);
        if (valread < 0) {
            perror("read");
            return -1;
            exit(1);
        }
        if (!strcmp(in, "quit")) {
            close(sock); // clean group and other stuff
            break;
        } 
        char* command = strtok(in, " ");
        to_lower_string(command);
        if (!strcmp(command, "send")) {
            printf("command: %s\n", command);
            int gid = atoi(strtok(NULL, " "));
            char* message = strtok(NULL, " ");
            char* name = strtok(NULL, "*");
            printf("gid= %d, message= %s, name= %s\n", gid, message, name);  

            if (gid > 4 || gid < 0) {
                continue;
            }
            // broadcast the message if it's joined in the group
            printf("counts[gid]= %d\n", counts[gid]);
            for (int i = 0; i < counts[gid] ; i++) {
                if (groups[gid * MAX_NUM_USERS + i] == sock) {
                    strcat(name, ":");
                    strcat(name, message); // this set name to "armin: SOMETHING"
                    //printf("final message: %s\n", name);  
                    int sock_i;
                    for (int j = 0; j < MAX_NUM_USERS; j++) {
                        printf("final message: %s\n", name);  
                        sock_i = groups[gid * MAX_NUM_USERS + j];
                        printf("sock_i: %d\n", sock_i);
                        if (sock_i != -1) {
                            int valsend = send(sock_i, name, strlen(name)+1, 0);
                            printf("valsend: %d\n", valsend);
                        }
                    }
                }
            }
        } else if(!strcmp(command, "join")) {
            printf("command: %s\n", command);
            int gid = atoi(strtok(NULL, " "));
            if (gid > 4 || gid < 0) { // no such group
                continue;
            } else if (counts[gid] >= MAX_NUM_USERS) { // group is full
                continue;
            }
            int i = 0;
            for (; i < MAX_NUM_USERS; i++) {
                if (groups[gid * MAX_NUM_USERS + i] == -1) {
                    debug();
                    groups[gid * MAX_NUM_USERS + i] = sock;
                    counts[gid]++;
                    break;
                }
            }
        } else if(!strcmp(command, "leave")) {
            printf("command: %s\n", command);
            int gid = atoi(strtok(NULL, " "));
            if (gid > 4 || gid < 0) { // no such group
                continue;
            } else if (counts[gid] > MAX_NUM_USERS) { // group is full
                continue;
            }
            int i = 0;
            for (; i < *counts; i++) {
                if (groups[gid * MAX_NUM_USERS + i] == sock) {
                    groups[gid * MAX_NUM_USERS + i] = -1;
                    counts[gid]--;
                    break;
                }
            }

        } else {
            printf("Error: format not recognized\n");
        }
    }
    return 0;
}

void init_2darray(int val, int arr[], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i * col + j] = val;
        }
    }
}

void to_lower_string(char* p) {
    for (; *p; p++) *p = tolower(*p);
}

void* create_shared_memory(int size) {
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    return ptr;
}

void debug() {
    printf("debug\n");
}
