#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>  

int main()
{
    FILE *fd = fopen("test_2.txt", "w+");
    if(fd == NULL){
        fprintf(stderr, "open file failed\n");
        exit(1);
    }
    
    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){  // child process
        const char *child_msg = "Child is writing to the file\n";
        fputs(child_msg, fd);
    }
    else{ // parent process
        const char *parent_msg = "Parent is writing to the file\n";
        fputs(parent_msg, fd);
    }

    fclose(fd);
    return 0;
}