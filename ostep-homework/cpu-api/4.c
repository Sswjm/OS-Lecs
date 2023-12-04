#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>

int main()
{
    // 这里我换了一种更常见的写法，pid_t实际上就是int
    pid_t pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(pid == 0){ // child process
        execl("/bin/ls", "ls", NULL);

        fprintf(stderr, "exec error\n");
        exit(1);
    }
    else{
        int wc = wait(NULL);
    }

    return 0;

}