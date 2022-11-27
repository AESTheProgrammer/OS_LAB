#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

struct shmseg {
    int cnt;
    int complete;
    char buf[BUF_SIZE];
};

int fill_buffer(char* bufptr, int size);

int main(int argc, char* argv[]) {
    int shmid;
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0664|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 1;
    }
    
    struct shmseg *shmp;
    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void*) -1) {
        perror("Shared memory attach");
        return 1;
    }

    char* bufptr;
    bufptr = shmp -> buf;
    int i = 0, space_available = BUF_SIZE;
    for (; i < 5; i++) {
        shmp -> cnt = fill_buffer(bufptr, space_available);
        shmp -> complete = 0;
        printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp -> cnt);
        bufptr = shmp -> buf;
        space_available = BUF_SIZE;
        sleep(3);
    }
    shmp -> complete = 1;
    if (shmdt(shmp) == -1) {
        perror("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }
    printf("Writing Process\n");
    return 0;
}


int fill_buffer(char* bufptr,int size) {
    static char ch = 'A';
    int filled_count;

    memset(bufptr, ch, size - 1);
    printf("debug \n");
    bufptr[size - 1] = '\0';
    if (ch > 122)
        ch = 65;
    if ( (ch >= 65) && (ch <= 122) ) {
        if ( (ch >= 91) && (ch <= 96) ) {
            ch = 65;
        }
    }
    filled_count = strlen(bufptr);
    ch++;
    return filled_count;
}
