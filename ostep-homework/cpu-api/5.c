#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h> 

int main()
{
    int rc = fork();

    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){ // child process
        int wc = wait(NULL);
        printf("wait return : %d\n", wc);
        printf("process %d print hello\n", (int)getpid());
    }
    else{  // parent process
        //int wc = wait(NULL);
        //printf("wait return : %d\n", wc);
        printf("process %d print goodbye\n", (int)getpid());
    }

    return 0;
}