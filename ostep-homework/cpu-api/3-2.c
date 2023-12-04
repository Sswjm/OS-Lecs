#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    int pipefd[2];
    int rc;
    char buffer;

    if(pipe(pipefd) == -1){
        fprintf(stderr, "create pipe failed\n");
        exit(1);
    }

    rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){  // child process
        close(pipefd[0]);  // close read end
        printf("hello\n");
        write(pipefd[1], "x", 1); // write an "x" to notify parent process
        close(pipefd[1]); // close write end
    }
    else{
        close(pipefd[1]); // close write end
        read(pipefd[0], &buffer, 1); // read char to wait child process
        printf("goodbye\n");
        close(pipefd[0]); // close read end
    }

    return 0;
}