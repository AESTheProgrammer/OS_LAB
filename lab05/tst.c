#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    srand(time(0));
    printf("%d\n", rand()%101);
    printf("%d\n", rand()%101);
    return 0;
}
