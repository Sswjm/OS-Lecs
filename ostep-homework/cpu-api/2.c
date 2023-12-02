#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>  

int main()
{
    FILE *f = fopen("test_2.txt", "r+");
    if(f == NULL){
        fprintf(stderr, "open file failed\n");
        exit(1);
    }

    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){
        
    }
}