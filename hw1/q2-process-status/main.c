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

    There is a zombie in this program where the parent is sleeping for 10seconds and the
    child is determeneted waiting for the parent to clean it up with wait();

    We are checking the status of the child and the parent with the command
    ps -o pid,ppid,stat,cmd -C $(BIN) to see the status of each process witch is
    displaying:

    My child is called 1529
    I'm the child 1529
    PID: 1529 says goodbye
        PID    PPID STAT CMD
    1527    1526 S+   ./q2
    1529    1527 Z+   [q2] <defunct>
    PID: 1527 says goodbye

    Where the parent is PID: 1527 sleeping and the child: 1529 is a 
    zombie indicated by the Z+, where the + is because
    it's been running in the background.

*/