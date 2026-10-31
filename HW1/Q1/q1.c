
#include <stdio.h>
#include <unistd.h>

#define N 3

int main() {
    for (int i = 0; i < N; i++) {
        fork();
        fork();
    }

    printf("PID: %d\n", getpid());
    return 0;
}
