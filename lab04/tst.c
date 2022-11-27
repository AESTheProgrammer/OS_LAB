#include <stdio.h>
#include <string.h>

int main() {
    //char ch = '0';
    //int charr = 122;
    //printf("%d\n", ch);
    //printf("%c\n", charr);

    if (fork() && fork()) {
        fork();
    }

    if (fork() || fork()) {
        fork();
    }
    printf("2\n");
    return 0;
}
