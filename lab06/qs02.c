#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>

#define PHIL_COUNT 5

pthread_t philosopher[PHIL_COUNT];
pthread_mutex_t chopstick[PHIL_COUNT+1];
int count = 0;

void* dine(void* arg) {
    int phil_num = ++count;
    while (true) {
        pthread_mutex_lock(&chopstick[PHIL_COUNT]);
        printf("entered\n");
        pthread_mutex_lock(&chopstick[phil_num-1]);
        pthread_mutex_lock(&chopstick[phil_num%PHIL_COUNT]);
        pthread_mutex_unlock(&chopstick[PHIL_COUNT]);
        printf("exited\n");
        printf("philosopher %d is eating using chopstick[%d] and chopstick[%d]!!\n",
                    phil_num, phil_num-1, phil_num);
        printf("philosopher %d is thinking!!\n", phil_num);
        pthread_mutex_unlock(&chopstick[phil_num-1]);
        pthread_mutex_unlock(&chopstick[phil_num%PHIL_COUNT]);
    }
}


int main() { 
    for (int i = 0; i < PHIL_COUNT+1; i++) {
        if (pthread_mutex_init(chopstick + i, NULL) != 0){
            perror("mutex_init");
            return 1;
        }
    }
    for (int i = 0; i < PHIL_COUNT; i++) {
        pthread_create(philosopher + i, NULL, &dine, &i); 
    }
    for (int i = 0; i < PHIL_COUNT; i++) {
        pthread_join(philosopher[i], NULL);
    }
    return 0;
}
