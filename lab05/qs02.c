#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 25 * sizeof(int)
#define LOCK_SIZE sizeof(pthread_mutex_t)
#define CHILD_NUM 1 
#define SAMPLE 5000

void* create_shared_memory(size_t);
void take_sample();

int* hist;
sem_t* mutex;


int main() {
    clock_t start = clock();
    hist = (int*) create_shared_memory(SIZE);
    mutex = (sem_t*)create_shared_memory(sizeof(sem_t)) ;
    sem_init(mutex, 1, 1);
    pid_t pid;
    for (int i = 0; i < CHILD_NUM; i++) {
        if ((pid = fork()) < 0) {
            perror("fork");
            abort();
        } else if (pid == 0) {
            take_sample();
            exit(0);
        }
    }

    int status;
    for (int i = 0; i < CHILD_NUM; i++) {
        pid = wait(&status);
    }
    clock_t end = clock();
    double time_spent = (double)(end-start)/CLOCKS_PER_SEC;
    printf("time spent: %f", time_spent * 1000000);
    return 0;
}


void take_sample() {
    int counter = 0;
    srand(time(0));
    int sample_count = SAMPLE / CHILD_NUM;
    for (int j = 0; j < sample_count; j++) {
        for (int i = 0; i < 12; i++) {
            if (rand()%101 > 48)
                counter++;
            else 
                counter--;
        }
        sem_wait(mutex);
        hist[counter + 12]++;
        sem_post(mutex);
        counter = 0;
    }
    exit(0);
}

void* create_shared_memory(size_t size) {
    // Our memory buffer will be readable and writable:
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    return mmap(NULL, size, protection, visibility, -1, 0);
}

