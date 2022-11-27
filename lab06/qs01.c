#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <semaphore.h>

#define MAX 5
#define BUFFER_SIZE sizeof(int)

void* create_shared_memory(size_t);
void write_to_buffer(int pid);
void read_from_buffer(int pid);
int create_reader(void);

int* count;
sem_t* mutex;

int main() {
    mutex = (sem_t*)create_shared_memory(sizeof(sem_t)) ;
    count = (int*) create_shared_memory(sizeof(int));
    sem_init(mutex, 1, 1);
    (*count) = 0;
    int child_pid01 = create_reader();
    int child_pid02 = create_reader();
    int parent_pid = getpid();
    write_to_buffer(parent_pid);

    pid_t pid;
    int status;
    pid = wait(&status);
    pid = wait(&status);
    sem_destroy(mutex);
    return 0;
}

int create_reader() {
    int pid = fork();
    //printf("child=%d,\n", getpid());
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        read_from_buffer(getpid());
        exit(0);
    }
    return pid;
}


void write_to_buffer(int pid) {
    int val = 1;
    while (*count < MAX) {
        sem_wait(mutex);
        //sem_getvalue(mutex, &val);
        //printf("entered sem=%d, pid=%d,\n", pid, val);
        (*count)++;
        printf("writer: pid=%d, count=%d\n", pid, *count);
        fflush(stdout);
        sem_post(mutex);
        //printf("exited sem=%d, pid=%d,\n", pid, val);
        //sem_getvalue(mutex, &val);
    }
}

void read_from_buffer(int pid) {
    int val = 1;
    while (*count < MAX) {
        sem_wait(mutex);
        //printf("entered sem=%d, pid=%d,\n", pid, val);
        printf("reader: pid=%d, count=%d\n", pid, *count);
        fflush(stdout);
        sem_post(mutex);
    }
}

// change child access to read-only
void* create_shared_memory(size_t size) {
    int protection = PROT_WRITE | PROT_READ;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, protection, visibility, -1, 0);
}

