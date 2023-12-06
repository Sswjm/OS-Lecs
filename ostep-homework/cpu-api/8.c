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
        char x;
        scanf("%c", &x);
        const char *buf = &x;
        write(pipefd[1], buf, 1); // write an "x" to notify parent process
        close(pipefd[1]); // close write end
    }
    else{
        close(pipefd[1]); // close write end
        read(pipefd[0], &buffer, 1); // read char to wait child process
        printf("input value is %c\n", buffer);
        close(pipefd[0]); // close read end
    }

    return 0;
}