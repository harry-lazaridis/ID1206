
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


/*

    1. Answer: 64. Every time fork() is executed, it multiplies the amount of processes by 2. Since there a 2 fork() in an iteration and there are 3 iterations the answer is 64 including the original parent.

    2. The verification can be done my priting out each process`s PID using getpid(). Running "make run" executes the program and uses wc -l to count the amount of lines which shows how many processes there are.

    3. Answer: 4^N

*/
