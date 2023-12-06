#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h> 
#include<fcntl.h>

int main()
{
    int rc = fork();

    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){ // child process
        close(STDOUT_FILENO);
        printf("process %d print hello\n", (int)getpid());
    }
    else{  // parent process
        printf("process %d print goodbye\n", (int)getpid());
    }

    return 0;
}