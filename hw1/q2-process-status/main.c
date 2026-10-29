#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = fork();
    if(pid == 0) {
        printf("I'm the child %d\n", getpid());
    } else {
        printf("My child is called %d\n", pid);
        sleep(10);
    }

    printf("PID: %d says goodbye\n", getpid());
    return 0;
}

/*
    https://www.geeksforgeeks.org/c/fork-system-call/

    fork() creates a child, one of the differences of a child and a parent is their 
    return value or pid, where a child will return 0 and a parent return > 0

*/