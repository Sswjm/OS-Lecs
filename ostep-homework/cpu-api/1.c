#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>  

int main()
{
    int x = 100;
    int rc = fork();

    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){ // child process
        printf("before child pid:%d change, x value is %d\n", (int)getpid(), x);
        x = 6; // child process set x to 6
        printf("after child pid:%d change, x value is %d\n", (int)getpid(), x);
    }
    else{
        printf("before parent pid:%d change, x value is %d\n", (int)getpid(), x);
        x = 7; // parent process set x to 7
        printf("after parent pid:%d change, x value is %d\n", (int)getpid(), x);
    }

    printf("the value of x is %d\n", x);

    return 0;
}