#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    clock_t start = clock();
    int hist[25];
    int counter = 0;
    srand(time(0));
    for (int j = 0; j < 5000; j++) {
        for (int i = 0; i < 12; i++) {
            if (rand()%101 > 48)
                counter++;
            else 
                counter--;
        }
        hist[counter + 12]++;
        //printf("%d\n", counter);
        counter = 0;
    }
    clock_t end = clock();
    double time_spent = (double)(end-start)/CLOCKS_PER_SEC;
    printf("time spent: %f", time_spent * 1000000);
    return 0;
}
