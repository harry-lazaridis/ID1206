#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define N 3

int main() {
    for (int i = 0; i < N; i++) {
        fork();
        fork();
    }

    printf("Hello world!, process_id(pid) = %d \n", getpid()); 
    return 0;
}

/** 
    https://www.geeksforgeeks.org/c/fork-system-call/

    1) In total there will be 2^2N = 4^N processes, 
        while 4^N - 1 processes created by the parent process. 
        In our case we will create 64 processes in total including the parent.
    2) We make us of the command wc -l to count 
        the amount of times the print function is called 
        by piping it after running the program.
    3) 64 processes including the parent.

*/